#pragma once
#include "Parser.h"
#include "Tables.h"
#include <cstdio>
#include <vector>

struct LabelInfo {
    int tokenCode;
    bool isDeclared = false;
    bool isDefined = false;
    bool isUsed = false;
};

class CodeGenerator {
private:
    TreeN* root;
    Tables* tables;
    FILE* outFile;
    bool FlagError;

    int procedureCode;
    std::vector<int> parameterList;
    void ExtractParameters(TreeN* node);

    void ExtractLabels(TreeN* node);
    std::vector<LabelInfo> labelTable;

    void Error(const char* msg, TreeN* node = nullptr);
    void SemanticPass(TreeN* node);
    void GenerateCodePass(TreeN* node);
public:
    CodeGenerator(TreeN* rootNode, Tables* tablesPtr);
    void Generate(const char* outputFilename);
};