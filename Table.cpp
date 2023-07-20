#include "Table.h"
#include <iomanip>      // std::setw

using std::cout;
using std::cerr;
using std::ios;

Table::Table() : table(nullptr)
			   , nRow(0) {}

Table::Table(const Table& rhs) 
		: table(nullptr)
		, nRow(0) {
	try {
		copy(rhs);
	}
	catch (const std::bad_alloc) {
		cerr << "Cannot copy these two objects. There is not enough space!!!\n";
	}
}

Table& Table::operator=(const Table& rhs) {
	if (this != &rhs)
		copy(rhs);

	return *this;
}

Table::~Table() {
	free();
}

bool Table::load(const char* path) {
	ifstream is(path);

	if (!is) {
		cerr << "Cannot open this file!!!\n";
		return false;
	}

	if (!setRC(is)) { 
		is.clear();
		is.close();
		return false; 
	}
	is.seekg(0, ios::beg);
	if (!readFromTextFile(is)) {
		is.clear();
		is.close();
		return false;
	}

	is.clear();
	is.close();
	return true;
}

void Table::print()const {
	unsigned size = 0;

	for (unsigned i = 0; i < nRow; ++i) {
		for (unsigned j = 0; j < table[i].getnColumn(); ++j) {
			size = longestElement(j);
			table[i].printElement(j);
			unsigned set = size - (table[i].getSizeofElement(j));
			if (set != size) set += 1;
			cout << std::setw(set);
			cout << "|" << std::setw(1);
		}

		cout << "\n";
	}
}

void Table::edit(unsigned row, unsigned column, const char* data) {
	if (row < 0 || row > nRow || column < 0 || column > table->getnColumn()) {
		cerr << "Invalid parameters for table are entered!!! I cannot change it due to this!!!\n";
		return;
	}

	if (!table[row - 1].edit(column - 1, data))
		cerr << "Unknown type is entered!!! Cannot edit the cell!!!\n";
}

void Table::save()const {
	ofstream os("saveFile.txt", ios::trunc);
	if (!os) {
		cerr << "Cannot open this file for writing! Cannot save your table";
		return;
	}
	unsigned size = 0;
	for (unsigned i = 0; i < nRow; ++i) {
		for (unsigned j = 0; j < table[i].getnColumn(); ++j) {
			size = longestElement(j);
			table[i].writeToFile(os, j);
			unsigned set = size - (table[i].getSizeofElement(j));
			if (set != size) set += 1;
			os << std::setw(set);
			os << "," << std::setw(1);
		}
		os << "\n";
	}

	os.clear();
	os.close();
}

void Table::sort(unsigned column, char c)const {
	if (column < 0 || column > table[0].getnColumn()) {
		cerr << "Invalid column for sort operation is entered!!!\n";
		return;
	}

	unsigned minIndex = 0;
	Row temp;
	for (unsigned i = 0; i < nRow - 1; ++i) {
		minIndex = i;
		for (unsigned j = i + 1; j < nRow; ++j) {
			if (c == 'i') {
				if (table[j].compareRows(table[minIndex], column - 1) == 'l')
					minIndex = j;
			}
			else {
				if (table[j].compareRows(table[minIndex], column - 1) == 'g')
					minIndex = j;
			}
		}
		if (minIndex != i) {
			temp = table[i];
			table[i] = table[minIndex];
			table[minIndex] = temp;
		}
	}
}

void Table::addRow() {
	Row* temp = new (std::nothrow) Row[nRow + 1];
	if (!temp) {
		cerr << "There is no enough space on the disc!!! Cannot add new row!!!!\n";
		return;
	}

	for (unsigned i = 0; i < nRow; ++i)
		temp[i] = table[i];

	temp[nRow].setEmptyRow(table->getnColumn());
	for(unsigned i = 0; i < table->getnColumn(); ++i)
		temp[nRow].create('\0');

	table = temp;
	nRow += 1;
}

void Table::addColumn() {
	for (unsigned i = 0; i < nRow; ++i)
		++table[i];
}

Row Table::operator[](Coordinates c)const {
	if (c.row > nRow) 
		throw std::invalid_argument("Invalid row is entered!!!\n");
	if (c.column > table->getnColumn())
		throw std::invalid_argument("Invalid column is entered!!!\n");

	if (c.row == 0) {
		Row temp(nRow);
		for (unsigned i = 0; i < nRow; ++i)
			temp.set(table[i].getElement(c.column - 1));
		return temp;
	}
	else if (c.column == 0) {
		Row temp(table->getnColumn());
		temp = table[c.row - 1];
		return temp;
	}
	else {
		Row r(1);
		r.set(table[c.row - 1].getElement(c.column - 1));
		return r;
	}
}

Table& Table::operator++() {
	Row* temp = new (std::nothrow) Row[nRow + 1];
	if (!temp) {
		cerr << "There is no enough space on the disc!!! Cannot add new row!!!!\n";
		return *this;
	}

	for (unsigned i = 0; i < nRow; ++i)
		temp[i] = table[i];

	temp[nRow].setEmptyRow(table->getnColumn());
	for (unsigned i = 0; i < table->getnColumn(); ++i)
		temp[nRow].create('\0');

	table = temp;
	nRow += 1;

	return *this;
}

Table Table::operator++(int) {
	Table oldValue(*this);
	++(*this);
	return oldValue;
}

unsigned Table::getnRow()const {
	return this->nRow;
}

bool Table::setRC(ifstream& is) {
	if (!is) {
		cerr << "Cannot open this file!!!\n";
		return false;
	}

	unsigned numberofRows = 0;
	unsigned numberofCommas = 0;
	unsigned maxCommas = 0;

	char* buffer = new char[1024];

	while (!is.eof()) {
		is.getline(buffer, 1024);
		if (!is && *buffer != '\0') {
			cerr << "Cannot do this operation!!!\n";
			return false;
		}
		while (*buffer != '\0') {
			if (*buffer == ',')
				++numberofCommas;
			if (*buffer == '\"') {
				++buffer;
				while (*buffer == '\"') ++buffer;
				while (*buffer != '\"' && *buffer != '\0') {
					if (*buffer == '\\') buffer += 2;
					else ++buffer;
				}
			}
			if (*buffer != '\0') ++buffer;
		}
		++numberofCommas;
		if (maxCommas < numberofCommas)
			maxCommas = numberofCommas;
		numberofCommas = 0;
		++numberofRows;
	}
	
	table = new Row[numberofRows];
	nRow = numberofRows;
	for(unsigned i = 0; i < nRow; ++i)
		table[i].setEmptyRow(maxCommas);
	
	is.clear();

	return true;
}

bool Table::readFromTextFile(ifstream& is) {
	if (!is) {
		cerr << "Cannot open this file!!!\n";
		return false;
	}
	char* buffer = new char[1024];

	unsigned currentCommas = 0;
	unsigned currentRows = 0;

	while (currentRows < nRow) {
		is.getline(buffer, 1024);
		if (*buffer == '\0' && currentCommas < table->getnColumn())
			setEmptyDate(currentRows, currentCommas);

		else {
			while (*buffer == ',') {
				table[currentRows].create('\0');
				++buffer, ++currentCommas;
				if (*buffer == '\0' && currentCommas < table->getnColumn())
					setEmptyDate(currentRows, currentCommas);
			}
			char* buff = splitRow(buffer);
			//The check for currentCommas is for the only one column we have
			while (*buff != '\0' && currentCommas < table[currentRows].getnColumn()) {
				if (!strcmp(buff, "false")) {
					std::cerr << "Error: row " << currentRows + 1
						<< " after colm " << currentCommas + 1
						<< " comma is missing!!!\n";
					return false;
				}

				if (!table[currentRows].create(buff)) {
					cerr << "Error row " << currentRows + 1 << ", col "
						<< currentCommas + 1 << ' ' << buff << " is unknown type!!!\n";
					return false;
				}
				++currentCommas;

				//if there are less symbols than the maxCommas
				//then the other cells are empty
				if (*buffer == '\0' && currentCommas < table[currentRows].getnColumn()) {
					setEmptyDate(currentRows, currentCommas);
					break;
				}

				//check whether there is no space between two commas
				//to set empty cell
				while (*buffer == ',') {
					++buffer;
					while (*buffer == ' ') ++buffer;
					if (*buffer == ',') {
						table[currentRows].create('\0');
						++currentCommas, ++buffer;
					}
					while (*buffer == ' ') ++buffer;
					if (*buffer == '\0' && currentCommas < table[currentRows].getnColumn()) {
						setEmptyDate(currentRows, currentCommas);
						break;
					}
				}
				buff = splitRow(buffer);
			}
		}
		++currentRows;
		currentCommas = 0;
	}

	return true;
}

char* Table::splitRow(char*& buffer) {
	char* buffer1 = new (std::nothrow) char[strlen(buffer) + 1];
	if (!buffer1)
		throw std::bad_alloc();

	unsigned i = 0;
	//To inform that buffer is char*
	bool flag = false;

	while (*buffer == ' ')
		++buffer;

	if (*buffer == '\"')
		flag = true;

	while (*buffer != '\0' && *buffer != ',') {
		//check if there are any missing commas
		if (*buffer == ' ' && !flag) {
			while (*buffer == ' ') ++buffer;
			if(*buffer != '\0' && *buffer != ','){
				strcpy(buffer1, "false");
				return buffer1;
			}
			else break;
		}

		buffer1[i] = *buffer;
		++buffer, ++i;
		if (*buffer == '\"') {
			buffer1[i] = *buffer;
			++buffer;
			while (*buffer == ' ') ++buffer;
			if (*buffer == '\0' || *buffer == ',') {
				++i;
				break;
			}
		}

		//check whether in the text there are any commas
		//that aren't used for separating 
		//because they are part of text that will be presented by char type
		if (flag && *buffer == ',') {
			buffer1[i] = *buffer;
			++i, ++buffer;
		}
	}

	buffer1[i] = '\0';
	if (*buffer == ' ') {
		while (*buffer != ',' || *buffer != '\0') ++buffer;
	}
	return buffer1;
}

void Table::setEmptyDate(unsigned row, unsigned column) {
	while (column < table->getnColumn()) {
		table[row].create('\0');
		++column;
	}
}

unsigned Table::longestElement(unsigned column)const {
	unsigned size = 0;
	unsigned max = 0;
	for (unsigned i = 0; i < nRow; ++i) {
		size = table[i].getSizeofElement(column);
		if (size > max) max = size;
	}
	return max;
}

void Table::copy(const Table& rhs) {
	Row* temp = new (std::nothrow) Row[rhs.nRow];
	if (!temp) throw std::bad_alloc();

	for (unsigned i = 0; i < rhs.nRow; ++i)
		temp[i] = rhs.table[i];

	free();
	table = temp;
	nRow = rhs.nRow;
}

void Table::free() {
	delete[] table;
}