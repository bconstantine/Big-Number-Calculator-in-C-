#pragma once
#include <iostream>
#include <string>
#include "BigNumber.h"
#include <map>
#include <sstream>
#include <windows.h>
using namespace std;

int tempFractionIndex = 0; //index for tempFraction name : __tempFraction0 for example

bool isNumber(char letter)
{
    if (letter >= '0' && letter <= '9')
    {
        return true;
    }
    else if(letter == '.')
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

bool isLetter(char letter)
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

bool isArithmeticSymbol(char letter)
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

bool isArithmeticEqualSymbol(char letter)
{
    //not sure, try again later (remainder symbol)
    if (letter == '+' || letter == '-' || letter == '*' || letter == '/' || letter == '%' || letter == '^' || letter == '!' || letter == '(' || letter == ')' || letter == '='||letter == ',')
    {
        return true;
    }
    else
    {
        return false;
    }
}

//Intent: adding space between the phrases
//Pre: a string line which is the line of the input
//Post: no return
void spaceAddition(string& line)
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

//Intent: checking for invalid symbol (space is excluded)
//Pre: a string line which is the line of the input (excluded set integer = or set decimal = or Power)
//Post: return bool whether the input is invalid
bool invalidLine(string& line, map<string, Integer> integerVariables, map<string, Decimal> decimalVariables)
{
    stringstream lineStream(line);
    string token;
    string unknownToken;
    bool makeSureNextIsEqual = false;
    bool matterSolved = false; //solved for makeSureNextIsEqual
    while (lineStream >> token)
    {
        if (makeSureNextIsEqual && !matterSolved)
        {
            if (token == "=")
            {
                matterSolved = true;
                continue;
            }
            else
            {
                SetConsoleTextAttribute(hConsole, 12);
                cout << "Token: \"" << unknownToken << "\" is undefined.\n";
                SetConsoleTextAttribute(hConsole, 7);
                return false;
            }
        }
        if (token == "Set")
        {
            continue;
        }
        else if (token == "Integer")
        {
            continue;
        }
        else if (token == "Decimal")
        {
            continue;
        }
        else if(token == "=")
        {
            continue;
        }
        else if (token == "Power")
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
                if (integerVariables.find(token) == integerVariables.end() && decimalVariables.find(token) == decimalVariables.end())
                {
                    if (!makeSureNextIsEqual)
                    {
                        unknownToken = token;
                        makeSureNextIsEqual = true;
                    }
                    else
                    {
                        SetConsoleTextAttribute(hConsole, 12);
                        cout << "Token: \"" << token << "\" is undefined.\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                }
                else
                {
                    continue;
                }
            }
        }
    }
    return true;
}


//Intent: this function finds parenthesis and return a substring
pair<int, int> paranthesisInside(vector <string>& lineVector)
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

//replace the finished operated substring to the main line
void replaceWithSubstring(vector <string>& lineVector, vector<string>::iterator startPoint, vector<string>::iterator endPoint, string substring)
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

vector<string> breakSubstringVector(string substring)
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

//check for double symbol except +- ( paranthesis has been omitted )
bool doubleSymbolCheck(vector <string>& substringBreakdown)
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

//bool return false if operation not succeed
bool stringToBigNumber(string toBeConverted, BigNumber &result)
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

//bool return false if operation not succeed
bool bigNumberToString(BigNumber toBeConverted, string &result)
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

bool decimalToBigNumber(Decimal &toBeConverted, BigNumber& result)
{
    if (toBeConverted.denominator == "0" || toBeConverted.denominator.empty())
    {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "Error when converting fraction to Big Number : zero division error!\n";
        SetConsoleTextAttribute(hConsole, 7);
        return false;
    }
    
    //no more error, begin converting
    BigNumber numeratorBig;
    if (!stringToBigNumber(toBeConverted.numerator, numeratorBig))
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "Error when converting numerator to bignumber\n";
        SetConsoleTextAttribute(hConsole, 7);
        return false;
    }
    BigNumber denominatorBig;
    if (!stringToBigNumber(toBeConverted.denominator, denominatorBig))
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "Error when converting denominator to bignumber\n";
        SetConsoleTextAttribute(hConsole, 7);
        return false;
    }
    result = BigNumber::divide(numeratorBig, denominatorBig);
    result.isNegative = toBeConverted.isNegative;
    //to be versatile, also change the toBeConverted
    toBeConverted.integer = result.integer;
    toBeConverted.decimal = result.decimal;
    return true;
}

bool decimalFillIntegerDecimal(Decimal &obj)
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
    if (!stringToBigNumber(obj.numerator, numerator) || !stringToBigNumber(obj.denominator, denominator))
    {
        SetConsoleTextAttribute(hConsole, 4);
        cout << "Error when decimal fill integer and decimal\n";
        SetConsoleTextAttribute(hConsole, 7);
        return false;
    }
    BigNumber convertedDecimal = BigNumber::divide(numerator, denominator);
    obj.integer = convertedDecimal.integer;
    obj.decimal = convertedDecimal.decimal;
    return true;
}

string decimalToVectorCodeName(Decimal &toBeInserted, map <string, Decimal> &fractionVariable)
{
    string codename = "__temporaryDecimal";
    codename += to_string(tempFractionIndex);
    tempFractionIndex++;
    fractionVariable[codename] = toBeInserted;
    //cout << "printing inside function: " << fractionVariable[codename];
    return codename;
}

//Not sure, check again later
//catch where two number is beside each other but no operator found
bool doOperation(vector <string>& substringBreakdown, string& returnString, map<string, Integer>& integerVariable, map <string, Decimal>& decimalVariable);

bool doOperationParanthesis(vector <string>& substringBreakdown, string& returnString, map<string, Integer>& integerVariable, map <string, Decimal>& decimalVariable);

//bool is used to return if the operation is invalid
bool findingFactorial(vector <string> &substringBreakdown, map<string, Integer> &integerVariable, map<string, Decimal> &decimalVariable)
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
                //check for variable first
                if (integerVariable.find(substringBreakdown[index - 1]) == integerVariable.end() && decimalVariable.find(substringBreakdown[index - 1]) == decimalVariable.end())
                {
                    //variable not exist, error
                    SetConsoleTextAttribute(hConsole, 12);
                    cout << "Factorial Error! Variable not exist!\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                else
                {
                    if (decimalVariable.find(substringBreakdown[index - 1]) != decimalVariable.end())
                    {
                        Decimal decimalParameter = decimalVariable[substringBreakdown[index - 1]];
                        if (!decimalParameter.decimal.empty())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Error! Factorial of a non zero decimal can't be done\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        else if(decimalParameter.isNegative)
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Error! Factorial of a negative decimal can't be done\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        else
                        {
                            Decimal resultDecimal = BigNumber::factorial(decimalParameter);
                            resultDecimal.isDecimal = true;
                            string resultString;
                            if (!bigNumberToString(resultDecimal, resultString))
                            {
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Factorial error: convertion of big number to string\n";
                                SetConsoleTextAttribute(hConsole, 7);
                                return false;
                            }
                            substringBreakdown[index - 1] = resultString;
                            substringBreakdown.erase(substringBreakdown.begin() + index); 
                            index--;
                        }
                    }
                    else
                    {
                        //variable exist, beginning factorial, but check first whether the variable is minus or not
                        BigNumber parameter = integerVariable[substringBreakdown[index - 1]];
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
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool findingPower(vector <string>& substringBreakdown, map <string, Integer> &integerVariable, map<string, Decimal> &decimalVariable)
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
            Decimal lhsDecimal;
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
                    if (decimalVariable.find(substringBreakdown[index - 1]) == decimalVariable.end())
                    {
                        if (integerVariable.find(substringBreakdown[index - 1]) == integerVariable.end())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Power error: left variable \"" << substringBreakdown[index - 1] << "\" doesn't exist.\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        else
                        {
                            //an integer
                            leftIsFraction = false;
                            lhs = integerVariable[substringBreakdown[index - 1]];
                        }
                    }
                    else
                    {
                        //a decimal
                        leftIsFraction = true;
                        lhsDecimal = decimalVariable[substringBreakdown[index - 1]];
                    }
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
                    if (decimalVariable.find(substringBreakdown[index + 1]) == decimalVariable.end())
                    {
                        if (integerVariable.find(substringBreakdown[index + 1]) == integerVariable.end())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Power error: right variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        else
                        {
                            //an integer
                            rightIsFraction = false;
                            rhs = integerVariable[substringBreakdown[index + 1]];
                        }
                    }
                    else
                    {
                        //a fraction
                        rightIsFraction = true;
                        bool operationSuccess = decimalToBigNumber(decimalVariable[substringBreakdown[index + 1]], rhs);
                        if (!operationSuccess)
                        {
                            SetConsoleTextAttribute(hConsole, 4);
                            cout << "Power error: when converting right hand side fraction into BigNumber, terminating \n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                    }
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
                    bool operationSuccess = doOperationParanthesis(substringSubstring, returnString, integerVariable, decimalVariable);
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
                    bool operationSuccess = doOperation(substringSubstring, returnString, integerVariable, decimalVariable);

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
                    if (decimalVariable.find(substringBreakdown[index + 2]) == decimalVariable.end())
                    {
                        if (integerVariable.find(substringBreakdown[index + 2]) == integerVariable.end())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Power error: left variable \"" << substringBreakdown[index + 2] << "\" doesn't exist.\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        else
                        {
                            //an integer
                            leftIsFraction = false;
                            lhs = integerVariable[substringBreakdown[index + 2]];
                        }
                    }
                    else
                    {
                        //a fraction
                        leftIsFraction = true;
                        lhsDecimal = decimalVariable[substringBreakdown[index + 2]];
                    }
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
                    if (decimalVariable.find(substringBreakdown[index + 4]) == decimalVariable.end())
                    {
                        if (integerVariable.find(substringBreakdown[index + 4]) == integerVariable.end())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Power error: right variable \"" << substringBreakdown[index + 4] << "\" doesn't exist.\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        else
                        {
                            //a decimal
                            rightIsFraction = false;
                            rhs = integerVariable[substringBreakdown[index + 4]];
                        }
                    }
                    else
                    {
                        //a fraction
                        rightIsFraction = true;
                        rhs = decimalVariable[substringBreakdown[index + 4]];
                    }
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
                    if (leftIsFraction)
                    {
                        if (lhsDecimal.isNegative)
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Power error! Left hand side is negative, can't conduct square root\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                    }
                    else
                    {
                        if (lhs.isNegative)
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Power error! Left hand side is negative, can't conduct square root\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                    }
                }

            }
            BigNumber result;
            Decimal resultDecimal;
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
                if (leftIsFraction)
                {
                    resultIsDecimal = true;
                    if (rhsEven.decimal.empty())
                    {
                        resultDecimal.isNegative = false;
                    }
                    else
                    {
                        if (lhsDecimal.isNegative)
                        {
                            resultDecimal.isNegative = true;
                        }
                        else
                        {
                            resultDecimal.isNegative = false;
                        }
                    }
                    resultDecimal.isDecimal = lhsDecimal.isDecimal;
                    BigNumber lhsDecimalNumerator;
                    BigNumber lhsDecimalDenominator;
                    if (!stringToBigNumber(lhsDecimal.numerator, lhsDecimalDenominator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting numerator to Big Number\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    BigNumber resultDecimalNumerator;
                    if (!stringToBigNumber(lhsDecimal.denominator, lhsDecimalNumerator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting numerator to Big Number\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    BigNumber resultDecimalDenominator;
                    resultDecimalNumerator = BigNumber::power(lhsDecimalNumerator, rhs);
                    resultDecimalDenominator = BigNumber::power(lhsDecimalDenominator, rhs);

                    //convert the bignumber to string
                    if (!bigNumberToString(resultDecimalNumerator, resultDecimal.numerator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting numerator big number to numerator string\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }

                    if (!bigNumberToString(resultDecimalDenominator, resultDecimal.denominator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting denominator big number to numerator string\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    bool sign = resultDecimal.isNegative;
                    decimalFillIntegerDecimal(resultDecimal);
                    resultDecimal.isNegative = sign;
                }
                else
                {
                    //left is number
                    if (lhs.isDecimal || rhs.isDecimal)
                    {
                        resultIsDecimal = true;
                        if (rhsEven.decimal.empty())
                        {
                            resultDecimal.isNegative = false;
                        }
                        else
                        {
                            if (lhsDecimal.isNegative)
                            {
                                resultDecimal.isNegative = true;
                            }
                            else
                            {
                                resultDecimal.isNegative = false;
                            }
                        }

                        resultDecimal.isDecimal = true;
                        resultDecimal.numerator = "1";
                        BigNumber resultDecimalDenominator;
                        resultDecimalDenominator = BigNumber::power(lhs, rhs);

                        //convert the bignumber to string
                        if (!bigNumberToString(resultDecimalDenominator, resultDecimal.denominator))
                        {
                            SetConsoleTextAttribute(hConsole, 4);
                            cout << "Power error: converting denominator big number to numerator string\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        bool sign = resultDecimal.isNegative;
                        decimalFillIntegerDecimal(resultDecimal);
                        resultDecimal.isNegative = sign;

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
            }
            else
            {
                if (leftIsFraction)
                {
                    resultIsDecimal = true;
                    BigNumber temporary;
                    temporary.integer = "2";
                    temporary.decimal = "";
                    temporary.isNegative = false;
                    temporary.isDecimal = false;
                    BigNumber rhsEven = BigNumber::divide(rhs, temporary);
                    if (rhsEven.decimal.empty())
                    {
                        resultDecimal.isNegative = false;
                    }
                    else
                    {
                        if (lhsDecimal.isNegative)
                        {
                            resultDecimal.isNegative = true;
                        }
                        else
                        {
                            resultDecimal.isNegative = false;
                        }
                    }
                    resultDecimal.isDecimal = lhsDecimal.isDecimal;
                    BigNumber lhsDecimalNumerator;
                    if (!stringToBigNumber(lhsDecimal.numerator, lhsDecimalNumerator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting numerator to Big Number\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    BigNumber resultDecimalNumerator;
                    resultDecimalNumerator = BigNumber::power(lhsDecimalNumerator, rhs);
                    BigNumber lhsDecimalDenominator;
                    if (!stringToBigNumber(lhsDecimal.denominator, lhsDecimalDenominator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting numerator to Big Number\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    BigNumber resultDecimalDenominator;
                    resultDecimalDenominator = BigNumber::power(lhsDecimalDenominator, rhs);

                    //convert the bignumber to string
                    if (!bigNumberToString(resultDecimalNumerator, resultDecimal.numerator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting numerator big number to numerator string\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }

                    if (!bigNumberToString(resultDecimalDenominator, resultDecimal.denominator))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Power error: converting denominator big number to numerator string\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    bool sign = resultDecimal.isNegative;
                    decimalFillIntegerDecimal(resultDecimal);
                    resultDecimal.isNegative = sign;
                }
                else
                {
                    //left is number
                    if (rhs.isDecimal)
                    {
                        resultIsDecimal = true;
                        resultDecimal = BigNumber::power(lhs, rhs);
                        bigNumberToString(resultDecimal, resultDecimal.numerator);
                        resultDecimal.denominator = "1";
                    }
                    else
                    {
                        resultIsDecimal = false;
                        result = BigNumber::power(lhs, rhs);
                    }
                }
            }
            
            //finish everything, now start inserting back to the vector
            string newBigNum;
            if (resultIsDecimal)
            {
                newBigNum = decimalToVectorCodeName(resultDecimal, decimalVariable);
                decimalVariable[newBigNum] = resultDecimal;
            }
            else
            {
                bool operationSuccess = bigNumberToString(result, newBigNum);
                if (!operationSuccess)
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Power error: Problem when converting bigNumber to string for final insertion\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
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

bool findingMultiplicative (vector <string>& substringBreakdown, map <string, Integer> &variable, map<string, Decimal> &decimalVariable)
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
            Decimal lhsFraction;
            bool rightIsNumber = true;
            bool rightIsFraction = false;
            BigNumber rhs;
            Decimal rhsFraction;

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
                if (decimalVariable.find(substringBreakdown[index - 1]) == decimalVariable.end())
                {
                    if (variable.find(substringBreakdown[index - 1]) == variable.end())
                    {
                        SetConsoleTextAttribute(hConsole, 12);
                        cout << "Multiplicative error: left variable \"" << substringBreakdown[index - 1] << "\" doesn't exist.\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    else
                    {
                        //an integer
                        leftIsFraction = false;
                        lhs = variable[substringBreakdown[index - 1]];
                    }
                }
                else
                {
                    //an decimal
                    leftIsFraction = true;
                    lhsFraction = decimalVariable[substringBreakdown[index - 1]];
                }
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
                if (decimalVariable.find(substringBreakdown[index + 1]) == decimalVariable.end())
                {
                    if (variable.find(substringBreakdown[index + 1]) == variable.end())
                    {
                        SetConsoleTextAttribute(hConsole, 12);
                        cout << "Multiplicative error: right variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    else
                    {
                        //an integer
                        rightIsFraction = false;
                        rhs = variable[substringBreakdown[index + 1]];
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
                }
                else
                {
                    //an decimal
                    rightIsFraction = true;
                    rhsFraction = decimalVariable[substringBreakdown[index + 1]];
                    if (changeSign)
                    {
                        if (rhsFraction.isNegative)
                        {
                            rhsFraction.isNegative = false;
                        }
                        else
                        {
                            rhsFraction.isNegative = true;
                        }
                    }
                }
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
            Decimal resultFraction;
            BigNumber resultFractionNumerator;
            BigNumber resultFractionDenominator;
            bool resultIsFraction = false; //to simplify whether it is possible to turn fraction to usual BigNumber
            //starting to multiply
            //check zero division error
            if (rightIsFraction)
            {
                decimalFillIntegerDecimal(rhsFraction);
                if (rhsFraction.integer == "0" && rhsFraction.decimal.empty())
                {
                    SetConsoleTextAttribute(hConsole, 12);
                    cout << "Multiplicative error: Zero division error\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
            }
            else
            {
                if (rhs.integer == "0" && rhs.decimal.empty() && substringBreakdown[index] == "/")
                {
                    SetConsoleTextAttribute(hConsole, 12);
                    cout << "Multiplicative error: Zero division error\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
            }
            //fraction x fraction
            if (leftIsFraction && rightIsFraction)
            {
                BigNumber lhsFractionNumerator;
                if (!stringToBigNumber(lhsFraction.numerator, lhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of lhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                BigNumber rhsFractionNumerator;
                if (!stringToBigNumber(rhsFraction.numerator, rhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of rhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                BigNumber lhsFractionDenominator;
                if (!stringToBigNumber(lhsFraction.denominator, lhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of lhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                BigNumber rhsFractionDenominator;
                if (!stringToBigNumber(rhsFraction.denominator, rhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of rhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                if (substringBreakdown[index] == "*")
                {
                    //fraction multiplication
                    
                    resultFractionNumerator = BigNumber::multiply(lhsFractionNumerator, rhsFractionNumerator);
                    resultFractionDenominator = BigNumber::multiply(lhsFractionDenominator, rhsFractionDenominator);
                    if (lhsFraction.isNegative == rhsFraction.isNegative)
                    {
                        resultFraction.isNegative = false;
                    }
                    else
                    {
                        resultFraction.isNegative = true;
                    }
                    resultFraction.isDecimal = true;
                }
                else
                {
                    //division multiplication
                    resultFractionNumerator = BigNumber::multiply(lhsFractionNumerator, rhsFractionDenominator);
                    resultFractionDenominator = BigNumber::multiply(lhsFractionDenominator, rhsFractionNumerator);
                    resultFraction.isDecimal = true;
                    if (lhsFraction.isNegative == rhsFraction.isNegative)
                    {
                        resultFraction.isNegative = false;
                    }
                    else
                    {
                        resultFraction.isNegative = true;
                    }
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
            }
            //big number x fraction
            else if (!leftIsFraction && rightIsFraction)
            {
                BigNumber rhsFractionNumerator;
                if (!stringToBigNumber(rhsFraction.numerator, rhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of rhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                BigNumber rhsFractionDenominator;
                if (!stringToBigNumber(rhsFraction.denominator, rhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of rhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                if (substringBreakdown[index] == "*")
                {
                    //bignumber x fraction
                    resultFractionNumerator = BigNumber::multiply(lhs, rhsFractionNumerator);
                    resultFractionDenominator = rhsFractionDenominator;
                    resultFraction.isDecimal = true;
                    if (lhs.isNegative == rhsFraction.isNegative)
                    {
                        resultFraction.isNegative = false;
                    }
                    else
                    {
                        resultFraction.isNegative = true;
                    }
                }
                else
                {
                    //bignumber/fraction
                    resultFractionNumerator = BigNumber::multiply(lhs, rhsFractionDenominator);
                    resultFractionDenominator = rhsFractionNumerator;
                    resultFraction.isDecimal = true;
                    if (lhs.isNegative == rhsFraction.isNegative)
                    {
                        resultFraction.isNegative = false;
                    }
                    else
                    {
                        resultFraction.isNegative = true;
                    }
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
            }
            //fraction x big number
            else if (leftIsFraction && !rightIsFraction)
            {
                BigNumber lhsFractionNumerator;
                if (!stringToBigNumber(lhsFraction.numerator, lhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of lhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }


                BigNumber lhsFractionDenominator;
                if (!stringToBigNumber(lhsFraction.denominator, lhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error: error when converting string to big number of lhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                if (substringBreakdown[index] == "*")
                {
                    //fraction x big number
                    resultFractionNumerator = BigNumber::multiply(lhsFractionNumerator, rhs);
                    resultFractionDenominator = lhsFractionDenominator;
                    resultFraction.isDecimal = true;
                    if (lhsFraction.isNegative == rhs.isNegative)
                    {
                        resultFraction.isNegative = false;
                    }
                    else
                    {
                        resultFraction.isNegative = true;
                    }
                }
                else
                {
                    //fraction / bigNumber
                    resultFractionNumerator = lhsFractionNumerator;
                    resultFractionDenominator = BigNumber::multiply(lhsFractionNumerator, rhs);
                    resultFraction.isDecimal = true;
                    if (lhsFraction.isNegative == rhs.isNegative)
                    {
                        resultFraction.isNegative = false;
                    }
                    else
                    {
                        resultFraction.isNegative = true;
                    }
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
            }
            //bignumber x bignumber
            else
            {
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
                    resultFractionNumerator = lhs;                 
                    resultFractionDenominator = rhs;
                    resultFraction.isDecimal = true;
                    result = BigNumber::divide(resultFractionNumerator, resultFractionDenominator);
                    if (result.decimal.empty())
                    {
                        resultIsFraction = false;
                    }
                    else
                    {
                        resultIsFraction = true;
                    }
                    if (lhs.isNegative == rhs.isNegative)
                    {
                        resultFraction.isNegative = false;
                    }
                    else
                    {
                        resultFraction.isNegative = true;
                    }
                }
                
            }

            //operation is finished
            //beginning replacing the String
            if (resultIsFraction)
            {
                //tidy the bigNumber numerator to string
                if (!bigNumberToString(resultFractionNumerator, resultFraction.numerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error while converting result fraction numerator from big number to string\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                if (!bigNumberToString(resultFractionDenominator, resultFraction.denominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative error while converting result fraction denominator from big number to string\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                bool sign = resultFraction.isNegative;
                if (!decimalFillIntegerDecimal(resultFraction))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Multiplicative Error: when filling the integer and decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                resultFraction.isNegative = sign;
                string codename = decimalToVectorCodeName(resultFraction, decimalVariable);
                replaceWithSubstring(substringBreakdown, substringBreakdown.begin() + index - 1, substringBreakdown.begin() + index + 2, codename);
                index--;
            }
            else
            {
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
    }
    return true;
}

bool findingAdditive(vector <string>& substringBreakdown, map <string, Integer> &integerVariable, map<string, Decimal>& decimalVariable)
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
                    if (decimalVariable.find(substringBreakdown[index + 1]) == decimalVariable.end())
                    {
                        if (integerVariable.find(substringBreakdown[index + 1]) == integerVariable.end())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "additive error: left variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        else
                        {
                            //an integer
                            if (substringBreakdown[index] == "-")
                            {
                                string returnString;
                                if (!bigNumberToString(integerVariable[substringBreakdown[index + 1]], returnString))
                                {
                                    SetConsoleTextAttribute(hConsole, 4);
                                    cout << "additive error: fail converting variable \"" << substringBreakdown[index + 1] << "\" to string.\n";
                                    SetConsoleTextAttribute(hConsole, 7);
                                    return false;
                                }

                                if (returnString[0] == '-')
                                {
                                    returnString.erase(returnString.begin());
                                }
                                else
                                {
                                    returnString.insert(0, "-");
                                }

                                substringBreakdown[index + 1] = returnString;
                                /*if (integerVariable[substringBreakdown[index + 1]].isNegative)
                                {
                                    integerVariable[substringBreakdown[index + 1]].isNegative = false;
                                }
                                else
                                {
                                    integerVariable[substringBreakdown[index + 1]].isNegative = true;
                                }*/
                            }
                            
                        }
                    }
                    else
                    {
                        //a decimal
                        if (substringBreakdown[index] == "-")
                        {
                            Decimal newObject = decimalVariable[substringBreakdown[index + 1]];
                            if (newObject.isNegative)
                            {
                                newObject.isNegative = false;
                            }
                            else
                            {
                                newObject.isNegative = true;
                            }
                            if (newObject.numerator[0] == '-')
                            {
                                newObject.numerator.erase(newObject.numerator.begin());
                            }
                            else
                            {
                                newObject.numerator.insert(newObject.numerator.begin(), '-');
                            }
                            string newCodeName = decimalToVectorCodeName(newObject, decimalVariable);
                            substringBreakdown[index + 1] = newCodeName;
                        }
                    }
                }
                else
                {
                    //lhs is number
                    if (substringBreakdown[index] == "-")
                    {
                        if (substringBreakdown[index + 1][0] == '-')
                        {
                            substringBreakdown[index + 1].erase(substringBreakdown[index+1].begin());
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
            Decimal lhsFraction;
            bool rightIsNumber = true;
            bool rightIsFraction = false;
            BigNumber rhs;
            Decimal rhsFraction;

            //this one
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
                if (decimalVariable.find(substringBreakdown[index - 1]) == decimalVariable.end())
                {
                    if (integerVariable.find(substringBreakdown[index - 1]) == integerVariable.end())
                    {
                        SetConsoleTextAttribute(hConsole, 12);
                        cout << "additive error: left variable \"" << substringBreakdown[index - 1] << "\" doesn't exist.\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    else
                    {
                        //an integer
                        leftIsFraction = false;
                        lhs = integerVariable[substringBreakdown[index - 1]];
                    }
                }
                else
                {
                    //an decimal
                    leftIsFraction = true;
                    lhsFraction = decimalVariable[substringBreakdown[index - 1]];
                }
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
                if (decimalVariable.find(substringBreakdown[index + 1]) == decimalVariable.end())
                {
                    if (integerVariable.find(substringBreakdown[index + 1]) == integerVariable.end())
                    {
                        SetConsoleTextAttribute(hConsole, 12);
                        cout << "Additive error: right variable \"" << substringBreakdown[index + 1] << "\" doesn't exist.\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    else
                    {
                        //an integer
                        rightIsFraction = false;
                        rhs = integerVariable[substringBreakdown[index + 1]];
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
                }
                else
                {
                    //a decimal
                    rightIsFraction = true;
                    rhsFraction = decimalVariable[substringBreakdown[index + 1]];
                    if (changeSign)
                    {
                        if (rhsFraction.isNegative)
                        {
                            rhsFraction.isNegative = false;
                        }
                        else
                        {
                            rhsFraction.isNegative = true;
                        }
                    }
                }
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
            Decimal resultFraction;
            BigNumber resultFractionNumerator;
            BigNumber resultFractionDenominator;
            bool resultIsFraction = false; //to simplify whether it is possible to turn fraction to usual BigNumber
            //starting to adding
            //fraction +- fraction
            //do the sign just for the numerator
            if (leftIsFraction && rightIsFraction)
            {
                BigNumber lhsFractionNumerator;
                if (!stringToBigNumber(lhsFraction.numerator, lhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "additive error: error when converting string to big number of lhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                BigNumber rhsFractionNumerator;
                if (!stringToBigNumber(rhsFraction.numerator, rhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "additive error: error when converting string to big number of rhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                lhsFractionNumerator.isNegative = lhsFraction.isNegative;
                rhsFractionNumerator.isNegative = rhsFraction.isNegative;

                BigNumber lhsFractionDenominator;
                if (!stringToBigNumber(lhsFraction.denominator, lhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "additive error: error when converting string to big number of lhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                BigNumber rhsFractionDenominator;
                if (!stringToBigNumber(rhsFraction.denominator, rhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "additive error: error when converting string to big number of rhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                if (substringBreakdown[index] == "+")
                {
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
                }
                else
                {
                    //fraction subtraction
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
            }
            //big number x fraction
            else if (!leftIsFraction && rightIsFraction)
            {
                BigNumber lhsFractionNumerator;
                BigNumber rhsFractionNumerator;
                if (!stringToBigNumber(rhsFraction.numerator, rhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Additive error: error when converting string to big number of rhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                rhsFractionNumerator.isNegative = rhsFraction.isNegative;
                BigNumber lhsFractionDenominator;
                BigNumber rhsFractionDenominator;
                if (!stringToBigNumber(rhsFraction.denominator, rhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Additive error: error when converting string to big number of rhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                if (substringBreakdown[index] == "+")
                {
                    //bignumber + fraction
                    lhsFractionNumerator = BigNumber::multiply(lhs, rhsFractionDenominator);
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
                }
                else
                {
                    //bignumber-fraction
                    lhsFractionNumerator = BigNumber::multiply(lhs, rhsFractionDenominator);
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
                    if (lhs.isNegative == rhsFraction.isNegative)
                    {
                        resultFraction.isNegative = true;
                    }
                    else
                    {
                        resultFraction.isNegative = false;
                    }
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
            }
            //fraction x big number
            else if (leftIsFraction && !rightIsFraction)
            {
                BigNumber lhsFractionNumerator;
                if (!stringToBigNumber(lhsFraction.numerator, lhsFractionNumerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "additive error: error when converting string to big number of lhs numerator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                BigNumber rhsFractionNumerator;

                BigNumber lhsFractionDenominator;
                if (!stringToBigNumber(lhsFraction.denominator, lhsFractionDenominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "additive error: error when converting string to big number of lhs denominator decimal\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                BigNumber rhsFractionDenominator;
                lhsFractionNumerator.isNegative = lhsFraction.isNegative;
                if (substringBreakdown[index] == "+")
                {
                    //fraction + big number
                    rhsFractionNumerator = BigNumber::multiply(lhsFractionDenominator, rhs);
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
                }
                else
                {
                    //fraction - bigNumber
                    rhsFractionNumerator = BigNumber::multiply(lhsFractionDenominator, rhs);
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
            }
            //bignumber x bignumber
            else
            {
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
                resultIsFraction = false;
            }

            //operation is finished
            //beginning replacing the String
            if (resultIsFraction)
            {
                //tidy the bigNumber numerator to string
                if (!bigNumberToString(resultFractionNumerator, resultFraction.numerator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Additive error while converting result fraction numerator from big number to string\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                if (!bigNumberToString(resultFractionDenominator, resultFraction.denominator))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Additive error while converting result fraction denominator from big number to string\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                string codename = decimalToVectorCodeName(resultFraction, decimalVariable);
                replaceWithSubstring(substringBreakdown, substringBreakdown.begin() + index - 1, substringBreakdown.begin() + index + 2, codename);
                index--;
            }
            else
            {
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
    }
    return true;
}

//check for equal sign
int checkForEqualSign(vector <string> substringBreakdown)
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


bool checkForNoArithmeticSign(vector <string>& substringBreakdown)
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

//do operation
bool doOperation(vector <string>& substringBreakdown, string& returnString, map<string, Integer>& integerVariable, map <string, Decimal>& decimalVariable)
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
        if (!findingFactorial(substringBreakdown, integerVariable, decimalVariable))
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Error when doing factorial operation\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        else
        {
            /*for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
            {
                cout << *iterator << " ";
            }
            cout << endl;*/
        }
        //check for power
        if (!findingPower(substringBreakdown, integerVariable, decimalVariable))
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Error when doing power operation\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        else
        {
            /*for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
            {
                cout << *iterator << " ";
            }
            cout << endl;*/
        }
        //check for multiplicative
        if (!findingMultiplicative(substringBreakdown, integerVariable, decimalVariable))
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Error when doing multiplicative operation\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        else
        {
            /*for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
            {
                cout << *iterator << " ";
            }
            cout << endl;*/
        }
        //check for additive
        if (!findingAdditive(substringBreakdown, integerVariable, decimalVariable))
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Error when doing additive operation\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        else
        {
           /* for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
            {
                cout << *iterator << " ";
            }
            cout << endl;*/
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

bool doOperationParanthesis(vector <string>& substringBreakdown, string& returnString, map<string, Integer>& integerVariable, map <string, Decimal>& decimalVariable)
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
        vector<string> smallerSubstring(substringBreakdown.begin()+paranthesisPair.first + 1, substringBreakdown.begin() + paranthesisPair.second - 1);
        string returnSmallerSubstring;
        if (!doOperation(smallerSubstring, returnSmallerSubstring, integerVariable, decimalVariable))
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Something wrong when doing the operation inside the paranthesis\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }

        replaceWithSubstring(substringBreakdown, substringBreakdown.begin()+paranthesisPair.first, substringBreakdown.begin()+paranthesisPair.second, returnSmallerSubstring);
        paranthesisPair = paranthesisInside(substringBreakdown);



        /*for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
        {
            cout << *iterator << " ";
        }
        cout << endl;*/
    }

    /*for (auto iterator = substringBreakdown.begin(); iterator != substringBreakdown.end(); iterator++)
    {
        cout << *iterator << " ";
    }
    cout << endl;*/
    //no more paranthesis, directly do one last final operation
    if (!doOperation(substringBreakdown, returnString, integerVariable, decimalVariable))
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


bool checkForNoNumberNoOperation(vector <string> substringBreakdown, vector<string>::iterator start, vector<string>::iterator endplus)
{
    start++;
    if (endplus - start != 1)
    {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "Two words found between two equal sign, immediately invalid\n";
        SetConsoleTextAttribute(hConsole, 7);
        return false;
    }
    while (start != endplus)
    {
        if (isArithmeticSymbol((*start)[0]))
        {
            //found arithmetic symbol
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Error, found an arithmetic symbol " << (*start)[0] << "between two equal sign, means not an lvalue\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        
        if (isNumber((*start)[0]))
        {
            //found arithmetic symbol
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Error, found a number on the first letter of a substring between two equal sign, means " << *start <<"not an lvalue\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        start++;
    }
    return true;
}

void printingInputPhase()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 6);
    cout << "=========================== INPUT PHASE ===========================" << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

void printingBlankPhase(int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    if (color == 10) cout << endl;
    cout << "===================================================================" << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

void printingOutputPhase()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "========================== OUTPUT PHASE ===========================" << endl;
    SetConsoleTextAttribute(hConsole, 7);
}

//setting
bool lineParsing(vector <string>& substringBreakdown, string& returnString, map<string, Integer>& integerVariable, map <string, Decimal>& decimalVariable, bool noPrint)
{
    bool subsegments = noPrint;
    if (substringBreakdown.size() == 0)
    {
        SetConsoleTextAttribute(hConsole, 12);
        cout << "line empty\n";
        SetConsoleTextAttribute(hConsole, 7);
        return false;
    }
    bool setCommand = false;
    if (substringBreakdown[0] == "Set")
    {
        setCommand = true;
    }
    
    if (setCommand)
    {
        if (substringBreakdown.size() < 5)
        {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "syntax error! Set command is too short\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        if (substringBreakdown[1] != "Integer" && substringBreakdown[1] != "Decimal")
        {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Syntax error! Unrecognized datatype: \"" << substringBreakdown[1] << "\" \n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        
        //it is recognized with either decimal or integer
        bool setDecimal = false;
        if (substringBreakdown[1] == "Decimal")
        {
            setDecimal = true;
        }
        
        string keyWord = substringBreakdown[2];
        //the first letter of the keyword must be a letter
        if (!isLetter(keyWord[0]))
        {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Error! First letter of the keyword for setting integer/decimal must be a letter or an underscore";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }

        //check for equal sign
        if (substringBreakdown[3] != "=")
        {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Syntax error! No equal sign on set sentence\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }
        int firstEqualIndex = 3;
        vector<string> rightHandSide(substringBreakdown.begin() + 4, substringBreakdown.end());
        //found the equal sign, make sure to check another equal sign
        if (checkForEqualSign(rightHandSide) != rightHandSide.size())
        {
            //an equal sign exist
            if (checkForNoNumberNoOperation(rightHandSide, rightHandSide.begin() + firstEqualIndex, rightHandSide.begin()+checkForEqualSign(rightHandSide)))
            {
                //still legal
                string smallSubstring;
                vector<string> smallerRightHandSide(rightHandSide.begin()+checkForEqualSign(rightHandSide), rightHandSide.end());
                if (!lineParsing(smallerRightHandSide, smallSubstring, integerVariable, decimalVariable, false))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Error while parsing the line\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }

                //success, try to replace the string
                replaceWithSubstring(rightHandSide, rightHandSide.begin()+checkForEqualSign(rightHandSide), rightHandSide.end(), smallSubstring);
            }
            else
            {
                //operation not legal since left of equal is not lvalue
                SetConsoleTextAttribute(hConsole, 12);
                cout << "Operation not legal because the left of equal sign is not lvalue\n";
                SetConsoleTextAttribute(hConsole, 7);
                return false;
            }

        }
        //start do operation
        string rightHandSideReturn;
        if (!doOperationParanthesis(rightHandSide, rightHandSideReturn, integerVariable, decimalVariable))
        {
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Operation paranthesis error when parsing the right hand side of equal sign\n";
            SetConsoleTextAttribute(hConsole, 7);
            return false;
        }

        bool rightHandSideIsNumber = true;
        bool rightHandSideIsFraction = false;

        //check if it is number or variable or fraction
        for (auto iterator = rightHandSideReturn.begin(); iterator != rightHandSideReturn.end(); iterator++)
        {
            if (!isNumber(*iterator))
            {
                rightHandSideIsNumber = false;
                break;
            }
        }

        if (!rightHandSideIsNumber)
        {
            if (decimalVariable.find(rightHandSideReturn) == decimalVariable.end())
            {
                if (integerVariable.find(rightHandSideReturn) == integerVariable.end())
                {
                    SetConsoleTextAttribute(hConsole, 12);
                    cout << "Error: left variable \"" << rightHandSideReturn << "\" doesn't exist.\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                else
                {
                    //an integer
                    rightHandSideIsFraction = false;
                }
            }
            else
            {
                //an decimal
                rightHandSideIsFraction = true;
            }
        }
        else
        {
            //rhs is number
        }

        if (setDecimal)
        {
            //set decimal
            Decimal toBePlaced;
            if (rightHandSideIsNumber)
            {
                //is number
                BigNumber rightHandSideBigNum;
                if (!stringToBigNumber(rightHandSideReturn, rightHandSideBigNum))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Error when converting right hand side string to big number\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                rightHandSideBigNum.isDecimal = true;
                toBePlaced = rightHandSideBigNum;
                bigNumberToString(toBePlaced, toBePlaced.numerator);
                toBePlaced.denominator = "1";
                //remember reach here
            }
            else
            {
                if (rightHandSideIsFraction)
                {
                    //existing fraction
                    toBePlaced = decimalVariable[rightHandSideReturn];
                }
                else
                {
                    //existing integer
                    toBePlaced = integerVariable[rightHandSideReturn];
                }
            }
            if (integerVariable.find(keyWord) != integerVariable.end())
            {
                integerVariable.erase(keyWord);
            }
            decimalVariable[keyWord] = toBePlaced;
        }
        else
        {
            //set integer
            Integer toBePlaced;
            if (rightHandSideIsNumber)
            {
                //is number
                BigNumber rightHandSideBigNum;
                if (!stringToBigNumber(rightHandSideReturn, rightHandSideBigNum))
                {
                    SetConsoleTextAttribute(hConsole, 4);
                    cout << "Error when converting right hand side string to big number\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                toBePlaced = rightHandSideBigNum;
            }
            else
            {
                if (rightHandSideIsFraction)
                {
                    //existing fraction
                    toBePlaced = decimalVariable[rightHandSideReturn];
                }
                else
                {
                    //existing integer
                    toBePlaced = integerVariable[rightHandSideReturn];
                }
            }
            if (decimalVariable.find(keyWord) != decimalVariable.end())
            {
                decimalVariable.erase(keyWord);
            }
            integerVariable[keyWord] = toBePlaced;
        }
        return true;
    }
    else
    {
        //word must be recognized, try checking for existing =
        bool printingSentence = true;
        if (checkForEqualSign(substringBreakdown) != substringBreakdown.size())
        {
            printingSentence = false;
        }

        if (!printingSentence)
        {
            //make sure that the left hand side is lvalue
            if (substringBreakdown[1] != "=")
            {
                SetConsoleTextAttribute(hConsole, 12);
                cout << "Unrecognized pattern, equal sign should have been the second word\n";
                SetConsoleTextAttribute(hConsole, 7);
                return false;
            }
            else
            {
                //make sure that the first word is an existing lvalue
                if (integerVariable.find(substringBreakdown[0]) == integerVariable.end() && decimalVariable.find(substringBreakdown[0]) == decimalVariable.end())
                {
                    SetConsoleTextAttribute(hConsole, 12);
                    cout << "Error! Lvalue \"" << substringBreakdown[0] << "\" not exist or invalid format\n";
                    SetConsoleTextAttribute(hConsole, 7);
                    return false;
                }
                else
                {
                    bool setDecimal = false;
                    if (decimalVariable.find(substringBreakdown[0]) != decimalVariable.end())
                    {
                        setDecimal = true;
                    }
                    string keyWord = substringBreakdown[0];
                    int firstEqualIndex = 2;
                    vector <string> rightHandSide(substringBreakdown.begin() + 2, substringBreakdown.end());
                    //found the equal sign, make sure to check another equal sign
                    if (checkForEqualSign(rightHandSide) != rightHandSide.size())
                    {
                        //an equal sign exist
                        if (checkForNoNumberNoOperation(rightHandSide, rightHandSide.begin() + firstEqualIndex, rightHandSide.begin()+checkForEqualSign(rightHandSide)))
                        {
                            //still legal
                            string smallSubstring;
                            vector<string> smallerRightHandSide(rightHandSide.begin()+checkForEqualSign(rightHandSide), rightHandSide.end());
                            if (!lineParsing(smallerRightHandSide, smallSubstring, integerVariable, decimalVariable, false))
                            {
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Error while parsing the line\n";
                                SetConsoleTextAttribute(hConsole, 7);
                                return false;
                            }

                            //success, try to replace the string
                            replaceWithSubstring(rightHandSide, rightHandSide.begin()+checkForEqualSign(rightHandSide), rightHandSide.end(), smallSubstring);
                        }
                        else
                        {
                            //operation not legal since left of equal is not lvalue
                            SetConsoleTextAttribute(hConsole, 4);
                            cout << "Operation not legal because the left of equal sign is not lvalue\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }

                    }
                    //start do operation
                    string rightHandSideReturn;
                    if (!doOperationParanthesis(rightHandSide, rightHandSideReturn, integerVariable, decimalVariable))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "Operation paranthesis error when parsing the right hand side of equal sign\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }

                    bool rightHandSideIsNumber = true;
                    bool rightHandSideIsFraction = false;

                    //check if it is number or variable or fraction
                    for (auto iterator = rightHandSideReturn.begin(); iterator != rightHandSideReturn.end(); iterator++)
                    {
                        if (!isNumber(*iterator))
                        {
                            rightHandSideIsNumber = false;
                            break;
                        }
                    }

                    if (!rightHandSideIsNumber)
                    {
                        if (decimalVariable.find(rightHandSideReturn) == decimalVariable.end())
                        {
                            if (integerVariable.find(rightHandSideReturn) == integerVariable.end())
                            {
                                SetConsoleTextAttribute(hConsole, 12);
                                cout << "Error: left variable \"" << rightHandSideReturn << "\" doesn't exist.\n";
                                SetConsoleTextAttribute(hConsole, 7);
                                return false;
                            }
                            else
                            {
                                //an integer
                                rightHandSideIsFraction = false;
                            }
                        }
                        else
                        {
                            //an decimal
                            rightHandSideIsFraction = true;
                        }
                    }
                    else
                    {
                        //lhs is number
                    }
                    if (setDecimal)
                    {
                        //set decimal
                        Decimal toBePlaced;
                        if (rightHandSideIsNumber)
                        {
                            //is number
                            BigNumber rightHandSideBigNum;
                            if (!stringToBigNumber(rightHandSideReturn, rightHandSideBigNum))
                            {
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Error when converting right hand side string to big number\n";
                                SetConsoleTextAttribute(hConsole, 7);
                                return false;
                            }
                            toBePlaced = rightHandSideBigNum;
                            toBePlaced.denominator = "1";
                            bigNumberToString(toBePlaced, toBePlaced.numerator);
                        }
                        else
                        {
                            if (rightHandSideIsFraction)
                            {
                                //existing fraction
                                toBePlaced = decimalVariable[rightHandSideReturn];
                            }
                            else
                            {
                                //existing integer
                                toBePlaced = integerVariable[rightHandSideReturn];
                                toBePlaced.denominator = "1";
                                bigNumberToString(toBePlaced, toBePlaced.numerator);
                            }
                        }
                        if (integerVariable.find(keyWord) != integerVariable.end())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "Conflicting keyword name find between integer and decimal, terminating\n";
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        decimalVariable[keyWord] = toBePlaced;
                    }
                    else
                    {
                        //set integer
                        Integer toBePlaced;
                        if (rightHandSideIsNumber)
                        {
                            //is number
                            BigNumber rightHandSideBigNum;
                            if (!stringToBigNumber(rightHandSideReturn, rightHandSideBigNum))
                            {
                                SetConsoleTextAttribute(hConsole, 4);
                                cout << "Error when converting right hand side string to big number\n";
                                SetConsoleTextAttribute(hConsole, 7);
                                return false;
                            }
                            toBePlaced = rightHandSideBigNum;
                        }
                        else
                        {
                            if (rightHandSideIsFraction)
                            {
                                //existing fraction
                                toBePlaced = decimalVariable[rightHandSideReturn];
                            }
                            else
                            {
                                //existing integer
                                toBePlaced = integerVariable[rightHandSideReturn];
                            }
                        }
                        if (integerVariable.find(keyWord) == integerVariable.end())
                        {
                            SetConsoleTextAttribute(hConsole, 12);
                            cout << "No such variable name exist: " << keyWord << endl;
                            SetConsoleTextAttribute(hConsole, 7);
                            return false;
                        }
                        integerVariable[keyWord] = toBePlaced;
                    }
                    return true;
                }
            }
        }
        else
        {
        //printing sequence
            string substringBreakdownReturn;
            if (!doOperationParanthesis(substringBreakdown, substringBreakdownReturn, integerVariable, decimalVariable))
            {
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Operation paranthesis error when parsing the string before printing\n";
                SetConsoleTextAttribute(hConsole, 7);
                return false;
            }
            bool substringIsNumber = true;
            bool substringIsFraction = false;

            //check if it is number or variable or fraction
            for (auto iterator = substringBreakdownReturn.begin(); iterator != substringBreakdownReturn.end(); iterator++)
            {
                if (!isNumber(*iterator))
                {
                    substringIsNumber = false;
                    break;
                }
            }

            if (!substringIsNumber)
            {
                if (decimalVariable.find(substringBreakdownReturn) == decimalVariable.end())
                {
                    if (integerVariable.find(substringBreakdownReturn) == integerVariable.end())
                    {
                        SetConsoleTextAttribute(hConsole, 12);
                        cout << "Error: left variable \"" << substringBreakdownReturn << "\" doesn't exist.\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    else
                    {
                        //an integer
                        if (!subsegments)
                        {
                            printingBlankPhase(6);
                            printingOutputPhase();
                            cout << integerVariable[substringBreakdownReturn] << endl;
                            printingBlankPhase(10);
                            printingInputPhase();
                        }
                        BigNumber temporary = integerVariable[substringBreakdownReturn];
                        bigNumberToString(temporary, returnString);
                    }
                }
                else
                {
                    //an decimal
                    if (!subsegments)
                    {
                        printingBlankPhase(6);
                        printingOutputPhase();
                        cout << decimalVariable[substringBreakdownReturn] << endl;
                        printingBlankPhase(10);
                        printingInputPhase();

                    }
                    BigNumber temporary;
                    decimalToBigNumber(decimalVariable[substringBreakdownReturn], temporary);
                    bigNumberToString(temporary, returnString);
                }
            }
            else
            {
                //lhs is number
                if (!subsegments)
                {
                    BigNumber printingBigNumber;
                    if (!stringToBigNumber(substringBreakdownReturn, printingBigNumber))
                    {
                        SetConsoleTextAttribute(hConsole, 4);
                        cout << "error when converting the final string for printing\n";
                        SetConsoleTextAttribute(hConsole, 7);
                        return false;
                    }
                    printingBlankPhase(6);
                    printingOutputPhase();
                    cout << printingBigNumber << endl;
                    printingBlankPhase(10);
                    printingInputPhase();
                }
                returnString = substringBreakdownReturn;
            }
        }
    }
    return true;
}