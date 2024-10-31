#include "String.h"

String* newString() {
	String* str = (String*)malloc(sizeof(String));
	if (str == NULL) return NULL;

	str->end = 0;
	str->size = STR_DEFAULT_SIZE;
	str->texts = (char*)malloc(sizeof(char) * (str->size + 1));
	if (str->texts == NULL) return NULL;

	str->texts[str->end] = '\0';

	return str;
}

String* makeString(char* texts, size_t length) {
	String* str = (String*)malloc(sizeof(String));
	if (str == NULL) return NULL;

	str->end = strlen(texts);
	str->size = length;

	str->texts = (char*)malloc(sizeof(char) * (length + 1));
	if (str->texts == NULL) return NULL;

	memcpy_s(str->texts, length + 1, texts, length);
	str->texts[str->end] = '\0';

	return str;
}

String* strCpy(String* src) {
	if (src == NULL || src->texts == NULL) return NULL;

	String* str = (String*)malloc(sizeof(String));
	if (str == NULL) return NULL;

	str->texts = (char*)malloc(sizeof(char) * (src->size + 1));
	if (str->texts == NULL) return NULL;

	str->end = src->end;
	str->size = src->size;
	memcpy_s(str->texts, str->size + 1, src->texts, src->end + 1);

	return str;
}

char* strToChar(String* src) {
	if (src == NULL || src->texts == NULL) return NULL;
	char* s = (char*)malloc(sizeof(char) * (src->end + 1));
	if (s == NULL) return NULL;

	memcpy(s, src->texts, src->end);
	s[src->end] = '\0';
	return s;
}

int strAppend(String* str, char text) {
	if (str == NULL || str->texts == NULL) return NULL;

	if (str->end == str->size) {
		int flag = str_double_size(str);
		if (flag == -1) return -1;
	}
	str->texts[str->end++] = text;
	str->texts[str->end] = '\0';
	return 1;
}

int strExpandc(String* dst, char* src) {
	if (dst == NULL || src == NULL || dst->texts == NULL) return -1;

	for (int i = 0; src[i] != '\0'; i++) {
		strAppend(dst, src[i]);
	}
	dst->texts[dst->end] = '\0';
	return 1;
}

int strExpands(String* dst, String* src) {
	if (dst == NULL || src == NULL || dst->texts == NULL || src->texts == NULL) return -1;

	for (int i = 0; i < src->end; i++) {
		strAppend(dst, src->texts[i]);
	}
	dst->texts[dst->end] = '\0';
	return 1;
}

void strClear(String* str) {
	if (str == NULL || str->texts == NULL) return;

	str->end = 0;
	str->size = STR_DEFAULT_SIZE;

	char* tmp = (char*)realloc(str->texts, sizeof(char) * (str->size + 1));
	if (tmp == NULL) return;

	str->texts = tmp;
	str->texts[str->end] = '\0';
	return;
}

void strPopBack(String* str) {
	if (str == NULL || str->texts == NULL) return;

	if (str->end == 0) return;
	str->end -= 1;
	str->texts[str->end] = '\0';

	if (str->end < str->size >> 2) {
		str_reduce_size(str);
	}
}

void strFree(String* str) {
	if (str == NULL) return -1;

	free(str->texts);
	free(str);
}

int str_double_size(String* str) {
	if (str == NULL || str->texts == NULL) return -1;

	size_t dsize = str->size << 1;
	char* tmp = (char*)realloc(str->texts, dsize + 1);
	if (tmp == NULL) return -1;

	str->size = dsize;
	str->texts = tmp;
	return 1;
}

int str_reduce_size(String* str) {
	if (str == NULL || str->texts == NULL) return -1;

	size_t rsize = str->size >> 1;
	if (rsize < STR_DEFAULT_SIZE) rsize == STR_DEFAULT_SIZE;

	char* tmp = (char*)realloc(str->texts, rsize + 1);
	if (tmp == NULL) return -1;

	str->size = rsize;
	str->texts = tmp;
	return 1;
}

