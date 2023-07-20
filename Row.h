#ifndef _ROW_HEADER_INCLUDED_
#define _ROW_HEADER_INCLUDED_

#include "Char.h"
#include "Date.h"
#include "Double.h"
#include "Int.h"

#include <iostream>

class Row {
public:
	Row();
	Row(unsigned size);
	Row(const Row& rhs);
	Row& operator=(const Row& rhs);
	~Row();

public:
	void setEmptyRow(unsigned size);
	void set(const Type* rhs);

public:
	bool create(const char* word);
	unsigned getnColumn()const;
	void printElement(unsigned column)const;
	bool edit(unsigned column, const char* data);
	void writeToFile(ofstream& os, unsigned column)const;
	char compareRows(const Row& rhs, unsigned column)const;

public:
	const Type*& operator[](unsigned index)const;
	Type*& operator[](unsigned index);

public:
	Row& operator++();
	Row operator++(int);

public:
	unsigned getSizeofElement(unsigned column)const;
	const Type* getElement(unsigned column)const;

private:
	char findType(const char* data)const;
	char check(const char* data) const;
	unsigned numberDots(const char* data)const;

private:
	Type* TypeFactory(char type, const char* data);

private:
	void resize(unsigned newSize);
	void copy(const Row& rhs);
	void free();

private:
	Type** row;
	unsigned curColumn;
	unsigned nColumn;
};

#endif
