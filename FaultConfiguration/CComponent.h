/*
 * CComponent.h
 *
 */

#ifndef CCOMPONENT_H_
#define CCOMPONENT_H_


#include <string>
#include <vector>

class CComponent{

public:
	std::string name;
	std::vector<std::string> signals;
	std::vector<std::string> primIn, primOut;
	std::vector<std::string> secIn, secOut;
	std::vector<std::string> allIn, allOut;

	CComponent();
};



#endif /* CCOMPONENT_H_ */
