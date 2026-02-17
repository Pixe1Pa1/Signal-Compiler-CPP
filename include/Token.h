#pragma once

struct Token {
	int code;
	int line, colum;
	Token* next;

	Token(int c, int l, int col) {
		code = c;
		line = l;
		colum = col;
		next = nullptr;
	}
};