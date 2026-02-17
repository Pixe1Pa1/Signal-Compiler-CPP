#pragma once

struct InfoTable {
	int code;
	char name[256];
	InfoTable* next;
	int type; // 0 - none, 1 - unsigned-integer
	InfoTable(int c, const char* n, int t = 0);
};

class Tables {
public:
	int Attributes[256];

	InfoTable* MultCharactL; // 301-400
	InfoTable* KeyWordsL;    // 401-500 
	InfoTable* ConstL;       // 501-1000
	InfoTable* IdnL;         // 1001-...

	int nextConst;
	int nextIdn;

	Tables();
	~Tables();

	int FindOrInsert(int mode, const char* name, int t = 0);
private:
	void InitAttributes();
	void InitMultCharact();
	void InitKeyWords();

	void ClearL(InfoTable* head);
};