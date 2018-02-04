#include <iostream>
#include <cstring>
#include "Tree.h"

using namespace std;

int ReadIn();
bool KeepGoing();

int main()
{
	Tree myTree;
	int data;

	do
	{
		data = ReadIn();
		myTree.Insert(data);

	} while(KeepGoing());

	myTree.Display();


	cout << "Now is time for remove" << endl;
	
	do
	{
		data = ReadIn();
		myTree.Remove(data);
	
	} while(KeepGoing());

	myTree.Display();

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
