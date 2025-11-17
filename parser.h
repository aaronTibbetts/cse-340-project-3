#ifndef _PARSER_H_
#define _PARSER_H_
#include <string>
#include <iostream>
#include <set>
#include "lexer.h"
#include "compiler.h"
#include <map> 
#include <stack>

using std::string;
using std::cout;
using std::cin;
using std::set;
using std::map;
using std::stack;

 

void printMem(int memCounter);

void print(struct InstructionNode* node);

void insertNode(struct InstructionNode* nodeToInsert, struct InstructionNode* current);

void memAllocationForID();

void memAllocationForConstant(int constant);

//make function to create a new instruction node for input 
struct InstructionNode* createInputInstruction(string varName);

struct InstructionNode* createAssignInstruction(string varName, int constantToAssign);

struct InstructionNode* createOutputInstruction(string varName);

struct InstructionNode* createAssignWithTwoConstants(string varName, int constantOne, int constantTwo, string operatorType);

struct InstructionNode* createAssignWithOperatorInstruction(string varName, string varToAddWith, int constantToAssign, string operatorType);

struct InstructionNode* createNOOPInstrutction();

struct InstructionNode* createIfInstruction(string varNameOne, string varNameTwo, string operatorType);

struct InstructionNode* createIfInstructionWithConstant(string varName, int constant, string operatorType);

struct InstructionNode* createVarAssignInstruction(string varNameOne, string varNameTwo);


struct InstructionNode* createAssignWithOperatorAndSecondVarInsutruction(string varNameOne, string varNameTwo, string varNameThree, string operatorType);

struct InstructionNode* createWhileInstruction(string varNameOne, int constant, string operatorType);

struct InstructionNode* createWhileWithTwoVars(string varNameOne, string varNameTwo, string operatorType);

struct InstructionNode* createJmpInstruction();

void endBlock(stack<struct typeOfCJMP> &stack, struct InstructionNode* current, int counter);

struct InstructionNode *parse_Generate_Intermediate_Representation();



#endif 
