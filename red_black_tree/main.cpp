#include "Tree.h"
#include <iostream>
#include <cstring>

using namespace std;

int ReadIn();
bool KeepGoing();

int main()
{
	int data;
	Tree tree;

	do
	{
		data = ReadIn();
		tree.Insert(data);

	} while(KeepGoing());

	tree.Display();

	cout << "Now is time for remove" << endl;
	
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

	cout << "Please enter the data: ";
	cin >> result; cin.ignore(1000, '\n');

	return result;
}

bool KeepGoing()
{
	char input;

	cout << "Keep going (Y/N)? ";
	cin >> input; cin.ignore(1000, '\n');

	input = toupper(input);

	return input == 'Y';
}
