#include "convertion.h"

// convert to Vector but in reverse function
void convertToVector(string tobeVect, vector<int>& dest)
{
  for(int i = 0; i < tobeVect.length(); i++)
  {
    dest.insert(dest.begin(), (tobeVect[i] - '0'));
  }
}

// erase zero for a integer
void eraseZero_Int(string& number)
{
  while(number.length() > 1 && number[0] == '0')
  {
    number.erase(number.begin());
  }
}

// erase zero for a decimal
void eraseZero_Dec(string& number)
{
  while(number.length() > 0 && number[number.length() - 1] == '0')
  {
    number.erase(number.begin() + (number.length() - 1));
  }
}

