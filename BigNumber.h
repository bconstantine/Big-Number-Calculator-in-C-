#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <Windows.h>
#include "convertion.h"
#include "miniOperation.h"
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

//dari string
//dari integer
//dari float
//dari double

class BigNumber
{
public:
	string integer;
	string decimal;
	bool isNegative;
	bool isDecimal;

	static string convertToString(const char* input)
	{
		string res = "";
		for (int i = 0; i < strlen(input); i++)
		{
			res.push_back(*(input + i));
		}
		return res;
	}

	BigNumber()
	{
		integer = "";
		decimal = "";
		isNegative = false;
		isDecimal = false;
	}

	BigNumber(string inte, string dec, bool isNeg, bool isDec)
	{
		integer = inte;
		decimal = dec;
		isNegative = isNeg;
		isDecimal = isDec;
	}

	static bool checkForNoArithmeticSign(vector <string>& substringBreakdown)
	{
		for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
		{
			if (iterator->length() == 1 && isArithmeticSymbol((*iterator)[0]))
			{
				return false;
			}
		}
		return true;
	}

	static void replaceWithSubstring(vector <string>& lineVector, vector<string>::iterator startPoint, vector<string>::iterator endPoint, string substring)
	{
		int index = startPoint - lineVector.begin();
		lineVector.erase(startPoint, endPoint);
		if (lineVector.size() == 0)
		{
			lineVector.push_back(substring);
		}
		else
		{
			lineVector.insert(lineVector.begin() + index, substring);
		}
	}


	static vector<string> breakSubstringVector(string substring)
	{
		vector <string> substringBreakDown;
		stringstream substringStream(substring);
		string token;

		while (substringStream >> token)
		{
			substringBreakDown.push_back(token);
		}

		return substringBreakDown;
	}

	BigNumber(string line)
	{
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
	}

	BigNumber(const char* input)
	{
		string line = convertToString(input);
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
	}

	string operator=(const char* input)
	{
		string line = convertToString(input);
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
		return line;
	}

	BigNumber(int input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
	}

	BigNumber(float input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
	}

	BigNumber(double input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
	}

	static bool isNumber(char letter)
	{
		if (letter >= '0' && letter <= '9')
		{
			return true;
		}
		else if (letter == '.')
		{
			return true;
		}
		else if (letter == '-')
		{
			//not sure, try again later
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool isLetter(char letter)
	{
		if (letter >= 'A' && letter <= 'Z')
		{
			return true;
		}
		else if (letter >= 'a' && letter <= 'z')
		{
			return true;
		}
		else if (letter == '_')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool isArithmeticSymbol(char letter)
	{
		//not sure, try again later (remainder symbol)
		if (letter == '+' || letter == '-' || letter == '*' || letter == '/' || letter == '%' || letter == '^' || letter == '!' || letter == '(' || letter == ')')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool isArithmeticEqualSymbol(char letter)
	{
		//not sure, try again later (remainder symbol)
		if (letter == '+' || letter == '-' || letter == '*' || letter == '/' || letter == '%' || letter == '^' || letter == '!' || letter == '(' || letter == ')' || letter == '=' || letter == ',')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static pair<int, int> paranthesisInside(vector <string>& lineVector)
	{
		//return 0 0 for no found, return end end for error / invalid input
		int iterator = 0;
		int startingIterator = iterator;
		int endingIterator = iterator;
		if (lineVector[iterator] == ")")
		{
			//error case when the first index is already )
			return (make_pair(lineVector.size(), lineVector.size()));
		}
		bool isPowerParanthesis = false;
		bool foundFront = false;
		bool foundBack = false;
		while (iterator < lineVector.size())
		{
			if (lineVector[iterator] == "(")
			{
				if (iterator != 0)
				{
					//check just in case this paranthesis is the power() paranthesis
					if (lineVector[iterator - 1] == "Power")
					{
						isPowerParanthesis = true;
					}
					else
					{
						foundFront = true;
						startingIterator = iterator;
						isPowerParanthesis = false;
					}
				}
				else
				{
					foundFront = true;
					startingIterator = iterator;
					isPowerParanthesis = false;
				}

			}
			else if (lineVector[iterator] == ")")
			{
				if (isPowerParanthesis)
				{
					//this means it is still the same power paranthesis from before, ignore
					isPowerParanthesis = false;
				}
				else if (!foundFront)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "didn't find opening (, terminating\n";
					SetConsoleTextAttribute(hConsole, 7);
					return (make_pair(lineVector.size(), lineVector.size()));
				}
				else
				{
					foundBack = true;
					endingIterator = iterator + 1;
					break;
				}
			}
			iterator++;
		}
		if (foundFront && !foundBack)
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "didn't find closing ), terminating\n";
			SetConsoleTextAttribute(hConsole, 7);
			return (make_pair(lineVector.size(), lineVector.size()));
		}
		else
		{
			return (make_pair(startingIterator, endingIterator));
		}
	}

	static int checkForEqualSign(vector <string> substringBreakdown)
	{
		int index;
		for (index = 0; index < substringBreakdown.size(); index++)
		{
			if (substringBreakdown[index] == "=")
			{
				return index;
			}
		}
		return index; // no found
	}

	static bool doubleSymbolCheck(vector <string>& substringBreakdown)
	{
		bool previousArithmetic = false;
		for (int index = 0; index < substringBreakdown.size(); index++)
		{
			if (substringBreakdown[index].length() == 1 && isArithmeticSymbol(substringBreakdown[index][0]))
			{
				if (!previousArithmetic)
				{
					previousArithmetic = true;
					continue;
				}
				else
				{
					previousArithmetic = true;
					if (substringBreakdown[index - 1] == "!")
					{
						continue;
					}
					if (substringBreakdown[index] == "(" || substringBreakdown[index - 1] == ")")
					{
						continue;
					}
					if ((substringBreakdown[index - 1] == "+" || substringBreakdown[index - 1] == "-") && (substringBreakdown[index] == "-" || substringBreakdown[index] == "+"))
					{
						//double addition symbol

						string replacementToken;
						if (substringBreakdown[index - 1] == substringBreakdown[index])
						{
							replacementToken = "+";
						}
						else
						{
							replacementToken = "-";
						}

						//replace the double symbol
						substringBreakdown[index - 1] = replacementToken;
						substringBreakdown.erase(substringBreakdown.begin() + index);
						index--;
					}
					else if ((substringBreakdown[index] == "-" || substringBreakdown[index] == "+"))
					{
						//unary symbol
						continue;
					}
					else
					{
						//invalid double symbol
						return false;
					}

				}
			}
			else
			{
				previousArithmetic = false;
			}
		}
		return true;
	}

	static bool findingFactorialModded(vector <string>& substringBreakdown)
	{
		for (int index = 0; index < substringBreakdown.size(); index++)
		{
			if (substringBreakdown[index] == "!")
			{
				bool numberParameter = true;
				BigNumber result;
				for (auto iterator = substringBreakdown[index - 1].begin(); iterator != substringBreakdown[index - 1].end(); iterator++)
				{
					if (!isNumber(*iterator))
					{
						numberParameter = false;
					}
				}
				if (numberParameter)
				{
					//directly do factorial operation
					BigNumber parameter;
					bool operationValid = stringToBigNumber(substringBreakdown[index - 1], parameter);
					if (!operationValid)
					{
						//operation invalid, no need to continue
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Factorial error: convertion of string to big number\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
					else
					{
						if (parameter.isDecimal)
						{
							if (!parameter.decimal.empty())
							{
								SetConsoleTextAttribute(hConsole, 12);
								cout << "Factorial error, number is a decimal";
								SetConsoleTextAttribute(hConsole, 7);
								return false;
							}
							else
							{
								if (parameter.isNegative)
								{
									SetConsoleTextAttribute(hConsole, 12);
									cout << "Factorial error, number is negative";
									SetConsoleTextAttribute(hConsole, 7);
									return false;
								}
								result = BigNumber::factorial(parameter);
								result.isDecimal = true;
								string resultString;
								bigNumberToString(result, resultString);
								substringBreakdown[index - 1] = resultString;
								substringBreakdown.erase(substringBreakdown.begin() + index);
								index--;
							}
						}
						else if (parameter.isNegative)
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Factorial error, number is negative";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
						else
						{
							result = BigNumber::factorial(parameter);
							string resultString;
							if (!bigNumberToString(result, resultString))
							{
								SetConsoleTextAttribute(hConsole, 4);
								cout << "Factorial error: convertion of big number to string\n";
								SetConsoleTextAttribute(hConsole, 7);
								return false;
							}
							substringBreakdown[index - 1] = resultString;
							substringBreakdown.erase(substringBreakdown.begin() + index);
							index--;
							continue;
						}
					}
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Factorial Error! Variable not exist!\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
					
				}
			}
		}
		return true;
	}

	static bool findingPowerModded(vector <string>& substringBreakdown)
	{
		for (int index = substringBreakdown.size() - 1; index >= 0; index--)
		{
			if (substringBreakdown[index] == "^" || substringBreakdown[index] == "Power")
			{
				//fault if both are in the end
				if (index == substringBreakdown.size() - 1)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Error! Not enough power parameter\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				//fault if in the beginning, but only the ^
				if (index == 0 && substringBreakdown[index] == "^")
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Error! Not enough power parameter\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}


				bool leftIsFraction = false;
				bool rightIsFraction = false;
				bool rightIsNumber = true;
				bool leftIsNumber = true;
				BigNumber lhs;
				BigNumber rhs;
				//classify each case for ^ or Power
				if (substringBreakdown[index] == "^")
				{
					//make sure the left and right is both number, if not invalid power parameter
					//check left
					for (auto iterator = substringBreakdown[index - 1].begin(); iterator != substringBreakdown[index - 1].end(); iterator++)
					{
						if (!isNumber(*iterator))
						{
							leftIsNumber = false;
							break;
						}
					}

					if (!leftIsNumber)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << "Power error: left variable \"" << substringBreakdown[index - 1] << "\" doesn't exist.\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
					else
					{
						//lhs is number
						bool operationSuccessful = stringToBigNumber(substringBreakdown[index - 1], lhs);
						if (!operationSuccessful)
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Power operation fail when converting left hand string to a big number, terminating!\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					}

					if (substringBreakdown[index + 1] == "-")
					{
						if (substringBreakdown[index + 2][0] != '-')
						{
							substringBreakdown[index + 2].insert(substringBreakdown[index + 2].begin(), '-');
						}
						else
						{
							substringBreakdown[index + 2].erase(substringBreakdown[index + 2].begin());
						}
						substringBreakdown.erase(substringBreakdown.begin() + index + 1);
					}
					for (auto iterator = substringBreakdown[index + 1].begin(); iterator != substringBreakdown[index + 1].end(); iterator++)
					{
						if (!isNumber(*iterator))
						{
							rightIsNumber = false;
							break;
						}
					}

					if (!rightIsNumber)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << "Power error: right variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
					else
					{
						//right is number
						bool operationSuccessful = stringToBigNumber(substringBreakdown[index + 1], rhs);
						if (!operationSuccessful)
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Power operation fail when converting right hand string to a big number, terminating!\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					}
				}
				else if (substringBreakdown[index] == "Power")
				{
					if (index > substringBreakdown.size() - 6)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << "Not enough parameter or syntax error in Power! \n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}

					if (substringBreakdown[index + 1] != "(")
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << "Syntax error! No opening ( found in Power\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}

					//( exist, now check for existing ,
					int iteratingIndex = index + 1; //starting with (
					bool paranthesisFound = false;
					do
					{
						iteratingIndex++;
						if (iteratingIndex == substringBreakdown.size())
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Error! No comma exist in Power! Not enough parameter\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
						if (substringBreakdown[iteratingIndex] == "(")
						{
							paranthesisFound = true;
						}
						else if (substringBreakdown[iteratingIndex] == ")")
						{
							if (paranthesisFound)
							{
								paranthesisFound = false;
							}
							else
							{
								SetConsoleTextAttribute(hConsole, 12);
								cout << "Error! No comma exist in Power! Not enough parameter\n";
								SetConsoleTextAttribute(hConsole, 7);
								return false;
							}
						}
						else if (substringBreakdown[iteratingIndex] == ",")
						{
							if (paranthesisFound)
							{
								continue;
							}
							else
							{
								break;
							}
						}
					} while (iteratingIndex != substringBreakdown.size());


					//check if the length of the iterator is long, it means there are operation needs to be done
					if ((iteratingIndex - index - 1) > 2)
					{
						//operation needs to be done for the lhs
						vector <string> substringSubstring;
						for (int iterator = index + 2; iterator < iteratingIndex; iterator++)
						{
							substringSubstring.push_back(substringBreakdown[iterator]);
						}
						//start to do the operation
						string returnString;
						bool operationSuccess = doOperationParanthesisModded(substringSubstring, returnString);
						if (operationSuccess)
						{
							//replace the substring
							vector<string>::iterator startIterator = substringBreakdown.begin() + index + 2;
							vector<string>::iterator endIterator = substringBreakdown.begin() + iteratingIndex;
							replaceWithSubstring(substringBreakdown, startIterator, endIterator, returnString);
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Error while doing operation on the left hand side of the \"Power\", kindly check the previous Error\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					}
					//check the Right hand side if it is longer than one
					iteratingIndex = index + 1; //starting with (
					do
					{
						iteratingIndex++;
						if (iteratingIndex == substringBreakdown.size() - 1 || substringBreakdown[iteratingIndex] == ")")
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Error! No comma exist in Power! Not enough parameter\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					} while (substringBreakdown[iteratingIndex] != ",");

					//make sure no another comma is found
					int closingIndex = iteratingIndex;

					do
					{
						closingIndex++;
						if (closingIndex == substringBreakdown.size())
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Error! No closing ) found in Power!\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
						else if (substringBreakdown[closingIndex] == ",")
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Error! Too much parameter found in Power!\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					} while (substringBreakdown[closingIndex] != ")");
					if (closingIndex - iteratingIndex > 2)
					{
						//operation needs to be done for the rhs
						vector <string> substringSubstring;
						for (int iterator = iteratingIndex + 1; iterator < closingIndex; iterator++)
						{
							substringSubstring.push_back(substringBreakdown[iterator]);
						}

						//start to do the operation
						string returnString;
						bool operationSuccess = doOperationModded(substringSubstring, returnString);

						if (operationSuccess)
						{
							//replace the substring
							vector<string>::iterator startIterator = substringBreakdown.begin() + iteratingIndex + 1;
							vector<string>::iterator endIterator = substringBreakdown.begin() + closingIndex;
							replaceWithSubstring(substringBreakdown, startIterator, endIterator, returnString);
						}
						else
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Error while doing operation on the right hand side of the \"Power\", kindly check the previous Error\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					}

					//leftIsFraction, rightIsFraction, leftIsNumber, rightIsNumber

					//check left
					for (auto iterator = substringBreakdown[index + 2].begin(); iterator != substringBreakdown[index + 2].end(); iterator++)
					{
						if (!isNumber(*iterator))
						{
							leftIsNumber = false;
							break;
						}
					}

					if (!leftIsNumber)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << "Power error: left variable \"" << substringBreakdown[index + 2] << "\" doesn't exist.\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
					else
					{
						//lhs is number
						bool operationSuccessful = stringToBigNumber(substringBreakdown[index + 2], lhs);
						if (!operationSuccessful)
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Power operation fail when converting left hand string to a big number, terminating!\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					}


					for (auto iterator = substringBreakdown[index + 4].begin(); iterator != substringBreakdown[index + 4].end(); iterator++)
					{
						if (!isNumber(*iterator))
						{
							rightIsNumber = false;
							break;
						}
					}

					if (!rightIsNumber)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << "Power error: right variable \"" << substringBreakdown[index + 4] << "\" doesn't exist.\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
					else
					{
						//right is number
						bool operationSuccessful = stringToBigNumber(substringBreakdown[index + 4], rhs);
						if (!operationSuccessful)
						{
							SetConsoleTextAttribute(hConsole, 4);
							cout << "Power operation fail when converting right hand string to a big number, terminating!\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					}
				}

				//everything is set, doing the power function

				//make sure rhs add by one is valid
				if (rhs.isDecimal)
				{
					if (!rhs.decimal.empty())
					{
						//rhs decimal exist, check again
						BigNumber rhsChecker;
						BigNumber temporary;
						temporary.integer = "0";
						temporary.decimal = "5";
						temporary.isNegative = false;
						temporary.isDecimal = true;
						rhsChecker = BigNumber::add(rhs, temporary);
						if (!rhsChecker.decimal.empty())
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Power error! Right hand side is not the multiples of 0.5\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}

						//by this point we need to conduct a check if lhs is negative or not, cause if negative, square root not exist
						if (lhs.isNegative)
						{
							SetConsoleTextAttribute(hConsole, 12);
							cout << "Power error! Left hand side is negative, can't conduct square root\n";
							SetConsoleTextAttribute(hConsole, 7);
							return false;
						}
					}

				}

				BigNumber result;
				bool resultIsDecimal = false;
				//rhs is valid
				if (rhs.isNegative)
				{
					rhs.isNegative = false; // just in case
					BigNumber temporary;
					temporary.integer = "2";
					temporary.decimal = "";
					temporary.isNegative = false;
					temporary.isDecimal = true;
					BigNumber rhsEven = BigNumber::divide(rhs, temporary);
					

					if (lhs.isDecimal || rhs.isDecimal)
					{
						result.isDecimal = true;
						resultIsDecimal = true;
						if (rhsEven.decimal.empty())
						{
							result.isNegative = false;
						}
						else
						{
							if (lhs.isNegative)
							{
								result.isNegative = true;
							}
							else
							{
								result.isNegative = false;
							}
						}
						//convert the bignumber to string
					}
					else
					{
						resultIsDecimal = false;
						if (lhs.integer == "1")
						{
							result.integer = "1";
							result.decimal = "";
							result.isDecimal = false;
							result.isNegative = false;
						}
						else
						{
							result.integer = "0";
							result.decimal = "";
							result.isDecimal = false;
							result.isNegative = false;
						}
					}
				}
				else
				{
					if (rhs.isDecimal)
					{
						result = BigNumber::power(lhs, rhs);
						result.isDecimal = true;
					}
					else
					{
						resultIsDecimal = false;
						result = BigNumber::power(lhs, rhs);
					}
				}

				//finish everything, now start inserting back to the vector
				string newBigNum;
				bool operationSuccess = bigNumberToString(result, newBigNum);
				if (!operationSuccess)
				{
					SetConsoleTextAttribute(hConsole, 4);
					cout << "Power error: Problem when converting bigNumber to string for final insertion\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				//time for insertion
				if (substringBreakdown[index] == "^")
				{
					replaceWithSubstring(substringBreakdown, substringBreakdown.begin() + index - 1, substringBreakdown.begin() + index + 2, newBigNum);
				}
				else if (substringBreakdown[index] == "Power")
				{
					replaceWithSubstring(substringBreakdown, substringBreakdown.begin() + index, substringBreakdown.begin() + index + 6, newBigNum);
				}
			}
		}
		return true;
	}

	static bool findingMultiplicativeModded(vector <string>& substringBreakdown)
	{
		//left hand side doesnt have to pay attention to sign, right hand side on the other hand should
		for (int index = 0; index < substringBreakdown.size(); index++)
		{
			if (substringBreakdown[index] == "*" || substringBreakdown[index] == "/")
			{
				//if it is the first and the last index, then that means the operation is invalid
				if (index == 0 || index == substringBreakdown.size() - 1)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Multiplicative error: find symbol on the beginning or the end of the substring\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}

				//for now it is valid because not at the very beginning or the very end
				//set lhs
				bool leftIsNumber = true;
				bool leftIsFraction = false;
				BigNumber lhs;
				bool rightIsNumber = true;
				bool rightIsFraction = false;
				BigNumber rhs;

				//check left
				for (auto iterator = substringBreakdown[index - 1].begin(); iterator != substringBreakdown[index - 1].end(); iterator++)
				{
					if (!isNumber(*iterator))
					{
						leftIsNumber = false;
						break;
					}
				}

				if (!leftIsNumber)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Multiplicative error: left variable \"" << substringBreakdown[index - 1] << "\" doesn't exist.\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				else
				{
					//lhs is number
					bool operationSuccessful = stringToBigNumber(substringBreakdown[index - 1], lhs);
					if (!operationSuccessful)
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Multiplicative operation fail when converting left hand string to a big number, terminating!\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
				}

				//check right
				bool changeSign = false;
				if (substringBreakdown[index + 1] == "-" || substringBreakdown[index + 1] == "+")
				{
					if (substringBreakdown[index + 1] == "-")
					{
						changeSign = true;
					}
					//delete the symbol
					substringBreakdown.erase(substringBreakdown.begin() + index + 1);
				}
				for (auto iterator = substringBreakdown[index + 1].begin(); iterator != substringBreakdown[index + 1].end(); iterator++)
				{
					if (!isNumber(*iterator))
					{
						rightIsNumber = false;
						break;
					}
				}

				if (!rightIsNumber)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Multiplicative error: right variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				else
				{
					//right is number
					bool operationSuccessful = stringToBigNumber(substringBreakdown[index + 1], rhs);
					if (!operationSuccessful)
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Multiplicative operation fail when converting right hand string to a big number, terminating!\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}

					if (changeSign)
					{
						if (rhs.isNegative)
						{
							rhs.isNegative = false;
						}
						else
						{
							rhs.isNegative = true;
						}
					}
				}

				BigNumber result;
				BigNumber resultFractionNumerator;
				BigNumber resultFractionDenominator;
				bool resultIsFraction = false; //to simplify whether it is possible to turn fraction to usual BigNumber
				//starting to multiply
				//check zero division error
				if (rhs.integer == "0" && rhs.decimal.empty() && substringBreakdown[index] == "/")
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Multiplicative error: Zero division error\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}

				if (substringBreakdown[index] == "*")
				{
					//big number x big number
					result = BigNumber::multiply(lhs, rhs);
					if (lhs.isDecimal || rhs.isDecimal)
					{
						result.isDecimal = true;
					}
					else
					{
						result.isDecimal = false;
					}
				}
				else
				{
					//bigNumber / bigNumber
					result = BigNumber::divide(lhs, rhs);
					if (lhs.isNegative == rhs.isNegative)
					{
						result.isNegative = false;
					}
					else
					{
						result.isNegative = true;
					}
				}


				//operation is finished
				//beginning replacing the String
				string replacementString;
				if (!bigNumberToString(result, replacementString))
				{
					SetConsoleTextAttribute(hConsole, 4);
					cout << "Multiplicative error while converting big number result to string, terminating\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				replaceWithSubstring(substringBreakdown, substringBreakdown.begin() + index - 1, substringBreakdown.begin() + index + 2, replacementString);
				index--;
			}
		}
		return true;
	}

	static bool findingAdditiveModded(vector <string>& substringBreakdown)
	{
		for (int index = 0; index < substringBreakdown.size(); index++)
		{
			if (substringBreakdown[index] == "+" || substringBreakdown[index] == "-")
			{
				if (index == substringBreakdown.size() - 1)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Additive error: finding symbol at the the last of the vector\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				else if (index == 0)
				{
					//meeting unary operator
					bool leftIsNumber = true;
					for (auto iterator = substringBreakdown[index + 1].begin(); iterator != substringBreakdown[index + 1].end(); iterator++)
					{
						if (!isNumber(*iterator))
						{
							leftIsNumber = false;
							break;
						}
					}

					if (!leftIsNumber)
					{
						SetConsoleTextAttribute(hConsole, 12);
						cout << "additive error: left variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
					else
					{
						//lhs is number
						if (substringBreakdown[index] == "-")
						{
							if (substringBreakdown[index + 1][0] == '-')
							{
								substringBreakdown[index + 1].erase(substringBreakdown[index + 1].begin());
							}
							else
							{
								substringBreakdown[index + 1].insert(0, "-");
							}
						}
					}
					substringBreakdown.erase(substringBreakdown.begin());
					index--;
					continue;
				}

				//just doing casual addition and subtraction
				//find the lhs and rhs and check whether any of them is fraction
				//set lhs
				bool leftIsNumber = true;
				bool leftIsFraction = false;
				BigNumber lhs;
				bool rightIsNumber = true;
				bool rightIsFraction = false;
				BigNumber rhs;

				//check left
				for (auto iterator = substringBreakdown[index - 1].begin(); iterator != substringBreakdown[index - 1].end(); iterator++)
				{
					if (!isNumber(*iterator))
					{
						leftIsNumber = false;
						break;
					}
				}

				if (!leftIsNumber)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "additive error: left variable \"" << substringBreakdown[index - 1] << "\" doesn't exist.\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				else
				{
					//lhs is number
					bool operationSuccessful = stringToBigNumber(substringBreakdown[index - 1], lhs);
					if (!operationSuccessful)
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "additive operation fail when converting left hand string to a big number, terminating!\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}
				}

				//check right
				bool changeSign = false;
				if (substringBreakdown[index + 1] == "-" || substringBreakdown[index + 1] == "+")
				{
					if (substringBreakdown[index + 1] == "-")
					{
						changeSign = true;
					}
					//delete the symbol
					substringBreakdown.erase(substringBreakdown.begin() + index + 1);
				}
				for (auto iterator = substringBreakdown[index + 1].begin(); iterator != substringBreakdown[index + 1].end(); iterator++)
				{
					if (!isNumber(*iterator))
					{
						rightIsNumber = false;
						break;
					}
				}

				if (!rightIsNumber)
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Additive error: right variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				else
				{
					//right is number
					bool operationSuccessful = stringToBigNumber(substringBreakdown[index + 1], rhs);
					if (!operationSuccessful)
					{
						SetConsoleTextAttribute(hConsole, 4);
						cout << "Additive operation fail when converting right hand string to a big number, terminating!\n";
						SetConsoleTextAttribute(hConsole, 7);
						return false;
					}

					if (changeSign)
					{
						if (rhs.isNegative)
						{
							rhs.isNegative = false;
						}
						else
						{
							rhs.isNegative = true;
						}
					}
				}

				BigNumber result;
				BigNumber resultFractionNumerator;
				BigNumber resultFractionDenominator;
				bool resultIsFraction = false; //to simplify whether it is possible to turn fraction to usual BigNumber
				//starting to adding
				
				if (substringBreakdown[index] == "+")
				{
					//big number + big number
					result = BigNumber::add(lhs, rhs);
				}
				else
				{
					//bigNumber - bigNumber
					result = BigNumber::subtract(lhs, rhs);
				}

				//operation is finished
				//beginning replacing the String
				string replacementString;
				if (!bigNumberToString(result, replacementString))
				{
					SetConsoleTextAttribute(hConsole, 4);
					cout << "Additive error while converting big number result to string, terminating\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
				replaceWithSubstring(substringBreakdown, substringBreakdown.begin() + index - 1, substringBreakdown.begin() + index + 2, replacementString);
				index--;
			}
		}
		return true;
	}

	static bool doOperationModded(vector <string>& substringBreakdown, string& returnString)
	{
		if (checkForEqualSign(substringBreakdown) != substringBreakdown.size())
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "Terminating operation for finding equal sign in the middle of operation line\n";
			SetConsoleTextAttribute(hConsole, 7);
			return false;
		}
		//check for double symbol (handles all the unary operator)
		while (!checkForNoArithmeticSign(substringBreakdown))
		{
			if (!doubleSymbolCheck(substringBreakdown))
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Error when checking double symbol\n";
				SetConsoleTextAttribute(hConsole, 7);
				return false;
			}
			//check for factorial
			if (!findingFactorialModded(substringBreakdown))
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Error when doing factorial operation\n";
				SetConsoleTextAttribute(hConsole, 7);
				return false;
			}
			//check for power
			if (!findingPowerModded(substringBreakdown))
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Error when doing power operation\n";
				SetConsoleTextAttribute(hConsole, 7);
				return false;
			}
			//check for multiplicative
			if (!findingMultiplicativeModded(substringBreakdown))
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Error when doing multiplicative operation\n";
				SetConsoleTextAttribute(hConsole, 7);
				return false;
			}
			//check for additive
			if (!findingAdditiveModded(substringBreakdown))
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Error when doing additive operation\n";
				SetConsoleTextAttribute(hConsole, 7);
				return false;
			}
		}

		//make sure that the left vector is only 1, if not that means there is error where the sign is too less
		if (substringBreakdown.size() > 1 || substringBreakdown.size() < 1)
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "Error since return string is longer than 1 size, try checking whether the symbol is too less\n";
			SetConsoleTextAttribute(hConsole, 7);
			return false;
		}
		else
		{
			returnString = substringBreakdown[0];
			return true;
		}
	}

	static bool doOperationParanthesisModded(vector <string>& substringBreakdown, string& returnString)
	{
		pair<int, int> paranthesisPair;
		paranthesisPair = paranthesisInside(substringBreakdown);
		while (paranthesisPair.first != 0 || paranthesisPair.second != 0)
		{
			if (paranthesisPair.first == substringBreakdown.size() && paranthesisPair.second == substringBreakdown.size())
			{
				SetConsoleTextAttribute(hConsole, 12);
				cout << "Something wrong when finding the paranthesis\n";
				SetConsoleTextAttribute(hConsole, 7);
				return false;
			}

			//start making a smaller subVector
			vector<string> smallerSubstring(substringBreakdown.begin() + paranthesisPair.first + 1, substringBreakdown.begin() + paranthesisPair.second - 1);
			string returnSmallerSubstring;
			if (!doOperationModded(smallerSubstring, returnSmallerSubstring))
			{
				SetConsoleTextAttribute(hConsole, 4);
				cout << "Something wrong when doing the operation inside the paranthesis\n";
				SetConsoleTextAttribute(hConsole, 7);
				return false;
			}

			replaceWithSubstring(substringBreakdown, substringBreakdown.begin() + paranthesisPair.first, substringBreakdown.begin() + paranthesisPair.second, returnSmallerSubstring);
			paranthesisPair = paranthesisInside(substringBreakdown);



			/*for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
			{
				cout << *iterator << " ";
			}*/
		}

		//no more paranthesis, directly do one last final operation
		if (!doOperationModded(substringBreakdown, returnString))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Something wrong when doing the final operation without paranthesis\n";
			SetConsoleTextAttribute(hConsole, 7);
			return false;
		}

		/*for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
		{
			cout << *iterator << " ";
		}
		cout << endl;*/
		return true;
	}

	static void spaceAdditionBigNumber(string& line)
	{
		int currentIndex = 0;
		while (currentIndex < line.length())
		{
			if (isArithmeticEqualSymbol(line[currentIndex]))
			{
				if (currentIndex != 0)
				{
					if (line[currentIndex - 1] != ' ')
					{
						line.insert(currentIndex, " ");
					}
				}

				if (currentIndex != line.length() - 1)
				{
					if (line[currentIndex + 1] != ' ')
					{
						line.insert(currentIndex + 1, " ");
					}
				}
			}
			currentIndex++;
		}
	}

	static bool symbolExist(string line)
	{
		for (auto iterator = line.begin(); iterator != line.end(); iterator++)
		{
			if (isArithmeticSymbol(*iterator))
			{
				return true;
			}
		}
		return false;
	}

	//different version than the parser
	static bool invalidLineBigNumber(string& line)
	{
		stringstream lineStream(line);
		string token;
		string unknownToken;
		while (lineStream >> token)
		{
			if (token == "Power")
			{
				continue;
			}
			else if (token == ",")
			{
				continue;
			}
			else if (token.length() == 1 && isArithmeticSymbol(token[0]))
			{
				continue;
			}
			else
			{
				bool allNumber = true;
				for (auto iterator = token.begin(); iterator != token.end(); iterator++)
				{
					if (!isNumber(*iterator))
					{
						allNumber = false;
					}
				}
				if (allNumber)
				{
					continue;
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "there is non number inside the string for the right value of the variable, terminating\n";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
			}
		}
		return true;
	}

	static bool bigNumberToString(BigNumber toBeConverted, string& result)
	{
		result.clear(); //just in case
		if (toBeConverted.isNegative)
		{
			result += "-";
		}
		result += toBeConverted.integer;
		if (toBeConverted.isDecimal)
		{
			result += ".";
			if (toBeConverted.decimal.empty())
			{
				result += "0";
			}
			else
			{
				for (int index = 0; index < 100; index++)
				{
					if (index == toBeConverted.decimal.length())
					{
						break;
					}
					else
					{
						result.push_back(toBeConverted.decimal[index]);
					}
				}
			}
		}
		return true;
	}

	static bool stringToBigNumber(string toBeConverted, BigNumber& result)
	{
		bool isNegative = false;
		if (toBeConverted[0] == '-')
		{
			isNegative = true;
			toBeConverted.erase(toBeConverted.begin());
		}
		bool isDecimal = false;
		int lastIntegerIndex;
		int index = 0;
		for (auto iterator = toBeConverted.begin(); iterator != toBeConverted.end(); iterator++)
		{
			if (index == 100)
			{
				break;
			}
			if (isDecimal)
			{
				index++;
			}
			if (*iterator == '.')
			{
				if (!isDecimal)
				{
					isDecimal = true;
					lastIntegerIndex = iterator - toBeConverted.begin();
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 12);
					cout << "Error! Two decimal points found";
					SetConsoleTextAttribute(hConsole, 7);
					return false;
				}
			}
		}

		if (isDecimal)
		{
			result.integer = toBeConverted.substr(0, lastIntegerIndex);
			result.decimal = toBeConverted.substr(lastIntegerIndex + 1, index);
			bool allZero = true;
			for (auto iterator = result.decimal.begin(); iterator != result.decimal.end(); iterator++)
			{
				if (*iterator != '0')
				{
					allZero = false;
				}
			}

			if (allZero)
			{
				result.decimal = "";
			}
			result.isNegative = isNegative;
			result.isDecimal = true;
		}
		else
		{
			//not decimal
			result.integer = toBeConverted.substr(0, toBeConverted.length());
			result.decimal = "";
			result.isNegative = isNegative;
			result.isDecimal = false;
		}
		if (result.integer.length() == 0)
		{
			result.integer = "0";
		}
		else
		{
			for (int index = 0; index < result.integer.size(); index++)
			{
				if (result.integer.size() == 1 || result.integer[index] != '0')
				{
					break;
				}
				else if (result.integer[index] == '0')
				{
					result.integer.erase(result.integer.begin() + index);
					index--;
				}
			}
		}
		return true; 
	}

	string getbigNumberToString()
	{
		string returnString;
		BigNumber::bigNumberToString(*this, returnString);
		return returnString;
	}

	static bool isLarger(string a, string b)
	{
		if (a.length() > b.length())
		{
			return true;
		}
		else if (a.length() < b.length())
		{
			return false;
		}
		else
		{
			int same = 0;
			for (int i = 0; i < a.length(); i++)
			{
				if (a[i] > b[i])
				{
					return true;
				}
				else if (a[i] < b[i])
				{
					return false;
				}
				else
				{
					same++;
				}
			}
			if (same == a.length())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	static bool isLargerThanError(BigNumber num)
	{
		BigNumber errorBound = {
		  "0",
		  "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001",
		  false,
		  true
		};

		if (num.integer != "0")
		{
			return true;
		}
		else
		{
			string numDecimals = num.decimal;
			for (int i = 0; i < 100 - numDecimals.length(); i++)
			{
				numDecimals += "0";
			}

			for (int i = 0; i < 100; i++)
			{
				if (numDecimals[i] > errorBound.decimal[i])
				{
					return true;
				}
				else if (numDecimals[i] < errorBound.decimal[i])
				{
					return false;
				}
			}
			return false; // means same

		}


	}

	static bool isEven(string num)
	{
		BigNumber number = {
		  num,
		  "",
		  false,
		  false
		};
		BigNumber constant2 = {
		  "2",
		  "",
		  false,
		  false
		};

		BigNumber res = divide(number, constant2);
		BigNumber res2 = multiply(res, constant2);
		// cout << "("<< res.integer << " " << res2.integer << ") ";

		if (number.integer == res2.integer)
		{
			return true;
		}
		else
		{
			return false;
		}


	}


	static BigNumber add(BigNumber numA, BigNumber numB)
	{
		eraseZero_Int(numA.integer);
		eraseZero_Int(numB.integer);
		eraseZero_Dec(numA.decimal);
		eraseZero_Dec(numB.decimal);
		bool doAddition = true;
		BigNumber finalRes;
		// initialize isNegative and isDecimal;
		if (!numA.isNegative && !numB.isNegative)
		{
			finalRes.isNegative = false;
			doAddition = true;
		}
		else if (numA.isNegative && numB.isNegative)
		{
			finalRes.isNegative = true;
			doAddition = true;
		}
		else if ((!numA.isNegative && numB.isNegative) || (numA.isNegative && !numB.isNegative))
		{
			// isNegative still unknown
			doAddition = false;
		}

		if (numA.isDecimal || numB.isDecimal)
		{
			finalRes.isDecimal = true;
		}
		else
		{
			finalRes.isDecimal = false;
		}

		if (doAddition)
		{
			// DO ADDITION
			vector<int> a, b, res; // temporary variables
			int excess = 0; // excess variable

			// ----------- DO DECIMALS -----------
			if (finalRes.isDecimal)
			{
				convertToVector(numA.decimal, a);
				convertToVector(numB.decimal, b);

				if (a.size() < b.size()) // let a length longer then b length
				{
					swap(a, b);
				}

				for (int i = 0; i < a.size() - b.size(); i++)
				{
					b.insert(b.begin(), 0);
				}

				for (int i = 0; i < a.size(); i++)
				{
					int tmp; // temporary result
					if (i <= b.size() - 1) // if i is still in b size range
					{
						tmp = miniAdd(a[i], b[i], excess);
					}
					else
					{
						// b is 0 here
						tmp = tmp = miniAdd(a[i], 0, excess);
					}

					res.push_back(tmp);
				}

				// overwrite to final result of DECIMALS
				string reverseVect = "";
				for (int i = res.size() - 1; i >= 0; i--)
				{
					reverseVect.push_back(res[i] + '0');
				}
				eraseZero_Dec(reverseVect);
				finalRes.decimal = reverseVect;
			}

			// ----------- END OF DO DECIMAL -----------

			// clear all temporary vairables
			a.clear();
			b.clear();
			res.clear();

			// ----------- DO INTEGER -----------
			convertToVector(numA.integer, a);
			convertToVector(numB.integer, b);
			if (a.size() < b.size()) // let a length longer then b length
			{
				swap(a, b);
			}

			for (int i = 0; i < a.size(); i++)
			{
				int tmp; // temporary result
				if (i <= b.size() - 1) // if i is still in b size range
				{
					tmp = miniAdd(a[i], b[i], excess);
				}
				else
				{
					// b is 0 here
					tmp = miniAdd(a[i], 0, excess);
				}

				res.push_back(tmp);
			}

			if (excess != 0)
			{
				res.push_back(excess);
			}

			string reverseVect = "";
			for (int i = res.size() - 1; i >= 0; i--)
			{
				reverseVect.push_back((char)(res[i] + '0'));
			}

			eraseZero_Int(reverseVect);
			finalRes.integer = reverseVect;

			// ----------- END OF DO INTEGER -----------

			// return final result
			return finalRes;
		}
		else
		{
			// DO SUBTRACTION
			vector<int> a, b, res; // temporary variables
			bool needSwap = false;

			if (numA.isNegative && !numB.isNegative)
			{
				swap(numA, numB);
			}

			// check whether swap is necessary
			convertToVector(numA.integer, a);
			convertToVector(numB.integer, b);
			if (a.size() < b.size()) // let a length longer then b length
			{
				needSwap = true;
			}
			else if (a.size() == b.size())
			{
				if (numA.integer == numB.integer)
				{
					if (!needSwap && finalRes.isDecimal) // check decimal
					{
						a.clear();
						b.clear();
						convertToVector(numA.decimal, a);
						convertToVector(numB.decimal, b);

						// insert zeros
						int lengthA = a.size(), lengthB = b.size();
						if (lengthA > lengthB)
						{
							for (int i = 0; i < lengthA - lengthB; i++)
							{
								b.insert(b.begin(), 0);
							}
						}
						else if (lengthA < lengthB)
						{
							for (int i = 0; i < lengthB - lengthA; i++)
							{
								a.insert(a.begin(), 0);
							}
						}

						for (int i = a.size() - 1; i >= 0; i--)
						{
							if (a[i] < b[i])
							{
								needSwap = true;
								break;
							}
						}

					}
				}
				else // check integer
				{
					for (int i = a.size() - 1; i >= 0; i--)
					{
						if (a[i] < b[i])
						{
							needSwap = true;
							break;
						}
					}
				}
			}
			else
			{
				needSwap = false;
			}

			a.clear();
			b.clear();

			// ----------- DO DECIMALS -----------
			bool borrowed = false;
			if (finalRes.isDecimal)
			{
				convertToVector(numA.decimal, a);
				convertToVector(numB.decimal, b);

				if (needSwap)
				{
					swap(a, b);
				}

				// insert zeros
				int lengthA = a.size(), lengthB = b.size();
				if (lengthA > lengthB)
				{
					for (int i = 0; i < lengthA - lengthB; i++)
					{
						b.insert(b.begin(), 0);
					}
				}
				else if (lengthA < lengthB)
				{
					for (int i = 0; i < lengthB - lengthA; i++)
					{
						a.insert(a.begin(), 0);
					}
				}

				for (int i = 0; i < a.size(); i++)
				{
					int tmp; // temporary result
					if (i <= b.size() - 1) // if i is still in b size range
					{
						tmp = miniSubtract(a[i], b[i], borrowed);
					}
					else
					{
						// b is 0 here
						tmp = miniSubtract(a[i], 0, borrowed);
					}

					res.push_back(tmp);
				}

				// overwrite to final result of DECIMALS
				string reverseVect = "";
				for (int i = res.size() - 1; i >= 0; i--)
				{
					reverseVect.push_back(res[i] + '0');
				}
				eraseZero_Dec(reverseVect);
				finalRes.decimal = reverseVect;
			}

			// ----------- END OF DO DECIMAL -----------

			// clear all temporary vairables
			a.clear();
			b.clear();
			res.clear();

			// ----------- DO INTEGER -----------
			convertToVector(numA.integer, a);
			convertToVector(numB.integer, b);
			if (needSwap)
			{
				swap(a, b);
			}

			for (int i = 0; i < a.size(); i++)
			{
				int tmp; // temporary result
				if (i <= b.size() - 1) // if i is still in b size range
				{
					tmp = miniSubtract(a[i], b[i], borrowed);
				}
				else
				{
					// b is 0 here
					tmp = miniSubtract(a[i], 0, borrowed);
				}

				res.push_back(tmp);
			}

			string reverseVect = "";
			for (int i = res.size() - 1; i >= 0; i--)
			{
				reverseVect.push_back(res[i] + '0');
			}
			eraseZero_Int(reverseVect);
			finalRes.integer = reverseVect;

			// ----------- END OF DO INTEGER -----------

			if (needSwap)
			{
				finalRes.isNegative = true;
			}
			else
			{
				finalRes.isNegative = false;
			}

			// return final result
			return finalRes;
		}

	}

	static BigNumber subtract(BigNumber numA, BigNumber numB)
	{
		eraseZero_Int(numA.integer);
		eraseZero_Int(numB.integer);
		eraseZero_Dec(numA.decimal);
		eraseZero_Dec(numB.decimal);
		bool doAddition = false;
		BigNumber finalRes;
		// initialize isNegative and isDecimal;
		if (!numA.isNegative && numB.isNegative)
		{
			finalRes.isNegative = false;
			doAddition = true;
		}
		else if (numA.isNegative && !numB.isNegative)
		{
			finalRes.isNegative = true;
			doAddition = true;
		}
		else if ((!numA.isNegative && !numB.isNegative) || (numA.isNegative && !numB.isNegative))
		{
			// isNegative still unknown
			doAddition = false;
		}

		if (numA.isDecimal || numB.isDecimal)
		{
			finalRes.isDecimal = true;
		}
		else
		{
			finalRes.isDecimal = false;
		}

		if (doAddition)
		{
			// DO ADDITION
			vector<int> a, b, res; // temporary variables
			int excess = 0; // excess variable

			// ----------- DO DECIMALS -----------
			if (finalRes.isDecimal)
			{
				convertToVector(numA.decimal, a);
				convertToVector(numB.decimal, b);

				if (a.size() < b.size()) // let a length longer then b length
				{
					swap(a, b);
				}

				for (int i = 0; i < a.size() - b.size(); i++)
				{
					b.insert(b.begin(), 0);
				}

				for (int i = 0; i < a.size(); i++)
				{
					int tmp; // temporary result
					if (i <= b.size() - 1) // if i is still in b size range
					{
						tmp = miniAdd(a[i], b[i], excess);
					}
					else
					{
						// b is 0 here
						tmp = tmp = miniAdd(a[i], 0, excess);
					}

					res.push_back(tmp);
				}

				// overwrite to final result of DECIMALS
				string reverseVect = "";
				for (int i = res.size() - 1; i >= 0; i--)
				{
					reverseVect.push_back(res[i] + '0');
				}
				eraseZero_Dec(reverseVect);
				finalRes.decimal = reverseVect;
			}

			// ----------- END OF DO DECIMAL -----------

			// clear all temporary vairables
			a.clear();
			b.clear();
			res.clear();

			// ----------- DO INTEGER -----------
			convertToVector(numA.integer, a);
			convertToVector(numB.integer, b);
			if (a.size() < b.size()) // let a length longer then b length
			{
				swap(a, b);
			}

			for (int i = 0; i < a.size(); i++)
			{
				int tmp; // temporary result
				if (i <= b.size() - 1) // if i is still in b size range
				{
					tmp = miniAdd(a[i], b[i], excess);
				}
				else
				{
					// b is 0 here
					tmp = tmp = miniAdd(a[i], 0, excess);
				}

				res.push_back(tmp);
			}

			if (excess != 0)
			{
				res.push_back(excess);
			}

			string reverseVect = "";
			for (int i = res.size() - 1; i >= 0; i--)
			{
				reverseVect.push_back(res[i] + '0');
			}
			eraseZero_Int(reverseVect);
			finalRes.integer = reverseVect;

			// ----------- END OF DO INTEGER -----------

			// return final result
			return finalRes;
		}
		else
		{
			// DO SUBTRACTION
			vector<int> a, b, res; // temporary variables
			bool needSwap = false;

			if (numA.isNegative && numB.isNegative)
			{
				swap(numA, numB);
			}

			// check whether swap is necessary
			convertToVector(numA.integer, a);
			convertToVector(numB.integer, b);

			if (a.size() < b.size()) // let a length longer then b length
			{
				needSwap = true;
			}
			else if (a.size() == b.size())
			{
				if (numA.integer == numB.integer)
				{
					if (!needSwap && finalRes.isDecimal) // check decimal
					{
						a.clear();
						b.clear();
						convertToVector(numA.decimal, a);
						convertToVector(numB.decimal, b);

						// insert zeros
						int lengthA = a.size(), lengthB = b.size();
						if (lengthA > lengthB)
						{
							for (int i = 0; i < lengthA - lengthB; i++)
							{
								b.insert(b.begin(), 0);
							}
						}
						else if (lengthA < lengthB)
						{
							for (int i = 0; i < lengthB - lengthA; i++)
							{
								a.insert(a.begin(), 0);
							}
						}

						for (int i = a.size() - 1; i >= 0; i--)
						{
							if (a[i] < b[i])
							{
								needSwap = true;
								break;
							}
						}

					}
				}
				else // check integer
				{
					for (int i = a.size() - 1; i >= 0; i--)
					{
						if (a[i] < b[i])
						{
							needSwap = true;
							break;
						}
					}
				}
			}
			else
			{
				needSwap = false;
			}

			a.clear();
			b.clear();

			// ----------- DO DECIMALS -----------
			bool borrowed = false;
			if (finalRes.isDecimal)
			{
				convertToVector(numA.decimal, a);
				convertToVector(numB.decimal, b);

				if (needSwap)
				{
					swap(a, b);
				}

				// insert zeros
				int lengthA = a.size(), lengthB = b.size();
				if (lengthA > lengthB)
				{
					for (int i = 0; i < lengthA - lengthB; i++)
					{
						b.insert(b.begin(), 0);
					}
				}
				else if (lengthA < lengthB)
				{
					for (int i = 0; i < lengthB - lengthA; i++)
					{
						a.insert(a.begin(), 0);
					}
				}

				for (int i = 0; i < a.size(); i++)
				{
					int tmp; // temporary result
					if (i <= b.size() - 1) // if i is still in b size range
					{
						tmp = miniSubtract(a[i], b[i], borrowed);
					}
					else
					{
						// b is 0 here
						tmp = miniSubtract(a[i], 0, borrowed);
					}

					res.push_back(tmp);
				}

				// overwrite to final result of DECIMALS
				string reverseVect = "";
				for (int i = res.size() - 1; i >= 0; i--)
				{
					reverseVect.push_back(res[i] + '0');
				}
				eraseZero_Dec(reverseVect);
				finalRes.decimal = reverseVect;
			}

			// ----------- END OF DO DECIMAL -----------

			// clear all temporary vairables
			a.clear();
			b.clear();
			res.clear();

			// ----------- DO INTEGER -----------
			convertToVector(numA.integer, a);
			convertToVector(numB.integer, b);
			if (needSwap)
			{
				swap(a, b);
			}

			for (int i = 0; i < a.size(); i++)
			{
				int tmp; // temporary result
				if (i <= b.size() - 1) // if i is still in b size range
				{
					tmp = miniSubtract(a[i], b[i], borrowed);
				}
				else
				{
					// b is 0 here
					tmp = miniSubtract(a[i], 0, borrowed);
				}

				res.push_back(tmp);
			}

			string reverseVect = "";
			for (int i = res.size() - 1; i >= 0; i--)
			{
				reverseVect.push_back(res[i] + '0');
			}
			eraseZero_Int(reverseVect);
			finalRes.integer = reverseVect;

			// ----------- END OF DO INTEGER -----------

			if (needSwap)
			{
				finalRes.isNegative = true;
			}
			else
			{
				finalRes.isNegative = false;
			}

			// return final result
			return finalRes;
		}


	}

	static BigNumber multiply(BigNumber numA, BigNumber numB)
	{
		eraseZero_Int(numA.integer);
		eraseZero_Int(numB.integer);
		eraseZero_Dec(numA.decimal);
		eraseZero_Dec(numB.decimal);
		BigNumber finalRes;
		// initalize isNegative and isDecimal
		if ((numA.isNegative && numB.isNegative) || (!numA.isNegative && !numB.isNegative))
		{
			finalRes.isNegative = false;
		}
		else if ((!numA.isNegative && numB.isNegative) || (numA.isNegative && !numB.isNegative))
		{
			finalRes.isNegative = true;
		}

		if (numA.isDecimal || numB.isDecimal)
		{
			finalRes.isDecimal = true;
		}
		else
		{
			finalRes.isDecimal = false;
		}

		// initialize string to be vector
		string stringA = numA.integer;
		string stringB = numB.integer;

		if (finalRes.isDecimal)
		{
			stringA += numA.decimal;
			stringB += numB.decimal;
		}

		vector<int> a, b, res; // temporary variables
		convertToVector(stringA, a);
		convertToVector(stringB, b);

		if (a.size() < b.size())
		{
			swap(a, b);
		}


		// DO MULTIPLICATION
		for (int i = 0; i < b.size(); i++)
		{
			vector<int> tmpRes, newRes; // temporary variables

			int excess = 0;
			for (int j = 0; j < a.size(); j++)
			{
				int tmp = miniMultiply(a[j], b[i], excess);
				tmpRes.push_back(tmp);
			}
			if (excess != 0)
			{
				tmpRes.push_back(excess);
			}

			// add tmpRes to res
			if (i == 0)
			{
				res = tmpRes;
			}
			else
			{
				for (int j = 0; j < i; j++)
				{
					tmpRes.insert(tmpRes.begin(), 0);
				}

				excess = 0; // reset excess
				for (int j = 0; j < tmpRes.size(); j++)
				{
					int tmp;
					if (j <= res.size() - 1)
					{
						tmp = miniAdd(tmpRes[j], res[j], excess);
					}
					else
					{
						tmp = miniAdd(tmpRes[j], 0, excess);
					}

					newRes.push_back(tmp);
				}

				if (excess != 0)
				{
					newRes.push_back(excess);
				}

				res = newRes;
			}
		}


		string reverseVect = "";
		for (int i = res.size() - 1; i >= 0; i--)
		{
			reverseVect.push_back(res[i] + '0');
		}

		if (finalRes.isDecimal)
		{
			int decimalLength = numA.decimal.length() + numB.decimal.length();
			int integerLength = reverseVect.length() - decimalLength;

			for (int i = 0; i < integerLength; i++)
			{
				finalRes.integer.push_back(reverseVect[i]);
			}

			for (int i = integerLength; i < integerLength + decimalLength; i++)
			{
				finalRes.decimal.push_back(reverseVect[i]);
			}

			eraseZero_Dec(finalRes.decimal);
			eraseZero_Int(finalRes.integer);
		}
		else
		{
			finalRes.decimal = "";
			finalRes.integer = reverseVect;
			eraseZero_Int(finalRes.integer);
		}

		return finalRes;
	}

	static BigNumber power(BigNumber numA, BigNumber numB)
	{
		// cout << "check B: "<< numB.integer << " is "<< isEven(numB.integer) << endl;
		// numB could be 0 or 5 ONLY
		BigNumber finalRes;
		int decimalCount = 0;
		string powerBase = numA.integer;
		string powerLoop = numB.integer;
		string countLoop = "0";
		string powerRes = "1";

		if (numA.isDecimal)
		{
			finalRes.isDecimal = true;
			powerBase += numA.decimal;
			decimalCount = 0;
		}

		if (numA.isDecimal || numB.isDecimal)
		{
			finalRes.isDecimal = true;
		}
		else
		{
			finalRes.isDecimal = false;
		}

		eraseZero_Dec(numB.decimal);

		vector<int> a, b;
		convertToVector(powerRes, a);
		convertToVector(powerBase, b);

		// START POWERING INTEGER PART
		while (countLoop != powerLoop)
		{
			// **** COUNTLOOP INCREMENT ****
			vector<int> tmp1, tmp2, newCountLoop;
			convertToVector(countLoop, tmp1);
			convertToVector("1", tmp2);
			if (tmp1.size() < tmp2.size()) // let a length longer then b length
			{
				swap(tmp1, tmp2);
			}

			int excess = 0;
			for (int i = 0; i < tmp1.size(); i++)
			{
				int tmp; // temporary result
				if (i <= tmp2.size() - 1) // if i is still in b size range
				{
					tmp = miniAdd(tmp1[i], tmp2[i], excess);
				}
				else
				{
					// tmp2 is 0 here
					tmp = miniAdd(tmp1[i], 0, excess);
				}

				newCountLoop.push_back(tmp);
			}

			if (excess != 0)
			{
				newCountLoop.push_back(excess);
			}

			string reverseVect = "";
			for (int i = newCountLoop.size() - 1; i >= 0; i--)
			{
				reverseVect.push_back((char)(newCountLoop[i] + '0'));
			}

			eraseZero_Int(reverseVect);
			countLoop = reverseVect;

			tmp1.clear();
			tmp2.clear();
			newCountLoop.clear();
			// cout << countLoop << endl;

			// **** END OF COUNTLOOP INCREMENT ****

			// **** DECIMAL COUNT ****

			if (finalRes.isDecimal)
			{
				decimalCount += numA.decimal.length();
			}

			// **** END OF DECIMAL COUNT ****

			// START MULTIPLYING
			vector<int> res;
			for (int i = 0; i < b.size(); i++)
			{
				vector<int> tmpRes, newRes; // temporary variables

				int excess = 0;
				for (int j = 0; j < a.size(); j++)
				{
					int tmp = miniMultiply(a[j], b[i], excess);
					tmpRes.push_back(tmp);
				}
				if (excess != 0)
				{
					tmpRes.push_back(excess);
				}

				// add tmpRes to res
				if (i == 0)
				{
					res = tmpRes;
				}
				else
				{
					for (int j = 0; j < i; j++)
					{
						tmpRes.insert(tmpRes.begin(), 0);
					}

					excess = 0; // reset excess
					for (int j = 0; j < tmpRes.size(); j++)
					{
						int tmp;
						if (j <= res.size() - 1)
						{
							tmp = miniAdd(tmpRes[j], res[j], excess);
						}
						else
						{
							tmp = miniAdd(tmpRes[j], 0, excess);
						}

						newRes.push_back(tmp);
					}

					if (excess != 0)
					{
						newRes.push_back(excess);
					}

					res = newRes;
				}
			}

			a = res;
		}

		string reverseVect = "";
		for (int i = a.size() - 1; i >= 0; i--)
		{
			reverseVect.push_back(a[i] + '0');
		}
		eraseZero_Int(reverseVect);
		powerRes = reverseVect;

		int integerCount = powerRes.length() - decimalCount;
		for (int i = 0; i < integerCount; i++)
		{
			finalRes.integer.push_back(powerRes[i]);
		}

		if (decimalCount > 0)
		{
			for (int i = integerCount; i < integerCount + decimalCount; i++)
			{
				finalRes.decimal.push_back(powerRes[i]);
			}
		}

		if (numA.isNegative && (numB.isDecimal && numB.decimal == "5"))
		{
			cout << "Exception found: (powerOperation.cpp) base is negative and power is decimal .5" << endl;
			BigNumber errorNumber = {
			  "-1",
			  "",
			  false,
			  false
			};
			return errorNumber;
		}
		else
		{
			// check even odd
			if (isEven(numB.integer)) // even
			{
				finalRes.isNegative = false;
			}
			else
			{
				// odd
				if (numA.isNegative)
				{
					finalRes.isNegative = true;
				}
				else
				{
					finalRes.isNegative = false;
				}
			}
		}



		a.clear();
		b.clear();

		if (numB.isDecimal && numB.decimal == "5")
		{
			// NB: ALL BIGNUMBER OPERATION

			BigNumber n, r, guess, prevGuess;
			BigNumber constant2;
			// init constant2
			constant2.integer = "2";
			constant2.decimal = "";
			constant2.isNegative = false;
			constant2.isDecimal = true;
			// init n
			n.integer = numA.integer;
			n.decimal = numA.decimal;
			n.isNegative = numA.isNegative;
			n.isDecimal = numA.isDecimal;

			// ------- PRE BABYLONIAN POWER -------

			guess = divide(n, constant2);

			// ------- START BABYLONIAN POWER -------

			// babylion algorithm
			do
			{
				r = divide(n, guess);
				prevGuess = guess;
				guess = divide(add(guess, r), constant2);
				BigNumber tmpRess = subtract(prevGuess, guess);
			} while (isLargerThanError(subtract(prevGuess, guess))); // babylion algorithm restrictions

			// ------- END OF BABYLONIAN POWER -------
			
			finalRes = multiply(finalRes, guess);
		}

		if (finalRes.decimal.length() > 100)
		{
			while (finalRes.decimal.length() != 100)
			{
				finalRes.decimal.erase(finalRes.decimal.length() - 1);
			}
		}

		return finalRes;
	}

	static BigNumber divide(BigNumber numA, BigNumber numB)
	{
		eraseZero_Int(numA.integer);
		eraseZero_Int(numB.integer);
		eraseZero_Dec(numA.decimal);
		eraseZero_Dec(numB.decimal);
		BigNumber finalRes;
		// initialize isNegative
		if ((numA.isNegative && numB.isNegative) || (!numA.isNegative && !numB.isNegative))
		{
			finalRes.isNegative = false;
		}
		else if ((!numA.isNegative && numB.isNegative) || (numA.isNegative && !numB.isNegative))
		{
			finalRes.isNegative = true;
		}

		string stringA = numA.integer;
		string stringB = numB.integer;
		// string toBePassres = "";

		// combine decimal and integer, then eras
		if (numA.isDecimal || numB.isDecimal)
		{
			finalRes.isDecimal = true;
			int maxDec = max(numA.decimal.length(), numB.decimal.length());

			stringA += numA.decimal;
			for (int i = 0; i < maxDec - numA.decimal.length(); i++)
			{
				stringA += "0";
			}

			stringB += numB.decimal;
			for (int i = 0; i < maxDec - numB.decimal.length(); i++)
			{
				stringB += "0";
			}
		}
		else
		{
			finalRes.isDecimal = false;
			finalRes.decimal = "";
		}

		// DO DIVISION
		string referencePoint = "";
		string quotient = "";
		for (int x = 0; x < stringA.length(); x++)
		{
			referencePoint.push_back(stringA[x]);
			int count = 0;
			while (isLarger(referencePoint, stringB))
			{
				count++;
				vector<int> a, b, res;
				convertToVector(referencePoint, a);
				convertToVector(stringB, b);

				bool borrowed = false;
				for (int i = 0; i < a.size(); i++)
				{
					int tmp; // temporary result
					if (i <= b.size() - 1) // if i is still in b size range
					{
						tmp = miniSubtract(a[i], b[i], borrowed);
					}
					else
					{
						// b is 0 here
						tmp = miniSubtract(a[i], 0, borrowed);
					}
					res.push_back(tmp);
				}

				string reverseVect = "";
				for (int i = res.size() - 1; i >= 0; i--)
				{
					reverseVect.push_back(res[i] + '0');
				}
				eraseZero_Int(reverseVect);
				referencePoint = reverseVect;
				if (referencePoint.length() == 1 && referencePoint == "0")
				{
					referencePoint = "";
				}
			}
			if (referencePoint.length() == 1 && referencePoint == "0")
			{
				referencePoint = "";
			}
			quotient.push_back(count + '0');
		}

		eraseZero_Int(quotient);
		
		finalRes.integer = quotient;
		

		string decQuotient = "";
		if (finalRes.isDecimal && (referencePoint == "" || referencePoint == "0"))
		{
			finalRes.decimal = "";
		}
		else if (finalRes.isDecimal && referencePoint != "")
		{
			while (decQuotient.length() < 100 && referencePoint != "")
			{
				referencePoint.push_back('0');
				int count = 0;
				while (isLarger(referencePoint, stringB))
				{
					count++;
					vector<int> a, b, res;
					convertToVector(referencePoint, a);
					convertToVector(stringB, b);

					bool borrowed = false;
					for (int i = 0; i < a.size(); i++)
					{
						int tmp; // temporary result
						if (i <= b.size() - 1) // if i is still in b size range
						{
							tmp = miniSubtract(a[i], b[i], borrowed);
						}
						else
						{
							// b is 0 here
							tmp = miniSubtract(a[i], 0, borrowed);
						}

						res.push_back(tmp);
					}

					string reverseVect = "";
					for (int i = res.size() - 1; i >= 0; i--)
					{
						reverseVect.push_back((char)(res[i] + '0'));
					}
					eraseZero_Int(reverseVect);
					referencePoint = reverseVect;

					if (referencePoint == "0")
					{
						referencePoint = "";
					}
				}
				decQuotient.push_back(count + '0');
			}
			finalRes.decimal = decQuotient;
		}

		return finalRes;

	}

	static BigNumber factorial(BigNumber num)
	{
		eraseZero_Int(num.integer);
		eraseZero_Dec(num.decimal);
		BigNumber finalRes = {
		  "1",
		  "",
		  false,
		  false
		};
		BigNumber tmp = num;
		BigNumber constant1 = {
		  "1",
		  "",
		  false,
		  false
		};

		bool firstTake = false;
		while (tmp.integer != "1" && tmp.integer != "0")
		{
			if (!firstTake)
			{
				firstTake = true;
				finalRes = tmp;
				tmp = subtract(tmp, constant1);
			}
			else
			{
				finalRes = multiply(finalRes, tmp);
				tmp = subtract(tmp, constant1);
			}
		}

		return finalRes;

	}

	
	//operator cin cout
	friend ostream& operator<<(ostream& output, BigNumber obj)
	{
		if (obj.isNegative)
		{
			if (obj.integer != "0" || !obj.decimal.empty())
			{
				output << "-";
			}
			
		}
		output << obj.integer;
		if (obj.isDecimal)
		{
			output << ".";
			if (obj.decimal.empty())
			{
				for (int rep = 0; rep < 100; rep++)
				{
					output << "0";
				}
				
			}
			else
			{
				output << obj.decimal;
				for (int rep = obj.decimal.length(); rep < 100; rep++)
				{
					output << "0";
				}
			}
		}
		
		return output;
	}

	friend istream& operator>>(istream& input, BigNumber& obj)
	{
		string inputString;
		input >> inputString;
		
		BigNumber::stringToBigNumber(inputString, obj);
		return input;
	}

	BigNumber operator-()
	{
		if (isNegative)
		{
			isNegative = false;
		}
		else
		{
			isNegative = true;
		}
		return *this;
	}

	BigNumber operator+()
	{
		//empty no change
		return *this;
	}

	BigNumber operator=(string line)
	{
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
		return *this;
	}

	BigNumber operator=(int input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		return *this;
	}

	BigNumber operator=(float input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		return *this;
	}

	BigNumber operator=(double input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		return *this;
	}

	friend BigNumber operator+(BigNumber numA, BigNumber numB);
	friend BigNumber operator-(BigNumber numA, BigNumber numB);
	friend BigNumber operator*(BigNumber numA, BigNumber numB);
	friend BigNumber operator/(BigNumber numA, BigNumber numB);


};

class Integer : public BigNumber{
public:
	Integer()
	{
		isDecimal = false;
	}

	//constructor dari BigNumber
	Integer(BigNumber obj)
	{
		isDecimal = false;
		isNegative = obj.isNegative;
		integer = obj.integer;
		decimal = "";
	}

	Integer(string line)
	{
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			this->decimal = "";
			this->isDecimal = false;
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
	}

	Integer(const char* input)
	{
		string line = BigNumber::convertToString(input);
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			this->decimal = "";
			this->isDecimal = false;
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
	}

	Integer operator=(const char* input)
	{
		string line = BigNumber::convertToString(input);
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			this->decimal = "";
			this->isDecimal = false;
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
		return *this;
	}

	Integer(int input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		this->isDecimal = false;
		this->decimal = "";
	}

	Integer(float input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		this->isDecimal = false;
		this->decimal = "";
	}

	Integer(double input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		this->isDecimal = false;
		this->decimal = "";
	}

	//operator= string or BigNumber or Integer
	Integer operator=(BigNumber obj)
	{
		isDecimal = false;
		isNegative = obj.isNegative;
		integer = obj.integer;
		decimal = "";
		return *this;
	}

	Integer operator=(Integer obj)
	{
		isDecimal = false;
		isNegative = obj.isNegative;
		integer = obj.integer;
		decimal = "";
		return *this;
	}

	Integer operator=(string line)
	{
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			this->decimal = "";
			this->isDecimal = false;
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
		return *this;
	}

	Integer operator=(int input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		this->isDecimal = false;
		this->decimal = "";
		return *this;
	}

	Integer operator=(float input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		this->isDecimal = false;
		this->decimal = "";
		return *this;
	}

	Integer operator=(double input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		this->isDecimal = false;
		this->decimal = "";
		return *this;
	}
	//operator cin cout
	friend ostream& operator<<(ostream& output, Integer obj)
	{
		if (obj.isNegative)
		{
			if (obj.integer != "0" || !obj.decimal.empty())
			{
				output << "-";
			}
		}
		output << obj.integer;
		
		return output;
	}

	friend istream& operator>>(istream& input, Integer& obj)
	{
		string inputString;
		input >> inputString;
		
		BigNumber::stringToBigNumber(inputString, obj);
		obj.isDecimal = false;
		obj.decimal = "";
		return input;
	}
	
	friend Integer operator+(Integer numA, Integer numB);
	friend Integer operator-(Integer numA, Integer numB);
	friend Integer operator*(Integer numA, Integer numB);
	friend Integer operator/(Integer numA, Integer numB);

	friend Integer operator+(Integer numA, BigNumber numB);
	friend Integer operator-(Integer numA, BigNumber numB);
	friend Integer operator*(Integer numA, BigNumber numB);
	friend Integer operator/(Integer numA, BigNumber numB);

	friend Integer operator+(BigNumber numA, Integer numB);
	friend Integer operator-(BigNumber numA, Integer numB);
	friend Integer operator*(BigNumber numA, Integer numB);
	friend Integer operator/(BigNumber numA, Integer numB);
};

class Decimal : public BigNumber{
public:
	string numerator;
	string denominator;
	Decimal()
	{
		isDecimal = true;
		numerator = "0";
		denominator = "1";
	}

	Decimal(string inte, string dec, bool isNeg, bool isDec)
	{
		integer = inte;
		decimal = dec;
		isNegative = isNeg;
		isDecimal = isDec;
		BigNumber::bigNumberToString(*this, numerator);
		denominator = "1";
	}


	static bool decimalFillintegerDecimal(Decimal& obj)
	{
		BigNumber numerator, denominator;
		bool numeratorMinus = false;
		bool denominatorMinus = false;
		if (obj.numerator[0] == '-')
		{
			numeratorMinus = true;
			obj.numerator.erase(obj.numerator.begin());
		}

		if (obj.denominator[0] == '-')
		{
			denominatorMinus = true;
			obj.denominator.erase(obj.denominator.begin());

		}

		if (numeratorMinus == denominatorMinus)
		{
			obj.isNegative = false;
		}
		else
		{
			obj.isNegative = true;
		}
		if (!BigNumber::stringToBigNumber(obj.numerator, numerator) || !BigNumber::stringToBigNumber(obj.denominator, denominator))
		{
			cout << "Error when decimal fill integer and decimal\n";
			return false;
		}
		BigNumber convertedDecimal = divide(numerator, denominator);
		obj.integer = convertedDecimal.integer;
		obj.decimal = convertedDecimal.decimal;
		return true;
	}

	//constructor dari BigNumber
	Decimal(BigNumber obj)
	{
		integer = obj.integer;
		decimal = obj.decimal;
		isNegative = obj.isNegative;
		isDecimal = true;
		BigNumber::bigNumberToString(obj, numerator);
		denominator = "1";
	}

	Decimal(Integer obj)
	{
		integer = obj.integer;
		decimal = obj.decimal;
		isNegative = obj.isNegative;
		isDecimal = true;
		BigNumber::bigNumberToString(obj, numerator);
		denominator = "1";
	}

	Decimal(int input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		BigNumber::bigNumberToString(*this, numerator);
		denominator = "1";
	}

	Decimal(float input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		BigNumber::bigNumberToString(*this, numerator);
		denominator = "1";
	}

	Decimal(double input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		BigNumber::bigNumberToString(*this, numerator);
		denominator = "1";
	}

	Decimal(string line)
	{
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			BigNumber::bigNumberToString(*this, numerator);
			denominator = "1";
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
	}

	Decimal(const char* input)
	{
		string line = BigNumber::convertToString(input);
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			BigNumber::bigNumberToString(*this, numerator);
			denominator = "1";
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
	}

	Decimal operator=(const char* input)
	{
		string line = BigNumber::convertToString(input);
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			BigNumber::bigNumberToString(*this, numerator);
			denominator = "1";
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
		return *this;
	}

	Decimal operator=(BigNumber obj)
	{
		integer = obj.integer;
		decimal = obj.decimal;
		isNegative = obj.isNegative;
		isDecimal = true;
		BigNumber::bigNumberToString(obj, numerator);
		denominator = "1";
		return *this;
	}

	Decimal operator=(Integer obj)
	{
		integer = obj.integer;
		decimal = obj.decimal;
		isNegative = obj.isNegative;
		isDecimal = true;
		BigNumber::bigNumberToString(obj, numerator);
		denominator = "1";
		return *this;
	}

	Decimal operator=(int input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		BigNumber::bigNumberToString(*this, numerator);
		denominator = "1";
		return *this;
	}

	Decimal operator=(double input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		BigNumber::bigNumberToString(*this, numerator);
		denominator = "1";
		return *this;
	}

	Decimal operator=(float input)
	{
		string inputString = to_string(input);
		BigNumber::stringToBigNumber(inputString, *this);
		BigNumber::bigNumberToString(*this, numerator);
		denominator = "1";
		return *this;
	}

	Decimal operator=(string line)
	{
		spaceAdditionBigNumber(line);
		if (invalidLineBigNumber)
		{
			vector<string> lineBreak = breakSubstringVector(line);
			string lineResult;
			doOperationParanthesisModded(lineBreak, lineResult);
			BigNumber::stringToBigNumber(lineResult, *this);
			BigNumber::bigNumberToString(*this, numerator);
			denominator = "1";
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "because string is invalid, big number is set as 0\n";
			integer = "0";
			decimal = "";
			isDecimal = false;
			isNegative = false;
			SetConsoleTextAttribute(hConsole, 7);
		}
		return *this;
	}
	//operator cin cout
	friend ostream& operator<<(ostream& output, Decimal obj)
	{
		if (obj.isNegative)
		{
			if (obj.integer != "0" || !obj.decimal.empty())
			{
				output << "-";
			}
		}
		Decimal::decimalFillintegerDecimal(obj);
		output << obj.integer;
		output << ".";
		output << obj.decimal;
		for (int rep = obj.decimal.size(); rep < 100; rep++)
		{
			output << "0";
		}
		return output;
	}

	friend istream& operator>>(istream& input, Decimal& obj)
	{
		string inputString;
		input >> inputString;

		BigNumber::stringToBigNumber(inputString, obj);
		obj.isDecimal = true;
		BigNumber::bigNumberToString(obj, obj.numerator);
		obj.denominator = "1";
		return input;
	}
	//operator set

	friend Decimal operator+(Decimal numA, Decimal numB);
	friend Decimal operator-(Decimal numA, Decimal numB);
	friend Decimal operator*(Decimal numA, Decimal numB);
	friend Decimal operator/(Decimal numA, Decimal numB);

	friend Decimal operator+(Decimal numA, BigNumber numB);
	friend Decimal operator-(Decimal numA, BigNumber numB);
	friend Decimal operator*(Decimal numA, BigNumber numB);
	friend Decimal operator/(Decimal numA, BigNumber numB);
	friend Decimal operator+(BigNumber numA, Decimal numB);
	friend Decimal operator-(BigNumber numA, Decimal numB);
	friend Decimal operator*(BigNumber numA, Decimal numB);
	friend Decimal operator/(BigNumber numA, Decimal numB);

	friend Decimal operator+(Decimal numA, Integer numB);
	friend Decimal operator-(Decimal numA, Integer numB);
	friend Decimal operator*(Decimal numA, Integer numB);
	friend Decimal operator/(Decimal numA, Integer numB);
	friend Decimal operator+(Integer numA, Decimal numB);
	friend Decimal operator-(Integer numA, Decimal numB);
	friend Decimal operator*(Integer numA, Decimal numB);
	friend Decimal operator/(Integer numA, Decimal numB);
};


// OPERATOR OVERLOADING for BigNumber & BigNumber
BigNumber operator+(BigNumber numA, BigNumber numB)
{
	BigNumber res;
	res = BigNumber::add(numA, numB);

	return res;
}

BigNumber operator-(BigNumber numA, BigNumber numB)
{
	BigNumber res;
	res = BigNumber::subtract(numA, numB);

	return res;
}

BigNumber operator*(BigNumber numA, BigNumber numB)
{
	BigNumber res;
	res = BigNumber::multiply(numA, numB);

	return res;
}

BigNumber operator/(BigNumber numA, BigNumber numB)
{
	BigNumber res;
	eraseZero_Dec(numB.decimal);
	eraseZero_Int(numB.integer);
	if (numB.isDecimal)
	{
		if (numB.integer == "0" && numB.decimal == "")
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "zero division error, return an 0 instead\n";
			SetConsoleTextAttribute(hConsole, 7);
			res.integer = "0";
			res.decimal = "";
			res.isDecimal = true;
			res.isNegative = false;
			return res;
		}
	}
	else
	{
		if (numB.integer == "0")
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "zero division error, return an 0 instead\n";
			SetConsoleTextAttribute(hConsole, 7);
			res.integer = "0";
			res.decimal = "";
			res.isDecimal = numA.isDecimal;
			res.isNegative = false;
			return res;
		}
	}
	res = BigNumber::divide(numA, numB);

	return res;
}


// OPERATOR OVERLOADING for Integer & Integer
Integer operator+(Integer numA, Integer numB)
{
	Integer res;
	res = BigNumber::add(numA, numB);

	return res;
}

Integer operator-(Integer numA, Integer numB)
{
	Integer res;
	res = BigNumber::subtract(numA, numB);

	return res;
}

Integer operator*(Integer numA, Integer numB)
{
	Integer res;
	res = BigNumber::multiply(numA, numB);

	return res;
}

Integer operator/(Integer numA, Integer numB)
{
	Integer res;
	if (numB.integer == "0")
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "zero division error, return an 0 instead\n";
		SetConsoleTextAttribute(hConsole, 7);
		res.integer = "0";
		res.decimal = "";
		res.isDecimal = numA.isDecimal;
		res.isNegative = false;
		return res;
	}
	res = BigNumber::divide(numA, numB);

	return res;
}

// OPERATOR OVERLOADING for Integer & BigNumber
Integer operator+(Integer numA, BigNumber numB)
{
	Integer res;
	res = BigNumber::add(numA, numB);

	return res;
}

Integer operator-(Integer numA, BigNumber numB)
{
	Integer res;
	res = BigNumber::subtract(numA, numB);

	return res;
}

Integer operator*(Integer numA, BigNumber numB)
{
	Integer res;
	res = BigNumber::multiply(numA, numB);

	return res;
}

Integer operator/(Integer numA, BigNumber numB)
{
	Integer res;
	eraseZero_Dec(numB.decimal);
	eraseZero_Int(numB.integer);
	if (numB.isDecimal)
	{
		if (numB.integer == "0" && numB.decimal == "")
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "zero division error, return an 0 instead\n";
			SetConsoleTextAttribute(hConsole, 7);
			res.integer = "0";
			res.decimal = "";
			res.isDecimal = true;
			res.isNegative = false;
			return res;
		}
	}
	else
	{
		if (numB.integer == "0")
		{
			SetConsoleTextAttribute(hConsole, 12);
			cout << "zero division error, return an 0 instead\n";
			SetConsoleTextAttribute(hConsole, 7);
			res.integer = "0";
			res.decimal = "";
			res.isDecimal = numA.isDecimal;
			res.isNegative = false;
			return res;
		}
	}
	res = BigNumber::divide(numA, numB);

	return res;
}

Integer operator+(BigNumber numA, Integer numB)
{
	Integer res;
	res = BigNumber::add(numA, numB);

	return res;
}

Integer operator-(BigNumber numA, Integer numB)
{
	Integer res;
	res = BigNumber::subtract(numA, numB);

	return res;
}

Integer operator*(BigNumber numA, Integer numB)
{
	Integer res;
	res = BigNumber::multiply(numA, numB);

	return res;
}

Integer operator/(BigNumber numA, Integer numB)
{

	Integer res;
	if (numB.integer == "0")
	{
		SetConsoleTextAttribute(hConsole, 12);
		cout << "zero division error, return an 0 instead\n";
		SetConsoleTextAttribute(hConsole, 7);
		res.integer = "0";
		res.decimal = "";
		res.isDecimal = numA.isDecimal;
		res.isNegative = false;
		return res;
	}
	res = BigNumber::divide(numA, numB);

	return res;
}

// OPERATOR OVERLOADING for Decimal & Decimal
Decimal operator+(Decimal numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	lhsFractionNumerator.isNegative = numA.isNegative;
	rhsFractionNumerator.isNegative = numB.isNegative;

	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	Decimal resultFraction;
	BigNumber result;
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	bool resultIsFraction;
	//fraction addition
	lhsFractionNumerator = BigNumber::multiply(lhsFractionNumerator, rhsFractionDenominator);
	rhsFractionNumerator = BigNumber::multiply(rhsFractionNumerator, lhsFractionDenominator);
	resultFractionNumerator = BigNumber::add(lhsFractionNumerator, rhsFractionNumerator);
	resultFractionDenominator = BigNumber::multiply(lhsFractionDenominator, rhsFractionDenominator);
	if (resultFractionNumerator.isNegative)
	{
		resultFraction.isNegative = true;
		resultFractionNumerator.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = false;
	}
	resultFraction.isDecimal = true;

	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator-(Decimal numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	lhsFractionNumerator.isNegative = numA.isNegative;
	rhsFractionNumerator.isNegative = numB.isNegative;

	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	Decimal resultFraction;
	BigNumber result;
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	bool resultIsFraction;
	//fraction addition
	lhsFractionNumerator = BigNumber::multiply(lhsFractionNumerator, rhsFractionDenominator);
	rhsFractionNumerator = BigNumber::multiply(rhsFractionNumerator, lhsFractionDenominator);
	resultFractionNumerator = BigNumber::subtract(lhsFractionNumerator, rhsFractionNumerator);
	resultFractionDenominator = BigNumber::multiply(lhsFractionDenominator, rhsFractionDenominator);
	if (resultFractionNumerator.isNegative)
	{
		resultFraction.isNegative = true;
		resultFractionNumerator.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = false;
	}
	resultFraction.isDecimal = true;

	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return false;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator*(Decimal numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	//fraction multiplication
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	BigNumber result;
	Decimal resultFraction;
	bool resultIsFraction;
	resultFractionNumerator = BigNumber::multiply(lhsFractionNumerator, rhsFractionNumerator);
	resultFractionDenominator = BigNumber::multiply(lhsFractionDenominator, rhsFractionDenominator);
	if (numA.isNegative == numB.isNegative)
	{
		resultFraction.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = true;
	}
	resultFraction.isDecimal = true;

	//try simplify the resultFraction
	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return false;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator/(Decimal numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	if (numB.integer == "0" && numB.decimal == "")
	{
		Decimal res;
		SetConsoleTextAttribute(hConsole, 12);
		cout << "zero division error, return an 0 instead\n";
		SetConsoleTextAttribute(hConsole, 7);
		res.integer = "0";
		res.decimal = "";
		res.isDecimal = true;
		res.isNegative = false;
		res.numerator = "0";
		res.denominator = "0";
		return res;
	}
	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	//fraction multiplication
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	BigNumber result;
	Decimal resultFraction;
	bool resultIsFraction;

	//division multiplication
	resultFractionNumerator = BigNumber::multiply(lhsFractionNumerator, rhsFractionDenominator);
	resultFractionDenominator = BigNumber::multiply(lhsFractionDenominator, rhsFractionNumerator);
	resultFraction.isDecimal = true;
	if (numA.isNegative == numB.isNegative)
	{
		resultFraction.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = true;
	}

	//try simplify the resultFraction
	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

// OPERATOR OVERLOAD for Decimal & BigNumber
Decimal operator+(Decimal numA, BigNumber numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::add(a, b);

	return res;
}

Decimal operator-(Decimal numA, BigNumber numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::subtract(a, b);

	return res;
}

Decimal operator*(Decimal numA, BigNumber numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::multiply(a, b);

	return res;
}

Decimal operator/(Decimal numA, BigNumber numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::divide(a, b);

	return res;
}

Decimal operator+(BigNumber numA, Decimal numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::add(a, b);

	return res;
}

Decimal operator-(BigNumber numA, Decimal numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::subtract(a, b);

	return res;
}

Decimal operator*(BigNumber numA, Decimal numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::multiply(a, b);

	return res;
}

Decimal operator/(BigNumber numA, Decimal numB)
{
	Decimal res;
	BigNumber a(numA.integer, numA.decimal, numA.isNegative, numA.isDecimal);
	BigNumber b(numB.integer, numB.decimal, numB.isNegative, numB.isDecimal);

	res = BigNumber::divide(a, b);

	return res;
}

// OPERATOR OVERLOAD for Decimal & Integer
Decimal operator+(Decimal numA, Integer numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	BigNumber rhsFractionNumerator;
	lhsFractionNumerator.isNegative = numA.isNegative;

	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	Decimal resultFraction;
	BigNumber result;
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	bool resultIsFraction;
	//fraction addition
	rhsFractionNumerator = BigNumber::multiply(numB, lhsFractionDenominator);
	resultFractionNumerator = BigNumber::add(lhsFractionNumerator, rhsFractionNumerator);
	resultFractionDenominator = lhsFractionDenominator;
	if (resultFractionNumerator.isNegative)
	{
		resultFraction.isNegative = true;
		resultFractionNumerator.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = false;
	}
	resultFraction.isDecimal = true;

	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator-(Decimal numA, Integer numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	BigNumber rhsFractionNumerator;
	lhsFractionNumerator.isNegative = numA.isNegative;

	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	Decimal resultFraction;
	BigNumber result;
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	bool resultIsFraction;
	//fraction addition
	rhsFractionNumerator = BigNumber::multiply(numB, lhsFractionDenominator);
	resultFractionNumerator = BigNumber::subtract(lhsFractionNumerator, rhsFractionNumerator);
	resultFractionDenominator = lhsFractionDenominator;
	if (resultFractionNumerator.isNegative)
	{
		resultFraction.isNegative = true;
		resultFractionNumerator.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = false;
	}
	resultFraction.isDecimal = true;

	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator*(Decimal numA, Integer numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}


	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	//fraction multiplication
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	BigNumber result;
	Decimal resultFraction;
	bool resultIsFraction;
	resultFractionNumerator = BigNumber::multiply(lhsFractionNumerator, numB);
	resultFractionDenominator = lhsFractionDenominator;
	if (numA.isNegative == numB.isNegative)
	{
		resultFraction.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = true;
	}
	resultFraction.isDecimal = true;

	//try simplify the resultFraction
	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator/(Decimal numA, Integer numB)
{
	Decimal errorOutput("0", "", false, true);

	if (numB.integer == "0")
	{
		Decimal res;
		SetConsoleTextAttribute(hConsole, 12);
		cout << "zero division error, return an 0 instead\n";
		SetConsoleTextAttribute(hConsole, 7);
		res.integer = "0";
		res.decimal = "";
		res.isDecimal = true;
		res.isNegative = false;
		res.numerator = "0";
		res.denominator = "0";
		return res;
	}
	BigNumber lhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numA.numerator, lhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber lhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numA.denominator, lhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of lhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	//fraction multiplication
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	BigNumber result;
	Decimal resultFraction;
	bool resultIsFraction;

	//division multiplication
	resultFractionNumerator = lhsFractionNumerator;
	resultFractionDenominator = BigNumber::multiply(lhsFractionDenominator, numB);
	resultFraction.isDecimal = true;
	if (numA.isNegative == numB.isNegative)
	{
		resultFraction.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = true;
	}

	//try simplify the resultFraction
	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator+(Integer numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	rhsFractionNumerator.isNegative = numB.isNegative;

	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	Decimal resultFraction;
	BigNumber result;
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	bool resultIsFraction;
	//fraction addition
	lhsFractionNumerator = BigNumber::multiply(numA, rhsFractionDenominator);
	resultFractionNumerator = BigNumber::add(lhsFractionNumerator, rhsFractionNumerator);
	resultFractionDenominator = rhsFractionDenominator;
	if (resultFractionNumerator.isNegative)
	{
		resultFraction.isNegative = true;
		resultFractionNumerator.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = false;
	}
	resultFraction.isDecimal = true;

	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator-(Integer numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber lhsFractionNumerator;
	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	rhsFractionNumerator.isNegative = numB.isNegative;

	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "additive error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	Decimal resultFraction;
	BigNumber result;
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	bool resultIsFraction;
	//fraction addition
	lhsFractionNumerator = BigNumber::multiply(numA, rhsFractionDenominator);
	resultFractionNumerator = BigNumber::subtract(lhsFractionNumerator, rhsFractionNumerator);
	resultFractionDenominator = rhsFractionDenominator;
	if (resultFractionNumerator.isNegative)
	{
		resultFraction.isNegative = true;
		resultFractionNumerator.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = false;
	}
	resultFraction.isDecimal = true;

	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator*(Integer numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}

	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	//fraction multiplication
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	BigNumber result;
	Decimal resultFraction;
	bool resultIsFraction;
	resultFractionNumerator = BigNumber::multiply(numA, rhsFractionNumerator);
	resultFractionDenominator = rhsFractionDenominator;
	if (numA.isNegative == numB.isNegative)
	{
		resultFraction.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = true;
	}
	resultFraction.isDecimal = true;

	//try simplify the resultFraction
	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}

Decimal operator/(Integer numA, Decimal numB)
{
	Decimal errorOutput("0", "", false, true);

	if (numB.integer == "0" && numB.decimal == "")
	{
		Decimal res;
		SetConsoleTextAttribute(hConsole, 12);
		cout << "zero division error, return an 0 instead\n";
		SetConsoleTextAttribute(hConsole, 7);
		res.integer = "0";
		res.decimal = "";
		res.isDecimal = true;
		res.isNegative = false;
		res.numerator = "0";
		res.denominator = "0";
		return res;
	}
	BigNumber rhsFractionNumerator;
	if (!BigNumber::stringToBigNumber(numB.numerator, rhsFractionNumerator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs numerator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	BigNumber rhsFractionDenominator;
	if (!BigNumber::stringToBigNumber(numB.denominator, rhsFractionDenominator))
	{
		SetConsoleTextAttribute(hConsole, 4);
		cout << "Multiplicative error: error when converting string to big number of rhs denominator decimal\n";
		SetConsoleTextAttribute(hConsole, 7);
		return errorOutput;
	}
	//fraction multiplication
	BigNumber resultFractionNumerator;
	BigNumber resultFractionDenominator;
	BigNumber result;
	Decimal resultFraction;
	bool resultIsFraction;

	//division multiplication
	resultFractionNumerator = BigNumber::multiply(numA, rhsFractionDenominator);
	resultFractionDenominator = rhsFractionNumerator;
	resultFraction.isDecimal = true;
	if (numA.isNegative == numB.isNegative)
	{
		resultFraction.isNegative = false;
	}
	else
	{
		resultFraction.isNegative = true;
	}

	//try simplify the resultFraction
	result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
	result.isDecimal = true;
	result.isNegative = resultFraction.isNegative;
	if (result.decimal.empty())
	{
		resultIsFraction = false;
	}
	else
	{
		resultIsFraction = true;
	}

	if (resultIsFraction)
	{
		//tidy the bigNumber numerator to string
		if (!BigNumber::bigNumberToString(resultFractionNumerator, resultFraction.numerator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction numerator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}

		if (!BigNumber::bigNumberToString(resultFractionDenominator, resultFraction.denominator))
		{
			SetConsoleTextAttribute(hConsole, 4);
			cout << "Additive error while converting result fraction denominator from big number to string\n";
			SetConsoleTextAttribute(hConsole, 7);
			return errorOutput;
		}
		bool sign = resultFraction.isNegative;
		Decimal::decimalFillintegerDecimal(resultFraction);
		resultFraction.isNegative = sign;
		return resultFraction;
	}
	else
	{
		resultFraction = result;
		BigNumber::bigNumberToString(resultFraction, resultFraction.numerator);
		resultFraction.denominator = "1";
		return resultFraction;
	}
}


