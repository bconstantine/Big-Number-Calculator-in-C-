#pragma once
#include <iostream> 

using namespace std;

int miniAdd(int digit1, int digit2, int& excess);
int miniSubtract(int digit1, int digit2, bool& borrowed);
int miniMultiply(int digit1, int digit2, int& excess);