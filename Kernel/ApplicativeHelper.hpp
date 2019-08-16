/**
 * @file ApplicativeHelper.hpp
 * Defines class ApplicativeHelper.
 */

#ifndef __ApplicativeHelper__
#define __ApplicativeHelper__

#include "Forwards.hpp"
#include "Signature.hpp"
#include "Lib/Deque.hpp"

using namespace Kernel;
using namespace Shell;

/**
 * A class with function @b elimLambda() that eliminates a lambda expressions
 * It does this by applying the well known rewrite rules for SKIBC combinators.
 *
 * These can be found:
 * https://en.wikipedia.org/wiki/Combinatory_logic
 */
class ApplicativeHelper {
public:


  ApplicativeHelper() {};

  static TermList createAppTerm(TermList sort, TermList arg1, TermList arg2);
  static TermList createAppTerm(TermList s1, TermList s2, TermList arg1, TermList arg2);
  static TermList createAppTerm3(TermList sort, TermList arg1, TermList arg2, TermList arg3);
  static TermList createAppTerm(TermList sort, TermList arg1, TermList arg2, TermList arg3, TermList arg4); 
  static TermList createAppTerm(TermList sort, TermList head, TermStack& terms); 
  static TermList getNthArg(TermList arrowSort, unsigned argNum);
  static TermList getResultApplieadToNArgs(TermList arrowSort, unsigned argNum);
  static TermList getResultSort(TermList sort);
  static void getHeadAndAllArgs(TermList term, TermList& head, TermStack& args); 
  static void getHeadAndArgs(TermList term, TermList& head, TermStack& args); 
  static void getHeadAndArgs(Term* term, TermList& head, TermStack& args);  
  static void getHeadAndArgs(const Term* term, TermList& head, Deque<TermList>& args); 
  static bool isComb(TermList t);
  static Signature::Combinator getComb(TermList t);
  static TermList getHead(TermList t);
  static bool isApp(const Term* t);
  static bool isArrowType(const Term* t);
  static bool isApp(const TermList* tl);
  static bool isUnderApplied(TermList head, unsigned argNum);
  static bool isExactApplied(TermList head, unsigned argNum);
  static bool isOverApplied(TermList head, unsigned argNum);

private:
  
};

#endif // __ApplicativeHelper__