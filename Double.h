#ifndef _DOUBLE_HEADER_INCLUDED_
#define _DOUBLE_HEADER_INCLUDED_

#include "Type.h"

class Double : public Type {
public:
	explicit Double(const char* buffer);
	virtual ~Double();

public:
	virtual Type* clone()const;
	virtual void print() const;
	virtual char compare(const Type& rhs) const; 
	virtual double convertTypeToDouble() const;
	virtual unsigned getSize()const;
	virtual void writeToTextFile(ofstream& os)const;

private:
	void convert(const char* buffer);
	int setNumber(char*& buffer, unsigned sizeT)const;
	unsigned cntNumbers(const char* buffer)const;

private:
	bool check(const char* buffer)const;

private:
	double number;
	unsigned size;
};

#endif
