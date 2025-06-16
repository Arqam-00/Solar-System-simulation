
#include "String.h"
//#include<iostream>
#include <fstream>
using namespace std;

int String::get_len(const char* arr) {
	int i = 0;
	for (; arr[i] != '\0'; i++) {}
	return i;
}
char* String::Regrow(int newCap) {
	if (newCap <= cap) {
		return str;
	}
	cap = newCap * 2 + 1;
	char* temp = new char[cap];

	for (int i = 0; i < len; i++) {
		temp[i] = str[i];
	}
	return temp;
}

String::String() :len{ 0 }, cap{ 0 }, str{ nullptr } {}
String::String(const char* arr) {
	this->len = get_len(arr);
	this->cap = len * 2 + 1;
	this->str = new char[cap + 1];
	for (int i = 0; i < this->len; i++) {
		this->str[i] = arr[i];
	}
	this->str[len] = '\0';
}
String::String(const String& ch) {
	this->len = ch.len;
	this->cap = ch.cap;
	this->str = new char[this->cap + 1];
	for (int i = 0; i < this->len; i++) {
		this->str[i] = ch.str[i];
	}
	this->str[len] = '\0';
}
String& String::operator = (const String& ch) {
	this->len = ch.len;
	this->cap = ch.cap;
	this->str = new char[this->cap + 1];
	for (int i = 0; i < this->len; i++) {
		this->str[i] = ch.str[i];
	}
	this->str[len] = '\0';
	return *this;
}
const char& String:: operator[](int i) const {
	if (i < 0 || i >= len) {
		cout << "Wrong input!.";
		return str[len - 1];
	}
	return str[i];
}
char& String:: operator[](int i) {
	if (i < 0 || i >= len) {
		cout << "Wrong input!.";
		return str[len - 1];
	}
	return str[i];
}
bool  String:: operator == (const String& M) {
	if (len != M.len) {
		return false;
	}
	for (int i = 0; i < len; i++) {
		if (str[i] != M.str[i]) {
			return false;
		}
	}
	return true;
}
bool String::operator > (const String& M) const {
	int Min_len;
	if (len < M.len) {
		Min_len = len;
	}
	else {
		Min_len = M.len;
	}
	for (int i = 0; i < Min_len; i++) {
		if (str[i] > M.str[i]) {
			return true;
		}
		else if (str[i] < M.str[i]) {
			return false;
		}
	}
	if (len > M.len) {
		return true;
	}
	else {
		return false;
	}
}
bool String::operator < (const String& M) const {
	int Min_len;
	if (len < M.len) {
		Min_len = len;
	}
	else {
		Min_len = M.len;
	}

	for (int i = 0; i < Min_len; i++) {
		if (str[i] < M.str[i]) {
			return true;
		}
		else if (str[i] > M.str[i]) {
			return false;
		}
	}
	if (len < M.len) {
		return true;
	}
	else {
		return false;
	}
}
String* String::split(const char delim, int& count) const {
	count = 1;
	for (int i = 0; i < len; i++) {
		if (str[i] == delim) {
			count++;
		}
	}

	String* res = new String[count];

	int start = 0;
	int part = 0;

	for (int i = 0; i <= len; i++) {
		if (str[i] == delim || str[i] == '\0') {
			int size = i - start;

			res[part] = String();
			res[part].len = size;
			res[part].cap = size * 2;
			res[part].str = new char[res[part].cap + 1];

			for (int j = 0; j < size; j++) {
				res[part].str[j] = str[start + j];
			}
			res[part].str[size] = '\0';

			part++;
			start = i + 1;
		}
	}
	return res;
}
String String::concat(const String& s2) const {
	int newLen = len + s2.len;
	int newCap = newLen * 2;

	String res;
	res.len = newLen;
	res.cap = newCap;
	res.str = new char[res.cap + 1];

	for (int i = 0; i < len; i++) {
		res.str[i] = str[i];
	}
	for (int i = 0; i < s2.len; i++) {
		res.str[len + i] = s2.str[i];
	}
	res.str[newLen] = '\0';

	return res;
}
String& String::append(const String& s2) {
	int newLen = len + s2.len;

	if (newLen > cap) {
		cap = newLen * 2;
		char* temp = new char[cap + 1];

		for (int i = 0; i < len; i++) {
			temp[i] = str[i];
		}
		delete[] str;
		str = temp;
	}
	for (int i = 0; i < s2.len; i++) {
		str[len + i] = s2.str[i];
	}
	len = newLen;
	str[len] = '\0';

	return *this;
}

String& String::operator += (const String& s2) {
	return append(s2);
}

String* String::tokenize(const char* delim, int& count) const {
	count = 1;
	for (int i = 0; i < len; i++) {
		for (int j = 0; delim[j] != '\0'; j++) {
			if (str[i] == delim[j]) {
				count++;
				break;
			}
		}
	}

	String* res = new String[count];
	int start = 0, part = 0;

	for (int i = 0; i <= len; i++) {
		bool isDelim = false;
		for (int j = 0; delim[j] != '\0'; j++) {
			if (str[i] == delim[j]) {
				isDelim = true;
				break;
			}
		}

		if (isDelim || str[i] == '\0') {
			int size = i - start;
			res[part] = String();
			res[part].len = size;
			res[part].cap = size * 2;
			res[part].str = new char[res[part].cap + 1];

			for (int j = 0; j < size; j++) {
				res[part].str[j] = str[start + j];
			}
			res[part].str[size] = '\0';

			part++;
			start = i + 1;
		}
	}
	return res;
}
int  String::find_first(char ch) {
	for (int i = 0; i < len; i++) {
		if (str[i] == ch) {
			return i;
		}
	}
	return -1;
}
int String::find_last(char ch) {
	for (int i = len - 1; i >= 0; i--) {
		if (str[i] == ch) {
			return i;
		}
	}
	return -1;
}
int* String::find_all(char ch, int& C) {
	C = 0;
	int* indices = nullptr;
	for (int i = 0; i < len; i++) {
		if (str[i] == ch) {
			C++;
		}
	}
	indices = new int[C];
	int index = 0;
	for (int i = 0; i < len; i++) {
		if (str[i] == ch) {
			index++;
			indices[index] = i;
		}
	}
	return indices;
}
int* String::all_sub_strings(const char* sub, int& count) {
	count = 0;
	int subLen = get_len(sub);
	int* indices = nullptr;
	for (int i = 0; i <= len - subLen; i++) {
		int j = 0;
		while (j < subLen && str[i + j] == sub[j]) {
			j++;
		}
		if (j == subLen) {
			count++;
		}
	}

	indices = new int[count];
	int index = 0;
	for (int i = 0; i <= len - subLen; i++) {
		int j = 0;
		while (j < subLen && str[i + j] == sub[j]) {
			j++;
		}
		if (j == subLen) {

			indices[index++] = i;
		}
	}

	return indices;
}

void String::Trim() {
	int start = 0;
	while (start < len && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n')) {
		start++;
	}
	int end = len - 1;
	while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')) {
		end--;
	}
	int newLen = end - start + 1;
	if (newLen <= 0) {
		len = 0;
		str[0] = '\0';
		return;
	}
	for (int i = 0; i < newLen; i++) {
		str[i] = str[start + i];
	}
	len = newLen;
	str[len] = '\0';
}


String::String(const float a) {
	int i = 1;
	int b = a;
	int c = a;
	bool negative = false;
	if (b < 0) {
		i++;
		b = -a;
		c = -a;
		negative = true;
	}
	while (b / 10 != 0) {
		i++;
		b /= 10;
	}
	len = i;
	cap = len * 2;
	str = new char[cap + 1];
	for (int j = i - 1; j >= 0; j--) {
		this->str[j] = '0' + (c % 10);
		c /= 10;
	}
	if (negative) {
		this->str[0] = '-';
	}
	this->str[i] = '\0';
}
String String::itos(const int a) {
	int i = 1;
	int b = a;
	int c = a;
	bool negative = false;
	if (b < 0) {
		i++;
		b = -a;
		c = -a;
		negative = true;
	}
	while (b / 10 != 0) {
		i++;
		b /= 10;
	}
	len = i;
	cap = len * 2;
	str = new char[cap + 1];
	for (int j = i - 1; j >= 0; j--) {
		this->str[j] = '0' + (c % 10);
		c /= 10;
	}
	if (negative) {
		this->str[0] = '-';
	}
	this->str[i] = '\0';
	return *this;
}
void  String::Replace_First(const char c) {
	str[0] = c;
}
int String::stoi(const String& s) {
	if (s.str == nullptr) {
		return 0;
	}
	int num = 0;
	int sign = 1;
	int i = 0;
	if (s.str[0] == '-') {
		sign = -1;
		i = 1;
	}
	for (; i < s.len; i++) {
		if (s.str[i] < '0' || s.str[i] > '9') {
			return 0;
		}
		num = num * 10 + (s.str[i] - '0');
	}
	return num * sign;
}
void String::Print() {
	cout << str;
}
void String::Shift_Right(int start, int times) {
	if (start > len - 1 || times - 1 < 0) {
		return;
	}
	for (int i = len - 1; i > start - 1; i--) {
		if (i + times < len) {
			str[i + times] = str[i];
		}
	}
	for (int i = 0; i < times && start + i < len; i++) {
		str[start + i] = ' ';
	}
	str[len] = '\0';
}

void String::Shift_Left(int start, int times) {
	if (start < 0 || times <= 0 || start >= len) {
		return;
	}
	for (int i = start; i < len; i++) {
		if (i - times >= 0) {
			str[i - times] = str[i];
		}
	}
	len -= times;
	if (len < 0) {
		len = 0;
	}
	str[len] = '\0';
}
void String::remove_at(int i) {
	if (i < 0 || i >= len) {
		return;
	}
	Shift_Left(i, 1);
}
void String::remove_first(char ch) {
	int index = find_first(ch);
	if (index != -1) {
		Shift_Left(index, 1);
	}
}
void String::remove_last(char ch) {
	int index = find_last(ch);
	if (index != -1) {
		Shift_Left(index, 1);
	}
}
void String::remove_all(char ch) {
	int C;
	int* indices = find_all(ch, C);

	if (indices) {
		for (int i = 0; i < C; i++) {
			Shift_Left(indices[i] - i, 1);
		}
		delete[] indices;
	}
}

void String::clear() {
	delete[] str;
	cap = 0;
	len = 0;
}


int String::Find_Substring(char sub[]) {
	int subLen = get_len(sub);

	for (int i = 0; i < len - subLen + 1; i++) {
		bool match = true;
		for (int j = 0; j < subLen; j++) {
			if (str[i + j] != sub[j]) {
				match = false;
				break;
			}
		}
		if (match) {
			return i;
		}
	}
	return -1;
}

void String::Delete_Substring(char sub[]) {
	int pos = Find_Substring(sub);
	if (pos != -1) {
		int lenToDel = get_len(sub);
		Shift_Left(pos, lenToDel);
	}
}

void String::Replace_Substring(char sub[], char newSub[]) {
	int lenSearch = get_len(sub);
	int lenEdited = get_len(newSub);
	int pos = Find_Substring(sub);

	if (lenEdited < lenSearch) {
		Shift_Left(pos, lenSearch - lenEdited);
	}
	else if (lenEdited > lenSearch) {
		Shift_Right(pos, lenEdited - lenSearch);
	}

	for (int i = 0; i < lenEdited; i++) {
		str[pos + i] = newSub[i];
	}
}
void String::Insert_At(int i, const char* newSub) {
	int lenEdited = get_len(newSub);
	int pos = i;
	len += lenEdited;
	char* temp = Regrow(lenEdited + cap);
	delete[] str;
	str = temp;

	Shift_Right(pos, lenEdited);

	for (int i = 0; i < lenEdited; i++) {
		str[pos + i] = newSub[i];
	}
}
void String::Insert_At(int i, char newSub) {
	int pos = i;
	len++;
	char* temp = Regrow(1 + cap);
	delete[] str;
	str = temp;
	Shift_Right(pos, 0);

	str[pos + i] = newSub;
}


ostream& operator << (ostream& os, const String& s) {
	os << s.str;
	return os;
}
istream& operator >> (istream& is, String& s) {
	char arr[100]{ '\0' };
	is.getline(arr, 100);
	s = String(arr);
	return is;
}
ifstream& operator >> (ifstream& ifs, String& s) {
	char arr[100]{ '\0' };
	ifs.getline(arr, 100);
	s = String(arr);
	return ifs;
}
const char* String::C_Str() const{
	return str;
}

String::~String() {
	delete[] str;
}
String operator + (const String& s1, const String& s2) {
	int newLen = s1.len + s2.len;
	int newCap = newLen * 2;

	String res;
	res.len = newLen;
	res.cap = newCap;
	res.str = new char[res.cap + 1];

	for (int i = 0; i < s1.len; i++) {
		res.str[i] = s1.str[i];
	}
	for (int i = 0; i < s2.len; i++) {
		res.str[s1.len + i] = s2.str[i];
	}
	res.str[newLen] = '\0';

	return res;
}
