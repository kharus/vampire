#include "Kernel/Clause.hpp"
#include "Kernel/SortHelper.hpp"
#include "Kernel/Term.hpp"

#include "Shell/Statistics.hpp"
#include "Shell/TheoryFinder.hpp"

#include "Saturation/ExtensionalityClauseContainer.hpp"

namespace Saturation
{

using namespace Shell;

/**
 * Check if clause is considered as an extensionality clause (according to
 * options). If so, track in extensionality container for extensionality
 * resolution inferences.
 *
 * Common to all extensionality clauses is a single positive variable equality
 * X=Y, which is returned in case of a positive match, 0 otherwise.
 */
Literal* ExtensionalityClauseContainer::addIfExtensionality(Clause* c) {
  // Clause is already in extensionality container. We only have to search X=Y.
  if (c->isExtensionality()) {
    return getSingleVarEq(c);
  }

  // We only consider extensionality clauses of at least length 2, but can also
  // specify a length limit.
  unsigned clen = c->length();
  if (clen < 2 || (_maxLen > 1 && clen > _maxLen))
    return 0;

  Literal* varEq = 0;
  unsigned sort;

  if (_onlyKnown) {
    // We only match agains specific extensionality axiom patterns (e.g. set,
    // array, ...).
    if(!TheoryFinder::matchKnownExtensionality(c))
      return 0;

    // We know that the patterns only have a single X=Y.
    varEq = getSingleVarEq(c);
    sort = varEq->twoVarEqSort();
  } else {
    // Generic filter for extensionality clauses.
    //   * Exactly one X=Y
    //   * No inequality of same sort as X=Y
    //   * No equality except X=Y (optional).
    static DArray<bool> negEqSorts(_sortCnt);
    negEqSorts.init(_sortCnt, false);
  
    for (Clause::Iterator ci(*c); ci.hasNext(); ) {
      Literal* l = ci.next();

      if (l->isTwoVarEquality() && l->isPositive()) {
        if (varEq != 0)
          return 0;

        sort = l->twoVarEqSort();
        if (negEqSorts[sort])
          return 0;

        varEq = l;
      } else if (l->isEquality()) {
        if (!_allowPosEq && l->isPositive())
          return 0;
      
        unsigned negEqSort = SortHelper::getEqualityArgumentSort(l);
        if (varEq == 0)
          negEqSorts[negEqSort] = true;
        else if (sort == negEqSort)
          return 0;
      }
    }
  }

  if (varEq != 0) {
    c->setExtensionality(true);
    add(ExtensionalityClause(c, varEq, sort));
    _size++;
    env.statistics->extensionalityClauses++;
    LOG_UNIT("sa_ext_added", c);
    return varEq;
  }

  return 0;
}

Literal* ExtensionalityClauseContainer::getSingleVarEq(Clause* c) {
  for (unsigned i = 0; i < c->length(); ++i) {
    Literal* varEq = (*c)[i];
    if (varEq->isTwoVarEquality() && varEq->isPositive()) {
      return varEq;
      break;
    }
  }
  ASSERTION_VIOLATION;
}

void ExtensionalityClauseContainer::add(ExtensionalityClause c) {
  ExtensionalityClauseList::push(c, _clausesBySort[c.sort]);
}

struct ExtensionalityClauseContainer::ActiveFilterFn
{
  ActiveFilterFn(ExtensionalityClauseContainer& parent) : _parent(parent) {}
  DECL_RETURN_TYPE(bool);
  bool operator()(ExtensionalityClause extCl)
  {
    if (extCl.clause->store() != Clause::ACTIVE) {
      extCl.clause->setExtensionality(false);
      _parent._size--;
      LOG_UNIT("sa_ext_removed", extCl.clause);
      return false;
    }
    return true;
  }
private:
  ExtensionalityClauseContainer& _parent;
};

ExtensionalityClauseIterator ExtensionalityClauseContainer::activeIterator(unsigned sort) {
  return pvi(getFilteredDelIterator(
               ExtensionalityClauseList::DelIterator(_clausesBySort[sort]),
               ActiveFilterFn(*this)));
}

void ExtensionalityClauseContainer::print (ostream& out) {
  out << "#####################" << endl;

  for(size_t i = 0; i < _clausesBySort.size(); ++i) {
    ExtensionalityClauseList::Iterator it(_clausesBySort[i]);
    while(it.hasNext()) {
      ExtensionalityClause c = it.next();
      out << c.clause->toString() << endl
          << c.literal->toString() << endl
          << c.sort << endl;
    }
  }
  
  out << "#####################" << endl;
}

}
