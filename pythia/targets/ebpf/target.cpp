#include "backends/pythia/pythia/targets/ebpf/target.h"

#include <stddef.h>

#include <map>
#include <string>
#include <vector>

#include "ir/ir.h"
#include "lib/cstring.h"
#include "lib/exceptions.h"
#include "lib/ordered_map.h"

#include "backends/pythia/pythia/lib/namespace_context.h"
#include "backends/pythia/pythia/targets/ebpf/constants.h"
#include "backends/pythia/pythia/targets/ebpf/test_backend.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

/* =============================================================================================
 *  EBPFPythiaTarget implementation
 ============================================================================================= */

EBPFPythiaTarget::EBPFPythiaTarget() : PythiaTarget("ebpf", "ebpf") {}

void EBPFPythiaTarget::make() {
    static EBPFPythiaTarget* INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new EBPFPythiaTarget();
    }
}

const EBPFProgramInfo* EBPFPythiaTarget::initProgram_impl(
    const IR::P4Program* program, const IR::Declaration_Instance* mainDecl) const {
    // The blocks in the main declaration are just the arguments in the constructor call.
    // Convert mainDecl->arguments into a vector of blocks, represented as constructor-call
    // expressions.
    const auto* ns = NamespaceContext::Empty->push(program);
    std::vector<const IR::Type_Declaration*> blocks;
    argumentsToTypeDeclarations(ns, mainDecl->arguments, blocks);

    // We should have six arguments.
    BUG_CHECK(blocks.size() == 2, "%1%: The EBPF architecture requires 2 blocks. Received %2%.",
              mainDecl, blocks.size());

    ordered_map<cstring, const IR::Type_Declaration*> programmableBlocks;
    for (size_t idx = 0; idx < blocks.size(); ++idx) {
        const auto* declType = blocks.at(idx);
        auto canonicalName = archSpec.getArchMember(idx)->blockName;
        programmableBlocks.emplace(canonicalName, declType);
    }

    return new EBPFProgramInfo(program, programmableBlocks);
}

EBPFTestBackend* EBPFPythiaTarget::getTestBackend_impl(const ProgramInfo& programInfo,
                                                        ExplorationStrategy& symbex,
                                                        const boost::filesystem::path& testPath,
                                                        boost::optional<uint32_t> seed) const {
    return new EBPFTestBackend(programInfo, symbex, testPath, seed);
}

int EBPFPythiaTarget::getPortNumWidth_bits_impl() const { return 9; }

EBPFCmdStepper* EBPFPythiaTarget::getCmdStepper_impl(ExecutionState& state, AbstractSolver& solver,
                                                      const ProgramInfo& programInfo) const {
    return new EBPFCmdStepper(state, solver, programInfo);
}

EBPFExprStepper* EBPFPythiaTarget::getExprStepper_impl(ExecutionState& state,
                                                        AbstractSolver& solver,
                                                        const ProgramInfo& programInfo) const {
    return new EBPFExprStepper(state, solver, programInfo);
}

const ArchSpec EBPFPythiaTarget::archSpec =
    ArchSpec("ebpfFilter", {// parser parse<H>(packet_in packet, out H headers);
                            {"parse", {nullptr, "*hdr"}},
                            // control filter<H>(inout H headers, out bool accept);
                            {"filter", {"*hdr", "*accept"}}});

const ArchSpec* EBPFPythiaTarget::getArchSpecImpl() const { return &archSpec; }

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia
