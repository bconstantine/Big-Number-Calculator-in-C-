#include "miniOperation.h"

// addition function for each digit
int miniAdd(int digit1, int digit2, int& excess)
{
  int tmp = digit1 + digit2 + excess;

  // computes new value for excess variable
  if(tmp >= 10)
  {
    excess = tmp / 10;
    tmp %= 10;
  }
  else
  {
    excess = 0;
  }

  return tmp;
}

// subtraction function for each digit
int miniSubtract(int digit1, int digit2, bool& borrowed)
{
  int tmp = digit1 - digit2;
  
  if(borrowed)
  {
    tmp--;
  }

  if(tmp < 0)
  {
    tmp += 10;
    borrowed = true;
  }
  else
  {
    borrowed = false;
  }

  return tmp;
}

// multiplication function for each digit
int miniMultiply(int digit1, int digit2, int& excess)
{
  int tmp = (digit1 * digit2) + excess;

  // computes new value for excess variable
  if(tmp >= 10)
  {
    excess = tmp / 10;
    tmp %= 10;
  }
  else
  {
    excess = 0;
  }

  return tmp;
}