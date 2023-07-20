#include "Row.h"

using std::cout;
using std::cerr;

Row::Row() : row(nullptr)
		   , curColumn(0)
		   , nColumn(0) {}

Row::Row(unsigned size)
	: row(nullptr)
	, curColumn(0)
	, nColumn(0) {
	if (size < 0)
		throw std::invalid_argument("Invalid size for the row creation is entered!!!\n");
	try {
		row = new Type*[size];
	}
	catch (const std::bad_alloc& e) {
		delete[] row;
		throw;
	}

	nColumn = size;

	for (unsigned i = 0; i < size; ++i)
		row[i] = nullptr;
}

Row::Row(const Row& rhs)
	: row(nullptr)
	, curColumn(0)
	, nColumn(0) {
	try {
		copy(rhs);
	}
	catch (const std::bad_alloc&) {
		cerr << "There is not enough space and cannot copy this object!!!\n";
		throw;
	}
}

Row& Row::operator=(const Row& rhs) {
	if (this != &rhs)
		copy(rhs);
	return *this;
}

Row::~Row() {
	free();
}

void Row::setEmptyRow(unsigned size) {
	if (size < 1)
		throw std::invalid_argument("Invalid argument!!!\n");
	
	Type** tempR = new (std::nothrow) Type*[size];
	if (!tempR)
		throw std::bad_alloc();

	for (unsigned i = 0; i < size; ++i)
		tempR[i] = nullptr;

	free();

	row = tempR;
	nColumn = size;
}

void Row::set(const Type* rhs) {
	if (curColumn >= nColumn) resize(curColumn + 1);
	if (rhs == nullptr) row[curColumn] = nullptr;
	else row[curColumn] = rhs->clone();
	++curColumn;
}

bool Row::create(const char* word) {
	char type = 'e';
	if (word != '\0') type = findType(word);

	if (type == 'u')
		return false;

	if (curColumn >= nColumn) {
		try {
			resize(curColumn + 1);
		}
		catch (const std::bad_alloc&) {
			cerr << "There is no enough space on the disk to add new row!!!\n";
			return false;
		}
	}
	try {
		row[curColumn] = TypeFactory(type, word);
	}
	catch (const std::invalid_argument&) {
		delete[] row[curColumn]; 
		return false;
	}
	++curColumn;

	return true;
}

unsigned Row::getnColumn()const {
	return this->nColumn;
}

void Row::printElement(unsigned column)const {
	if (row[column] == nullptr) {
		cout << ' ';
		return;
	}
	row[column]->print();
}

bool Row::edit(unsigned column, const char* data) {
	char c = findType(data);

	if (c == 'u') return false;

	Type* newType = nullptr;
	try {
		newType = TypeFactory(c, data);
	}
	catch (const std::invalid_argument&) {
		return false;
	}
	delete[] row[column];
	row[column] = newType;
	
	return true;
}

void Row::writeToFile(ofstream& os, unsigned column)const {
	if (row[column] == nullptr) {
		os << ' ';
		return;
	}
	row[column]->writeToTextFile(os);
}

char Row::compareRows(const Row& rhs, unsigned column)const {
	if (row[column] == nullptr) return 'l';
	if (rhs.row[column] == nullptr) return 'g';
 	if (row[column]->compare(*rhs.row[column]) == 'm') {
		if (row[column]->convertTypeToDouble() > rhs.row[column]->convertTypeToDouble())
			return 'g';
		if (row[column]->convertTypeToDouble() < rhs.row[column]->convertTypeToDouble())
			return 'l';
		return 'e';
	}

	return row[column]->compare(*rhs.row[column]);
}

const Type*& Row::operator[](unsigned index)const {
	if (index < 0 || index > nColumn) 
		throw std::invalid_argument("Invalid argument!!!\n");

	return const_cast<const Type*&>(row[index]);
}

Type*& Row::operator[](unsigned index) {
	if (index < 0 || index > nColumn)
		throw std::invalid_argument("Invalid argument!!!\n");

	return row[index];
}

Row& Row::operator++() {
	set(nullptr);
	return *this;
}

Row Row::operator++(int) {
	Row oldValue(*this);
	++(*this);
	return oldValue;
}

unsigned Row::getSizeofElement(unsigned column)const {
	if (row[column] == nullptr) return 0;
	return row[column]->getSize();
}

const Type* Row::getElement(unsigned column)const{
	return row[column];
}

char Row::findType(const char* data)const {
	if (data == '\0') return 'e';

	while (*data == ' ') ++data;
	
	if (*data == '\"') {
		if (*(data + (strlen(data) - 1)) == '\"') return 'c';
	}

	else if (*data == '+' || *data == '-' ||
			(*data >= 43 || *data <= 57)) return check(data);
	
	else return 'u';
}

char Row::check(const char* data)const {
	while (*data == ' ')
		++data;

	unsigned dots = numberDots(data);

	if (dots == 2) {
		unsigned tempDots = dots;
		//This variable is for the Date
		//to see if the format is the right one
		unsigned cntNumbers = 0;
		unsigned firstcntNumbers = 0;
		while (*data != '\0') {
			++cntNumbers;
			++data;
			if ((*data == '.' || *data == '\0' || *data == ' ')) {
				if (dots == 2 || dots == 0) {
					if (cntNumbers == 3 || cntNumbers > 4) return 'u';
					if (tempDots == 2) firstcntNumbers = cntNumbers;
					if (tempDots == 0) {
						if (cntNumbers == firstcntNumbers) return 'u';
						break;
					}
				}
				if (tempDots == 1) {
					if (cntNumbers > 2) return 'u';
				}
				--tempDots;
				cntNumbers = 0;
				++data;
			}
			while (*data == ' ')
				++data;
		}
	}
	if (dots == 1) return 'f';
	else if (dots == 2) return 'd';
	else return 'i';

	return 'u';
}

unsigned Row::numberDots(const char* data)const {
	int dots = 0;
	while (*data != '\0') {
		if (*data == '.')
			++dots;
		++data;
	}
	return dots;
}

Type* Row::TypeFactory(char type, const char* data) {
	Type* rhs = nullptr;

	if (type == 'c') 
		return rhs = new Char(data);
	if (type == 'd')
		return rhs = new Date(data);
	if (type == 'f')
		return rhs = new Double(data);
	if (type == 'i')
		return rhs = new Int(data);

	return rhs;
}

void Row::resize(unsigned newSize) {
	Type** temp = new (std::nothrow) Type*[newSize];
	if (!temp) throw std::bad_alloc();

	for (unsigned i = 0; i < nColumn; ++i) {
		if (row[i] == nullptr) temp[i] = nullptr;
		else temp[i] = row[i]->clone();
	}

	free();

	row = temp;
	nColumn = newSize;
}

void Row::copy(const Row& rhs) {
	Type** temp = new (std::nothrow) Type*[rhs.nColumn];
	if (!temp)
		throw std::bad_alloc();

	for (unsigned i = 0; i < rhs.nColumn; ++i) {
		if (rhs.row[i] == nullptr) temp[i] = nullptr;
		else temp[i] = rhs.row[i]->clone();
	}
	
	free();
	row = temp;

	curColumn = rhs.curColumn;
	nColumn = rhs.nColumn;
}

void Row::free() {
	for (unsigned i = 0; i < nColumn; ++i)
		delete[] row[i];
	delete[] row;
}