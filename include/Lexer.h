#pragma once
#include "Token.h"
#include "Tables.h"
#include <cstdio>

class Lexer {
private:
	FILE* FINP;
	Tables* tables;

	Token* currentToken;
	Token* tokenHead;
	Token* tokenEnd;

	int symbol;
	char buf[256];
	int line;
	int colum;
	int currentAttr;
	int tab = 4;
	void Gets();
	void Error(const char* msg, int lin, int col, int t = -1);
	void AddTokenEnd(int code, int lin, int col);

public:
	Lexer(const char* file, Tables* t);
	~Lexer();

	void scan();
	Token* GetTokenList();
};