#include "backends/pythia/pythia/targets/bmv2/target.h"

#include <stddef.h>

#include <map>
#include <string>
#include <vector>

#include "ir/ir.h"
#include "lib/cstring.h"
#include "lib/exceptions.h"
#include "lib/ordered_map.h"

#include "backends/pythia/pythia/lib/namespace_context.h"
#include "backends/pythia/pythia/targets/bmv2/constants.h"
#include "backends/pythia/pythia/targets/bmv2/test_backend.h"

namespace Pythia {

namespace P4Pythia {

namespace Bmv2 {

/* =============================================================================================
 *  BMv2_V1ModelPythiaTarget implementation
 * ============================================================================================= */

BMv2_V1ModelPythiaTarget::BMv2_V1ModelPythiaTarget() : PythiaTarget("bmv2", "v1model") {}

void BMv2_V1ModelPythiaTarget::make() {
    static BMv2_V1ModelPythiaTarget* INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new BMv2_V1ModelPythiaTarget();
    }
}

const BMv2_V1ModelProgramInfo* BMv2_V1ModelPythiaTarget::initProgram_impl(
    const IR::P4Program* program, const IR::Declaration_Instance* mainDecl) const {
    // The blocks in the main declaration are just the arguments in the constructor call.
    // Convert mainDecl->arguments into a vector of blocks, represented as constructor-call
    // expressions.
    const auto* ns = NamespaceContext::Empty->push(program);
    std::vector<const IR::Type_Declaration*> blocks;
    argumentsToTypeDeclarations(ns, mainDecl->arguments, blocks);

    // We should have six arguments.
    BUG_CHECK(blocks.size() == 6, "%1%: The BMV2 architecture requires 6 pipes. Received %2%.",
              mainDecl, blocks.size());

    ordered_map<cstring, const IR::Type_Declaration*> programmableBlocks;
    std::map<int, int> declIdToGress;

    // Add to parserDeclIdToGress, mauDeclIdToGress, and deparserDeclIdToGress.
    for (size_t idx = 0; idx < blocks.size(); ++idx) {
        const auto* declType = blocks.at(idx);

        auto canonicalName = archSpec.getArchMember(idx)->blockName;
        programmableBlocks.emplace(canonicalName, declType);

        if (idx < 3) {
            declIdToGress[declType->declid] = BMV2_INGRESS;
        } else {
            declIdToGress[declType->declid] = BMV2_EGRESS;
        }
    }

    return new BMv2_V1ModelProgramInfo(program, programmableBlocks, declIdToGress);
}

Bmv2TestBackend* BMv2_V1ModelPythiaTarget::getTestBackend_impl(
    const ProgramInfo& programInfo, ExplorationStrategy& symbex,
    const boost::filesystem::path& testPath, boost::optional<uint32_t> seed) const {
    return new Bmv2TestBackend(programInfo, symbex, testPath, seed);
}

int BMv2_V1ModelPythiaTarget::getPortNumWidth_bits_impl() const { return 9; }

BMv2_V1ModelCmdStepper* BMv2_V1ModelPythiaTarget::getCmdStepper_impl(
    ExecutionState& state, AbstractSolver& solver, const ProgramInfo& programInfo) const {
    return new BMv2_V1ModelCmdStepper(state, solver, programInfo);
}

BMv2_V1ModelExprStepper* BMv2_V1ModelPythiaTarget::getExprStepper_impl(
    ExecutionState& state, AbstractSolver& solver, const ProgramInfo& programInfo) const {
    return new BMv2_V1ModelExprStepper(state, solver, programInfo);
}

const ArchSpec BMv2_V1ModelPythiaTarget::archSpec =
    ArchSpec("V1Switch", {// parser Parser<H, M>(packet_in b,
                          //                     out H parsedHdr,
                          //                     inout M meta,
                          //                     inout standard_metadata_t standard_metadata);
                          {"Parser", {nullptr, "*hdr", "*meta", "*standard_metadata"}},
                          // control VerifyChecksum<H, M>(inout H hdr,
                          //                              inout M meta);
                          {"VerifyChecksum", {"*hdr", "*meta"}},
                          // control Ingress<H, M>(inout H hdr,
                          //                       inout M meta,
                          //                       inout standard_metadata_t standard_metadata);
                          {"Ingress", {"*hdr", "*meta", "*standard_metadata"}},
                          // control Egress<H, M>(inout H hdr,
                          //            inout M meta,
                          //            inout standard_metadata_t standard_metadata);
                          {"Egress", {"*hdr", "*meta", "*standard_metadata"}},
                          // control ComputeChecksum<H, M>(inout H hdr,
                          //                       inout M meta);
                          {"ComputeChecksum", {"*hdr", "*meta"}},
                          // control Deparser<H>(packet_out b, in H hdr);
                          {"Deparser", {nullptr, "*hdr"}}});

const ArchSpec* BMv2_V1ModelPythiaTarget::getArchSpecImpl() const { return &archSpec; }

}  // namespace Bmv2

}  // namespace P4Pythia

}  // namespace Pythia
