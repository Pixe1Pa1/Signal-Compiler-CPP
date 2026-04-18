#include "Parser.h"
#include <iostream>

Parser::Parser(Token* head, Tables* t)
{
	currentToken = head;
	tables = t;
	rootN = new TreeN("<signal-program>");
	rootN->child1 = PROGRAM();
	
	if (currentToken != nullptr) {
		Error("Tokens after the end of the program");
	}
}

Parser::~Parser()
{
	DeleteT(rootN);
}

void Parser::DeleteT(TreeN* node) {
	if (node == nullptr) return;
	DeleteT(node->child1);
	DeleteT(node->child2);
	DeleteT(node->child3);
	DeleteT(node->child4);
	DeleteT(node->child5);
	DeleteT(node->child6);
	delete node;
}

void Parser::Error(const char* msg)
{
	if (currentToken) {
		std::cerr << "Parser: Error" << "(" << currentToken->line << "," << currentToken->colum << ") " << msg << std::endl;
	}
	else { std::cerr << "Parser: Error Unexpected End of File" << msg << std::endl; }
	system("pause");
	exit(1);
}

Token* Parser::GetNextToken()
{
	if (currentToken == nullptr) return nullptr;
	Token* result = currentToken;
	currentToken = currentToken->next;
	return result;
}

TreeN* Parser::PROGRAM()
{
	TreeN* node = new TreeN("<program>");
	if (Code() == 401) {
		node->child1 = new TreeN("PROCEDURE");
		node->child1->setToken(currentToken);
		GetNextToken();
		node->child2 = PROCEDURE_IDENTIFIER();
		node->child3 = PARAMETERS_LIST();
		if (Code() == ';') {
			node->child4 = new TreeN(";");
			node->child4->setToken(currentToken);
			GetNextToken();
			node->child5 = BLOCK();
			if (Code() == ';') {
				node->child6= new TreeN(";");
				node->child6->setToken(currentToken);
				GetNextToken();
			}
			else { Error("<program> Not Single-character separator ';'"); }
		}
		else { Error("<program> Not Single-character separator ';'"); }
	}
	else { Error("<program> Not KEYWORD 'PROCEDURE'"); }
	return node;
}

TreeN* Parser::BLOCK()
{
	TreeN* node = new TreeN("<block>");
	node->child1 = DECLARATIONS();
	if (Code() == 402) {
		node->child2 = new TreeN("BEGIN");
		node->child2->setToken(currentToken);
		GetNextToken();
		node->child3 = STATEMENTS_LIST();
		if (Code() == 403) {
			node->child4 = new TreeN("END");
			node->child4->setToken(currentToken);
			GetNextToken();
		}
		else { Error("<block> Not KEYWORD 'END'"); }
	}
	else { Error("<block> Not KEYWORD 'BEGIN'"); }
	return node;
}

TreeN* Parser::DECLARATIONS()
{
	TreeN* node = new TreeN("<declarations>");
	node->child1 = LABEL_DECLARATIONS();
	return node;
}

TreeN* Parser::LABEL_DECLARATIONS()
{
	TreeN* node = new TreeN("<label-declarations>");
	if (Code() == 404) {
		node->child1 = new TreeN("LABEL");
		node->child1->setToken(currentToken);
		GetNextToken();
		node->child2 = UNSIGNED_INTEGER();
		node->child3 = LABELS_LIST();
		if (Code() == ';') {
			node->child4 = new TreeN(";");
			node->child4->setToken(currentToken);
			GetNextToken();
		}
		else { Error("<label-declarations> Not Single-character separator ';'"); }
	}
	else { node->child1 = EMPTY(); }

	return node;
}

TreeN* Parser::LABELS_LIST()
{
	TreeN* node = new TreeN("<labels-list>");
	if (Code() == ',') {
		node->child1 = new TreeN(",");
		node->child1->setToken(currentToken);
		GetNextToken();
		node->child2 = UNSIGNED_INTEGER();
		node->child3 = LABELS_LIST();
	}
	else { node->child1 = EMPTY(); }
	return node;
}

TreeN* Parser::PARAMETERS_LIST()
{
	TreeN* node = new TreeN("<parameters-list>");
	if (Code() == '(') {
		node->child1 = new TreeN("(");
		node->child1->setToken(currentToken);
		GetNextToken();
		node->child2 = VARIABLE_IDENTIFIER();
		node->child3 = IDENTIFIERS_LIST();
		if (Code() == ')') {
			node->child4 = new TreeN(")");
			node->child4->setToken(currentToken);
			GetNextToken();
		}
		else { Error("<parameters-list> Not Single-character separator ')'"); }
	}
	else { node->child1 = EMPTY(); }
	return node;
}

TreeN* Parser::IDENTIFIERS_LIST()
{
	TreeN* node = new TreeN("<identifiers-list>");
	if (Code() == ',') {
		node->child1 = new TreeN(",");
		node->child1->setToken(currentToken);
		GetNextToken();
		node->child2 = VARIABLE_IDENTIFIER();
		node->child3 = IDENTIFIERS_LIST();
	}
	else { node->child1 = EMPTY(); }
	return node;
}

TreeN* Parser::STATEMENTS_LIST()
{
	TreeN* node = new TreeN("<statements-list>");
	if ((Code() >= 501 && Code() <= 1000) || Code() == 405 || Code() == 406 || Code() == 301 || Code() == ';') {
		node->child1 = STATEMENT();
		node->child2 = STATEMENTS_LIST();
	}
	else { node->child1 = EMPTY(); }
	return node;
}

TreeN* Parser::STATEMENT()
{
	TreeN* node = new TreeN("<statement>");
	if (Code() >= 501 && Code() <= 1000) {
		node->child1 = UNSIGNED_INTEGER();
		if (Code() == ':') {
			node->child2 = new TreeN(":");
			node->child2->setToken(currentToken);
			GetNextToken();
			node->child3 = STATEMENT();
		}
		else { Error("<statement> Not Single-character separator ':'"); }
	}
	else if (Code() == 405) {
		node->child1 = new TreeN("GOTO");
		node->child1->setToken(currentToken);
		GetNextToken();
		node->child2 = UNSIGNED_INTEGER();
		if (Code() == ';') {
			node->child3 = new TreeN(";");
			node->child3->setToken(currentToken);
			GetNextToken();
		}
		else { Error("<statement> Not Single-character separator ';'"); }
	}
	else if (Code() == 406) {
		node->child1 = new TreeN("RETURN");
		node->child1->setToken(currentToken);
		GetNextToken();
		if (Code() == ';') {
			node->child2 = new TreeN(";");
			node->child2->setToken(currentToken);
			GetNextToken();
		}
		else { Error("<statement> Not Single-character separator ';'"); }
	}
	else if (Code() == ';') {
		node->child1 = new TreeN(";");
		node->child1->setToken(currentToken);
		GetNextToken();
	}
	else if (Code() == 301) {
		node->child1 = new TreeN("($");
		node->child1->setToken(currentToken);
		GetNextToken();
		node->child2 = ASSEMBLY_INSERT_FILE_IDENTIFIER();
		if (Code() == 302) {
			node->child3 = new TreeN("$)");
			node->child3->setToken(currentToken);
			GetNextToken();
		}
		else { Error("<statement> Not Separator '$)'"); }
	}
	else { Error("<statement> Not Statement"); }
	return node;
}

TreeN* Parser::EMPTY()
{
	TreeN* node = new TreeN("<empty>");
	return node;
}

TreeN* Parser::VARIABLE_IDENTIFIER()
{
	TreeN* node = new TreeN("<variable-identifier>");
	node->child1 = IDENTIFIER(2);
	return node;
}

TreeN* Parser::PROCEDURE_IDENTIFIER()
{
	TreeN* node = new TreeN("<procedure-identifier>");
	node->child1 = IDENTIFIER(3);
	return node;
}

TreeN* Parser::ASSEMBLY_INSERT_FILE_IDENTIFIER()
{
	TreeN* node = new TreeN("<assembly-insert-file-identifier>");
	node->child1 = IDENTIFIER(4);
	return node;
}

TreeN* Parser::IDENTIFIER(int t)
{
	TreeN* node = new TreeN("<identifier>");
	if (Code() >= 1001) {
		node->setToken(currentToken);
		InfoTable* current = tables->IdnL;
		while (current != nullptr) {
			if (current->code ==Code()) {
				current->type = t;
				break;
			}
			current = current->next;
		}
		GetNextToken();
	}
	else { Error("Not IDENTIFIER"); }
	return node;
}

TreeN* Parser::UNSIGNED_INTEGER()
{
	TreeN* node = new TreeN("<unsigned-integer>");
	if (Code() >= 501 && Code() <= 1000) {
		node->setToken(currentToken);
		GetNextToken();
	}
	else { Error("Not UNSIGNED_INTEGER"); }
	return node;
}

int Parser::Code()
{
	return currentToken ? currentToken->code : -1;
}
