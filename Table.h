#ifndef _TABLE_HEADER_INCLUDED_
#define _TABLE_HEADER_INCLUDED_

#include "Row.h"
#include <fstream>

using std::ifstream;
using std::ofstream;

struct Coordinates {
	unsigned row;
	unsigned column;
};

class Table {
public:
	Table();
	Table(const Table& rhs);
	Table& operator=(const Table& rhs);
	~Table();

public:
	bool load(const char* path);
	void print()const;
	void edit(unsigned row, unsigned column, const char* data);
	void save()const;
	void sort(unsigned column, char c)const;

public:
	void addRow();
	void addColumn();

public:
	Table& operator++();
	Table operator++(int);
	Row operator[](Coordinates c)const;

public:
	unsigned getnRow()const;

private:
	bool setRC(ifstream& is);
	bool readFromTextFile(ifstream& is);
	char* splitRow(char*& buffer);
	void setEmptyDate(unsigned row, unsigned column);

private:
	unsigned longestElement(unsigned column)const;

private:
	void copy(const Table& rhs);
	void free();

private:
	Row* table;
	unsigned nRow;
};

#endif
