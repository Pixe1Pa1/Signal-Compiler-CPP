#include <iostream>
#include "Tables.h"
#include "Lexer.h"
#include "LexerUI.h"

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


	Token* p = TokensL;
	while (p) {
		Token* temp = p;
		p = p->next;
		delete temp;
	}
	system("pause");
	return 0;
}

