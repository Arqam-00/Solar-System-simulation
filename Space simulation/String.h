#include <iostream>
using namespace std;

#ifndef STRING_H
#define STRING_H
class String {
	int len;
	int cap;
	char* str;
	int get_len(const char* arr);
	char* Regrow(int newCap);


public:
	String();
	String(const char* arr);
	String(const String& ch);
	String& operator = (const String& ch);
	const char& operator[](int i) const;
	char& operator[](int i);
	bool operator == (const String& M);
	friend String operator + (const String& s1, const String& s2);
	bool operator < (const String& M) const;
	bool operator > (const String& M) const;

	String* split(const char delim, int& count) const;
	String concat(const String& s2) const;
	String& append(const String& s2);

	String& operator += (const String& s2);
	String* tokenize(const char* delim, int& count) const;

	int find_first(char ch);
	int find_last(char ch);
	int* find_all(char ch, int& C);
	int* all_sub_strings(const char* sub, int& count);
	void Trim();

	void Shift_Right(int start, int times);
	void Shift_Left(int start, int times);
	void remove_at(int i);
	void remove_first(char ch);
	void remove_last(char ch);
	void remove_all(char ch);

	int Find_Substring(char sub[]);
	void Delete_Substring(char sub[]);
	void Replace_Substring(char sub[], char newSub[]);
	void Insert_At(int i, const char* newSub);
	void Insert_At(int i, char newSub);

	String(const int a);
	String itos(const int a);
	void Replace_First(const char c);
	int stoi(const String& s);
	void clear();
	const char* C_Str() const;

	friend ostream& operator << (ostream& os, const String& s);
	friend istream& operator >> (istream& is, String& s);
	void Print();

	~String();
};
#endif