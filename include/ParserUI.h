#pragma once
#include "Tables.h"
#include "Parser.h"

class ParserUI
{
private:
	static void PrintSingleL(const char* title, InfoTable* head);
public:
	static void PrintTree(TreeN* node, int level = 0);
	static void PrintAllL(Tables& tables);
};