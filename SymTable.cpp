#include <iostream>
#include <vector>
#include <string>
#include <list>

#include "SymTable.h"
#include "Sym.h"

using namespace std;

SymTable::SymTable() {
    currentIndentLevel = 0;
}

SymTable::~SymTable() {}

void SymTable::indent() {
    for (int i = 0; i < currentIndentLevel; i++) {
        cout << "  ";
    }
}

void SymTable::indent(int count) {
    for (int i = 0; i < count; i++) {
        cout << "  ";
    }
}

void SymTable::addSymbol(Sym newSym) {
    bool sym_exists = false;
    bool sym_added = false;
    int sym_loc = 0;
    cout << "Looking for a symtable match" << endl;
    if (symbols.size() == 0) {
        symbols.push_back(newSym);
        sym_added = true;
    }
    else {
        for (size_t i = 0; i < symbols.size(); i++) {
            if (symbols[i].getStr() == newSym.getStr()) {
                sym_exists = true;
                sym_loc = static_cast<int>(i);
            }
        }
    }
    if (sym_exists) {
        symbols[sym_loc].addOcc(newSym.getLn(), newSym.getIx());
    }
    else {
        if (!sym_added) {
            symbols.push_back(newSym);
        }
    }
}

void SymTable::printTable() {
    indent();
    cout << ">>> Symbol Table" << endl;
    currentIndentLevel++;
    if (symbols.size() == 0) {
        indent();
        cout << "0 symbols found." << endl;
    }
    else {
        indent();
        cout << symbols.size() << " symbol(s) found:" << endl;
        currentIndentLevel++;
        for (size_t i = 0; i < symbols.size(); i++) {
            indent();
            cout << "Symbol: " << symbols[i].getStr();
            indent(1);
            cout << "First Found On Line: " << symbols[i].getLn();
            indent(1);
            cout << "Index: " << symbols[i].getIx();
            indent(1);
            cout << "Double Value: " << symbols[i].getValDbl();
            indent(1);
            cout << "String Value: " << symbols[i].getValStr() << endl;
            currentIndentLevel++;
            if (symbols[i].getNumOccs() == 0) {
                indent();
                cout << "This symbol has only 1 occurrence" << endl;
            }
            else {
                indent();
                cout << symbols[i].getNumOccs() << " additional occurrence(s) found:" << endl;
                currentIndentLevel++;
                for (size_t j = 0; j < symbols[i].getNumOccs(); j++) {
                    indent();
                    cout << "Line: " << symbols[i].getOccLineNum(static_cast<int>(j));
                    indent(1);
                    cout << "Index: " << symbols[i].getOccTokenNum(static_cast<int>(j));
                    indent(1);
                    cout << "Is definition: " << symbols[i].isOccDef(static_cast<int>(j)) << endl;
                }
                currentIndentLevel--;
            }
            currentIndentLevel--;
        }
    }
//    cout << "finished printing symtable" << endl;
}
