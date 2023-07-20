#include "Double.h"
#include <iostream>
#include <exception>
#include <iomanip> //for setprecision

using std::cout;

Double::Double(const char* buffer)
	: Type()
	, number(0.0) {
	if (!check(buffer))
		throw std::invalid_argument("Invalid argument is entered!!!\n");
	convert(buffer);
	size = strlen(buffer);
}

Double::~Double() {}

Type* Double::clone()const {
	return new Double(*this);
}

void Double::print() const {
	cout << std::setprecision(size) << number;
}

char Double::compare(const Type& rhs)const {
	const Double* ptr = dynamic_cast<const Double*>(&rhs);
	if (ptr) {
		if (number > ptr->number) return 'g';
		else if (number < ptr->number) return 'l';
		else return 'e';
	}
	return 'm';
}

double Double::convertTypeToDouble() const {
	return number;
}

unsigned Double::getSize()const {
	return size;
}

void Double::writeToTextFile(ofstream& os)const {
	os << std::setprecision(size) << number;
}

void Double::convert(const char* buffer) {
	char* buff = new (std::nothrow) char[strlen(buffer) + 1];
	if (!buff)
		throw std::bad_alloc();
	strcpy(buff, buffer);

	bool isNegative = false;
	if (*buff == '-') {
		isNegative = true;
		++buff;
	}

	int number1 = setNumber(buff, cntNumbers(buff));
	unsigned temp = cntNumbers(buff);
	double number2 = (double)setNumber(buff, temp) / (int)(pow(10, temp + 1));

	buff = nullptr;
	number = number1 + number2;

	if (isNegative) number = 0 - number;
}

 int Double::setNumber(char*& buffer, unsigned sizeT) const {
	int result = 0;

	unsigned temp = 1;
	int pow = 1;
	while (temp <= sizeT) {
		pow *= 10;
		++temp;
	}

	while (*buffer != '\0' && *buffer != ' ') {
		result += ((*buffer - '0')  * pow);
		pow /= 10;
		--sizeT, ++buffer;
		if (*buffer == '.') {
			++buffer;
			break;
		}
	}

	return result;
}

//This variable is for count the numbers 
//so that we can convert it to double 
//by multiplying or dividing with the power of cntNumbers - 1
unsigned Double::cntNumbers(const char* buffer)const {
	unsigned sizeT = 0;
	while (*buffer != '\0') {
		if (*buffer != ' ')
			++sizeT;
		++buffer;
		if (*buffer == '.') {
			++buffer;
			break;
		}
	}
	return --sizeT;
}

bool Double::check(const char* buffer)const {
	while (*buffer != '\0') {
		if ((*buffer - '0') > 9) {
			buffer = nullptr;
			return false;
		}
		++buffer;
		if (*buffer == '.') ++buffer;
	}
	return true;
}