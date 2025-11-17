#include "parser.h"

map <string, int> locationTable; 
 map <int,int> constantLocationTable;

 struct typeOfCJMP{
    struct InstructionNode* currentInstruction;
    TokenType cjmpType; 
 };

void printMem(int memCounter){
    for(int i = 0; i < memCounter; i++){
        cout<< "mem in mem" << i << " is: "<< mem[i] <<"\n";
    }
}
void print(struct InstructionNode* node){
    
    if(node->next == NULL){
        cout <<"Only node is type: " <<node->type <<"\n";
        return;
    }

    while(node!= NULL){
        cout <<"Current node is type: " <<node->type <<" it's address is "<< node << "\n";
        if(node->type == CJMP){
            if(node->cjmp_inst.target->next == NULL){
                cout<<"If's NOOP node's next is null\n";
                cout<<"If's NOOP node deals with var:" <<node->cjmp_inst.op1_loc <<"\n";
            } else {
                cout<<"If's NOOP node's next is type: " <<node->cjmp_inst.target->next->type<< " it's address is: "<< node->cjmp_inst.target->next <<"\n";
                cout<<"If's NOOP node deals with var:" <<node->cjmp_inst.op1_loc <<"\n";

            }
        }
        if(node->type == JMP){
            if(node->jmp_inst.target == NULL){
                cout<<"JMP's target is null\n";
            } else{
                cout<<"JMP's target is : " <<node->jmp_inst.target<<"\n";
            }
            
            if(node->next == NULL){
                cout<<"JMP's next is null\n";
            } else{
                cout<<"JMP's next is type: " <<node->next->type <<  " it's address is" << node->next <<"\n";
            }
        }
        
        node = node->next;
    }
}

void insertNode(struct InstructionNode* nodeToInsert, struct InstructionNode* current){
    current->next = nodeToInsert;
   
}


void memAllocationForID(){
    int addr = next_available;
    mem[addr] = 0;
    next_available++;
}

void memAllocationForConstant(int constant){
    int addr = next_available;
    mem[addr] = constant;
    next_available++;
}

//make function to create a new instruction node for input 
struct InstructionNode* createInputInstruction(string varName){
    struct InstructionNode* newNode = new InstructionNode;
    int addr = locationTable[varName];

    newNode->type = IN;
    newNode->input_inst.var_loc = addr;
    newNode->next = NULL;

    return newNode;
}

struct InstructionNode* createAssignInstruction(string varName, int constantToAssign){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = ASSIGN;
    int addr = locationTable[varName];
    
    newNode->assign_inst.lhs_loc = addr;
    newNode->assign_inst.op = OPERATOR_NONE;

    int constantAddr = constantLocationTable[constantToAssign];
    //cout << "constant addr is: " << constantAddr << "\n";
    newNode->assign_inst.op1_loc = constantAddr;
    newNode->next = NULL;
    return newNode;
}

struct InstructionNode* createOutputInstruction(string varName){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = OUT;
    int addr = locationTable[varName];
    newNode->output_inst.var_loc = addr;
    newNode->next = NULL;
    return newNode;
}

struct InstructionNode* createAssignWithTwoConstants(string varName, int constantOne, int constantTwo, string operatorType){
    struct InstructionNode* newNode= new InstructionNode;
    int addr = locationTable[varName];
    int addrOne = constantLocationTable[constantOne];
    int addrTwo = constantLocationTable[constantTwo];
    ArithmeticOperatorType op;
    if(operatorType == "+"){
        op = OPERATOR_PLUS;
    } else if(operatorType == "-"){
        op = OPERATOR_MINUS;
    } else if(operatorType == "*"){
        op = OPERATOR_MULT;
    } else if(operatorType == "/"){
        op = OPERATOR_DIV;
    }

    newNode->type = ASSIGN;
    newNode->assign_inst.lhs_loc = addr;
    newNode->assign_inst.op = op;
    newNode->assign_inst.op1_loc = addrOne;
    newNode->assign_inst.op2_loc = addrTwo;
    newNode->next = NULL;
    return newNode;
}

struct InstructionNode* createAssignWithOperatorInstruction(string varName, string varToAddWith, int constantToAssign, string operatorType){
    struct InstructionNode* newNode = new InstructionNode;
    int addr = locationTable[varName];
    ArithmeticOperatorType op;
    if(operatorType == "+"){
        op = OPERATOR_PLUS;
    } else if(operatorType == "-"){
        op = OPERATOR_MINUS;
    } else if(operatorType == "*"){
        op = OPERATOR_MULT;
    } else if(operatorType == "/"){
        op = OPERATOR_DIV;
    }
    int addrofVarToAddWith = locationTable[varToAddWith];
    int constantAddr = constantLocationTable[constantToAssign];

    newNode->type = ASSIGN;
    newNode->assign_inst.lhs_loc = addr;


    newNode->assign_inst.op = op; 
    newNode->assign_inst.op1_loc = addrofVarToAddWith;
    newNode->assign_inst.op2_loc = constantAddr;
    newNode->next = NULL;
    return newNode;
}

struct InstructionNode* createNOOPInstrutction(){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = NOOP;
    newNode->next = NULL;
    return newNode;
}

struct InstructionNode* createIfInstruction(string varNameOne, string varNameTwo, string operatorType){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = CJMP;
    ConditionalOperatorType op;
    if(operatorType == ">"){
        op = CONDITION_GREATER;
    } else if(operatorType == "<"){
        op = CONDITION_LESS;
    } else if(operatorType == "<>"){
        op = CONDITION_NOTEQUAL;
    }

    newNode->cjmp_inst.condition_op = op;

    int addr = locationTable[varNameOne];
    int addr2 = locationTable[varNameTwo];
    newNode->cjmp_inst.op1_loc = addr;
    newNode->cjmp_inst.op2_loc = addr2;
    newNode->cjmp_inst.target = createNOOPInstrutction();
    newNode->next = NULL;
    return newNode;

}

struct InstructionNode* createIfInstructionWithConstant(string varName, int constant, string operatorType){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = CJMP;
    ConditionalOperatorType op;
    if(operatorType == ">"){
        op = CONDITION_GREATER;
    } else if(operatorType == "<"){
        op = CONDITION_LESS;
    } else if(operatorType == "<>"){
        op = CONDITION_NOTEQUAL;
    }

    newNode->cjmp_inst.condition_op = op;
    int addr = locationTable[varName];
    int addrTwo = constantLocationTable[constant];
    newNode->cjmp_inst.op1_loc = addr;
    newNode->cjmp_inst.op2_loc = addrTwo;
    newNode->cjmp_inst.target = createNOOPInstrutction();
    newNode->next = NULL;
    return newNode;

}

struct InstructionNode* createVarAssignInstruction(string varNameOne, string varNameTwo){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = ASSIGN;
    int addr = locationTable[varNameOne];
    int addr2 = locationTable[varNameTwo];
    newNode->assign_inst.lhs_loc = addr;
    newNode->assign_inst.op = OPERATOR_NONE;
    newNode->assign_inst.op1_loc = addr2;
    newNode->next = NULL;
    return newNode;
}


struct InstructionNode* createAssignWithOperatorAndSecondVarInsutruction(string varNameOne, string varNameTwo, string varNameThree, string operatorType){
    struct InstructionNode* newNode = new InstructionNode;
    int addr = locationTable[varNameOne];
    ArithmeticOperatorType op;
    if(operatorType == "+"){
        op = OPERATOR_PLUS;
    } else if(operatorType == "-"){
        op = OPERATOR_MINUS;
    } else if(operatorType == "*"){
        op = OPERATOR_MULT;
    } else if(operatorType == "/"){
        op = OPERATOR_DIV;
    }


    int addrofVarOneToAddWith = locationTable[varNameTwo];
    int addrofVarTwoToAddWith = locationTable[varNameThree];
    
    newNode->type = ASSIGN;
    newNode->assign_inst.lhs_loc = addr;
    newNode->assign_inst.op = op;
    newNode->assign_inst.op1_loc = addrofVarOneToAddWith;
    newNode->assign_inst.op2_loc = addrofVarTwoToAddWith;
    newNode->next = NULL;
    return newNode;

}

struct InstructionNode* createWhileInstruction(string varNameOne, int constant, string operatorType){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = CJMP;
    int addr = locationTable[varNameOne];
    int addr2 = constantLocationTable[constant];
    ConditionalOperatorType op;
    if(operatorType == ">"){
        op = CONDITION_GREATER;
    } else if(operatorType == "<"){
        op = CONDITION_LESS;
    } else if(operatorType == "<>"){
        op = CONDITION_NOTEQUAL;
    }
    newNode->cjmp_inst.condition_op = op;
    newNode->cjmp_inst.op1_loc = addr;
    newNode->cjmp_inst.op2_loc = addr2;
    newNode->cjmp_inst.target = createNOOPInstrutction();
    newNode->next = NULL;
    return newNode;
}

struct InstructionNode* createWhileWithTwoVars(string varNameOne, string varNameTwo, string operatorType){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = CJMP;
    int addr = locationTable[varNameOne];
    int addrTwo = locationTable[varNameTwo];
    ConditionalOperatorType op;
    if(operatorType == ">"){
        op = CONDITION_GREATER;
    } else if(operatorType == "<"){
        op = CONDITION_LESS;
    } else if(operatorType == "<>"){
        op = CONDITION_NOTEQUAL;
    }
    newNode->cjmp_inst.condition_op = op;
    newNode->cjmp_inst.op1_loc = addr;
    newNode->cjmp_inst.op2_loc = addrTwo;
    newNode->cjmp_inst.target = createNOOPInstrutction();
    newNode->next = NULL;
    return newNode;
}

struct InstructionNode* createJmpInstruction(){
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = JMP;
    newNode->next = NULL;
    return newNode;
}

void endBlock(stack<struct typeOfCJMP> &stack, struct InstructionNode* current, int counter){
    if(current->type == CJMP && counter == 1){
        struct typeOfCJMP node = stack.top();
        stack.pop();
        struct typeOfCJMP actualNodeWeWant = stack.top();
        stack.pop();
        actualNodeWeWant.currentInstruction->cjmp_inst.target->next = current;
        stack.push(node);
    } else if(current->type == CJMP && counter > 1){
        struct typeOfCJMP node = stack.top();
        stack.pop();
        for(int i = 0; i < counter; i++){
            struct typeOfCJMP node = stack.top();
            stack.pop();
            node.currentInstruction->cjmp_inst.target->next = current;
        }
       stack.push(node);
    }else{
        //cout << "stack size is: " << stack.size() << "\n";
        //cout << "counter is: " << counter << "\n";
        int i = 0;
        while(!stack.empty() && i < counter){
            struct typeOfCJMP node = stack.top();
            stack.pop();
            node.currentInstruction->cjmp_inst.target->next = current;
            i++;
        }
    }
}

struct InstructionNode *parse_Generate_Intermediate_Representation(){
    LexicalAnalyzer lexer = LexicalAnalyzer();
    Token token;
    int memcounter = 0; 
    int rBraceCounter = 0;
    bool ifEnd; 
    bool inForLoop = false;
    set <string> seenIDs;
    set <int> seenConstants; 
    int lineCounter = 0; 
    stack<struct typeOfCJMP> cjmpStack;
    stack<struct InstructionNode*> prevNodes;
    struct InstructionNode* current; 
    struct InstructionNode* first;
    struct InstructionNode* prev;


    while((token = lexer.GetToken()).token_type != END_OF_FILE){
        //token.Print();
        if(token.token_type == LBRACE && inForLoop){
            current->next = cjmpStack.top().currentInstruction;
            current = cjmpStack.top().currentInstruction;
            inForLoop = false;
        }

        if(token.token_type == ID){
            if(seenIDs.find(token.lexeme) == seenIDs.end()){
                memAllocationForID();
                locationTable[token.lexeme] = memcounter;
                memcounter++;
                seenIDs.insert(token.lexeme);
            }
            if(lexer.peek(1).token_type == EQUAL){
                int i = 2;
                string varNameOne;
                string varNameTwo;
                string operatorType; 
                int constant = -99999999;
                int constantTwo = -99999999;
                TokenType currentTokenType;
                while((currentTokenType = lexer.peek(i).token_type) != SEMICOLON){ //might need to build up in case there are bigger operations and other edge cases 
                    if(currentTokenType == ID){
                        if(varNameOne.empty()){
                            varNameOne = lexer.peek(i).lexeme; 
                        }else {
                            varNameTwo = lexer.peek(i).lexeme;
                        }
                        i++;
                    }
                    if(currentTokenType == NUM){
                        if(constant == -99999999){
                            if(seenConstants.find(stoi(lexer.peek(i).lexeme)) == seenConstants.end()){
                                constant = stoi(lexer.peek(i).lexeme);
                                memAllocationForConstant(constant);
                                constantLocationTable[constant] = memcounter;
                                memcounter++;
                                seenConstants.insert(constant);
                                i++;
                            } else{
                                constant = stoi(lexer.peek(i).lexeme);
                                i++;
                            }
                        } else {
                            if(seenConstants.find(stoi(lexer.peek(i).lexeme)) == seenConstants.end()){
                                constantTwo = stoi(lexer.peek(i).lexeme);
                                memAllocationForConstant(constantTwo);
                                constantLocationTable[constantTwo] = memcounter;
                                memcounter++;
                                seenConstants.insert(constantTwo);
                                i++;
                            } else{
                                constantTwo = stoi(lexer.peek(i).lexeme);
                                i++;
                            }
                        }
                    }
                    if(currentTokenType == PLUS){
                        operatorType = "+";
                        i++;
                    }
                    if(currentTokenType == MINUS){
                        operatorType = "-";
                        i++;
                    }
                    if(currentTokenType == MULT){
                        operatorType = "*";
                        i++;
                        
                    }
                    if(currentTokenType == DIV){
                        operatorType = "/";
                        i++;
                    }                    
                }
                if(!inForLoop){
                    if(!varNameOne.empty() && !varNameTwo.empty()){
                        struct InstructionNode* inputNode = createAssignWithOperatorAndSecondVarInsutruction(token.lexeme, varNameOne, varNameTwo, operatorType);
                        if(lineCounter > 0){
                            insertNode(inputNode, current);
                            current = inputNode;
                        } else{
                            first = inputNode;
                            current = inputNode;
                        }
                    }else if(!varNameOne.empty() && constant != -99999999){
                        struct InstructionNode* inputNode = createAssignWithOperatorInstruction(token.lexeme,varNameOne,constant,operatorType);
                        if(lineCounter > 0){
                            insertNode(inputNode, current);
                            current = inputNode;
                        } else{
                            first = inputNode;
                            current = inputNode;
                        }
                    } else if(!varNameOne.empty() && constant == -99999999){
                        struct InstructionNode* inputNode = createVarAssignInstruction(token.lexeme, varNameOne);
                        if(lineCounter > 0){
                            insertNode(inputNode, current);
                            current = inputNode;
                        } else{
                            first = inputNode;
                            current = inputNode;
                        }
                    } else if(varNameOne.empty() && (constant != -99999999) && (constantTwo == -99999999)){
                        struct InstructionNode* inputNode = createAssignInstruction(token.lexeme, constant);
                        if(lineCounter > 0){
                            insertNode(inputNode, current);
                            current = inputNode;
                        } else{
                            first = inputNode;
                            current = inputNode;
                        }
                    } else if(constant != -99999999 && constantTwo != -99999999){
                        struct InstructionNode* inputNode= createAssignWithTwoConstants (token.lexeme, constant, constantTwo, operatorType);
                        if(lineCounter > 0){
                            insertNode(inputNode, current);
                            current = inputNode;
                        } else{
                            first = inputNode;
                            current = inputNode;
                        }
                    }else if (varNameOne.empty() && constant != -99999999){
                        struct InstructionNode* inputNode = createAssignInstruction(token.lexeme, constant);
                        if(lineCounter >0){
                            insertNode(inputNode, current);
                            current = inputNode;
                        } else{
                            first = inputNode;
                            current = inputNode;
                        }
                    }
                }
                if(inForLoop){
                    // b = 1
                    //cout <<"varNameOne empty ?: " <<varNameOne.empty() << "\n"; 
                    //cout <<"varNameTwo empty ?: " <<varNameTwo.empty() << "\n";
                    if(varNameOne.empty() && constant != -99999999){
                        struct InstructionNode* inputNode = createAssignInstruction(token.lexeme, constant);
                        insertNode(inputNode, current);
                        current = inputNode;
                        //cout << "b = 1\n";
                        //b = b + 1;
                    } else if(!varNameOne.empty() && constant != -99999999){
                        struct InstructionNode* inputNode = createAssignWithOperatorInstruction(token.lexeme, varNameOne, constant, operatorType);
                        prevNodes.push(inputNode);
                        //cout <<"b = b + 1\n";
                        //b = a; 
                    } else if(!varNameOne.empty() && constant == -99999999 && varNameTwo.empty()){
                        struct InstructionNode* inputNode = createVarAssignInstruction(token.lexeme, varNameOne);
                        insertNode(inputNode, current);
                        current = inputNode;
                        //cout << "b = a\n";
                        //b = b + a;
                    } else if(!varNameOne.empty() && !varNameTwo.empty()){
                       // cout << "b = b + a\n";
                        struct InstructionNode* inputNode = createAssignWithOperatorAndSecondVarInsutruction(token.lexeme, varNameOne, varNameTwo, operatorType);
                        prevNodes.push(inputNode);
                        //cout << "b = b + a\n";
                    }
                }
                lineCounter++;
            }
            
        } else if(token.token_type == NUM && lexer.peek(1).token_type != NUM){
            if(lexer.peek(1).token_type == END_OF_FILE){
                break;
            } else if(seenConstants.find(stoi(token.lexeme)) == seenConstants.end()){
                memAllocationForConstant(stoi(token.lexeme));
                constantLocationTable[stoi(token.lexeme)] = memcounter;
                memcounter++;
                seenConstants.insert(stoi(token.lexeme));
            }
        } else if(token.token_type == NUM && lexer.peek(1).token_type == NUM){
            inputs.push_back(stoi(token.lexeme));
        }

        if(token.token_type == NUM && lexer.peek(1).token_type == END_OF_FILE){
            inputs.push_back(stoi(token.lexeme));
        }

        if(token.token_type == IF){
            Token currentToken;
            int i = 1;
            string varNameOne;
            string varNameTwo;
            int constant = -99999999;
            string operatorType;

            while((currentToken = lexer.peek(i)).token_type != LBRACE){
                //cout << "currentToken is: " << currentToken.lexeme << "\n";
                if(currentToken.token_type == ID){
                    if(varNameOne.empty()){
                        varNameOne = lexer.peek(i).lexeme;
                        //cout << "varNameOne is: " << varNameOne << "\n";
                        i++;
                    } else{
                        varNameTwo = lexer.peek(i).lexeme;
                        i++;
                    }
                }
                if(currentToken.token_type == NUM){
                    if(seenConstants.find(stoi(lexer.peek(i).lexeme)) == seenConstants.end()){
                        constant = stoi(lexer.peek(i).lexeme);
                        memAllocationForConstant(constant);
                        constantLocationTable[constant] = memcounter;
                        memcounter++;
                        seenConstants.insert(constant);
                        i++;
                    } else{
                        constant = stoi(lexer.peek(i).lexeme);
                        i++;
                    }
                }
                if(currentToken.token_type == GREATER){
                    operatorType = ">";
                    i++;
                }
                if(currentToken.token_type == NOTEQUAL){
                    operatorType = "<>";
                    i++;
                }

                if(currentToken.token_type == LESS){
                    operatorType = "<";
                    i++;
                }
            }
            if(!varNameTwo.empty()){
                struct InstructionNode* inputNode = createIfInstruction(varNameOne, varNameTwo, operatorType);
                if(lineCounter > 0){
                    insertNode(inputNode, current);
                    struct typeOfCJMP node;
                    node.currentInstruction = inputNode;
                    node.cjmpType = token.token_type;
                    cjmpStack.push(node);
                    current = inputNode;
                } else{
                    first = inputNode;
                    current = inputNode;
                }
            }else{
                struct InstructionNode* inputNode = createIfInstructionWithConstant(varNameOne, constant, operatorType);
                if(lineCounter > 0){
                    insertNode(inputNode, current);
                    struct typeOfCJMP node;
                    node.currentInstruction = inputNode;
                    node.cjmpType = token.token_type;
                    cjmpStack.push(node);
                    current = inputNode;
                } else{
                    first = inputNode;
                    current = inputNode;
                }
                
            }
            lineCounter++;
        }

        if(token.token_type == INPUT){
            string varName = lexer.peek(1).lexeme;
            struct InstructionNode* inputNode = createInputInstruction(varName);
            if(lineCounter >0){
                insertNode(inputNode, current);
                current = inputNode;
            } else{
                first = inputNode;
                current = inputNode;
            }
            lineCounter++;
        }

        if(token.token_type == OUTPUT){
            string varName = lexer.peek(1).lexeme;
            struct InstructionNode* outputNode = createOutputInstruction(varName);
            if(lineCounter >0){
                insertNode(outputNode, current);
                current = outputNode;
            } else{
                first = outputNode;
                current = outputNode;
            }
            lineCounter++;
        }

        if(token.token_type == WHILE){
           // cout << "found a while" << "\n";
            //token.Print();
            Token currentToken;
            int i = 1;
            string varNameOne;
            string varNameTwo;
            int constant;
            string operatorType;
            while((currentToken = lexer.peek(i)).token_type != LBRACE){
                if(currentToken.token_type == ID){
                    if(varNameOne.empty()){
                        varNameOne = currentToken.lexeme;
                        i++;
                    }else {
                        i++;
                        varNameTwo = currentToken.lexeme;
                    }
                }
                if(currentToken.token_type == GREATER){
                    operatorType = ">";
                    i++;
                }
                if(currentToken.token_type == NOTEQUAL){
                    operatorType = "<>";
                    i++;
                }
                if(currentToken.token_type == LESS){
                    operatorType = "<";
                    i++;
                }
                if(currentToken.token_type == NUM){
                    if(seenConstants.find(stoi(lexer.peek(i).lexeme)) == seenConstants.end()){
                        constant = stoi(lexer.peek(i).lexeme);
                        memAllocationForConstant(constant);
                        constantLocationTable[constant] = memcounter;
                        memcounter++;
                        seenConstants.insert(constant);
                        i++;
                    } else{
                        constant = stoi(lexer.peek(i).lexeme);
                        i++;
                    }
                }
            } 
            if(varNameTwo.empty()){
                //cout <<"varNameTwo is empty\n";
                struct InstructionNode* inputNode = createWhileInstruction(varNameOne, constant, operatorType);
                if(lineCounter > 0){
                    insertNode(inputNode, current);
                    struct typeOfCJMP node;
                    node.currentInstruction = inputNode;
                    node.cjmpType = token.token_type;
                    cjmpStack.push(node);
                    //cout <<"pushed\n";
                    //cout << cjmpStack.size() << "\n";
                    current = inputNode;
                } else{
                    first = inputNode;
                    current = inputNode;
                }
            } else {
                //cout <<"varNameTwo is not empty\n";
                struct InstructionNode* inputNode = createWhileWithTwoVars(varNameOne, varNameTwo, operatorType);
                if(lineCounter > 0){
                    insertNode(inputNode,current);
                    struct typeOfCJMP node;
                    node.currentInstruction = inputNode;
                    node.cjmpType = token.token_type;
                    cjmpStack.push(node);
                    current = inputNode;
                } else{
                    first = inputNode;
                    current = inputNode;
                }
            }
            
            lineCounter++;
        }

        if(token.token_type == FOR){
            int i = 2;
            Token currentToken = lexer.peek(i);
            string varNameOne;
            string varNameTwo;
            int constant;
            string operatorType; 
            //need to grab the conditional, first while gets past the assignment
            while(currentToken.token_type != SEMICOLON ){
                currentToken = lexer.peek(i++);
            }
            currentToken = lexer.peek(i++);
            //grabs the conditional
            while(currentToken.token_type != SEMICOLON){
                if(currentToken.token_type == ID && varNameOne.empty()){
                    varNameOne = currentToken.lexeme;
                } else if (currentToken.token_type == ID && !varNameOne.empty()){
                    varNameTwo = currentToken.lexeme;
                }

                if(currentToken.token_type == LESS){
                    operatorType = "<";
                }

                if(currentToken.token_type == GREATER){
                    operatorType = ">";
                }

                if(currentToken.token_type == NUM){
                    if(seenConstants.find(stoi(currentToken.lexeme)) == seenConstants.end()){
                        constant = stoi(currentToken.lexeme);
                        memAllocationForConstant(constant);
                        constantLocationTable[constant] = memcounter;
                        memcounter++;
                        seenConstants.insert(constant);
                    } else{
                        constant = stoi(currentToken.lexeme);
                    }
                }
                currentToken = lexer.peek(i++);
            }

            if(varNameTwo.empty()){
                struct InstructionNode * inputNode = createWhileInstruction(varNameOne, constant, operatorType);
                struct typeOfCJMP node;
                node.cjmpType = FOR;
                node.currentInstruction = inputNode;
                cjmpStack.push(node);
            } else{
                struct InstructionNode* inputNode = createWhileWithTwoVars(varNameOne, varNameTwo, operatorType);
                struct typeOfCJMP node;
                node.cjmpType = FOR;
                node.currentInstruction = inputNode;
                cjmpStack.push(node);

            }
            if(lineCounter == 0){
                first = cjmpStack.top().currentInstruction;
            }

            lineCounter++;
            inForLoop = true;
        }

        
        if(!cjmpStack.empty() && ifEnd){
            //cout << "calling endBlock\n";
            endBlock(cjmpStack, current, rBraceCounter);
            rBraceCounter = 0;
            ifEnd = false; 
        }
       
        
        if(token.token_type == RBRACE){
            //token.Print();
            //cout << cjmpStack.size() << "\n";
            if(cjmpStack.size() > 0) {
                if(lexer.peek(1).token_type != NUM && lexer.peek(1).token_type != END_OF_FILE && lexer.peek(1).token_type == RBRACE){
                    rBraceCounter++;
                    //cout << "only incrementing\n";
                } 
                if(lexer.peek(1).token_type != RBRACE && lexer.peek(1).token_type != NUM && lexer.peek(1).token_type != END_OF_FILE){
                    rBraceCounter++;
                    //cout << "rBraceCounter is: " << rBraceCounter << " => " << cjmpStack.top().cjmpType  << "\n";
                    if(cjmpStack.top().cjmpType == WHILE){
                        struct InstructionNode* jmpNode = createJmpInstruction();
                        struct typeOfCJMP node = cjmpStack.top();
                        cjmpStack.pop();
                        jmpNode->jmp_inst.target= node.currentInstruction;
                        current->next = jmpNode;
                        current = jmpNode;
                        cjmpStack.push(node);
                        
                    }
                    if(cjmpStack.top().cjmpType == FOR && prevNodes.size()>0){
                        struct InstructionNode* jmpNode = createJmpInstruction();
                        struct typeOfCJMP node = cjmpStack.top();
                        cjmpStack.pop();
                        jmpNode->jmp_inst.target= node.currentInstruction;
                        struct InstructionNode* prenode = prevNodes.top();
                        prevNodes.pop();
                        current->next = prenode;
                        current = prenode;
                        current->next = jmpNode;
                        current = jmpNode;
                        cjmpStack.push(node);
                    }
                    ifEnd = true;
                    
                }
                if(lexer.peek(1).token_type == RBRACE && lexer.peek(2).token_type == NUM){
                    //cout << "howdy\n";
                    if(cjmpStack.top().cjmpType == WHILE){
                        //cout << "is a while\n";
                        struct InstructionNode* jmpNode = createJmpInstruction();
                        struct typeOfCJMP node = cjmpStack.top();
                        cjmpStack.pop();
                        jmpNode->jmp_inst.target= node.currentInstruction;
                        current->next = jmpNode;
                        current = jmpNode;
                        cjmpStack.push(node);
                    }
                    if(cjmpStack.top().cjmpType == FOR){
                        //cout << "sup\n";
                        struct InstructionNode* jmpNode = createJmpInstruction();
                        struct typeOfCJMP node = cjmpStack.top();
                        //cout <<"got node from top of stack\n";
                        cjmpStack.pop();
                        jmpNode->jmp_inst.target= node.currentInstruction;
                        struct InstructionNode* prenode = prevNodes.top();
                        //cout<<"got prenode from top of stack\n";
                        prevNodes.pop();
                        current->next = prenode;
                        //cout<<"next is prenode\n";
                        current = prenode;
                        if(cjmpStack.size()>0 && cjmpStack.top().cjmpType == FOR){
                            node.currentInstruction->cjmp_inst.target->next = prevNodes.top();
                        }
                        current->next = jmpNode;
                        current = jmpNode;
                    }
                    ifEnd = false;

                }
                if(lexer.peek(1).token_type == RBRACE && lexer.peek(2).token_type != NUM){
                    //cout << "peek(1) is a brace and peek(2) is not a num\n";
                    if(!cjmpStack.empty()){
                        //cout << "not empty\n";
                        struct typeOfCJMP n = cjmpStack.top();
                        cjmpStack.pop();
                        if(cjmpStack.top().cjmpType == FOR){
                            //cout << "top is FOR\n";
                            struct InstructionNode* jmpNode = createJmpInstruction();
                            struct typeOfCJMP node = cjmpStack.top();
                            //cout << "got node from top of stack\n";
                            //cjmpStack.pop();
                            //cout<<"popped\n";
                            jmpNode->jmp_inst.target= node.currentInstruction;
                            struct InstructionNode* prenode = prevNodes.top();
                            //cout << "got prenode from top of stack\n";
                            prevNodes.pop();
                            //cout <<"pre is popped\n";
                            n.currentInstruction->cjmp_inst.target->next = prenode;
                            current->next = prenode;
                            prenode->next = jmpNode;
                            current = jmpNode;
                            //cjmpStack.push(node);
                            //ifEnd = true;
                        } else{
                            cjmpStack.push(n);
                        }
                    }
                }
                if(lexer.peek(1).token_type == RBRACE && (lexer.peek(2).token_type == NUM || lexer.peek(2).token_type == END_OF_FILE)){
                    //cout<<"last one\n";
                    if(!cjmpStack.empty()){
                        //cout<<"not empty\n";
                        //cout << cjmpStack.top().cjmpType << "\n";
                        if(cjmpStack.top().cjmpType == FOR){
                            //cout <<"in if\n";
                            struct InstructionNode* jmpNode = createJmpInstruction();
                            struct typeOfCJMP node = cjmpStack.top();
                            //cout<<"got node from top of stack\n";
                            cjmpStack.pop();
                            //cout << "popped node from stack\n";
                            jmpNode->jmp_inst.target= node.currentInstruction;
                            struct InstructionNode* prenode = prevNodes.top();
                            //cout<<"got prenode from top of stack\n";
                            prevNodes.pop();
                            //cout << "popped prenode from stack\n";
                            current->next = prenode;
                            current = prenode;
                            if(cjmpStack.size()>0 && cjmpStack.top().cjmpType == FOR){
                                //cout << "about to set\n";
                                node.currentInstruction->cjmp_inst.target->next = prevNodes.top();
                            }
                            current->next = jmpNode;
                            current = jmpNode;
                        } else{
                            if(cjmpStack.size()> 1){
                                struct typeOfCJMP node = cjmpStack.top();
                                cjmpStack.pop();
                                if(cjmpStack.top().cjmpType == FOR){
                                    struct InstructionNode* jmpNode = createJmpInstruction();
                                    jmpNode->jmp_inst.target= cjmpStack.top().currentInstruction;
                                    struct InstructionNode* prenode = prevNodes.top();
                                    prevNodes.pop();
                                    node.currentInstruction->cjmp_inst.target->next = prenode;
                                    current->next = prenode;
                                    prenode->next = jmpNode;
                                    current = jmpNode;
                                }
                                if(cjmpStack.top().cjmpType == WHILE){
                                    struct InstructionNode* jmpNode = createJmpInstruction();
                                    jmpNode->jmp_inst.target= cjmpStack.top().currentInstruction;
                                    struct InstructionNode* prenode = prevNodes.top();
                                    prevNodes.pop();
                                    node.currentInstruction->cjmp_inst.target->next = prev;
                                    current->next = prenode;
                                    prenode->next = jmpNode;
                                    current = jmpNode;
                                }
                                
                            }
                        }
                        ifEnd = false; 
                    }
                }
            }
        }       

    }
    inputs.push_back(stoi(token.lexeme));
   
    print(first);
    
    return first;
}
