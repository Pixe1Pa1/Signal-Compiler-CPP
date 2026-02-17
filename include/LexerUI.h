#pragma once
#include "Tables.h"
#include "Lexer.h"

class LexerUI
{
private:
	static const char* FindInL(InfoTable* head, int code);
	static const char* GetLexemeName(int code, Tables* tables);
	static void PrintSingleL(const char* title, InfoTable* head);
public:
	static void PrintAllL(Tables& tables);
	static void PrintTokensL(Token* head, Tables& tables);
};

