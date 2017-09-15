/*
 * File: Basic.cpp
 * ---------------
 * Name: Yang Jianbang
 * Student ID: 515030910223
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * This program is a BASIC language interpreter intempleted by c++
 * and can be runned in command line.
 * 
 * Syntax:
 *
 *--------------------------------------------------------------------------------------
 *                                  SEQUENTIAL STATEMENTS
 *--------------------------------------------------------------------------------------
 * REM         This statement is used for comments.
 *--------------------------------------------------------------------------------------
 * LET         var = exp This statement is BASIC’s assignment statement.
 *--------------------------------------------------------------------------------------
 * PRINT exp   This statement print the value of the expression on the console and then
 *             print a newline character.
 *--------------------------------------------------------------------------------------
 * INPUT var   This statement print a prompt consisting of the string " ? " and then to
 *             read in a value to be stored in the variable.
 *--------------------------------------------------------------------------------------
 * END         Marks the end of the program. Execution halts when this line is reached.
 *             Execution also stops if the program continues past the last numbered line.
 *--------------------------------------------------------------------------------------
 *
 *
 *
 *--------------------------------------------------------------------------------------
 *                                  CONTROL STATEMENTS
 *--------------------------------------------------------------------------------------
 * GOTO n                    This statement transfers control unconditionally to line n 
 *                           in the program. If line n does not exist, your BASIC 
 *                           interpreter should generate an error message informing the
 *                           user of that fact.
 *--------------------------------------------------------------------------------------
 * IF exp cmp exp THEN n     var = exp This statement is BASIC’s assignment statement.
 *--------------------------------------------------------------------------------------
 *
 *
 *
 *--------------------------------------------------------------------------------------
 *                                  CONTROL COMMANDS
 *--------------------------------------------------------------------------------------
 * RUN        This command starts program execution beginning at the lowest-numbered line.
 *--------------------------------------------------------------------------------------
 * LIST       This command lists the steps in the program in numerical sequence.
 *--------------------------------------------------------------------------------------
 * CLEAR      This command deletes all program and variables.
 *--------------------------------------------------------------------------------------
 * QUIT       This command exits from the BASIC interpreter by calling exit(0).
 *--------------------------------------------------------------------------------------
 * HELP       This command provides a simple help message describing your interpreter.
 *--------------------------------------------------------------------------------------
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include <stdlib.h>
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } 
      catch (ErrorException & ex) {
         cout <<ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);

   string token = scanner.nextToken();
   TokenType type = scanner.getTokenType(token);
   
   int controlFlag = 0;
   int lineNum = -1;

   if(token == ""){
      return;
   }
   
   else if(type == NUMBER){
      int lineNum = stringToInteger(token);

      if(!scanner.hasMoreTokens()){
         program.addSourceLine(lineNum,line);
         program.setParsedStatement(lineNum,NULL);
         return;
      }
      
      token = scanner.nextToken();
      type = scanner.getTokenType(token);
      
      if(token == "REM"){
         program.addSourceLine(lineNum,line);
         CommentStat *stat = new CommentStat(line);
         program.setParsedStatement(lineNum,stat);
      }

      else if(token == "LET"){
         TokenScanner checkScanner;
         checkScanner.ignoreWhitespace();
         checkScanner.scanNumbers();
         checkScanner.setInput(line);
         string checkToken = checkScanner.nextToken();
         bool haveEqual = false;
         while(checkToken != ""){
            if(checkToken == "=") haveEqual = true;
            checkToken = checkScanner.nextToken();
         }
         if(!haveEqual){
            error("SYNTAX ERROR");
            return;
         }
         Expression *exp = parseExp(scanner);
         AssignmentStat *stat = new AssignmentStat(exp);
         program.addSourceLine(lineNum,line);
         program.setParsedStatement(lineNum,stat);
      }

      else if(token == "PRINT"){
         TokenScanner checkScanner;
         checkScanner.ignoreWhitespace();
         checkScanner.scanNumbers();
         checkScanner.setInput(line);
         string checkToken = checkScanner.nextToken();
         bool haveEqual = false;
         while(checkToken != ""){
            if(checkToken == "=") haveEqual = true;
            checkToken = checkScanner.nextToken();
         }
         if(haveEqual){
            error("SYNTAX ERROR");
            return;
         }

         Expression *exp = parseExp(scanner);
         PrintStat *stat = new PrintStat(exp);
         program.addSourceLine(lineNum,line);
         program.setParsedStatement(lineNum,stat);
      }

      else if(token == "INPUT"){
         string var = scanner.nextToken();
         if(scanner.getTokenType(var) != WORD || scanner.hasMoreTokens()) error("SYNTAX ERROR");
         program.addSourceLine(lineNum,line);
         InputStat *stat = new InputStat(var);
         program.setParsedStatement(lineNum,stat);
      }

      else if(token == "END"){
         if(scanner.hasMoreTokens())error("SYNTAX ERROR");
         program.addSourceLine(lineNum,line);
         EndStat *stat = new EndStat();
         program.setParsedStatement(lineNum,stat);
      }

      else if(token == "GOTO"){
         token = scanner.nextToken();
         if(scanner.getTokenType(token) != NUMBER || scanner.hasMoreTokens())error("SYNTAX ERROR");
         program.addSourceLine(lineNum,line);
         int goToLine = stringToInteger(token);

         GotoStat *stat = new GotoStat(goToLine);
         program.setParsedStatement(lineNum,stat);
      }

      else if(token == "IF"){
         Expression *exp1 = readE(scanner,0);
         Expression *exp2;
         string op = scanner.nextToken();
         if(op == "THEN"){
            scanner.saveToken(op);
            if(exp1->getType() != COMPOUND)error("SYNTAX ERROR");
            Expression *exp = exp1;
            if(exp -> getOp() != "=") error("SYNTAX ERROR");
            exp1 = exp ->getLHS();
            exp2 = exp ->getRHS();
            op = "=";
         }

         else{
            if(op != "<" && op != ">") error("SYNTAX ERROR");
            exp2 = readE(scanner,0);
         }
         token = scanner.nextToken();
         if(token != "THEN") error("SYNTAX ERROR");
         token = scanner.nextToken();
         if(scanner.getTokenType(token)!= NUMBER||scanner.hasMoreTokens()) error("SYNTAX ERROR");
         
         int goToLine = stringToInteger(token);

         program.addSourceLine(lineNum,line);
         ConditionStat *stat = new ConditionStat(goToLine,exp1,exp2,op);
         program.setParsedStatement(lineNum,stat);
      }

      else error("SYNTAX ERROR");
   }

   else if(type == WORD){
      if(token == "REM"){
         return;
      }
      
      else if(token == "LET"){
         TokenScanner checkScanner;
         checkScanner.ignoreWhitespace();
         checkScanner.scanNumbers();
         checkScanner.setInput(line);
         string checkToken = checkScanner.nextToken();
         bool haveEqual = false;
         while(checkToken != ""){
            if(checkToken == "=") haveEqual = true;
            checkToken = checkScanner.nextToken();
         }
         if(!haveEqual){
            error("SYNTAX ERROR");
            return;
         }

         Expression *exp = parseExp(scanner);
         AssignmentStat *stat = new AssignmentStat(exp);
         stat ->execute(state,lineNum,controlFlag);
         delete exp;
         delete stat;
      }

      else if(token == "PRINT"){
         TokenScanner checkScanner;
         checkScanner.ignoreWhitespace();
         checkScanner.scanNumbers();
         checkScanner.setInput(line);
         string checkToken = checkScanner.nextToken();
         bool haveEqual = false;
         while(checkToken != ""){
            if(checkToken == "=") haveEqual = true;
            checkToken = checkScanner.nextToken();
         }
         if(haveEqual){
            error("SYNTAX ERROR");
            return;
         }

         Expression *exp = parseExp(scanner);
         PrintStat *stat = new PrintStat(exp);
         stat ->execute(state,lineNum,controlFlag);
         delete exp;
         delete stat;
      }

      else if(token == "INPUT"){
         string var = scanner.nextToken();
         if(scanner.getTokenType(var) != WORD || scanner.hasMoreTokens()) error("SYNTAX ERROR");
         InputStat *stat = new InputStat(var);
         stat ->execute(state,lineNum,controlFlag);
         delete stat;
      }

      else if(token == "RUN"){
         if(scanner.hasMoreTokens())error("SYNTAX ERROR");
         lineNum = program.getFirstLineNumber();
         if(lineNum == -1) return;
         while(lineNum != -1){
            if(program.getParsedStatement(lineNum) != NULL){
               program.getParsedStatement(lineNum)->execute(state,lineNum,controlFlag);
            }

            if(controlFlag == -1) break;

            if(controlFlag == 0){ 
               lineNum = program.getNextLineNumber(lineNum);
            }
            else{
               if(program.getSourceLine(lineNum) == ""){
                  error("LINE NUMBER ERROR");
               }
               controlFlag = 0;
            }
         }
      }

      else if(token == "LIST"){
         if(scanner.hasMoreTokens()){
            error("SYNTAX ERROR");
         }
         lineNum = program.getFirstLineNumber();
         if(lineNum == -1) return;
         while(lineNum != -1){
            if(program.getParsedStatement(lineNum) != NULL){
               cout<<program.getSourceLine(lineNum)<<endl;
            }
            lineNum = program.getNextLineNumber(lineNum);
         }
      }

      else if(token == "CLEAR"){
         if(scanner.hasMoreTokens())error("SYNTAX ERROR");
         state.clear();
         program.clear();
      }

      else if(token == "QUIT"){
         if(scanner.hasMoreTokens())error("SYNTAX ERROR");
         exit(0);

      }

      else if(token == "HELP"){
         if(scanner.hasMoreTokens())error("SYNTAX ERROR");
         cout<<"Yet another basic interpreter"<<endl;
      }
      else error("SYNTAX ERROR");
   }
   else error("SYNTAX ERROR");
}