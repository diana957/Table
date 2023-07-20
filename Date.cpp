#include "Date.h"
#include <iostream>
#include <exception>

using std::cout;

Date::Date(const char* buffer)
	: Type()
	, day(1)
	, month(1)
	, year(1900) {
	convert(buffer);
	if (!validDate() || !checkDots(buffer)) {
		day = 1, month = 1, year = 1900;
		throw std::invalid_argument("Invalid arguments are entered!!!\n");
	}
}

Date::~Date() {}

Type* Date::clone()const {
	return new Date(*this);
}

void Date::print()const {
	cout << day << '.' << month << '.' << year;
}

char Date::compare(const Type& rhs)const {
	const Date* ptr = dynamic_cast<const Date*>(&rhs);
	if (ptr) {
		if (isEqual(*ptr)) return 'e';
		if (year >= 1900 && year <= 3000) {
			if (year > ptr->year && year > ptr->day) return 'g';
			if (year < ptr->year && year < ptr->day) return 'l';
		}
		else if (day >= 1900 && day <= 3000) {
			if (day > ptr->day && day > ptr->year) return 'g';
			if (day < ptr->day && day < ptr->year) return 'l';
		}
		else {
			if (month > ptr->month) return 'g';
			else if (month < ptr->month) return 'l';
			else return compareDays(*ptr);
		}
	}
	else return 'm';
}

double Date::convertTypeToDouble()const {
	unsigned result = 0;
	if (01 <= day && day <= 31) {
		if (year == 1900) result = 0;
		else result += ((year - 1900) * 365);
		result += (day - 1);
	}
	else {
		if (day == 1900) result = 0;
		else result += ((day - 1900) * 365);
		result += (year - 1);
	}

	unsigned difference = month - 01;
	result += monthTodays(difference);

	return result;
}

unsigned Date::getSize()const {
	unsigned size = 0;

	if ((day >= 1 && day <= 9) || (year >= 1 && year <= 9))
		size += 5;

	if ((day >= 10 && day <= 31) || (year >= 10 && year <= 31))
		size += 6;

	if (month >= 1 && month <= 9) size += 1;
	else size += 2;
	size += 2;
	return size;
}

void Date::writeToTextFile(ofstream& os)const {
	os << day << '.' << month << '.' << year;
}

void Date::convert(const char* buffer) {	
	char temp[5];
	unsigned i = 0;
	unsigned j = 0;
	while (*buffer != '\0') {
		temp[j] = *buffer;
		j++;
		++buffer;
		if (*buffer == '.' || *buffer == '\0') {
			temp[j] = '\0';
			unsigned size = strlen(temp);
			if (i == 0)  day = set(temp, size - 1);
			if (i == 1)  month = set(temp, size - 1);
			if (i == 2)  year = set(temp, size - 1);
			++i;
			if(*buffer == '.')
				++buffer;
			j = 0;
		}
	}
}

unsigned Date::set(char* buffer, unsigned sizeT) {
	int result = 0;
	while (*buffer != '\0') {
		result += (*buffer - '0') * pow(10, sizeT);
		--sizeT, ++buffer;
	}
	return result;
}

bool Date::validDate() const {
	if (month >= 1 && month > 12)
		return false;

	if (year >= 1900 || year <= 3000) {
		if (!checkDay(day)) {
			if(isLeapYear(year)) {
				if (month == 2 && day > 29)
					return false;
			}
		}
		return true;
	}

	if (day >= 1900 || day <= 3000) {
		if (!checkDay(year)) {
			if (isLeapYear(day)) {
				if (month == 2 && year > 29)
					return false;
			}
		}
		return true;
	}	
}

bool Date::isLeapYear(int year)const {
	if (year % 4 == 0){
		if (year % 100 == 0){
			if (year % 400 == 0)
				return true;
			else
				return false;
		}
		else
			return true;
	}
	else
		return false;
}

bool Date::checkDay(int day)const {
	if (month >= 7) {
		if (month % 2 == 0) {
			if (day < 1 || day > 30)
				return true;
		}
		else {
			if (day < 1 || day > 31)
				return false;
		}
	}
	
	else {
		if (month % 2 == 0) {
			if (day < 1 || day > 31)
				return true;
		}
		else {
			if (day < 1 || day > 30)
				return false;
		}
	}

	return true;
}

bool Date::checkDots(const char* buffer)const {
	int dots = 0;
	while (*buffer != '\0') {
		if (*buffer == '.')
			++dots;
		++buffer;
	}


	if (dots != 2) return false;

	return true;
}

unsigned Date::monthTodays(unsigned difference) const {
	//The variable cnt is to count the month 
	//with 31 days
	unsigned cnt = 0;
	unsigned i = 1;

	while (i <= difference) {
		if (i <= 7) {
			if (i % 2 == 1) ++cnt;
		}
		else {
			if (i % 2 == 0) ++cnt;
		}
		++i;
	}

	unsigned result = 0;
	if (difference == 1)
		return result = 31;
	else {
		if (year > 1990) {
			if (isLeapYear(year)) result = 29;
			else result = 28;
		}
		else {
			if (isLeapYear(day)) result = 29;
			else result = 28;
		}
		return cnt * 31 + result + (difference - (cnt + 1)) * 30;
	}
}

bool Date::isEqual(const Date& rhs)const {
	if (month != rhs.month) return false;
	if (year >= 1900 && year <= 3000) {
		if (rhs.year >= 1900 && rhs.year <= 3000) {
			if (year != rhs.year) return false;
		}
		else if (year != rhs.day) return false;
	}
	else {
		if (rhs.year >= 1900 && rhs.year <= 3000) {
			if (day != rhs.year) return false;
		}
		else if (day != rhs.day) return false;
	}
	if (day >= 1 && day <= 31) {
		if (rhs.day >= 1 && rhs.day <= 31) {
			if (day != rhs.day) return false;
		}
		else if (day != rhs.year) return false;
	}
	else {
		if (rhs.day >= 1 && rhs.day <= 31) {
			if (year != rhs.day) return false;
		}
		else if (year != rhs.year) return false;
	}

	return true;
}

char Date::compareDays(const Date& rhs)const {
	if (day >= 1 && day <= 31) {
		if (day > rhs.day && day > rhs.year) return 'g';
		else return 'l';
	}
	else {
		if (year > rhs.year && year > rhs.day) return 'g';
		else return 'l';
	}
}