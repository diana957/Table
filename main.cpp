//Name: Diana Nikolova
//FN: 45098

#include <iostream>
#include "Table.h"

using std::cout;
using std::cin;
using std::cerr;

void menu(Table& a) {
	cout << "To create your table, enter path to text file!!!\n";
	char path[64];
	cin.getline(path, 64);
	if (!a.load(path)) return;

	cout << "Commands for working with table! \n"
		<< "Choose \n"
		<< "1. l to load \n"
		<< "2. e to edit \n"
		<< "3. s to sort \n"
		<< "4. p to print on the screen \n"
		<< "5. r to add new empty Row at the end of the Table \n"
		<< "6. c to add new empty Column at the end of the Table \n"
		<< "7. q to quit \n";

	char c;
	cin >> c;

	unsigned numberofRows = 0;
	unsigned numberofColumns = 0;

	while (c != 'q') {
		if (c == 'l') {
			cin.get();
			char path[64];
			cout << "Enter new path!!\n";
			cin.getline(path, 64);
			if (!a.load(path)) return;
		}
		if (c == 'e') {
			cout << "Enter the number of row and column that you want to edit! \n";
			cin >> numberofRows >> numberofColumns;
			char buffer[1024];
			cin.get();
			cout << "Enter the content that will be changed! ";
			cin.getline(buffer, 1024);
			a.edit(numberofRows, numberofColumns, buffer);
		}
		if (c == 's') {
			cout << "Enter the number of column that will be sorted! \n";
			cin >> numberofColumns;
			cout << "If you have a preference for sorting, press y, otherwise n: ";
			char s;
			cin >> s;
			if (s == 'y') {
				cout << "Choose i for increase or d for decrease: ";
				char p;
				cin >> p;
				a.sort(numberofColumns, p);
			}
			else a.sort(numberofColumns, 'i');
		}

		if (c == 'p') a.print();
		if (c == 'r') a.addRow();
		if (c == 'c') a.addColumn();

		cout << "Choose again: ";
		cin >> c;
	}
}

int main(int argc, char* argv[]) {
	Table a;
	menu(a);

	return 0;
}