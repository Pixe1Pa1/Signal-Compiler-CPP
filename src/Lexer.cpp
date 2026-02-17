#include "Lexer.h"
#include <iostream>

Lexer::Lexer(const char* file, Tables* t) {
	tables = t;
	tokenHead = nullptr;
	tokenEnd = nullptr;
	if (fopen_s(&FINP, file, "r") != 0) {
		std::cerr << "Error: Could not open file" << std::endl;
		system("pause");
		exit(1);
	}
	currentToken = new Token(0, 0, 0);
	line = 1;
	colum = 0;
	Gets();
}

Lexer::~Lexer() {
	Token* P = tokenHead;
	while (P != nullptr) {
		Token* temp = P;
		P = P->next;
		delete temp;
	}
	if (currentToken != nullptr) {
		delete currentToken;
	}
	if (FINP != nullptr) {
		fclose(FINP);
	}
}

void Lexer::Gets()
{
	symbol = fgetc(FINP);
	currentToken->code = symbol;
	currentToken->line = line;
	currentToken->colum = colum;

	currentAttr = tables->Attributes[(unsigned char)symbol];
	if (symbol == '\n') {
		line++;
		colum = 0;
	}
	else if (symbol == '\t') {
		colum += tab;
	}
	else {
		colum++;
	}
}

void Lexer::Error(const char* msg, int lin, int col, int t)
{
	char s = (t != -1) ? (char)t : (char)symbol;
	std::cerr << "Lexer: Error " << "(" << lin << "," << col << ") " << msg << " '" << s << "'" << std::endl;
}

void Lexer::AddTokenEnd(int code, int lin, int col)
{
	Token* newToken = new Token(code, lin, col);
	if (tokenHead == nullptr) {
		tokenHead = newToken;
		tokenEnd = newToken;
	}
	else {
		tokenEnd->next = newToken;
		tokenEnd = newToken;
	}
}

void Lexer::scan()
{
	int i = 0;
	int code = 0;
	int startLine = 0;
	int startColumn = 0;
	bool tooLong = false;
	while (symbol != EOF) {
		i = 0;
		startLine = line;
		startColumn = colum;
		tooLong = false;
		switch (currentAttr) {
		case 0:
			Gets();
			break;
		case 1:
			while (currentAttr == 1) {
				if (i < 255) buf[i++] = symbol;
				else tooLong = true;
				Gets();
			}
			buf[i] = '\0';
			if (tooLong == true) {
				Error("The Constant is too large", startLine, startColumn);
				tooLong = false;
			}
			code = tables->FindOrInsert(1, buf, 1);
			AddTokenEnd(code, startLine, startColumn);
			break;
		case 2:
			while (currentAttr == 2 || currentAttr == 1) {
				if (i < 255) buf[i++] = symbol;
				else tooLong = true;
				Gets();
			}
			buf[i] = '\0';
			if (tooLong == true) {
				Error("The Identifier is too large", startLine, startColumn);
				tooLong = false;
			}
			code = tables->FindOrInsert(2, buf);
			AddTokenEnd(code, startLine, startColumn);
			break;
		case 3:
			AddTokenEnd(symbol, startLine, startColumn);
			Gets();
			break;
		case 4:
			switch (symbol) {
			case '(':
				Gets();
				switch (symbol) {
				case '*':
					Gets();
					//delete com
					while (true) {
						if (symbol == EOF) {
							Error("Comment not closed", startLine, startColumn, '(');
							return;
						}
						else if (symbol == '*') {
							Gets();
							if (symbol == ')')
								break;
						}
						else Gets();
					}
					Gets();
					break;
				case '$':
					AddTokenEnd(301, startLine, startColumn);
					Gets();
					break;
				default:
					AddTokenEnd('(', startLine, startColumn);
					break;
				}
				break;
			case '$':
				Gets();
				if (symbol == ')') {
					AddTokenEnd(302, startLine, startColumn);
					Gets();
				}
				else {
					Error("Illegal symbol", startLine, startColumn, '$');
				}
				break;
			}
			break;
		case 5:
			Error("Illegal symbol", startLine, startColumn);
			Gets();
			break;
		default:
			Gets();
			break;
		}
	}
}

Token* Lexer::GetTokenList()
{
	Token* head = tokenHead;
	tokenHead = nullptr;
	tokenEnd = nullptr;
	return head;
}
