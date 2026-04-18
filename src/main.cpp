#include <iostream>
#include "Tables.h"
#include "Lexer.h"
#include "LexerUI.h"
#include "Parser.h"
#include "ParserUI.h"

int main() {
	Tables tables;
	Token* TokensL = nullptr;

	char filename[256];
	std::cout << "Test file name (e.g test1.txt): ";
	std::cin >> filename;
	{
		Lexer lexertest1(filename, &tables);
		lexertest1.scan();
		TokensL = lexertest1.GetTokenList();
	}

	LexerUI::PrintTokensL(TokensL, tables);
	LexerUI::PrintAllL(tables);
	std::cout << std::endl << "Result Parse tree:" << std::endl;
	TreeN* Root = nullptr;
	Parser parser1(TokensL, &tables);
	Root = parser1.GetRoot();
	
	Token* p = TokensL;
	while (p) {
		Token* temp = p;
		p = p->next;
		delete temp;
	}

	ParserUI::PrintTree(Root);
	ParserUI::PrintAllL(tables);
	system("pause");
	return 0;
}

