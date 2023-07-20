#ifndef _DATE_HEADER_INCLUDED_
#define _DATE_HEADER_INCLUDED_

#include "Type.h"
#include "Int.h"

class Date : public Type {
public:
	explicit Date(const char* buffer);
	virtual ~Date();

public:
	virtual Type* clone()const;
	virtual void print() const;
	virtual char compare(const Type& rhs) const;
	virtual double convertTypeToDouble()const;
	virtual unsigned getSize()const;
	virtual void writeToTextFile(ofstream& os)const;

private:
	void convert(const char* buffer);
	unsigned set(char* buffer, unsigned sizeT);

private:
	bool validDate()const;
	bool isLeapYear(int year)const;
	bool checkDay(int day)const;
	bool checkDots(const char* buffer)const;

private:
	unsigned monthTodays(unsigned difference) const;
	bool isEqual(const Date& rhs)const;
	char compareDays(const Date& rhs)const;

private:
	unsigned day;
	unsigned month;
	unsigned year;
};

#endif
