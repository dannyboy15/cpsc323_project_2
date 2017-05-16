#include <string>
#include <vector>
#include <map>

#include "Parser.h"
#include "Token.h"
#include "Grammar.h"
#include "SymTable.h"
#include "Node.h"

using namespace std;

const static string PRT_LEVEL_FILL = "   ";
static int funcCount = 0;

Parser::Parser() {
    gmr = new Grammar();
}

Parser::Parser(Grammar* rGmr) {
    gmr = rGmr;
}

Parser::~Parser() {
    // delete gmr;
    // delete pst;
    // delete ast;
    // delete mom;
}


void Parser::printAST(Node * current) {
    if (current == NULL) {
        cout << "null" << endl;
        return;
    }

    printAST(current->getChildren()->at(0));
    cout << current->getSymbol()->getName() << endl;
    printAST(current->getChildren()->at(1));
}

void Parser::printPST(Node* current, Node* root = nullptr) {
    if (current == nullptr) {
        cout << "null" << endl;
        return;
    }
    else if(current->getChildren()->size() == 0) {//checks for root
        cout << "\t" <<current->getSymbol()->getName() + " >>> " << endl;
        //current = current->getParent();
        return;
    }

    //  Lollipop mom
    cout << current->getSymbol()->getName() << " >>> " << endl;
    for (size_t i = 0; i < current->getChildren()->size(); ++i) {
        
        current = current->getChildren()->at(i);
        //printPST(current->getChildren()->at(i),root);
        printPST(current, root);
        current = current->getParent();
    }
    cout << endl;

}

void Parser::addTokensToParser(vector<Token> _TokenList) {
    tokenList = _TokenList;
    cout << ">>>>>> Parser received " << tokenList.size() << " token(s)" << endl;
}

Node* Parser::makePST() {
    
    cout << "Starting to parse token list" << endl;

    // Setup
    vector<Symbol*> workingStack;

    // Add the eof symbol to stack
    workingStack.push_back(gmr->getTerminal("$"));

    // Add $ as root
    pst = new Node(workingStack[0], NULL, 0);

    // Add the start symbol to stack
    cout << gmr->getNonTerminal("Pgm");
    workingStack.push_back(gmr->getNonTerminal("Pgm"));  

    // Add Pgm as a child
    pst->insert(new Node(workingStack[1], pst, 1));     
    
    // Add the eof token to input 
    tokenList.push_back(Token("$"));

    // Set mom to Pgm
    mom = pst->getChild(0);

    cout << "Finished setup" << endl;

    int tokenIx = 1; // base 1
    while(!workingStack.empty()) {
        Symbol* top = workingStack.back();
        Token front = tokenList.front();

        if(top->isTerm()) {
            // if it's an epsilon rule pop it off
            if(top->getName() == "eps") {
                
                // check to see if mom has sibling
                while( !( ( mom->getParent() )->getChild( mom->getIx() + 1 ) ) ) {
                    
                    if( mom->getParent() ) {
                        mom = mom->getParent();
                    }
                }
                mom = ( mom->getParent() )->getChild( mom->getIx() + 1 );
                
                cout << "Popping of eps rule." << endl;
                workingStack.pop_back();
                
            }

            // if top it terminal check if it matches front
            else if(*(static_cast<Terminal*>(top)->getToken()) == front) {
                cout << "Matched: " << front.getId();
                cout << ". Popping off the top and the front.\n";
                cout << "\tln: " << front.getLn() << " ix: " << front.getIx() << endl;

                // Add token to symtable if it is identifier
                if (front.getId() == "id") {
                    Sym* temp = new Sym(front.getLn(), front.getId(), front.getIx(), front.getStr(), 0, "null");
                    symTable.addSymbol(*temp);
                }

                bool isAtTop = false;
                // Set mom to next kid
                if(mom->getParent()) {
                    while( !( ( mom->getParent() )->getChild( mom->getIx() + 1 ) ) ) {
                        
                        if( mom->getParent() ) {
                            mom = mom->getParent();
                            
                            // if we're at the top of the tree
                            if(mom == pst) {
                                isAtTop = true;
                                break;
                            }
                        }
                    }
                    if(!isAtTop) {
                        mom = ( mom->getParent() )->getChild( mom->getIx() + 1 );
                    }
                }
                
                
                // Pop the top
                workingStack.pop_back();

                // Pop the front
                tokenList.erase(tokenList.begin());
                tokenIx++;
            }

            // if top doesn't match with front throw error
            else {
                cout << "Error occured when parsing token " << tokenIx << endl;
                cout << "Predicted: " << (static_cast<Terminal*>(top)->getToken())->getId();
                cout << " but instead found: " << front.getId() << endl;

                break;
            }
        } 
        else {
            // M1: Top is a NonTerminal
            Rule rule = gmr->getRuleAt(top->getName(), front.getId());

            cout << "Checking for rule: ";
            cout << "(" << top->getName() << ", " << front.getId() << ")";
            cout << endl;

            if(!rule.isEmpty()) {

                cout << "This rule was found: ";
                cout << rule.toString() << endl;

                // Remove the top symbol from stack
                workingStack.pop_back();
                    
                // Add the rule in reverse
                vector<Symbol*> rhsRev = rule.getRhsReversed();
                for (size_t i = 0; i < rhsRev.size(); ++i) {
                    workingStack.push_back(rhsRev[i]);
                }

                // ************ added **************** //
                Node* t = new Node(rule.getLhs());
                t->copyGutsTo(mom);
                
                // Add rule rhs to mom
                mom->insertChildren(rule.getRhs());
                
                // Set mom to first child
                mom = mom->getChild(0);

            } else {
                cout << "The rule is empty => there is no prediction for this";
                cout << endl;

                // print stack
                cout << "Current stack" << endl;
                for(int i = static_cast<int>(workingStack.size()) - 1; i >= 0; i--) {
                    //cout << *workingStack[i] << endl;
                }

                // print input stream
                cout << endl <<  "Current token stream" << endl;
                for (size_t  i = 0; i < tokenList.size(); ++i) {
                    cout << tokenList[i].getId() << endl;
                }

                break;
            } 
        }
    }
    
    return pst;

}

void Parser::parseAST(Node* mum) {
    
    // base step
    if(!mum) {
        return;
    }

    //
    cout << mum->getSymbol()->getName() << " : " << static_cast<NonTerminal*>(mum->getSymbol()) << endl;
    
    // Base step
    if(mum->getSymbol()->isTerm()) {
        return;
    }
    
    //
    if (static_cast<NonTerminal*>(mum->getSymbol())->
        getRule()->getOpChildIx() == -1) {
        
        bool childChk = true;
        while (childChk) {
            mum = mum->getParent();
            
            if(mum->getChild(0) || mum->getChild(1)) {
                childChk = false;
            }
        }
        
        int chIx = mum->getIx();
        
        mum = mum->getParent();
        
        mum->removeChildAt(chIx);
        
        return;
    }
    
    // Base step 3
    if(static_cast<NonTerminal*>(mum->getSymbol())->
        getRule()->getOpChildIx() == -2) {
        
        // Child to copy from
        int cIx = static_cast<NonTerminal*>(mum->getSymbol())->
        getRule()->getLChildIx();

        Node* chld = mum->getChild(cIx);

        mum = chld->copyGutsTo(mum);

        parseAST(mum);
        return;
    }

    // pointing at kwdprog for first iteration
    Node* tmp = mum->getOpChildToHoist();

    // Assigning ast children
    // lchild -> NULL; rchild -> Block
    tmp->addAstChildren(mum);

    // Copy self guts to mom
    mum = tmp->copyGutsTo(mum);

    // Go to left child
    parseAST(mum->getChild(0));

    // Go to right child
    parseAST(mum->getChild(1));
    
    
    // ************** addded ************ //
    if (!mum->getSymbol()->isTerm() || mum->getSymbol()->getName() == "eps") {
        int chIx = mum->getIx();
        
        mum = mum->getParent();
        
        mum->removeChildAt(chIx);
        
        return;
    }
}

void Parser::newParseAST(Node* mom) {
    
    // print tree
    cout << ++funcCount << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
//    printAST(mom);
    
    if (!mom) {
        return;
    }
    
    if(mom->getSymbol()->isTerm()) {
        return;
    }
    
    int opCIx = static_cast<NonTerminal*>(mom->getSymbol())->getRule()->getOpChildIx();
    
    if (opCIx == -1) {
        // delete up
//        int cIx = mom->getIx();
//        mom->getParent()->removeChildAt(cIx);
        int cIx = mom->getParent()->getIx();
        mom->getParent()->getParent()->removeChildAt(cIx);
        return;
    }
    
    if(opCIx == -2) {
        // Child to copy from
        int cIx = static_cast<NonTerminal*>(mom->getSymbol())->getRule()->getLChildIx();
        
        Node* chld = mom->getChild(cIx);
        
        mom = chld->copyGutsTo(mom);
        
        newParseAST(mom);
        return;
    }
    
    Node* child = mom->getOpChildToHoist();
    
    if(!(child->getSymbol()->isTerm())) {
        newParseAST(child);
    }
    
    if(child->getSymbol()->getName() == "eps"){
        // delete up
        int cIx = mom->getIx();
        mom->getParent()->removeChildAt(cIx);
        return;
    }

    
    // Hoisting part because child isTerm
    else {
        // Assigning ast children
        child->addAstChildren(mom);
        
        // Copy self guts to mom
        mom = child->copyGutsTo(mom);
        
        // Go to left child
        newParseAST(mom->getChild(0));
        
        // Go to right child
        newParseAST(mom->getChild(1));
    }
}

void Parser::removeEpsRules(Node* mom) {
    if(!mom) {
        return;
    }
    
    if(mom->getSymbol()->getName() == "eps") {
        int cIx = mom->getParent()->getIx();
        mom->getParent()->getParent()->removeChildAt(cIx);
    }
    else {
        for (int i = 0; i < mom->getChildCount(); i++) {
            if(mom->getChild(i)) {
                removeEpsRules(mom->getChild(i));
            }
        }
    }
}

//void Parser::hoist() {
//    
//}

Node* Parser::makeAST() {

    // Make ast point to pst
    ast = pst;

    // We are pointing at Pgm
    Node* mm = ast->getChild(0);

//    removeEpsRules(mm);
//    parseAST(mm);
    newParseAST(mm);
    removeEpsRules(mm);
    funcCount = 0;


    // Go to the bottom right most node
    // Node* ptr = pst;
    // while(ptr->getChildren()->size() > 0) {
    //     for (int i = ptr->getChildren()->size() - 1; i >= 0; --i) {
    //         if (!ptr->getChild(i)->getSymbol()->isTerm()) {
    //             ptr = ptr->getChild(i);
    //             break;
    //         }
    //     }
    // }

    // // Get its parent
    // ptr = ptr->getParent();

    // Get child that will be hoisted
    // ptr = ptr->getOpChildToHoist();

    // Add needed child pointers to current node
    // ptr->addAstChildren(ptr->getParent());

    // Copy guts to parent/mom and hoist
    // ptr = ptr->copyGutsTo(ptr->getParent());

    cout << "current symbol is: ";
    // cout << ptr->getSymbol()->toString() << endl;
    return new Node();
}

//This is the print function of a PST tree.
//Implemented as a recursive pre-order function.
//NOTE: This is finished
void Parser::printPST(Node * current, int level = 0)
{
    if (current == nullptr) {
        cout << "null" << endl;
        return;
    }
    /*if (current->getParent() == nullptr) // ---------Checks for root - Found no use for this but kept just in case------------
     {
     }*/
    
    // For loop which makes the tree level more obvious
    string r;
    for (int i = 0; i < level; ++i) {
        // Uses filler to show levels(actual variable is found near the top)
        r += PRT_LEVEL_FILL;
    }
    
    cout << r;
    
    // print paren 1
    cout << "(";
    
    // print self
    cout << current->localInfoStr() << "\n";
    
    int cLevel = ++level;
    // Print out the children of current node
    for (size_t i = 0; i < current->getChildren()->size();++i) {
        printPST(current->getChild(static_cast<int>(i)), cLevel);
    }
    
    cout << r << ")\n";
}

void Parser::printPST() {
    printPST(pst->getChild(0), 0);
}

void Parser::printAST() {
    printPST(ast->getChild(0), 0);
}


void Parser::printSymTable() {
    symTable.printTable();
}
