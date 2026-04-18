#include "ParserUI.h"
#include <iostream>

void ParserUI::PrintAllL(Tables& tables) {
	std::cout << std::endl;
	PrintSingleL("Identifiers:", tables.IdnL);
	std::cout << std::endl;
	PrintSingleL("Constants:", tables.ConstL);
}

void ParserUI::PrintSingleL(const char* title, InfoTable* head) {
	std::cout << title << std::endl;
	std::cout << "Code\tLexem\tType" << std::endl;
	InfoTable* current = head;
	while (current != nullptr) {
		std::cout << current->code << "\t" << current->name << "\t";
		if (current->type == 1) std::cout << "unsigned-integer";
		else if (current->type == 2) std::cout << "variable-identifier";
		else if (current->type == 3) std::cout << "procedure-identifier";
		else if (current->type == 4) std::cout << "assembly-insert-file-identifier";
		else std::cout << "-";
		std::cout << std::endl;
		current = current->next;
	}
}

void ParserUI::PrintTree(TreeN* node, int level)
{
	if (node == nullptr) return;
	for (int i = 0; i < level; ++i) 
		std::cout << "  |";
	std::cout << "--";
	
	if (node->tokenCode != -1)
		std::cout << " " << node->tokenCode << " ";
	std::cout << node->nodeName;
	std::cout << std::endl;

	PrintTree(node->child1, level + 1);
	PrintTree(node->child2, level + 1);
	PrintTree(node->child3, level + 1);
	PrintTree(node->child4, level + 1);
	PrintTree(node->child5, level + 1);
	PrintTree(node->child6, level + 1);
}