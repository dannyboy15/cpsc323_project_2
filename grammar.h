#pragma once

#include <list>
#include <map>
#include "symbol.h"
#include <string>

using namespace std;

class Rule {

	symbol* lhs;

	list<symbol*> rhs;

public:
    Rule(symbol* rLhs, list<symbol*> rRhs) {
        lhs = rLhs;
        rhs = rRhs;
    }

    Rule() {}

    ~Rule() {}
    
    string printRule() {
    	string s;
    	s += lhs->getName();
    	s += " = ";
    	for (auto it = rhs.begin(); it != rhs.end(); ++it) {
    		s += (*it)->getName();
    		s += " ";
    	}

    	return s;
    }

    // list<symbol> rhsReversed() {
    //     list<symbol> reversed(rhs);
    //     reversed.reverse();
    //     return reversed;
    // }

};



class Grammar {
    // static map<string, symbol> rules;
    // static map<string, symbol*> terminals;

    void createTerminals() {
        terminals["kwdprog"] = new Terminal("kwdprog");
        terminals["kwdinput"] = new Terminal("kwdinput");
        terminals["kwdprint"] = new Terminal("kwdprint");
        terminals["kwdwhile"] = new Terminal("kwdwhile");
        terminals["kwdif"] = new Terminal("kwdif");
        terminals["kwdelseif"] = new Terminal("kwdelseif");
        terminals["kwdelse"] = new Terminal("kwdelse");
        terminals["paren1"] = new Terminal("paren1");
        terminals["paren2"] = new Terminal("paren2");
        terminals["brace1"] = new Terminal("brace1");
        terminals["brace2"] = new Terminal("brace2");
        terminals["comma"] = new Terminal("comma");
        terminals["semi"] = new Terminal("semi");
        terminals["equal"] = new Terminal("equal");
        terminals["plus"] = new Terminal("plus");
        terminals["minus"] = new Terminal("minus");
        terminals["aster"] = new Terminal("aster");
        terminals["slash"] = new Terminal("slash");
        terminals["caret"] = new Terminal("caret");
        terminals["id"] = new Terminal("id");
        terminals["int"] = new Terminal("int");
        terminals["float"] = new Terminal("float");
        terminals["string"] = new Terminal("string");
        terminals["$"] = new Terminal ("$");
        terminals["eps"] = new Terminal("eps");
    }

    void createNonTerminals() {
    	// rules[] = {nonTerminals["Opmul"], terminal["aster"] }
    	nonTerminals["Opmul"] = new NonTerminal("Opmul", 1);
		nonTerminals["Opadd"] = new NonTerminal("Opadd", 2);
		nonTerminals["Fatom"] = new NonTerminal("Fatom", 3);
		nonTerminals["Pexpr"] = new NonTerminal("Pexpr", 4);
		nonTerminals["F"] = new NonTerminal("F", 5);
		nonTerminals["T"] = new NonTerminal("T", 5);
		nonTerminals["S"] = new NonTerminal("S", 5);
		nonTerminals["E"] = new NonTerminal("E", 5);
		nonTerminals["R"] = new NonTerminal("R", 5);
		nonTerminals["Elist2"] = new NonTerminal("Elist2", 6);
		nonTerminals["Elist"] = new NonTerminal("Elist", 6);
		nonTerminals["Else2"] = new NonTerminal("Else2", 6);
		nonTerminals["Fstmt"] = new NonTerminal("Fstmt", 6);
		nonTerminals["Wstmt"] = new NonTerminal("Wstmt", 6);
		nonTerminals["Ostmt"] = new NonTerminal("Ostmt", 6);
		nonTerminals["Y"] = new NonTerminal("Y", 7);
		nonTerminals["Astmt"] = new NonTerminal("Astmt", 8);
		nonTerminals["Stmt"] = new NonTerminal("Stmt", 8);
		nonTerminals["Stmts"] = new NonTerminal("Stmts", 8);
		nonTerminals["Block"] = new NonTerminal("Block", 8);
		nonTerminals["Pgm"] = new NonTerminal("Pgm", 9);
    }

    void createRules() {
    	list<symbol*> temp;

    	rules.push_back(new Rule()); // no rule at index 0

    	// Rule 1
    	temp.push_back(terminals["kwdprog"]);
    	temp.push_back(nonTerminals["Block"]);
    	rules.push_back(new Rule(nonTerminals["Pgm"], temp));
    	temp.clear();

    	// Rule 2
    	temp.push_back(terminals["brace1"]);
    	temp.push_back(nonTerminals["Stmts"]);
    	temp.push_back(terminals["brace2"]);
    	rules.push_back(new Rule(nonTerminals["Block"], temp));
    	temp.clear();

    	// Rule 3
    	temp.push_back(nonTerminals["Stmt"]);
    	temp.push_back(terminals["semi"]);
    	temp.push_back(nonTerminals["Stmts"]);
    	rules.push_back(new Rule(nonTerminals["Stmts"], temp));
    	temp.clear();

    	// Rule 4
    	temp.push_back(terminals["eps"]);
    	rules.push_back(new Rule(nonTerminals["Stmts"], temp));
    	temp.clear();

		// Rule 5
    	temp.push_back(nonTerminals["Astmt"]);
    	rules.push_back(new Rule(nonTerminals["Stmt"], temp));
    	temp.clear();

    	// Rule 6
    	temp.push_back(nonTerminals["Ostmt"]);
    	rules.push_back(new Rule(nonTerminals["Stmt"], temp));
    	temp.clear();

    	// Rule 7
    	temp.push_back(nonTerminals["Wstmt"]);
    	rules.push_back(new Rule(nonTerminals["Stmt"], temp));
    	temp.clear();

    	// Rule 8
    	temp.push_back(nonTerminals["Fstmt"]);
    	rules.push_back(new Rule(nonTerminals["Stmt"], temp));
    	temp.clear();

    	// Rule 9
    	temp.push_back(terminals["id"]);
    	temp.push_back(terminals["equal"]);
    	temp.push_back(nonTerminals["Y"]);
    	rules.push_back(new Rule(nonTerminals["Astmt"], temp));
    	temp.clear();

    	// Rule 10
    	temp.push_back(terminals["kwdinput"]);
    	rules.push_back(new Rule(nonTerminals["Y"], temp));
    	temp.clear();

    	// Rule 11
    	temp.push_back(nonTerminals["E"]);
    	rules.push_back(new Rule(nonTerminals["Y"], temp));
    	temp.clear();

    	// Rule 12
    	temp.push_back(terminals["kwdprint"]);
    	temp.push_back(terminals["paren1"]);
    	temp.push_back(nonTerminals["Elist"]);
    	temp.push_back(terminals["paren2"]);
    	rules.push_back(new Rule(nonTerminals["Ostmt"], temp));
    	temp.clear();

    	// Rule 13
    	temp.push_back(terminals["kwdwhile"]);
    	temp.push_back(nonTerminals["Pexpr"]);
    	temp.push_back(nonTerminals["Block"]);
    	rules.push_back(new Rule(nonTerminals["Wstmt"], temp));
    	temp.clear();

    	// Rule 14
    	temp.push_back(terminals["kwdif"]);
    	temp.push_back(nonTerminals["Pexpr"]);
    	temp.push_back(nonTerminals["Block"]);
    	temp.push_back(nonTerminals["Else2"]);
    	rules.push_back(new Rule(nonTerminals["Fstmt"], temp));
    	temp.clear();

    	// Rule 15
    	temp.push_back(terminals["kwdelseif"]);
    	temp.push_back(nonTerminals["Pexpr"]);
    	temp.push_back(nonTerminals["Block"]);
    	temp.push_back(nonTerminals["Else2"]);
    	rules.push_back(new Rule(nonTerminals["Else2"], temp));
    	temp.clear();

    	// Rule 16
    	temp.push_back(terminals["kwdelse"]);
    	temp.push_back(nonTerminals["Block"]);
    	rules.push_back(new Rule(nonTerminals["Else2"], temp));
    	temp.clear();

    	// Rule 17
    	temp.push_back(terminals["eps"]);
    	rules.push_back(new Rule(nonTerminals["Else2"], temp));
    	temp.clear();

    	// Rule 18
    	temp.push_back(nonTerminals["E"]);
    	temp.push_back(nonTerminals["Elist2"]);
    	rules.push_back(new Rule(nonTerminals["Elist"], temp));
    	temp.clear();

    	// Rule 19
    	temp.push_back(terminals["eps"]);
    	rules.push_back(new Rule(nonTerminals["Elist"], temp));
    	temp.clear();

    	// Rule 20
    	temp.push_back(terminals["comma"]);
    	temp.push_back(nonTerminals["Elist"]);
    	rules.push_back(new Rule(nonTerminals["Elist2"], temp));
    	temp.clear();

    	// Rule 21
    	temp.push_back(nonTerminals["Opadd"]);
    	temp.push_back(nonTerminals["T"]);
    	temp.push_back(nonTerminals["R"]);
    	rules.push_back(new Rule(nonTerminals["R"], temp));
    	temp.clear();

    	// Rule 22
    	temp.push_back(nonTerminals["T"]);
    	temp.push_back(nonTerminals["R"]);
    	rules.push_back(new Rule(nonTerminals["E"], temp));
    	temp.clear();

    	// Rule 23
    	temp.push_back(terminals["eps"]);
    	rules.push_back(new Rule(nonTerminals["R"], temp));
    	temp.clear();

    	// Rule 24
    	temp.push_back(nonTerminals["Opmul"]);
    	temp.push_back(nonTerminals["F"]);
    	temp.push_back(nonTerminals["S"]);
    	rules.push_back(new Rule(nonTerminals["S"], temp));
    	temp.clear();

    	// Rule 25
    	temp.push_back(nonTerminals["F"]);
    	temp.push_back(nonTerminals["S"]);
    	rules.push_back(new Rule(nonTerminals["T"], temp));
    	temp.clear();

    	// Rule 26
    	temp.push_back(terminals["eps"]);
    	rules.push_back(new Rule(nonTerminals["S"], temp));
    	temp.clear();

    	// Rule 27
    	temp.push_back(nonTerminals["Fatom"]);
    	rules.push_back(new Rule(nonTerminals["F"], temp));
    	temp.clear();

    	// Rule 28
    	temp.push_back(nonTerminals["Pexpr"]);
    	rules.push_back(new Rule(nonTerminals["F"], temp));
    	temp.clear();

    	// Rule 29
    	temp.push_back(terminals["paren1"]);
    	temp.push_back(nonTerminals["E"]);
    	temp.push_back(terminals["paren2"]);
    	rules.push_back(new Rule(nonTerminals["Pexpr"], temp));
    	temp.clear();

    	// Rule 30
    	temp.push_back(terminals["id"]);
    	rules.push_back(new Rule(nonTerminals["Fatom"], temp));
    	temp.clear();

    	// Rule 31
    	temp.push_back(terminals["int"]);
    	rules.push_back(new Rule(nonTerminals["Fatom"], temp));
    	temp.clear();

    	// Rule 32
    	temp.push_back(terminals["float"]);
    	rules.push_back(new Rule(nonTerminals["Fatom"], temp));
    	temp.clear();

    	// Rule 33
    	temp.push_back(terminals["string"]);
    	rules.push_back(new Rule(nonTerminals["Fatom"], temp));
    	temp.clear();

    	// Rule 34
    	temp.push_back(terminals["plus"]);
    	rules.push_back(new Rule(nonTerminals["Opadd"], temp));
    	temp.clear();

    	// Rule 35
    	temp.push_back(terminals["minus"]);
    	rules.push_back(new Rule(nonTerminals["Opadd"], temp));
    	temp.clear();

    	// Rule 36
    	temp.push_back(terminals["aster"]);
    	rules.push_back(new Rule(nonTerminals["Opmul"], temp));
    	temp.clear();

    	// Rule 37
    	temp.push_back(terminals["slash"]);
    	rules.push_back(new Rule(nonTerminals["Opmul"], temp));
    	temp.clear();

    	// Rule 38
    	temp.push_back(terminals["caret"]);
    	rules.push_back(new Rule(nonTerminals["Opmul"], temp));
    	temp.clear();
    }


public:

    map<string, symbol*> terminals;
    map<string, symbol*> nonTerminals;
    list<Rule*> rules;

    Grammar() {
        createTerminals();
        createNonTerminals();
        createRules();
    }

    ~Grammar() {}

    Rule getRuleAt(string top, string next) {
        return Rule();
    }

    symbol getRule(string rule) {
        return *terminals[rule];
    }

};









	/*const array<Rule,25> rules = {	------ERROR
		"kwdprog",
		"kwdinput",
		"kwdprint",
		"kwdwhile",
		"kwdif",
		"kwdelseif",
		"kwdelse",
		"paren1",
		"paren2",
		"brace1",
		"brace2",
		"comma",
		"semi",
		"equal",
		"plus",
		"minus",
		"aster",
		"slash",
		"caret",
		'id',
		'int',
		"float",
		"string",
		'$'
	};*/

 /*void createRules() {										------ERROR
    rules["Opmul"] = Terminal("Opmul");
    rules["Opadd"] = Terminal("Opadd");
    rules["Fatom"] =   NonTerminal("Fatom",    );
    rules["Pexpr"] =   NonTerminal("Pexpr",    );
    rules['F'] =   NonTerminal("F",    );
    rules['T'] =   NonTerminal("T",    );
    rules['S'] =   NonTerminal("S",    );
    rules['E'] =   NonTerminal("E",    );
    rules['R'] =   NonTerminal("R",    );
    rules["Elist2"]    =   NonTerminal("Elist2",   );
    rules["Elist"] =   NonTerminal("Elist",    );
    rules["Else2"] =   NonTerminal("Else2",    );
    rules["Fstmt"] =   NonTerminal("Fstmt",    );
    rules["Wstmt"] =   NonTerminal("Wstmt",    );
    rules["Ostmt"] =   NonTerminal("Ostmt",    );
    rules['Y'] =   NonTerminal("Y",    );
    rules["Astmt"] =   NonTerminal("Astmt",    );
    rules["Stmt"]  =   NonTerminal("Stmt", );
    rules["Stmts"]  =   NonTerminal("Stmts",    );
    rules["Block"] =   NonTerminal("Block",    );
    rules["Pgm"]   =   NonTerminal("Pgm",  {Terminal("kwdp"),  NonTerminal("Block")});




    rules['Pgm'] = NonTerminal("Pgm", {Terminal("kwdp"), NonTerminal("Block")});
    rules["Block"] = NonTerminal("Block", );
    rules["Stmts"] = NonTerminal("Stmts", );
    rules["Stmt"] = NonTerminal("Stmt", );
    rules["Astmt"] = NonTerminal("Astmt", );
    rules['Y'] = NonTerminal("Y", );
    rules["Ostmt"] = NonTerminal("Ostmt", );
    rules["Wstmt"] = NonTerminal("Wstmt", );
    rules["Fstmt"] = NonTerminal("Fstmt", );
    rules["Else2"] = NonTerminal("Else2", );
    rules["Elist"] = NonTerminal("Elist", );
    rules["Elist2"] = NonTerminal("Elist2", );
    rules['R'] = NonTerminal("R", );
    rules['E'] = NonTerminal("E", );
    rules['S'] = NonTerminal("S", );
    rules['T'] = NonTerminal("T", );
    rules['F'] = NonTerminal("F", );
    rules["Pexpr"] = NonTerminal("Pexpr", );
    rules["Fatom"] = NonTerminal("Fatom", );
    rules["Opadd"] = NonTerminal("Opadd", );
    rules["Opmul"] = NonTerminal("Opmul", );
 }  */
