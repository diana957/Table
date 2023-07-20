#include "Char.h"

#include <iostream>
#include <exception>

using std::cout;

Char::Char(const char* buffer) 
	: Type()
	, word(nullptr) {
	try {
		this->word = new char[strlen(buffer) + 1];
	}
	catch (const std::bad_alloc& e) {
		delete[] word;
		throw;
	}

	if (checkLines(buffer) % 2 == 1) {
		free();
		throw std::invalid_argument("Invalid argument is entered!!!\n");
	}
	convert(buffer);
}

Char::Char(const Char& rhs) 
	: Type()
	, word(nullptr) {
	copy(rhs);
}

Char& Char::operator=(const Char& rhs) {
	if (this != &rhs) 
		copy(rhs);
	
	return *this;
}

Char::~Char() {
	free();
}

Type* Char::clone()const {
	return new Char(*this);
}

void Char::print()const {
	cout << word;
}

char Char::compare(const Type& rhs)const{
	const Char* ptr = dynamic_cast<const Char*>(&rhs);
	if (ptr) {
		if (!strcmp(word, ptr->word)) return 'e';
		else if (strcmp(word, ptr->word) > 0) return 'g';
		else return 'l';
	}
	//m for do not match
	else return 'm';
}

double Char::convertTypeToDouble()const {
	return atof(word);
}

unsigned Char::getSize()const {
	return strlen(word);
}

void Char::writeToTextFile(ofstream& os)const {
	os << word;
}

void Char::convert(const char* buffer) {
	unsigned lines = 0;
	unsigned totalLines = checkLines(buffer);

	unsigned i = 0;

	if (*buffer == '\"')
		++buffer, ++lines;

	while (*buffer == ' ') ++buffer;

	while (*buffer != '\0') {
		if (*buffer == '\"') ++lines;
		if (lines == totalLines) break;

		if (*buffer == '\"' && *(buffer + 1) == '\\') { 
			++buffer;
			word[i] = '"'; 
		}
		else if (*buffer == '\\' && *(buffer + 1) == '\"') {
			++buffer;
			word[i] = '"';
			++lines;
		}
		else if (*buffer == '\\' && *(buffer + 1) == '\\') {
			++buffer; 
			word[i] = '\\';
		}
		else word[i] = *buffer;

		++buffer, ++i;
	}
	word[i] = '\0';
	withOutSpaceEnd();
}

int Char::checkLines(const char* buffer)const {
	unsigned cntLines = 0;

	while (*buffer != '\0') {
		if (*buffer == '\"')
			++cntLines;
		++buffer;
	}

	return cntLines;
}

void Char::withOutSpaceEnd() {
	unsigned index = 0;

	for (unsigned i = strlen(word); i > 0; --i) {
		if (*(word + i) == ' ') index = i;
	}

	if (index != 0) {
		char* temp = new (std::nothrow) char[strlen(word) + 1];
		if (!temp) throw std::bad_alloc();

		for (unsigned i = 0; i < index; ++i) {
			temp[i] = word[i];
			if (i == index - 1) {
				++i;
				temp[i] = '\0';
			}
		}

		free();
		word = temp;
	}
}

void Char::copy(const Char& rhs) {
	char* buffer = new (std::nothrow) char[strlen(rhs.word) + 1];
	if (!buffer)
		throw std::bad_alloc();

	strcpy(buffer, rhs.word);
	free();

	word = buffer;
}

void Char::free() {
	delete[] word;
}