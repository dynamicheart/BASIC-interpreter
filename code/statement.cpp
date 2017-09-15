/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include <iostream>
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"
#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
#include "exp.h"
#include "parser.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

CommentStat::CommentStat(string comment){
	this ->comment = comment;
}

void CommentStat::execute(EvalState&state,int&lineNum,int&controlFlag){
	/* Empty */
}

AssignmentStat::AssignmentStat(Expression *exp){
	this ->exp = exp;
}

void AssignmentStat::execute(EvalState&state,int&lineNum,int&controlFlag){
	exp ->eval(state);
}

PrintStat::PrintStat(Expression *exp){
	this -> exp = exp;
}

void PrintStat::execute(EvalState&state,int&lineNum,int&controlFlag){
	cout<<exp->eval(state)<<endl;
}

InputStat::InputStat(string var){
	this -> var = var;
}

void InputStat::execute(EvalState&state,int&lineNum,int&controlFlag){
    string token;
    bool sign = true;
    int value = 0;
    while(true){
    	cout<<" ? ";
    	TokenScanner inputScanner;
    	inputScanner.ignoreWhitespace();
    	inputScanner.scanNumbers();
    	inputScanner.setInput(getLine());
    	token = inputScanner.nextToken();
    	if(inputScanner.getTokenType(token) == NUMBER && !inputScanner.hasMoreTokens()){
    		try{
    			stringToInteger(token);
    		}
    		catch(ErrorException&ex){
    			cout<<"INVALID NUMBER"<<endl;
    			continue;
    		}
    		break;
    	}
    	else if(token == "-"){
    		token = inputScanner.nextToken();
    		if(inputScanner.getTokenType(token) == NUMBER && !inputScanner.hasMoreTokens()){
    			try{
    				stringToInteger(token);
    			}
    			catch(ErrorException&ex){
    				cout<<"INVALID NUMBER"<<endl;
    				continue;
    			}
    			sign = false;
    			break;
    		}
    	}
    	else cout<<"INVALID NUMBER"<<endl;
    }
    if(sign == true){
    	value = stringToInteger(token);
    }
    else{
    	value = -stringToInteger(token);
    }

    state.setValue(var,value);
}

void EndStat::execute(EvalState&state,int&lineNum,int&controlFlag){
	controlFlag = -1;
}

GotoStat::GotoStat(int lineNumber){
	this -> lineNumber = lineNumber;
}

void GotoStat::execute(EvalState&state,int&lineNum,int&controlFlag){
	lineNum = lineNumber;
	controlFlag = 1;
}

ConditionStat::ConditionStat(int lineNumber,Expression*exp1,Expression*exp2,string op){
	this -> lineNumber = lineNumber;
    this -> exp1 = exp1;
    this -> exp2 = exp2;
    this -> op = op;
}

void ConditionStat::execute(EvalState&state,int&lineNum,int&controlFlag){
	if(op == "="&&((exp1->eval(state)) == (exp2 -> eval(state)))){
		lineNum = lineNumber;
		controlFlag = 1;
	}
    else if(op == "<"&&((exp1->eval(state)) < (exp2 -> eval(state)))){
        lineNum = lineNumber;
        controlFlag = 1;
    }
    else if(op == ">"&&((exp1->eval(state)) > (exp2 -> eval(state)))){
        lineNum = lineNumber;
        controlFlag = 1;
    }
}