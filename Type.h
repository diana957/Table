#ifndef _TYPE_HEADER_INCLUDED_
#define _TYPE_HEADER_INCLUDED_

#include <fstream>
#include <iostream>

using std::ofstream;

class Type {
public:
	Type() {}
	virtual ~Type() {}

public:
	virtual Type* clone()const = 0;
	virtual void print() const = 0;
	virtual char compare(const Type& rhs) const = 0;
	virtual double convertTypeToDouble() const = 0;
	virtual unsigned getSize()const = 0;
	virtual void writeToTextFile(ofstream& os)const = 0;
};

#endif
