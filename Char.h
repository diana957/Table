#ifndef _CHAR_HEADER_INCLUDED_
#define _CHAR_HEADER_INCLUDED_

#include "Type.h"

class Char : public Type {
public:
	explicit Char(const char* buffer);
	Char(const Char& rhs);
	Char& operator=(const Char& rhs);
	virtual ~Char();

public:
	virtual Type* clone()const;
	virtual void print()const;
	virtual char compare(const Type& rhs) const;
	virtual double convertTypeToDouble()const;
	virtual unsigned getSize()const;
	virtual void writeToTextFile(ofstream& os)const;

private:
	void convert(const char* buffer);
	int checkLines(const char* buffer)const;
	void withOutSpaceEnd();

private:
	void copy(const Char& rhs);
	void free();

private:
	char* word;
};

#endif
