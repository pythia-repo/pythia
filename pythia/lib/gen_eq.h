#ifndef PYTHIA_LIB_GEN_EQ_H_
#define PYTHIA_LIB_GEN_EQ_H_

#include "ir/ir.h"

namespace Pythia {

namespace P4Pythia {

/// Generates an equality on a target expression and a keyset expression, recursing into lists.
/// This supports fuzzy matching on singleton lists: singleton lists are considered the same as
/// their singleton elements. This is implemented by eagerly recursing into singleton lists before
/// attempting to generate the equality.
class GenEq {
 public:
    static const IR::Expression* equate(const IR::Expression* target, const IR::Expression* keyset);

    static const IR::Expression* equate(const IR::Expression* target,
                                        const IR::DefaultExpression* keyset);

    static const IR::Expression* equate(const IR::Expression* target,
                                        const IR::ListExpression* keyset);

    static const IR::Expression* equate(const IR::Expression* target, const IR::Mask* keyset);

    static const IR::Expression* equate(const IR::Expression* target, const IR::Range* keyset);

    static const IR::Expression* equate(const IR::ListExpression* target,
                                        const IR::ListExpression* keyset);

 private:
    /// Convenience method for producing a typed Eq node on the given expressions.
    static const IR::Equ* mkEq(const IR::Expression* e1, const IR::Expression* e2);
};

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_LIB_GEN_EQ_H_ */
