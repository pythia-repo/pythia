#include "backends/pythia/pythia/targets/ebpf/expr_stepper.h"

#include <cstddef>
#include <functional>
#include <ostream>
#include <vector>

#include <boost/multiprecision/number.hpp>

#include "backends/pythia/common/core/solver.h"
#include "backends/pythia/common/lib/formulae.h"
#include "backends/pythia/common/lib/ir.h"
#include "backends/pythia/common/lib/symbolic_env.h"
#include "backends/pythia/common/lib/trace_events.h"
#include "lib/cstring.h"
#include "lib/error.h"
#include "lib/exceptions.h"
#include "lib/gmputil.h"
#include "lib/ordered_map.h"
#include "lib/safe_vector.h"

#include "backends/pythia/pythia/core/externs.h"
#include "backends/pythia/pythia/core/small_step/small_step.h"
#include "backends/pythia/pythia/lib/continuation.h"
#include "backends/pythia/pythia/lib/exceptions.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/targets/ebpf/table_stepper.h"
#include "backends/pythia/pythia/targets/ebpf/target.h"
#include "backends/pythia/pythia/targets/ebpf/test_spec.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

EBPFExprStepper::EBPFExprStepper(ExecutionState& state, AbstractSolver& solver,
                                 const ProgramInfo& programInfo)
    : ExprStepper(state, solver, programInfo) {}

void EBPFExprStepper::evalExternMethodCall(const IR::MethodCallExpression* call,
                                           const IR::Expression* receiver, IR::ID name,
                                           const IR::Vector<IR::Argument>* args,
                                           ExecutionState& state) {
    // Provides implementations of eBPF externs.
    static const ExternMethodImpls EXTERN_METHOD_IMPLS({
        /* ======================================================================================
         *  emit_ebpf_hdrs
         *  Emit the eBPF headers after filtering.
         * ====================================================================================== */
        {"*.emit_ebpf_hdrs",
         {},
         [this](const IR::MethodCallExpression* /*call*/, const IR::Expression* /*receiver*/,
                IR::ID& /*methodName*/, const IR::Vector<IR::Argument>* args,
                const ExecutionState& state, SmallStepEvaluator::Result& result) {
             auto* nextState = new ExecutionState(state);

             auto progInfo = getProgramInfo().to<EBPFProgramInfo>();
             const auto* archSpec = PythiaTarget::getArchSpec();
             const auto* programmableBlocks = progInfo.getProgrammableBlocks();
             // Just grab the parser from the programmable blocks.
             const auto* typeDecl = programmableBlocks->at("parse");
             const auto* archMember = archSpec->getArchMember(0);
             const auto* blockParams = &archMember->blockParams;
             const auto* p4parser = typeDecl->checkedTo<IR::P4Parser>();
             const auto* params = p4parser->getApplyParameters();
             // The user metadata is typically the second parameter.
             const auto* param = params->getParameter(1);
             const auto* paramType = param->type;
             auto archRef = blockParams->at(1);
             // We need to resolve type names.
             if (const auto* tn = paramType->to<IR::Type_Name>()) {
                 paramType = nextState->resolveType(tn);
             }
             const auto* paramPath = new IR::PathExpression(paramType, new IR::Path(archRef));
             std::vector<Continuation::Command> cmds;
             const auto* ts = paramType->checkedTo<IR::Type_StructLike>();

             for (const auto* field : ts->fields) {
                 const auto* fieldType = field->type->getP4Type();
                 if (const auto* tn = fieldType->to<IR::Type_Name>()) {
                     fieldType = nextState->resolveType(tn);
                 }
                 auto* fieldLabel = new IR::Member(fieldType, paramPath, field->name);
                 auto* args = new IR::Vector<IR::Argument>();
                 args->push_back(new IR::Argument(fieldLabel));
                 auto* paramList = new IR::ParameterList();
                 paramList->parameters.push_back(
                     new IR::Parameter(IR::ID("hdr"), IR::Direction::In, new IR::Type_Name("T")));
                 auto* callExpr = new IR::MethodCallExpression(
                     IR::Type_Void::get(),
                     new IR::Member(new IR::Type_Method(paramList, "emit"),
                                    new IR::PathExpression(new IR::Type_Extern("packet_out"),
                                                           new IR::Path("packet_out")),
                                    "emit"),
                     args);
                 // callExpr->typeArguments->push_back(fieldType);
                 auto* call = new IR::MethodCallStatement(callExpr);
                 cmds.emplace_back(call);
             }
             nextState->replaceTopBody(&cmds);
             result->emplace_back(nextState);
         }},
    });

    if (!EXTERN_METHOD_IMPLS.exec(call, receiver, name, args, state, result)) {
        ExprStepper::evalExternMethodCall(call, receiver, name, args, state);
    }
}  // NOLINT

bool EBPFExprStepper::preorder(const IR::P4Table* table) {
    // Delegate to the tableStepper.
    EBPFTableStepper tableStepper(this, table);

    return tableStepper.eval();
}

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia
