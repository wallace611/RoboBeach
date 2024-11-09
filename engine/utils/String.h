#pragma once

#include <string.h>
#include <stdlib.h>

#define STR_DEFAULT_SIZE 16

typedef struct _str String;

struct _str {
	char* texts;
	size_t size;
	size_t end;
};

String* newString();
String* makeString(char* texts, size_t length);
String* strCpy(String* src);
char* strToChar(String* src);
int strAppend(String* str, char text);
int strExpandc(String* dst, char* src);
int strExpands(String* dst, String* src);
void strClear(String* str);
void strPopBack(String* str);
void strFree(String* str);

static int str_double_size(String* str);
static int str_reduce_size(String* str);