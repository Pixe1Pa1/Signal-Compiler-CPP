#pragma once
#include "Token.h"
#include "Tables.h"
#include <cstdio>

struct TreeN {
	const char* nodeName;
	int tokenCode;
	int tokenLin;
	int tokenCol;

	TreeN* child1;
	TreeN* child2;
	TreeN* child3;
	TreeN* child4;
	TreeN* child5;
	TreeN* child6;

	TreeN(const char* name) {
		nodeName = name;
		tokenCode = -1;
		tokenLin = 0;
		tokenCol = 0;
		child1 = nullptr; child2 = nullptr; 
		child3 = nullptr; child4 = nullptr; 
		child5 = nullptr; child6 = nullptr;
	}
	
	void setToken(Token* t) {
		if (t != nullptr) {
			tokenCode = t->code;
			tokenLin = t->line;
			tokenCol = t->colum; 
		}
	}
};

class Parser {
private:
	Token* currentToken;
	Tables* tables;
	TreeN* rootN;

	void Error(const char* msg);
	Token* GetNextToken();

	TreeN* PROGRAM();
	TreeN* BLOCK();
	TreeN* DECLARATIONS();
	TreeN* LABEL_DECLARATIONS();
	TreeN* LABELS_LIST();
	TreeN* PARAMETERS_LIST();
	TreeN* IDENTIFIERS_LIST();
	TreeN* STATEMENTS_LIST();
	TreeN* STATEMENT();
	TreeN* EMPTY();

	TreeN* VARIABLE_IDENTIFIER();
	TreeN* PROCEDURE_IDENTIFIER();
	TreeN* ASSEMBLY_INSERT_FILE_IDENTIFIER();
	TreeN* IDENTIFIER(int t = 0);
	TreeN* UNSIGNED_INTEGER();

	int Code();
	void DeleteT(TreeN* node);
	void setToken(Token* t);
public:
	Parser(Token* head, Tables* t);
	~Parser();

	TreeN* GetRoot() { return rootN; }
};