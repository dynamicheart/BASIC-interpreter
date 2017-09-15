/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface specified in Chapter 17, which is an
 * excellent model for the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include <string>

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state,int&lineNum,int&controlFlag) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */


/*
 * Class: CommentStat
 * ----------------
 * This class is used to represent a comment statement like
 * "REM <SOMETHING>"in a program.
 */

class CommentStat : public Statement {
public:

/*
 * Constructor: CommentStat
 * ----------------------
 * The class constructor get a string.
 */

	CommentStat(std::string comment);
/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * Do nothing
 */

	void execute(EvalState&state,int&lineNum,int&controlFlag);
private:
	std::string comment;
};

/*
 * Class: AssignmentStat
 * ----------------
 * This class is used to represent a statement like
 * "LET <INDETIFIER> = <EXPRESSION>"in a program.
 */

class AssignmentStat : public Statement {
public:

/*
 * Constructor: AssignmentStat
 * ----------------------
 * The class constructor get a exp.
 */

	AssignmentStat(Expression *exp);

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * Eval the exp
 */

	void execute(EvalState&state,int&lineNum,int&controlFlag);
private:
	Expression *exp;
};

/*
 * Class: PrintStat
 * ----------------
 * This class is used to represent a statement like
 * "PRINT <EXPRESSION>"in a program.
 */

class PrintStat : public Statement {
public:	

/*
 * Constructor: PrintStat
 * ----------------------
 * The class constructor get a exp.
 */

	PrintStat(Expression *exp);

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * Cout the value of Expression
 */

	void execute(EvalState&state,int&lineNum,int&controlFlag);
private:
	Expression *exp;
};

/*
 * Class: InputStat
 * ----------------
 * This class is used to represent a statement like
 * "INPUT <INDENTIFIER>"in a program.
 */

class InputStat : public Statement {
public:

/*
 * Constructor: InputStat
 * ----------------------
 * The class constructor get a string.
 */

	InputStat(string var);

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * Evalue the identifier with a value
 */

	void execute(EvalState&state,int&lineNum,int&controlFlag);
private:
	string var;
};

/*
 * Class: EndStat
 * ----------------
 * This class is used to represent a statement like
 * "END"in a program.
 */

class EndStat : public Statement {
public:

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * The program stops running when it read an END statement.
 */

	void execute(EvalState&state,int&lineNum,int&controlFlag);
};

/*
 * Class: GotoStat
 * ----------------
 * This class is used to represent a statement like
 * "GOTO <NUMBER>"in a program.
 */

class GotoStat : public Statement {
public:

/*
 * Constructor: GotoStat
 * ----------------------
 * The class constructor get a int.
 */

	GotoStat(int lineNumber);

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * The program goes to another line.
 */

	void execute(EvalState&state,int&lineNum,int&controlFlag);
private:
	int lineNumber;
};

/*
 * Class: ConditionStat
 * ----------------
 * This class is used to represent a statement like
 * "IF <exp> <op> <exp> THEN <number>"in a program.
 * The <op> can only be one of "<",">" and "=".
 */

class ConditionStat : public Statement {
public:

/*
 * Constructor: ConditionStat
 * ----------------------
 * The class constructor get two exp and a string.
 */

	ConditionStat(int lineNumber,Expression*exp1,Expression*exp2,string op);
	
/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * If true go to the line.
 */

	void execute(EvalState&state,int&lineNum,int&controlFlag);
private:
	int lineNumber;
	Expression *exp1;
	Expression *exp2;
	string op;

};


#endif
