/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

/*
 * Implementation notes: the Program class
 * ----------------------------------
 * This program class uses Map class to store the 
 * information of the Program.
 */

Program::Program() {
   /* Empty */
}

Program::~Program() {
   /* Empty */
}

void Program::clear() {
	sourceLineList.clear();
	parsedStatementList.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
	sourceLineList[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
	sourceLineList.remove(lineNumber);
	parsedStatementList.remove(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
	if(!sourceLineList.containsKey(lineNumber)){
		return "";
	}
	else 
	return sourceLineList[lineNumber];
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
	parsedStatementList[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
	if(!parsedStatementList.containsKey(lineNumber)){
		return NULL;
	}
	else return parsedStatementList[lineNumber];
}

int Program::getFirstLineNumber() {
	if(sourceLineList.isEmpty())return -1;
	return *sourceLineList.begin();
}

int Program::getNextLineNumber(int lineNumber) {
	Map<int,string>::iterator it = sourceLineList.begin();
	while(it!=sourceLineList.end()){
		if(*it==lineNumber){
			it++;
			if(it==sourceLineList.end()) return -1;
			else return *it;
			break;
		}
		it++;
	}
	return -1;
}
