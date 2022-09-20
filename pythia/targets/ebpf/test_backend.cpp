#include "backends/pythia/pythia/targets/ebpf/test_backend.h"

#include <map>
#include <ostream>
#include <string>
#include <utility>

#include <boost/none.hpp>

#include "backends/pythia/common/lib/ir.h"
#include "gsl/gsl-lite.hpp"
#include "ir/ir.h"
#include "lib/cstring.h"
#include "lib/exceptions.h"
#include "lib/gmputil.h"

#include "backends/pythia/pythia/options.h"
#include "backends/pythia/pythia/targets/ebpf/backend/stf/stf.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

const big_int EBPFTestBackend::ZERO_PKT_VAL = 0x2000000;
const big_int EBPFTestBackend::ZERO_PKT_MAX = 0xffffffff;
const std::vector<std::string> EBPFTestBackend::SUPPORTED_BACKENDS = {"STF"};

EBPFTestBackend::EBPFTestBackend(const ProgramInfo& programInfo, ExplorationStrategy& symbex,
                                 const boost::filesystem::path& testPath,
                                 boost::optional<uint32_t> seed)
    : TestBackEnd(programInfo, symbex) {
    cstring testBackendString = PythiaOptions::get().testBackend;
    if (testBackendString == "STF") {
        testWriter = new STF(testPath.c_str(), seed);
    } else {
        std::stringstream supportedBackendString;
        bool isFirst = true;
        for (const auto& backend : SUPPORTED_BACKENDS) {
            if (!isFirst) {
                supportedBackendString << ", ";
            } else {
                isFirst = false;
            }
            supportedBackendString << backend;
        }
        P4C_UNIMPLEMENTED(
            "Test back end %1% not implemented for this target. Supported back ends are %2%.",
            testBackendString, supportedBackendString.str());
    }
}

TestBackEnd::TestInfo EBPFTestBackend::produceTestInfo(
    const ExecutionState* executionState, const Model* completedModel,
    const IR::Expression* outputPacketExpr, const IR::Expression* outputPortExpr,
    const std::vector<gsl::not_null<const TraceEvent*>>* programTraces) {
    auto testInfo = TestBackEnd::produceTestInfo(executionState, completedModel, outputPacketExpr,
                                                 outputPortExpr, programTraces);
    // This is a hack to deal with a behavioral model quirk.
    // Packets that are too small are truncated to 02000000 (in hex) with width 32 bit.
    if (testInfo.outputPacket->type->width_bits() == 0) {
        int outPktSize = ZERO_PKT_WIDTH;
        testInfo.outputPacket =
            IRUtils::getConstant(IRUtils::getBitType(outPktSize), EBPFTestBackend::ZERO_PKT_VAL);
        testInfo.packetTaintMask =
            IRUtils::getConstant(IRUtils::getBitType(outPktSize), EBPFTestBackend::ZERO_PKT_MAX);
    }
    return testInfo;
}

const TestSpec* EBPFTestBackend::createTestSpec(const ExecutionState* executionState,
                                                const Model* completedModel,
                                                const TestInfo& testInfo) {
    // Create a testSpec.
    TestSpec* testSpec = nullptr;

    const auto* ingressPayload = testInfo.inputPacket;
    const auto* ingressPayloadMask = IRUtils::getConstant(IRUtils::getBitType(1), 1);
    const auto ingressPacket = Packet(testInfo.inputPort, ingressPayload, ingressPayloadMask);

    boost::optional<Packet> egressPacket = boost::none;
    if (!testInfo.packetIsDropped) {
        egressPacket = Packet(testInfo.outputPort, testInfo.outputPacket, testInfo.packetTaintMask);
    }
    testSpec = new TestSpec(ingressPacket, egressPacket, testInfo.programTraces);
    // We retrieve the individual table configurations from the execution state.
    const auto uninterpretedTableConfigs = executionState->getTestObjectCategory("tableconfigs");
    // Since these configurations are uninterpreted we need to convert them. We launch a
    // helper function to solve the variables involved in each table configuration.
    for (const auto& tablePair : uninterpretedTableConfigs) {
        const auto tableName = tablePair.first;
        const auto* uninterpretedTableConfig = tablePair.second->checkedTo<TableConfig>();
        const auto* const tableConfig = uninterpretedTableConfig->evaluate(*completedModel);
        testSpec->addTestObject("tables", tableName, tableConfig);
    }
    // TODO: Move this to target specific test specification.
    const auto actionProfiles = executionState->getTestObjectCategory("action_profile");
    for (const auto& testObject : actionProfiles) {
        const auto profileName = testObject.first;
        const auto* actionProfile = testObject.second->checkedTo<ActionProfile>();
        const auto* evaluatedProfile = actionProfile->evaluate(*completedModel);
        testSpec->addTestObject("action_profiles", profileName, evaluatedProfile);
    }

    // TODO: Move this to target specific test specification.
    const auto actionSelectors = executionState->getTestObjectCategory("action_selector");
    for (const auto& testObject : actionSelectors) {
        const auto selectorName = testObject.first;
        const auto* actionSelector = testObject.second->checkedTo<ActionSelector>();
        const auto* evaluatedSelector = actionSelector->evaluate(*completedModel);
        testSpec->addTestObject("action_selectors", selectorName, evaluatedSelector);
    }
    return testSpec;
}

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia
