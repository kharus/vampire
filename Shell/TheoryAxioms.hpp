/**
 * @file TheoryAxioms.hpp
 * Defines class TheoryAxioms.
 */

#ifndef __TheoryAxioms__
#define __TheoryAxioms__

#include "Forwards.hpp"

namespace Shell {

using namespace Lib;
using namespace Kernel;

class TheoryAxioms {
public:

  struct Arithmetic;

  void apply(UnitList*& units, Property* prop);

private:
  Unit* replaceFunctions(Unit* u);
  Formula* replaceFunctions(Formula* f);
  FormulaList* replaceFunctions(FormulaList* fs);
  Literal* replaceFunctions(Literal* l);
};

}

#endif // __TheoryAxioms__
