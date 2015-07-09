/*
 * CGate.cpp
 *
 */

#include "CGate.h"

CGate::CGate(){
    name = ' ';
    type = ' ';
    primIn.clear();
    primOut.clear();
    preGates.clear();
	postGates.clear();
    nmbrOfIn = nmbrOfOut = level = 0;
    visited = false;
}

CGate::CGate(std::string mName){
	name = mName;
	primIn.clear();
	primOut.clear();
	preGates.clear();
	postGates.clear();
	nmbrOfIn = nmbrOfOut = level = 0;
	visited = false;
}

void CGate::assignName(std::string a){
  name = a;
}

void CGate::addPrimeInput(std::string in){
  primIn.push_back(in);
  nmbrOfIn += 1;
}

void CGate::addPrimeOutput(std::string out){
  primOut.push_back(out);
  nmbrOfOut += 1;
}

