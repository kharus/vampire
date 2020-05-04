#include "RebalancingElimination.hpp"
#include "Kernel/Rebalancing.hpp"
#include "Kernel/Rebalancing/Inverters.hpp"
#include "Kernel/Clause.hpp"
#include "Kernel/EqHelper.hpp"
#include "Kernel/InterpretedLiteralEvaluator.hpp"
#include "Inferences/InterpretedEvaluation.hpp"

#define DEBUG(...) // DBG(__VA_ARGS__)

namespace Inferences {
  using Balancer = Kernel::Rebalancing::Balancer<Kernel::Rebalancing::Inverters::NumberTheoryInverter>;

Clause* RebalancingElimination::simplify(Clause* in) 
{
  CALL("RebalancingElimination::simplify")
  ASS(in)
  Clause* out = in;
  
  auto performStep = [&](Clause& cl) -> Clause& {

    for(int i = 0; i < cl.size(); i++) {
      auto& lit = *cl[i];
      if (lit.isEquality() && lit.isNegative()) { 
        for (auto b : Balancer(lit)) {

          /* found a rebalancing: lhs = rhs[lhs, ...] */
          auto lhs = b.lhs();
          auto rhs = b.buildRhs();
          ASS_REP(lhs.isVar(), lhs);

          if (!rhs.containsSubterm(lhs)) {
            /* lhs = rhs[...] */
            DEBUG(lhs, " -> ", rhs);

            return *rewrite(cl, lhs, rhs, i);
          }
        }
      }
    }
    return cl;

  };

  while(true) {
    Clause* step = &performStep(*out);
    if (step == out) 
      break;
    else 
      out = step;
  }


  // static InterpretedEvaluation ev = InterpretedEvaluation();
  // return ev.simplify(out);
  return out;
}

Clause* RebalancingElimination::rewrite(const Clause& cl, TermList find, TermList replace, unsigned skipLiteral) const 
{
  CALL("RebalancingElimination::rewrite")
  static Inference inf = Inference(Kernel::Inference::Rule::REBALANCING_ELIMINIATION);

  auto sz = cl.size() - 1;
  Clause& out = *new(sz) Clause(sz, cl.inputType(), &inf); 
  for (int i = 0; i < skipLiteral; i++) {
    out[i] = EqHelper::replace(cl[i], find, replace);
  }

  for (int i = skipLiteral; i < sz; i++)  {
    out[i] = EqHelper::replace(cl[i+1], find, replace);
  }

  // for (int i = 0; i < sz; i++) {
  //   Lit
  // }
  
  return &out;
}

} // namespace Inferences 
