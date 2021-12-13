#include <iostream>
#include "InputParsing.h"
#include <map>
#include "BigNumber.h"
#include "welcomePage.h"
#include <windows.h>
#include <string>
using namespace std;

void ruleList()
{
	cout << "Welcome to TypeMath" << endl;
	cout << "RULES:" << endl;
	cout << "1. First character of the variables' name must be a letter." << endl;
	cout << "2. If your equation is too complex, PLEASE wait until the program is done." << endl;
	cout << "3. First mention variable must use 'Set', the value will NOT be saved is it is invalid value." << endl;
	cout << "4. CTRL+Z to stop the program." << endl;
	cout << endl << "Let's start typing! XD" << endl;

	system("pause");
	system("cls");
}

int main()
{
	printWelcomePage(); //comment this line to disable the welcome page :)
	string line;
	map <string, Integer> integerVariable;
	map<string, Decimal> decimalVariable;
	cout << "Type CLS to clear screen" << endl;
	cout << "Type RULES to see Rule Lists" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	printingInputPhase();
	while (getline(cin, line))
	{
		if (line == "CLS")
		{
			system("cls");
			cout << "Type CLS to clear screen" << endl;
			cout << "Type RULES to see Rule Lists" << endl;
			printingInputPhase();
			continue;
		}
		else if (line == "RULES")
		{
			system("cls");
			ruleList();
			cout << "Type CLS to clear screen" << endl;
			cout << "Type RULES to see Rule Lists" << endl;
			printingInputPhase();
			continue;
		}
		spaceAddition(line);
		if (!invalidLine(line, integerVariable, decimalVariable))
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "Terminating because invalid symbol\n";
			SetConsoleTextAttribute(hConsole, 7);
			continue;
		}
		vector <string> brokenDownLine = breakSubstringVector(line);
		string returnString;
		bool operationSuccess = lineParsing(brokenDownLine, returnString, integerVariable, decimalVariable, false);
	}
	printingBlankPhase(14);
	SetConsoleTextAttribute(hConsole, 2);
	cout << "Byebye :)" << endl;
	SetConsoleTextAttribute(hConsole, 7);
	system("pause");

	/*Integer x = "123 * 8 + 456";
	Decimal y = "-1.0 / 3 - 45 / 13.0";


	vector<BigNumber*> nums;
	nums.push_back(&x);
	nums.push_back(&y);
	for (const auto& num : nums)
		cout << *num << endl;*/

	/*vector<BigNumber*> nums;
	nums.push_back(&d);
	for (auto iterator = nums.begin(); iterator != nums.end(); iterator++)
	{
		cout << *(*iterator) << endl;
	}*/

}
	