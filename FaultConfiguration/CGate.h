/*
 * CGate.h
 *
 */

#ifndef CGATE_H_
#define CGATE_H_

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <locale>
#include <vector>
#include <sstream>
#include <list>
#include <map>

class CGate{

public:
  std::string name, type;
  std::vector<std::string> primIn, primOut;
  int nmbrOfIn, nmbrOfOut, level;
  std::vector<CGate*> preGates, postGates;
  bool visited;

  CGate();
  CGate(std::string);

  void assignName(std::string);

  void addPrimeInput(std::string);
  void addPrimeOutput(std::string);

};



#endif /* CGATE_H_ */
