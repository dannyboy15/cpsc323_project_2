#include <string>
#include <vector>
#include <sstream>

#include "Node.h"
#include "NonTerminal.h"


using namespace std;

Node::Node() {}

Node::Node(Symbol* rContent) {
    content = rContent;
}

Node::Node(Symbol* rContent, Node* rParent, int rIx) {
    content = rContent;
    parent = rParent;
    kidIx = rIx;
}

Node::Node(Node* oldNode) {
    this->content = oldNode->content;
    
}

Node::~Node() {
    delete content;
    delete parent;
    
    for (size_t i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

void Node::setParent(Node* _parent) {
    parent = _parent;
}

void Node::setSymbol(Symbol * rSym) {
    content = rSym;
}

void Node::setIx(int ix) {
    kidIx = ix;
}

int Node::getIx() {
    return kidIx;
}

Symbol* Node::getSymbol() {
    return content;
}

Node* Node::getParent() {
    if(parent) {
        return parent;
    } else {
        return NULL;
    }
}

Node* Node::getChild(int ix) {
    if(ix >= children.size() || ix < 0) {
        return NULL;
    }
    else {
        return children[ix];
    }
}

vector<Node*>* Node::getChildren() {
    return &children;
}

int Node::getChildCount() {
    return childCount;
}

Node* Node::getOpChildToHoist() {
    int opSymIx = static_cast<NonTerminal*>(content)->getRule()->getOpChildIx();
    if (opSymIx < 0) {
        return NULL;
    }
    else {
        return children[opSymIx];
    }
}

Node* Node::getLChildToHoist() {
    int lSymIx = static_cast<NonTerminal*>(content)->getRule()->getLChildIx();
    if (lSymIx < 0) {
        return NULL;
    }
    else {
        return children[lSymIx];
    }
}
Node* Node::getRChildToHoist() {
    int rSymIx = static_cast<NonTerminal*>(content)->getRule()->getRChildIx();
    if (rSymIx < 0) {
        return NULL;
    }
    else {
        return children[rSymIx];
    }
}

void Node::insert(Node* newChild) {
        
    // Assign the kid his id
    // which is the current size
    newChild->kidIx = static_cast<int>(children.size());
    
    // set parent
    newChild->parent = this;
    
    // Add kid to kid list
    children.push_back(newChild);
    childCount = static_cast<int>(children.size());
}

void Node::insert(Node* newChild, bool b) {
        
    // Add kid to kid list
    children.push_back(newChild);
    childCount = static_cast<int>(children.size());
}

void Node::insertChildren(vector<Symbol*> children) {
    for(int i = 0; i < children.size(); i++) {
        insert(new Node(children[i], this, i), true);
    }
}

Node Node::find(string node) {
    if (children.empty()) {
        //return;
    }
    
    for (size_t i = 0; i < children.size(); ++i) {}
    
    return Node();
    
}

void Node::remove(Node* child) {
    // children.erase(/*child position*/);
    childCount = static_cast<int>(children.size());
}

void Node::removeChildAt(int chldIx) {
    if (chldIx >= 0 && chldIx < children.size()) {
        children.erase(children.begin() + chldIx);
    }
    childCount = static_cast<int>(children.size());
}

void Node::addAstChildren(Node* mom) {
        children.clear();
        children.push_back(mom->getLChildToHoist());
        children.push_back(mom->getRChildToHoist());
        childCount = static_cast<int>(children.size());
}

Node* Node::copyGutsTo(Node* target) {
    
    // Copy self symbol to parent
    target->setSymbol(this->content);

    // Copy child count
    target->childCount = this->childCount;

    // Copy children
    target->children = this->children;

    return target;
}

string Node::localInfoStr() {
    stringstream ss;
    ss << this;
    
    string s;
    s += "(";
    s += "Node:";
    s += " name: ";
    s += content->getName();
    s += " id: ";
    s += ss.str();
    s += " isTerminal: ";
    s += content->isTerm() ? "true" : "false";
    s += ")";
    
    return s;
}

string Node::toString(Node* ptr) {
    // Base step
    if(!ptr) {
        return "";
    }
    
    string s;
    
    s += ptr->content->toString();
    s += "\n";
    
    for(size_t i = 0; i < ptr->children.size(); ++i) {
        s += toString(ptr->children[i]);
        s += "\n";
    }
    
    return s;
}
