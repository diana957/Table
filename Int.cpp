#include "Int.h"
#include <exception>
 
using std::cout;

Int::Int(int number) : number(number) {}

Int::Int(const char* buffer)
	: Type()
	, number(0) {
	convert(buffer);
}

Int::~Int() {}

Type* Int::clone()const {
	return new Int(*this);
}

void Int::print() const {
	cout << number;
}

char Int::compare(const Type& rhs)const {
	const Int* ptr = dynamic_cast<const Int*>(&rhs);
	if (ptr) {
		if (number > ptr->number) return 'g';
		else if (number < ptr->number) return 'l';
		else return 'e';
	}
	return 'm';
}

double Int::convertTypeToDouble() const {
	return number;
}

unsigned Int::getSize()const {
	return findSize();
}

void Int::writeToTextFile(ofstream& os)const {
	os << number;
}

void Int::convert(const char* buffer) {
	char* buff = new (std::nothrow) char[strlen(buffer) + 1];
	if (!buff)
		throw std::bad_alloc();
	strcpy(buff, buffer);

	number = setNumber(buff, strlen(buffer));
	buff = nullptr;
}

int Int::setNumber(char* buffer, unsigned sizeT)const {
	int result = 0;
	char sign = '+';

	if (*buffer == '+' || *buffer == '-') {
		if (*buffer == '-') sign = '-';
		++buffer, --sizeT;
	}

	unsigned temp = 1;
	int pow = 1;
	while (temp < sizeT) {
		pow *= 10;
		++temp;
	}

	while (*buffer != '\0' && *buffer != ' ') {
		temp = *buffer - '0';
		if (temp >= 0 && temp <= 9)
			result += ((*buffer - '0')  * pow);
		else 
			throw std::invalid_argument("invalid argument!!!\n");
		pow /= 10;
		--sizeT, ++buffer;
	}

	if (sign == '-') result = 0 - result;

	return result;
}

unsigned Int::findSize()const {
	int temp = number;
	unsigned cnt = 0;

	while (temp != 0) {
		temp /= 10;
		++cnt;
	}
	return cnt;
}