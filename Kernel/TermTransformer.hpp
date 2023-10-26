/*
 * This file is part of the source code of the software program
 * Vampire. It is protected by applicable
 * copyright laws.
 *
 * This source code is distributed under the licence found here
 * https://vprover.github.io/license.html
 * and in the source directory
 */
/**
 * @file TermTransformer.hpp
 * Defines class TermTransformer.
 */

#ifndef __TermTransformer__
#define __TermTransformer__

#include "Forwards.hpp"



namespace Kernel {

/**
 * Class to allow for easy transformations of subterms in shared literals.
 *
 * The inheriting class implements function transformSubterm(TermList)
 * and then the functions transform(Literal*)/transform(Term*) use it to transform subterms
 * of the given literal/term.
 *
 * The literal and subterms returned by the transformSubterm(TermList) function have
 * to be shared.
 *
 * This class can be used to transform sort arguments as well by suitably
 * implementing the transformSubterm(TermList) function
 *
 * TermTransformer goes top down but does no recurse into the replaced term
 *
 * Note that if called via transform(Term* term) the given term itself will not get transformed, only possibly its proper subterms
 */
class TermTransformer {
public:
  virtual ~TermTransformer() {}
  Term* transform(Term* term);
  Literal* transform(Literal* lit);
protected:
  virtual TermList transformSubterm(TermList trm) = 0;
  Term* transformSpecial(Term* specialTerm);
  virtual TermList transform(TermList ts);
  virtual Formula* transform(Formula* f);
};

/**
 * Has similar philosophy to TermTransformer, but:
 *  goes bottom up and so subterms of currently considered terms
 *  might already be some replacements that happened earlier, e.g.:
 *  transforming g(f(a,b)) will consider (provided transformSubterm is the identity function)
 *  the following sequence: a,b,f(a,b),g(f(a,b)) 
 *  and if transformSubterm is the identitify everywhere except for f(a,b) for which it returns c,
 *  the considered sequence will be: a,b,f(a,b)->c,g(c)
 */
class BottomUpTermTransformer {
public:
  virtual ~BottomUpTermTransformer() {}
  Term* transform(Term* term);
  Literal* transform(Literal* lit);
protected:
  virtual TermList transformSubterm(TermList trm) = 0;
  /**
   * TODO: these functions are similar as in TermTransformer, code duplication could be removed
   */
  TermList transform(TermList ts);
  Formula* transform(Formula* f);
};


}

#endif // __TermTransformer__
