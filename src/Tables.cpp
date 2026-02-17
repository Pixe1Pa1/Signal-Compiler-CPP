#include "Tables.h"
#include <iostream>

InfoTable::InfoTable(int c, const char* n, int t) : code(c), next(nullptr), type(t) {
	int i = 0;
	while (n[i] != '\0' && i < 255) {
		name[i] = n[i];
		i++;
	}
	name[i] = '\0';
}

Tables::Tables()
{
	MultCharactL = nullptr;
	KeyWordsL = nullptr;
	ConstL = nullptr;
	IdnL = nullptr;

	nextConst = 501;
	nextIdn = 1001;

	InitAttributes();
	InitMultCharact();
	InitKeyWords();
}
Tables::~Tables() {
	ClearL(KeyWordsL);
	ClearL(MultCharactL);
	ClearL(ConstL);
	ClearL(IdnL);
}

void Tables::ClearL(InfoTable* head) {
	while (head != nullptr) {
		InfoTable* temp = head;
		head = head->next;
		delete temp;
	}
}

void Tables::InitAttributes() {
	for (int i = 0; i < 256; i++) {
		Attributes[i] = 5;
	}

	Attributes[8] = 0;
	Attributes[9] = 0;
	Attributes[10] = 0;
	Attributes[11] = 0;
	Attributes[12] = 0;
	Attributes[13] = 0;
	Attributes[32] = 0;

	for (int i = '0'; i <= '9'; i++) {
		Attributes[i] = 1;
	}

	for (int i = 'A'; i <= 'Z'; i++) {
		Attributes[i] = 2;
	}

	Attributes['$'] = 4;
	Attributes['('] = 4;

	Attributes[')'] = 3;
	Attributes[','] = 3;
	Attributes[':'] = 3;
	Attributes[';'] = 3;
}

void Tables::InitMultCharact() {
	MultCharactL = new InfoTable(301, "($");
	MultCharactL->next = new InfoTable(302, "$)");
}

void Tables::InitKeyWords() {
	KeyWordsL = new InfoTable(401, "PROCEDURE");
	InfoTable* P = KeyWordsL;
	P->next = new InfoTable(402, "BEGIN"); P = P->next;
	P->next = new InfoTable(403, "END"); P = P->next;
	P->next = new InfoTable(404, "LABEL"); P = P->next;
	P->next = new InfoTable(405, "GOTO"); P = P->next;
	P->next = new InfoTable(406, "RETURN"); P = P->next;
}

int Tables::FindOrInsert(int mode, const char* name, int t)
{
	InfoTable** listHead = nullptr;
	int* counter = nullptr;

	if (mode == 1) {
		// CNS
		listHead = &ConstL;
		counter = &nextConst;
	}
	else if (mode == 2) {
		// IND or Key
		InfoTable* k = KeyWordsL;
		while (k != nullptr) {
			int i = 0;
			while (name[i] != '\0' && k->name[i] != '\0' && name[i] == k->name[i])
				i++;
			if (name[i] == '\0' && k->name[i] == '\0')
				return k->code;
			k = k->next;
		}

		listHead = &IdnL;
		counter = &nextIdn;
	}
	else {
		return 0;
	}

	InfoTable* curr = *listHead;
	InfoTable* last = nullptr;

	while (curr != nullptr) {
		int i = 0;
		while (name[i] != '\0' && curr->name[i] != '\0' && name[i] == curr->name[i])
			i++;
		if (name[i] == '\0' && curr->name[i] == '\0')
			return curr->code;

		last = curr;
		curr = curr->next;
	}

	// Add
	InfoTable* newNode = new InfoTable((*counter)++, name, t);
	if (last == nullptr) {
		*listHead = newNode;
	}
	else {
		last->next = newNode;
	}
	return newNode->code;
}