#include <iostream>
#include <cstring>
#include "Tree.h"

using namespace std;

int ReadIn();
bool KeepGoing();

int main()
{
	Tree tree;
	int data;

	cout << "We will insert data into the tree" << endl;

	do
	{
		data = ReadIn();
		tree.Insert(data);
	} while(KeepGoing());

	tree.Display();

	cout << "\n\nWe will remove data from the tree" << endl;

	do
	{
		data = ReadIn();
		tree.Remove(data);
	} while(KeepGoing());

	tree.Display();

	return 0;
}

int ReadIn()
{
	int result;

	cout << "Please enter a number" << endl;
	cin >> result; cin.ignore(1000, '\n');

	return result;
}

bool KeepGoing()
{
	char input;

	cout << "Keep going? (Y/N)" << endl;
	cin >> input; cin.ignore(1000, '\n');

	input = toupper(input);

	return input == 'Y';
}
