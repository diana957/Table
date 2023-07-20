#ifndef _INT_HEADER_INCUDED_
#define _INT_HEADER_INCUDED_

#include "Type.h"

class Int : public Type {
public:
	explicit Int(int number);
	explicit Int(const char* buffer);
	virtual ~Int();

public:
	virtual Type* clone()const;
	virtual void print()const;
	virtual char compare(const Type& rhs)const;
	virtual double convertTypeToDouble() const;
	virtual unsigned getSize()const;
	virtual void writeToTextFile(ofstream& os)const;

private:
	void convert(const char* buffer);
	int setNumber(char* buffer, unsigned sizeT)const;

private:
	unsigned findSize()const;

private:
	int number;
};

#endif
