#include "LexerUI.h"
#include <iostream>

void LexerUI::PrintAllL(Tables& tables) {
	PrintSingleL("Identifiers:", tables.IdnL);
	std::cout << std::endl;
	PrintSingleL("Constants:", tables.ConstL);
}

void LexerUI::PrintSingleL(const char* title, InfoTable* head) {
	std::cout << title << std::endl;
	std::cout << "Code\tLexem\tType" << std::endl;
	InfoTable* current = head;
	while (current != nullptr) {
		std::cout << current->code << "\t" << current->name << "\t";
		if (current->type == 1) std::cout << "unsigned-integer";
		else std::cout << "-";
		std::cout << std::endl;
		current = current->next;
	}
}

void LexerUI::PrintTokensL(Token* head, Tables& tables)
{
	std::cout << "Result Lexem List:" << std::endl;
	std::cout << "Row\tCol\tCode\tLexem" << std::endl << std::endl;

	Token* current = head;

	while (current != nullptr) {
		std::cout << current->line << "\t" << current->colum << "\t" << current->code << "\t";

		if (current->code < 256)
			std::cout << (char)current->code; // ASCII
		else {
			const char* name = GetLexemeName(current->code, &tables);
			if (name != nullptr)
				std::cout << name;
		}
		std::cout << std::endl;
		current = current->next;
	}
	std::cout << std::endl;
}

const char* LexerUI::FindInL(InfoTable* head, int code) {
	while (head != nullptr) {
		if (head->code == code) {
			return head->name;
		}
		head = head->next;
	}
	return nullptr;
}

const char* LexerUI::GetLexemeName(int code, Tables* tables) {
	const char* name = nullptr;

	if (code >= 301 && code <= 400) {
		name = FindInL(tables->MultCharactL, code);
	}
	else if (code >= 401 && code <= 500) {
		name = FindInL(tables->KeyWordsL, code);
	}
	else if (code >= 501 && code <= 1000) {
		name = FindInL(tables->ConstL, code);
	}
	else if (code >= 1001) {
		name = FindInL(tables->IdnL, code);
	}
	return name;
}

