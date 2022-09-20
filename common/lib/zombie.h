#ifndef COMMON_LIB_ZOMBIE_H_
#define COMMON_LIB_ZOMBIE_H_

#include "backends/pythia/common/lib/formulae.h"
#include "ir/ir.h"
#include "lib/cstring.h"

namespace Pythia {

/// Zombies are variables internal to Pythia. They are variables that do not exist in the P4
/// program itself, but are generated and added to the environment by the Pythia tooling. These
/// variables are also used for SMT solvers as symbolic variables.
class Zombie {
 private:
    /// The name of the top-level struct containing all zombie state.
    static const cstring P4tZombie;

    /// The name of the struct below P4tZombie that contains all symbolic constants.
    static const cstring Const;

 public:
    /// Determines whether the given member expression represents a symbolic constant. Symbolic
    /// constants are references to fields under the nested struct p4t*zombie.const.
    static bool isSymbolicConst(const IR::Member*);

    /// @returns the zombie variable with the given @type, @incarnation, and @name.
    ///
    /// A BUG occurs if this was previously called with the same @name and @incarnation, but with a
    /// different @type.
    static const StateVariable& getVar(const IR::Type* type, int incarnation, cstring name);

    /// @returns the zombie symbolic constant with the given @type, @incarnation, and @name.
    ///
    /// A BUG occurs if this was previously called with the same @name and @incarnation, but with a
    /// different @type.
    static const StateVariable& getConst(const IR::Type* type, int incarnation, cstring name);

 private:
    /// @see getVar and getConst.
    static const StateVariable& getZombie(const IR::Type* type, bool isConst, int incarnation,
                                          cstring name);

    static const StateVariable* mkZombie(const IR::Type* type, bool isConst, int incarnation,
                                         cstring name);
};

}  // namespace Pythia

#endif /* COMMON_LIB_ZOMBIE_H_ */
