#include "CodeGenerator.h"
#include <iostream>
#include <cstring>

CodeGenerator::CodeGenerator(TreeN* rootNode, Tables* tablesPtr) {
	root = rootNode;
	tables = tablesPtr;
	FlagError = false;
}

void CodeGenerator::Error(const char* msg, TreeN* node) {
	if (node) {
		std::cerr << "Semantic: Error (" << node->tokenLin << "," << node->tokenCol << ") " << msg;
		const char* lexeme = tables->GetNameByCode(node->tokenCode);
		if (lexeme != nullptr) std::cerr << " '" << lexeme << "'";
		std::cerr << std::endl;
	}
	else { std::cerr << "CodeGenerator: Error " << msg << std::endl; }
	FlagError = true;
	return;
}

void CodeGenerator::Generate(const char* outputFilename) {
	SemanticPass(root);

	for (size_t i = 0; i < labelTable.size(); i++) {
		if (labelTable[i].isUsed == true && labelTable[i].isDefined == false) {
			const char* labelName = tables->GetNameByCode(labelTable[i].tokenCode);
			std::cerr << "Semantic: Error GOTO refers to label '" << labelName << "' that was never defined in the code" << std::endl;
			FlagError = true;
		}
	}

	if (FlagError) {
		Error("Code generation aborted due to semantic errors.");
		return;
	}

	if (fopen_s(&outFile, outputFilename, "w") != 0) {
		Error("Cannot open/create output file.");
		return;
	}

	fprintf(outFile, ".DATA\n");
	for (size_t i = 0; i < parameterList.size(); i++) {
		const char* paramName = tables->GetNameByCode(parameterList[i]);
		fprintf(outFile, "\t%s DD ?\n", paramName); // 4 bytes allocation
	}
	
	fprintf(outFile, ".CODE\n");
	GenerateCodePass(root);

	fprintf(outFile, "END\n");
	return;
}

void CodeGenerator::ExtractParameters(TreeN* node) {
	if (!node || strcmp(node->nodeName, "<empty>") == 0) return;

	if (node->child2 && strcmp(node->child2->nodeName, "<variable-identifier>") == 0) {
		TreeN* idNode = node->child2->child1;
		int parameterCode = idNode->tokenCode;
		bool isDuplicate = false;

		if (idNode && parameterCode != -1) {

			if (parameterCode == procedureCode) {
				Error("Parameter name matches procedure name", idNode);
			}

			for (size_t i = 0; i < parameterList.size(); i++) {
				if (parameterList[i] == parameterCode) {
					Error("Duplicate parameter name", idNode);
					isDuplicate = true;
					break;
				}
			}
			if (!isDuplicate) {
				parameterList.push_back(parameterCode);
			}
		}
	}
	ExtractParameters(node->child3);
}

void CodeGenerator::ExtractLabels(TreeN* node) {
	if (!node || strcmp(node->nodeName, "<empty>") == 0) return;

	if (node->child2 && strcmp(node->child2->nodeName, "<unsigned-integer>") == 0) {
		int newLabelCode = node->child2->tokenCode;
		bool isDuplicate = false;

		for (size_t i = 0; i < labelTable.size(); i++) {
			if (labelTable[i].tokenCode == newLabelCode) {
				std::cerr << "Semantic: Warning (" << node->child2->tokenLin << "," << node->child2->tokenCol << ") " << "Duplicate label declaration";
				std::cerr << " '" << tables->GetNameByCode(node->child2->tokenCode) << "'" << std::endl;
				isDuplicate = true;
				break;
			}
		}

		if (!isDuplicate) {
			labelTable.push_back({ newLabelCode, true });
		}
	}
	ExtractLabels(node->child3);
}

void CodeGenerator::SemanticPass(TreeN* node) {
	if (!node) return;

	if (strcmp(node->nodeName, "<program>") == 0) {
		if (node->child2 && node->child2->child1) {
			procedureCode = node->child2->child1->tokenCode;
		}
		if (node->child3) {
			ExtractParameters(node->child3);
		}
	}
	if (strcmp(node->nodeName, "<label-declarations>") == 0) {
		ExtractLabels(node);
	}
	if (strcmp(node->nodeName, "<statement>") == 0) {
		if (node->child1 && strcmp(node->child1->nodeName, "<unsigned-integer>") == 0) {
			int labelCode = node->child1->tokenCode;
			LabelInfo* lbl = nullptr;
			for (size_t i = 0; i < labelTable.size(); i++) {
				if (labelTable[i].tokenCode == labelCode) {
					lbl = &labelTable[i];
					break;
				}
			}

			if (lbl == nullptr) {
				Error("Label defined but not declared in LABEL section", node->child1);
			}
			else {
				if (lbl->isDefined) {
					Error("Label is defined more than once", node->child1);
				}
				else {
					lbl->isDefined = true;
				}
			}
		}
		else if (node->child1 && strcmp(node->child1->nodeName, "GOTO") == 0) {
			int targetCode = node->child2->tokenCode;
			LabelInfo* lbl = nullptr;
			for (size_t i = 0; i < labelTable.size(); i++) {
				if (labelTable[i].tokenCode == targetCode) {
					lbl = &labelTable[i];
					break;
				}
			}

			if (lbl == nullptr) {
				Error("GOTO refers to undeclared label", node->child2);
			}
			else {
				lbl->isUsed = true;
			}
		}
		else if (node->child1 && strcmp(node->child1->nodeName, "($") == 0) {
			TreeN* fileNode = node->child2->child1;
			const char* filename = tables->GetNameByCode(fileNode->tokenCode);
			if (filename != nullptr) {
				FILE* assemblFile;
				if (fopen_s(&assemblFile, filename, "r") != 0) {
					Error("Assembly insert file not found", fileNode);
				}
				else {
					fclose(assemblFile);
				}
			}
		}
	}

	SemanticPass(node->child1);
	SemanticPass(node->child2);
	SemanticPass(node->child3);
	SemanticPass(node->child4);
	SemanticPass(node->child5);
	SemanticPass(node->child6);
	return;
}

void CodeGenerator::GenerateCodePass(TreeN* node) {
	if (!node) return;
	
	if (strcmp(node->nodeName, "<program>") == 0) {
		const char* procName = tables->GetNameByCode(procedureCode);
		fprintf(outFile, "%s PROC\n", procName);

		for (int i = (int)parameterList.size() - 1; i >= 0; i--) {
			const char* paramName = tables->GetNameByCode(parameterList[i]);
			fprintf(outFile, "\tpop %s\n", paramName);
		}
	}
	if (strcmp(node->nodeName, "<statement>") == 0) {

		// 1: <statement>
		if (node->child1 && strcmp(node->child1->nodeName, "<unsigned-integer>") == 0) {
			const char* labelNumber = tables->GetNameByCode(node->child1->tokenCode);
			fprintf(outFile, "L_%s:\n", labelNumber);
		}

		// GOTO 1 ;
		else if (node->child1 && strcmp(node->child1->nodeName, "GOTO") == 0) {
			const char* targetNumber = tables->GetNameByCode(node->child2->tokenCode);
			fprintf(outFile, "\tjmp L_%s\n", targetNumber);
		}

		// RETURN
		else if (node->child1 && strcmp(node->child1->nodeName, "RETURN") == 0) {
			fprintf(outFile, "\tret\n");
		}
		
		// ;
		else if (node->child1 && strcmp(node->child1->nodeName, ";") == 0) {
			fprintf(outFile, "\tnop\n");
		}

		// ($ <assembly-insert-file-identifier> $)
		else if (node->child1 && strcmp(node->child1->nodeName, "($") == 0) {
			TreeN* fileNode = node->child2->child1;
			const char* filename = tables->GetNameByCode(fileNode->tokenCode);
			if (filename != nullptr) {
				FILE* assemblFile;
				if (fopen_s(&assemblFile, filename, "r") == 0) {
					int ch;
					while ((ch = fgetc(assemblFile)) != EOF) {
						fputc(ch, outFile);
					}
					fprintf(outFile, "\n");
					fclose(assemblFile);
				}
			}
		}
	}
	GenerateCodePass(node->child1);
	GenerateCodePass(node->child2);
	GenerateCodePass(node->child3);
	GenerateCodePass(node->child4);
	GenerateCodePass(node->child5);
	GenerateCodePass(node->child6);
	
	if (strcmp(node->nodeName, "<program>") == 0) {
		const char* procName = tables->GetNameByCode(procedureCode);
		fprintf(outFile, "\tret\n");
		fprintf(outFile, "%s ENDP\n", procName);
	}
	return;
}