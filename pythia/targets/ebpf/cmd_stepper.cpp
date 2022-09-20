#include "backends/pythia/pythia/targets/ebpf/cmd_stepper.h"

#include <stddef.h>

#include <list>
#include <map>
#include <utility>

#include <boost/none.hpp>

#include "backends/pythia/common/core/solver.h"
#include "backends/pythia/common/lib/formulae.h"
#include "backends/pythia/common/lib/ir.h"
#include "ir/ir.h"
#include "lib/error.h"
#include "lib/exceptions.h"
#include "lib/ordered_map.h"

#include "backends/pythia/pythia/core/target.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/targets/ebpf/constants.h"
#include "backends/pythia/pythia/targets/ebpf/program_info.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

EBPFCmdStepper::EBPFCmdStepper(ExecutionState& state, AbstractSolver& solver,
                               const ProgramInfo& programInfo)
    : CmdStepper(state, solver, programInfo) {}

const EBPFProgramInfo& EBPFCmdStepper::getProgramInfo() const {
    return CmdStepper::getProgramInfo().to<EBPFProgramInfo>();
}

void EBPFCmdStepper::initializeTargetEnvironment(ExecutionState* nextState) const {
    auto programInfo = getProgramInfo();
    const auto* archSpec = PythiaTarget::getArchSpec();
    const auto* programmableBlocks = programInfo.getProgrammableBlocks();

    // eBPF initializes all metadata to zero. To avoid unnecessary taint, we retrieve the type and
    // initialize all the relevant metadata variables to zero.
    size_t blockIdx = 0;
    for (const auto& blockTuple : *programmableBlocks) {
        const auto* typeDecl = blockTuple.second;
        const auto* archMember = archSpec->getArchMember(blockIdx);
        initializeBlockParams(typeDecl, &archMember->blockParams, nextState);
        blockIdx++;
    }

    const auto* nineBitType = IRUtils::getBitType(9);
    // Set the input ingress port to 0.
    nextState->set(programInfo.getTargetInputPortVar(), IRUtils::getConstant(nineBitType, 0));
    // eBPF implicitly sets the output port to 0.
    nextState->set(programInfo.getTargetOutputPortVar(), IRUtils::getConstant(nineBitType, 0));
}

boost::optional<const Constraint*> EBPFCmdStepper::startParser_impl(
    const IR::P4Parser* /*parser*/, ExecutionState* /*nextState*/) const {
    return boost::none;
}

std::map<Continuation::Exception, Continuation> EBPFCmdStepper::getExceptionHandlers(
    const IR::P4Parser* /*parser*/, Continuation::Body /*normalContinuation*/,
    const ExecutionState* /*nextState*/) const {
    std::map<Continuation::Exception, Continuation> result;
    auto programInfo = getProgramInfo();

    result.emplace(Continuation::Exception::Reject, Continuation::Body({}));
    result.emplace(Continuation::Exception::PacketTooShort, Continuation::Body({}));
    // NoMatch is equivalent to Reject in ebpf_model.
    result.emplace(Continuation::Exception::NoMatch, Continuation::Body({}));

    return result;
}

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia
