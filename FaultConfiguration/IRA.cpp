//============================================================================
// Name        : IRA.cpp
// Author      : Daniel
// Version     :
// Copyright   : Your copyright notice
//============================================================================

#include <windows.h>
#include <string>
#include "CGate.h"
#include "CComponent.h"

std::string ExePath();
void  FindFile(const std::wstring &directory, std::wstring &name ,std::vector<std::wstring> &foundFiles);



std::wstring HDD = L"F:\\Altera";
std::string PROJECT_PATH = ExePath();
std::string TEST_SCRIPT = "test_scriptLoop.tcl ";
std::ofstream TEST_FILE("test_file_quartus.bat");

//add global variables (strings) for things like clk, lcOut_, lc_, mod_, ...

//declaration of standard gates
std::string mnand2_entity = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"\n"
							"entity MNAND2 is port(\n"
							"	a, b: in std_logic;\n	"
							"c: out std_logic);\n"
							"end MNAND2;\n"
							"\n"
							"architecture behaviour of MNAND2 is\n"
							"begin\n"
							"	c <= not(a and b);\n"
							"end behaviour;\n";

std::string mnand2_comp = "component MNAND2\n\t"
							"port (a, b: in std_logic; c: out std_logic);\n\t"
							"end component;\n";

std::string mand2_entity = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"\n"
							"entity MAND2 is port(\n"
							"a, b: in std_logic;\n"
							"c: out std_logic);\n"
							"end MAND2;\n"
							"\n"
							"architecture behaviour of MAND2 is\n"
							"begin\n"
							"	c <= a and b;\n"
							"end behaviour;\n";

std::string mand2_comp = "component MAND2\n\t"
						"port (a, b: in std_logic; c: out std_logic);\n\t"
						"end component;\n";

std::string mor2_entity = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"\n"
							"entity MOR2 is port(\n"
							"	a, b: in std_logic;\n"
							"	c: out std_logic);\n"
							"end MOR2;\n"
							"\n"
							"architecture behaviour of MOR2 is\n"
							"begin\n"
							"	c <= a or b;\n"
							"end behaviour;\n";

std::string mor2_comp = "component MOR2\n\t"
							"port (a, b: in std_logic; c: out std_logic);\n\t"
							"end component;\n";

std::string mnor2_entity = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"\n"
							"entity MNOR2 is port(\n"
							"	a, b: in std_logic;\n"
							"	c: out std_logic);\n"
							"end MNOR2;\n"
							"\n"
							"architecture behaviour of MNOR2 is\n\n"
							"begin\n"
							"	c <= not(a or b);\n"
							"end behaviour;\n";

std::string mnor2_comp = "component MNOR2\n\t"
						"port (a, b: in std_logic; c: out std_logic);\n\t"
						"end component;\n";

std::string mxor2_entity = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"\n"
							"entity MXOR2 is port(\n"
							"	a, b: in std_logic;\n"
							"	c: out std_logic);\n"
							"end MXOR2;\n"
							"\n"
							"architecture behaviour of MXOR2 is\n"
							"begin\n"
							"	c <= a xor b;\n"
							"end behaviour;\n";

std::string mxor2_comp = "component MXOR2\n\t"
						"port (a, b: in std_logic; c: out std_logic);\n\t"
						"end component;\n";

std::string minv_entity = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"\n"
							"entity MINV is port(\n"
							"	a: in std_logic;\n"
							"	b: out std_logic);\n"
							"end MINV;\n"
							"\n"
							"architecture behaviour of MINV is\n"
							"begin\n"
							"	b <= not a;\n"
							"end behaviour;\n";

std::string minv_comp = "component MINV\n\t"
						"port (a: in std_logic; b: out std_logic);\n\t"
						"end component;\n";

std::string mdff_entity = "library ieee;\n"
					"use ieee.std_logic_1164.all;\n"
					"use ieee.std_logic_unsigned.all;\n"
					"\n"
					"entity MDFF is port(\n"
					"	d, clk: in std_logic;\n"
					"	q: out std_logic);\n"
					"end MDFF;\n"
					"\n"
					"architecture behaviour of MDFF is\n"
					"begin\n"
					"	process (clk)\n"
					"	begin\n"
					"		if (clk'event and clk = '1') then\n"
					"		q <= d;\n"
					"		end if;\n"
					"	end process;\n"
					"end behaviour;\n";

std::string mdff_comp = "component MDFF\n\t"
						"port (d, clk: in std_logic; q: out std_logic);\n\t"
						"end component;\n";

std::string lcell_entity = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"\n"
							"entity MLCELL is port(\n"
							"\tInput_A: in std_logic;\n"
							"\tInput_B: in std_logic;\n"
							"\tOutput_Z: out std_logic);\n"
							"end MLCELL;\n";

std::string lcell_comp = "component LCELL\n\t"
						"port (a_in : in std_logic; a_out : out std_logic);\n\t"
						"end component;\n";

std::string includeString = "library ieee;\n"
							"use ieee.std_logic_1164.all;\n"
							"use ieee.std_logic_unsigned.all;\n"
							"LIBRARY altera_mf;\n"
							"USE altera_mf.altera_mf_components.all;\n\n";

std::vector<CGate*> orderedList;
std::map<int, CGate*> cone, process;

//extract the name of the gate (i.e. the word after "entity")
// text: the input string(the whole line), size: size of the string before the word I want to extract
// pos: the position where I found the word
std::string getWordAfter(std::string text, int size , int pos){
	std::string gateName;
	std::locale loc;
	//position in text, when the word is over
	int endpos = pos+size;

	if(text.empty()) return " ";
	//go to the position of the word after text
	while(std::isspace(text[endpos+1], loc)) {
		//gateName.append(text, endpos+1, 1);
		endpos++;
	}

	//append all letters of the word after entity to gateName
	while((!std::isspace(text[endpos+1],loc)) && ((unsigned)endpos < text.length() - 1)){
		gateName.append(text, endpos+1, 1);
		endpos++;
	}
	return gateName;
}


//ensures, that there is a whitespace character before and after the word in the row
//if the word is at first position it checks only for after
bool ensureSpace(std::string row, std::string word){
	std::locale loc;
	//word at first place in line
	if((row.find(word) == 0) && (std::isspace(row[row.find(word)+word.size()], loc)))
		return true;
	//space before and after word
	else if ((std::isspace(row[row.find(word)-1], loc)) &&
			(std::isspace(row[row.find(word)+word.size()], loc)))
		return true;
	else if ((ispunct(row[row.find(word)-1])) &&
			(std::isspace(row[row.find(word)+word.size()], loc)))
		return true;

	else return false;
}


//find a word in the inFile and returns the whole line as std::string
std::string findLine (std::ifstream& inFile, std::string word){
	std::string line;
	int linecnt = 0;
//	ensure that you are at the beginning of the file!
	inFile.seekg(0, std::ios::beg);

	while(!inFile.eof()){
			std::getline(inFile, line);
			//set actual read line to lower case letters, to ensure case insensitivity
			std::transform(line.begin(), line.end(), line.begin(), tolower);
			linecnt++;

			if(line.find(word)!= std::string::npos){

				if(ensureSpace(line, word)){

					return line;
				}

				}
			}
	return line = " ";
}

// Returns the Word directly after the searched word
std::string getName(std::ifstream& inFile, std::string word){
	std::string comp;

	comp = getWordAfter(findLine(inFile, word), word.size(),
			findLine(inFile, word).find(word));

	return comp;
}



//split the content of the line by ",", before a ":" occurs
//returns a CGate with the corresponding primIn's and primOut's
void splitIt(std::string lineA, CComponent*& gateB, int prop){

	std::string tmp, tmp2;
	//erase all the whitespace characters in the string
	lineA.erase(std::remove_if(lineA.begin(), lineA.end(), isspace), lineA.end());
	std::istringstream ss(lineA);
	//when there is a ":" in line
	if((lineA.find(":") != 0) && (lineA.find(":") != std::string::npos)){
	while(std::getline(ss, tmp, ':').good()){
		std::istringstream ss2(tmp);
		while(std::getline(ss2, tmp2, ',')){
			switch(prop){
			//0 means input
			case 0:
				//check here for the ff inputs which are not needed!
				if(tmp2 == "clk") break;
				if(tmp2 == "set") break;
				if(tmp2 == "reset") break;
				gateB->primIn.push_back(tmp2);
				break;
			//1 means output
			case 1:
//				if(tmp2 == "clk" || "set" || "reset") break;
				gateB->primOut.push_back(tmp2);
				break;
			//2 means signal
			case 2:
				//remove the first word, that is "signal"
				if(tmp2.find("signal") != std::string::npos){
					tmp2.erase(0, 6);
				}
				gateB->signals.push_back(tmp2);
				break;
			default: break;
			}
		}
	}
	}
}


//finds the "entity"-description of the component, goes to the line with the in/out-puts
//and returns a CGate with the corresponding primIn's and primOut's
void primInOut(std::ifstream& inFile, std::string entity, CComponent*& gateA){
	std::string line;

	if(getName(inFile, entity) == gateA->name){
		while(!inFile.eof()){
			std::getline(inFile, line);
			//set actual read line to lower case letters, to ensure case insensitivity
			std::transform(line.begin(), line.end(), line.begin(), tolower);
			if(line.find("in") != std::string::npos){
				if(ensureSpace(line, "in")){
					//delimit the line by "," and
					//write every In-port to the primIn with CGate.addPrimInput(string)
					splitIt(line, gateA, 0);
				}
			} else if(line.find("out") != std::string::npos){
				if(ensureSpace(line, "out")){
					//delimit the line by "," and
					//write every Out-port to the primOut CGate.addPrimOutput(string)
					splitIt(line, gateA, 1);
				}
			}
//			completness check: check, whether an "in" or "out" was detected before here
//			(additional)
			else if(line.find("end " + gateA->name) != std::string::npos){
//				//you are at the end of the entity description!
//				//Check, whether primIn and primOut are empty
				if(gateA->primIn.empty() || gateA->primOut.empty()) std::cout<<"ERROR!\n";
				break;
			}
		}
		//assign signals
		while(!inFile.eof()){
			std::getline(inFile, line);
			std::transform(line.begin(), line.end(), line.begin(), tolower);
			if(line.find("signal") != std::string::npos){
				splitIt(line, gateA, 2);
			}
		}
	}
}


std::vector<CGate> assignNmbrInOut(std::vector<CGate> myVector, std::string line
		, std::ifstream& inFile, int cnt){
	std::string newLine;
	std::cout<<"line of component "<<myVector[cnt].name<<": ";
	std::cout<<line<<std::endl;

	return myVector;
}

//for returning the nmbr of in/outputs of a component(perhaps not needed)
//id = 1 means inputs, id = 2 means outputs
int cntPin(std::string line, int id){
	int nmbr = 0;
	std::string tmp;
	line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
	//for input
	if(id == 1){
		//pass everything between "(" and ":" to a tmp-string
		for(int i = line.find("(") + 1; (unsigned)i<line.find(":"); i++ ) tmp += line[i];
		nmbr = std::count(tmp.begin(), tmp.end(), ',');
	}
	//for output
	if(id == 2){
		for(int i = line.find(";") + 1; (unsigned)i<line.find(":"); i++ ) tmp += line[i];
		nmbr = std::count(tmp.begin(), tmp.end(), ',');
	}
	std::cout<<"tmp: "<<tmp<<std::endl;
	std::cout<<"nmbr: "<<nmbr<<std::endl;
	return nmbr+=1;
}


//generate a vector with all components, that will be used
std::vector<CGate> findComponents(std::ifstream& inFile, std::string word, std::vector<CGate> compVector){
	std::string line;
//	ensure that you are at the beginning of the file!
	inFile.seekg(0, std::ios::beg);

	while(!inFile.eof()){
			std::getline(inFile, line);
			//set actual read line to lower case letters, to ensure case insensitivity
			std::transform(line.begin(), line.end(), line.begin(), tolower);


			if(line.find(word) != std::string::npos){
				if(ensureSpace(line, word)){
					std::string tmp = getWordAfter(line, word.size(), line.find(word));
					//go to the line with "port"
					while(!(line.find("port") != std::string::npos)){
						std::getline(inFile, line);
						if(line.find("end component") != std::string::npos){
							std::cout<<"ERROR: no port!!!"<<std::endl;
							return compVector;
						}
					}
					//this is the line, where you have the port!
					compVector.push_back(CGate(tmp));
				}
			}
	}
	return compVector;
}

CGate* getGate(std::string line){
	CGate* ptrTmpGate = new CGate();
	std::string name, tmp, type;
	std::locale loc;
	if((line.find("port map") != std::string::npos) && ensureSpace(line, "port map")){
		name = "";
		tmp = "";
		type = "";
		//extract gatename and assign it to tmpGate
		//the gatename is before the ":"
		for(int i = 0; (unsigned)i < line.find(":"); i++){
			if(!std::isspace(line[i], loc))  name += line[i];
		}
		ptrTmpGate->name = name;

		//assign type
		for(int i = line.find(":") + 1; (unsigned)i < line.find("port map"); i++){
			if(!std::isspace(line[i], loc))  type += line[i];
		}
		ptrTmpGate->type = type;

		//extract input and outputs between "(" and ")" (last element is output, rest input)
		for(int i = line.find("(") + 1; (unsigned)i <= line.find(")"); i++){
			if(!std::isspace(line[i], loc)){
				if((line[i] != ',') && (line[i] != ')')) tmp += line[i];
				//line[i] == ( ',' OR ')' ) !!!!
				else {
					if(line[i] == ','){
						ptrTmpGate->addPrimeInput(tmp);
						tmp = "";
					}

					if(line[i] == ')'){
						ptrTmpGate->addPrimeOutput(tmp);
						tmp = "";
					}
				}
			}
		}
	}
	return ptrTmpGate;
}

std::vector<CGate*> extractGates(std::ifstream& input , std::string key){
	std::vector<CGate*> gates;
	std::string tmp;
	bool firstLine = true;
	input.seekg(0, std::ios::beg);

	while(!input.eof()){
		std::getline(input, tmp);
		//set actual read line to lower case letters, to ensure case insensitivity
		std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
		if((tmp.find(key) != std::string::npos) && ensureSpace(tmp, key)){
			//go to the "begin" statement
			while(!(tmp.find("begin") != std::string::npos)){
				std::getline(input, tmp);
				std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
				//go to the next line, which is not empty
			}
			std::getline(input, tmp);
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
			//go to the first line of the list
			while(tmp.empty()) std::getline(input, tmp);
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
			while(!(tmp.find("end structure") != std::string::npos)){
				if(!tmp.empty()){
					if(!firstLine) tmp[0] = '\n';
					firstLine = false;
					//here extract gate
					gates.push_back(getGate(tmp));
				}
				std::getline(input, tmp);
				std::transform(tmp.begin(), tmp.end(), tmp.begin(), tolower);
			}
		}
	}
	return gates;
}

//returns the gate with the name, specified by "key"
CGate* findByName(std::vector<CGate*> gateVector, std::string key){
	CGate* foundGate = new CGate();
	for(int i = 0; (unsigned)i < gateVector.size(); i++){
		if(gateVector[i]->name == key) return gateVector[i];
	}
	return foundGate;
}

//returns a vector of the gates, which have the signal "inKey" as input
std::vector<CGate*> findByInput(std::vector<CGate*> gateVector, std::string inKey){
	std::vector<CGate*> foundGates;
	for(int i = 0; (unsigned)i < gateVector.size(); i++){
		for(int j = 0; (unsigned)j < gateVector[i]->primIn.size(); j++){
			if(gateVector[i]->primIn[j] == inKey) foundGates.push_back(gateVector[i]);
		}
	}
	return foundGates;
}

//returns the gate, which has the signal "outKey" as output
CGate* findByOutput(std::vector<CGate*> gateVector, std::string outKey){
	CGate* foundGate = new CGate();
	for(int i = 0; (unsigned)i < gateVector.size(); i++){
		if(!gateVector[i]->primOut.empty()){
			if(gateVector[i]->primOut[0] == outKey) return gateVector[i];
		}
		}
	return foundGate;
}


//scan the gates for FF and add the secondary inputs and outputs
void assignSecInOut(CComponent*& component, std::vector<CGate*> gates){
	for (int i = 0; (unsigned)i < gates.size(); i++){
		if(gates[i]->type == "mdff"){
			component->secIn.push_back(gates[i]->primOut[0]);
			component->secOut.push_back(gates[i]->primIn[0]);
		}
	}
	//or: component.allIn = component.primIn;
	component->allIn.insert(component->allIn.end(), component->primIn.begin(), component->primIn.end());
	component->allIn.insert(component->allIn.end(), component->secIn.begin(), component->secIn.end());
	//or: component.allOut = component.primOut;
	component->allOut.insert(component->allOut.end(), component->primOut.begin(), component->primOut.end());
	component->allOut.insert(component->allOut.end(), component->secOut.begin(), component->secOut.end());

	//return component;
}

//erase all gates in the gatelist with a certain type
void eraseByType(std::vector<CGate*>& gateList, std::string type){
	std::vector<CGate*> newList;
	for(std::vector<CGate*>::iterator it = gateList.begin(); it != gateList.end(); it++){
		if((*it)->type != type){
			newList.push_back(*it);
		}
	}
	gateList = newList;
}

void addInOutGates(std::vector<CGate*>& gateVector, CComponent* component){
	//for the primIns
	for(int i = 0; (unsigned)i < component->allIn.size(); i++){
		CGate* tmpGate = new CGate();
		tmpGate->addPrimeOutput(component->allIn[i]);
		tmpGate->name = "PI_" + component->allIn[i];
		tmpGate->type = "input";
		gateVector.push_back(tmpGate);
	}

	//for the primOuts
	for(int i = 0; (unsigned)i < component->allOut.size(); i++){
		CGate* tmpGate = new CGate();
		tmpGate->addPrimeInput(component->allOut[i]);
		tmpGate->name = "PO_" + component->allOut[i];
		tmpGate->type = "output";
		gateVector.push_back(tmpGate);
	}
}

void eraseByName(std::vector<CGate*>& gateList, std::string name){
	std::vector<CGate*> tmp;
	for(int i = 0; (unsigned)i < gateList.size(); i++){
		if(gateList[i]->name != name) tmp.push_back(gateList[i]);
	}
	gateList = tmp;
}

void assignPrePostGates(std::vector<CGate*>& gateList){
	for(int i = 0; (unsigned)i < gateList.size(); i++){
		//assign preGates
		if(!gateList[i]->primIn.empty()){
			for(int k = 0; (unsigned)k < gateList[i]->primIn.size(); k++){
				gateList[i]->preGates.push_back(findByOutput(gateList, gateList[i]->primIn[k]));
			}
		}


		//assign postGates
		if(!gateList[i]->primOut.empty()){
			//do it like this, because one can assume each gate has at most one output!
			std::vector<CGate*> tmpVector = findByInput(gateList, gateList[i]->primOut[0]);
			gateList[i]->postGates = tmpVector;
		}

	}
}

bool allVisited(std::vector<CGate*> gateVector){
	for(int i = 0; (unsigned)i < gateVector.size(); i++){
		if(!gateVector[i]->visited) return false;
	}
	return true;
}

void visit(CGate* gate){
	gate->visited = true;
	//check if all the postGates of the gate were already visited or if it has no postGates
	//then add it to orderedVec
	if( (gate->postGates.empty()) || (allVisited(gate->postGates)) ){
		orderedList.push_back(gate);
	} else {
		for(std::vector<CGate*>::iterator it = gate->postGates.begin();
					it != gate->postGates.end(); it++){
			if(!(*it)->visited) visit(*it);
		}
		orderedList.push_back(gate);
	}
}

void assignLevel(){
	int cnt = 1;
	if(!orderedList.empty()){
		for(std::vector<CGate*>::reverse_iterator rit = orderedList.rbegin();
						rit != orderedList.rend(); ++rit){
			(*rit)->level = cnt;
			cnt++;
		}
	}
	orderedList.clear();
}


void levelize(std::vector<CGate*>& gateVector){
	orderedList.clear();
	while(!allVisited(gateVector)){
		for(std::vector<CGate*>::iterator it = gateVector.begin();
				it != gateVector.end(); it++){
			//ensure that you take an unvisited gate
			if(!(*it)->visited){
				visit(*it);
			}
		}
	}
	assignLevel();
}


CGate* findHighestLevel(std::vector<CGate*> gateVector){
	int highestLevel = 0;
	CGate* highGate;
	for(std::vector<CGate*>::iterator it = gateVector.begin();
				it != gateVector.end(); it++){
		if((*it)->level >= highestLevel){
			highestLevel = (*it)->level;
			highGate = *it;
		}
	}
	return highGate;
}

void addPredecessors(CGate* gate){
	for(std::vector<CGate*>::reverse_iterator rit = gate->preGates.rbegin();
			rit != gate->preGates.rend(); rit++){
		process.insert(std::pair<int, CGate*>((*rit)->level, *rit));
	}
}

void checkProcess(){
	bool erase = false;
		std::map<int, CGate*>::iterator it = --process.end();
		for(std::vector<CGate*>::iterator it2 = (*it).second->postGates.begin();
				it2 != (*it).second->postGates.end(); it2++){
			//check if the postGate is not in the cone
			if(cone.count((*it2)->level) == 0) erase = true;
		}
		if(!erase){
			cone.insert(*it);
			addPredecessors((*it).second);
		}
		process.erase(it);
}


void coneize(std::vector<CGate*> gateList, CGate* gate){
	cone.insert(std::pair<int, CGate*>(gate->level, gate));
	addPredecessors(gate);
	while(!process.empty()){
	checkProcess();
	}
}

//return the number of inputs to the cone
int getInputs(){
	//to ensure, that you see multiple connections to one gate out of the cone as one connection
	std::map<int, CGate*> testMap;
	int in = 0;
	for(std::map<int, CGate*>::iterator it = cone.begin();
			it != cone.end(); it++){
		//check if it is a PrimInputGate
		if(!(*it).second->preGates.size()){
			in++;
		}
		else {
			for(std::vector<CGate*>::iterator it2 = (*it).second->preGates.begin();
					it2 != (*it).second->preGates.end(); it2++){
				//if you don't have the preGate in the cone, increase the input
				if(cone.count((*it2)->level) == 0){
					//insert the preGate in the testMap
					testMap.insert(std::pair<int, CGate*>((*it2)->level, *it2));
				}
			}
		}

	}
	in += testMap.size();
	return in;
}

void removePre(CGate* gate){
	for(std::vector<CGate*>::iterator it = gate->preGates.begin();
			it != gate->preGates.end(); it++){
		if(cone.count((*it)->level)) cone.erase(cone.find((*it)->level));
	}
}

void removeElement(){

	for(std::map<int, CGate*>::iterator it = cone.begin();
			it != cone.end(); it++){
		if((*it).second->preGates.size() ){
			removePre((*it).second);
			cone.erase(it);
			break;
		}
	}
}

void makeConeFeasible(){
	int k = 4;
	int in = getInputs();

	while(k < in){
		//remove an element and check again
		removeElement();
		in = getInputs();
	}
}

void eraseFromVector(std::vector<CGate*>& gateList){
	for(std::map<int, CGate*>::iterator it = cone.begin();
			it != cone.end(); it++){
		eraseByName(gateList, (*it).second->name);
	}
}

void addLcell(std::vector<CGate*>& gateList, CComponent*& main, int k){
	CGate* lcell = new CGate();
	std::stringstream kStream;
	kStream << k;
	std::string tmpName = "lc_";
	std::string tmpOut = "lcOut_";
	tmpName += kStream.str();
	tmpOut += kStream.str();
	lcell->name = tmpName;
	lcell->type = "lcell";
	lcell->addPrimeOutput(tmpOut);

	std::map<int, CGate*>::reverse_iterator it = cone.rbegin();
	//is output gate
	if((*it).second->primOut.empty()){
		lcell->addPrimeInput((*it).second->primIn[0]);
	} else { //is a "normal" gate with in and outputs
		lcell->addPrimeInput((*it).second->primOut[0]);
	}
	gateList.push_back(lcell);
	main->signals.push_back(lcell->primOut[0]);
}

void makeMFFC(std::vector<CGate*>& gateList, CComponent*& main){
	int cnt = 0;
	std::vector<CGate*> tmpList = gateList;
	
	std::vector<std::wstring> foundFiles;
	std::string quartusPath;
	    std::wstring name =  L"quartus_cdb.exe";
	FindFile(HDD, name, foundFiles);
	
	if(foundFiles.size() < 1) {
		std::cout << "quartus_cdb.exe could not be found in C:" << std::endl;
	}
	else if (foundFiles.size() == 1){
		std::string s( foundFiles[0].begin(), foundFiles[0].end() );
		quartusPath = s;
		std::cout << "quartus_cdb.exe found in " << quartusPath << std::endl;
	}
	
	else {
		std::cout << "choose quartus path" << std::endl;
		for (int i = 0; i < foundFiles.size(); i++){
			std::wcout << i << "  " <<  foundFiles[i] << std::endl;
		}
		int a;
		std::cin >> a; 
		std::string s( foundFiles[a].begin(), foundFiles[a].end() );
		quartusPath = s;
	}
	
	TEST_FILE << "\"" +  quartusPath + "\" -t " + TEST_SCRIPT;
	
	std::vector<std::string> lutValues;
	
	while(!tmpList.empty()){
		CGate* highestGate = findHighestLevel(tmpList);

		cone.clear();
		process.clear();
		coneize(tmpList, highestGate);
		//check here for the k-feasibility of the cone!
		makeConeFeasible();
		//ensure that you don't introduce a LCELL for a cone, containing only a output gate
		std::map<int, CGate*>::reverse_iterator it = cone.rbegin();
		if(!((cone.size() == 1) && ((*it).second->type == "output"))){
//			// TODO Here you have all the gates for the lcell in your cone! compute all SA Faults and add them to a std::vector<std::string>

			// Just for testsequence...
			if (cnt == 0){
				lutValues.push_back("00FF");
				lutValues.push_back("FFFF");
			}
			if (cnt == 3){
				lutValues.push_back("FF00");
				lutValues.push_back("0000");
			}
			if (cnt == 4){
				lutValues.push_back("FFFF");
				lutValues.push_back("0000");
			}

			addLcell(gateList, main, cnt);
			//write testsequences to .bat file
			//just for testsequence...
			if((cnt == 0) || (cnt == 3) || (cnt == 4)){
				std::string lcellName = "modCirc_lc_";
				for(std::vector<std::string>::iterator it = lutValues.begin();
						it != lutValues.end(); it++){
					TEST_FILE << lcellName;
					TEST_FILE << cnt;
					TEST_FILE << ' ' + *it + ' ';
				}
				lutValues.clear();
			}
		}
		else cnt--;
		cnt++;
		//erase the elements of the cone from gateList
		eraseFromVector(tmpList);
	}
	TEST_FILE <<"\n pause\n";
	TEST_FILE.close();
}

bool checkListForType(std::vector<CGate*> gateList, std::string key){
	for(int i = 0; (unsigned)i < gateList.size(); i++){
		if(gateList[i]->type == key) return true;
	}
	return false;
}

std::string getInSs(CGate* gate){
	std::stringstream ss;
	for(int i = 0; (unsigned)i < gate->primIn.size(); i++){
		ss << gate->primIn[i];
		ss << ", ";
	}
	return ss.str();
}

std::vector<CGate*> findByType(std::vector<CGate*> wholeList, std::string key){
	std::vector<CGate*> vec;
	for(int i = 0; (unsigned)i < wholeList.size(); i++){
		if(wholeList[i]->type == key) vec.push_back(wholeList[i]);
	}
	return vec;
}

bool isSignal(CComponent* main, std::string key){
	for(int i = 0; (unsigned)i < main->signals.size(); i++){
		//checks also for the primOut of the circuit
		if((main->signals[i] == key) || main->primOut[0] == key) return true;
	}
	return false;
}

void addTmpLcell(std::vector<CGate*>& gateList, CGate* lcell, CGate*& ff, CComponent*& main, int cnt){
	CGate* tmpLcell = new CGate();
	std::stringstream kStream;
	kStream << cnt;
	std::string tmpName = "tmpLc_";
	std::string tmpOut = "tmpLcOut_";
	tmpName += kStream.str();
	tmpOut += kStream.str();
	tmpLcell->name = tmpName;
	tmpLcell->type = "lcell";

	tmpLcell->addPrimeInput(lcell->primOut[0]);
	tmpLcell->addPrimeOutput(tmpOut);

	gateList.push_back(tmpLcell);
	main->signals.push_back(tmpOut);

	ff->primIn[0] = tmpLcell->primOut[0];
}

void adjustCircuit(std::vector<CGate*>& gateList, CComponent*& main){
	std::vector<CGate*> lcellVec = findByType(gateList, "lcell");
	int cnt = 0;

	//adjusting the input signals of the gates to the outsignals of the LCELLs
	for(int k = 0; (unsigned)k < lcellVec.size(); k++){
		for(int i = 0; (unsigned)i < gateList.size(); i++){
			for(int j = 0; (unsigned)j < gateList[i]->primIn.size(); j++){
				if(gateList[i]->primIn[j] == lcellVec[k]->primIn[0])
					if(gateList[i]->type != "lcell") gateList[i]->primIn[j] = lcellVec[k]->primOut[0];
			}
		}

		//adding a gate(an Lcell) in front of a FF if the output of the Lcell, going to the FF is not unique
		std::vector<CGate*> inVec = findByInput(gateList, lcellVec[k]->primOut[0]);
		std::vector<CGate*> tmpVec;
		for(int h = 0; (unsigned)h < inVec.size(); h++){
			if(inVec[h]->type != "output") tmpVec.push_back(inVec[h]);
		}
		if(tmpVec.size() > 1){
			//check whether you have to do it
			for(int l = 0; (unsigned)l < tmpVec.size(); l++){
				if(tmpVec[l]->type == "mdff"){
					addTmpLcell(gateList, lcellVec[k], tmpVec[l], main, cnt);
					cnt++;
				}
			}
		}
	}

	//adjust the signals of the modified circuit!
	for(std::vector<CGate*>::iterator it = gateList.begin();
			it != gateList.end(); it++){
		//iterate over inputs
		for(int i = 0; (unsigned)i < (*it)->primIn.size(); i++){
			if((isSignal(main, (*it)->primIn[i]) &&
					!(((*it)->primIn[i].find("lcOut_") != std::string::npos) || (*it)->primIn[i].find("tmpLcOut_") != std::string::npos))){
				(*it)->primIn[i] = "mod_" + (*it)->primIn[i];
			}else if((!main->secIn.empty()) && (((*it)->primIn[i]) == "scan_data_out")){
				(*it)->primIn[i] = "mod_" + (*it)->primIn[i];
			}
		}
		//iterate over outputs
		for(int l = 0; (unsigned)l < (*it)->primOut.size(); l++){
			if((isSignal(main, (*it)->primOut[l])) &&
					!(((*it)->primOut[l].find("lcOut_") != std::string::npos) || ((*it)->primOut[l].find("tmpLcOut_") != std::string::npos))){
				(*it)->primOut[l] = "mod_" + (*it)->primOut[l];
			}else if((!main->secIn.empty()) && (((*it)->primOut[l]) == "scan_data_out")){
				(*it)->primOut[l] = "mod_" + (*it)->primOut[l];
			}
		}
	}
}

void addFF(std::vector<CGate*>& gateList, CComponent* component){
	//ensure that nothing went wrong before...
	if(component->secIn.size() == component->secOut.size()){
		for(int i = 0; (unsigned)i < component->secIn.size(); i++){
			CGate* ff = new CGate();
			std::stringstream iStream;
			iStream << i;
			ff->name = "ff_" + iStream.str();
			ff->type = "mdff";
			ff->primIn.push_back(component->secOut[i]);
			ff->primIn.push_back("clk");
			ff->primOut.push_back(component->secIn[i]);
			gateList.push_back(ff);
		}
	}
}

void addNewSignals(CComponent*& component){
	int range = component->signals.size();
	for(int i = 0; i < range; i++){
		if(!(component->signals[i].find("lcOut_") != std::string::npos)){
			component->signals.push_back("mod_" + component->signals[i]);
		}
	}
}

void writeCircuit(std::vector<CGate*> gateList, CComponent* main, std::ofstream& output){
	std::string circuitName = "testModLcell";
	//new name of the primOut of the circuit
	std::string newOut = "circ_out";
	bool hasFF = !main->secIn.empty();

	output << includeString;
	output << "Entity " + circuitName + " is port(\n\t";

	//add a clk signal, if you have a FF
	if(hasFF) output << "clk, ";
	//write the primInputs
	for(int i = 0; (unsigned)i < main->primIn.size(); i++){
		output << main->primIn[i];
		if((unsigned)i != main->primIn.size() - 1) output << ", ";
		else output << ": in std_logic;\n\t";
	}

	//write the primOut
	output << newOut;
	if(hasFF) output << ", circ_scan_data_out";
	output << ": out std_logic);\n";

	output << "end " + circuitName + ";\n\n";

	output << "architecture structure of " + circuitName + " is\n\t";
	//write all the component descriptions
	if(checkListForType(gateList, "minv")) output << minv_comp + "\n\t";
	if(checkListForType(gateList, "mnand2")) output << mnand2_comp + "\n\t";
	if(checkListForType(gateList, "mand2")) output << mand2_comp + "\n\t";
	if(checkListForType(gateList, "mnor2")) output << mnor2_comp + "\n\t";
	if(checkListForType(gateList, "mor2")) output << mor2_comp + "\n\t";
	if(checkListForType(gateList, "lcell")) output << lcell_comp + "\n\t";
	if(hasFF) output << mdff_comp + "\n\t";
	//you'll need a XOR anyway, to compare the two circuits
	output << mxor2_comp + "\n";

	//Add the signals once more with mod_s2, but not for lcOut_ ...
	addNewSignals(main);

	if(hasFF){
		main->signals.push_back("scan_data_out");
		main->signals.push_back("mod_scan_data_out");
	}

	//add the FF to the circuit again
	addFF(gateList, main);

	//stringstream for the original circuit
	std::stringstream circSS;
	circSS << "begin\n";
	circSS << "-- Original circuit\n";
	for(std::vector<CGate*>::iterator it = gateList.begin(); it != gateList.end(); it++){
		//don't write any lcells, input or output gates
		if(!(((*it)->type == "lcell") ||
				((*it)->type == "input") ||
				((*it)->type == "output"))){
			std::string inString = getInSs(*it);
			circSS << "\t" << (*it)->name << ": " << (*it)->type << " port map (" << inString
					<< (*it)->primOut[0] <<");\n";
		}
	}

	adjustCircuit(gateList, main);

	//	write the signals
	output << "\tsignal ";
	for (int i = 0; (unsigned)i < main->signals.size(); i++){
		output << main->signals[i] + ", ";
		//for better readability
		if((i > 0) && (i % 10 == 0)) output <<"\n\t\t";
	}
	output << main->primOut[0] <<", " << "mod_" + main->primOut[0] << ": std_logic;\n" <<std::endl;

	//write the original circuit
	output << circSS.rdbuf();

	//write the modified circuit
	output << "\n-- Circuit with LCELLs"<< std::endl;
	for(std::vector<CGate*>::iterator it = gateList.begin(); it != gateList.end(); it++){
		//don't write any lcells, input or output gates
		if(!(((*it)->type == "input") ||
				((*it)->type == "output"))){
			std::string inString = getInSs(*it);
			output << "\t" << "modCirc_" + (*it)->name << ": " << (*it)->type << " port map (" << inString
					<< (*it)->primOut[0] <<");\n";
		}
	}

	//add the XOR to compare the circuits
	std::vector<CGate*> tmpList = findByInput(gateList, "mod_" + main->primOut[0]);
	if(tmpList.size() == 1){
		output << "\n\tg00: mxor2 port map ("<< main->primOut[0] << ", " << tmpList[0]->primOut[0] << ", "
				<< newOut << ");\n";
	}
	if(hasFF){
		output << "\tg01: mxor2 port map (scan_data_out, mod_scan_data_out, circ_scan_data_out);\n";
	}

	output << "\nend structure;\n";

	//write all the behavioural descriptions of the components
	output << "\n-- Behavioural description of all components\n";
	if(checkListForType(gateList, "minv")) output << minv_entity + "\n";
	if(checkListForType(gateList, "mnand2")) output << mnand2_entity + "\n";
	if(checkListForType(gateList, "mand2")) output << mand2_entity + "\n";
	if(checkListForType(gateList, "mnor2")) output << mnor2_entity + "\n";
	if(checkListForType(gateList, "mor2")) output << mor2_entity + "\n";
	if(checkListForType(gateList, "lcell")) output << lcell_entity + "\n";
	if(hasFF) output << mdff_entity + "\n";
	//you'll need a XOR anyway, to compare the two circuits
	output << mxor2_entity + "\n";

}

std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
}

void  FindFile(const std::wstring &directory, std::wstring &name ,std::vector<std::wstring> &foundFiles)
{
    std::wstring tmp = directory + L"\\*";
    WIN32_FIND_DATAW file;
    
    HANDLE search_handle = FindFirstFileW(tmp.c_str(), &file);
    if (search_handle != INVALID_HANDLE_VALUE)
    {
        std::vector<std::wstring> directories;
		
        do
        {
            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
                    continue;
            }

            tmp = directory + L"\\" + std::wstring(file.cFileName);

            if( std::wstring(file.cFileName) == name) {
				foundFiles.push_back(tmp);
			}

            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                directories.push_back(tmp);
        }
        while (FindNextFileW(search_handle, &file));

        FindClose(search_handle);

        for(std::vector<std::wstring>::iterator iter = directories.begin(), end = directories.end(); iter != end; ++iter)
            FindFile(*iter, name, foundFiles);
    }
}

int main(int argc, char** args) {

	std::ifstream MyInFile;
	std::ofstream MyOutFile("testModLcell.vhd");

	std::string archString ("architecture structure of");
	std::string entityString ("entity");
	std::string componentString("component");
	std::string gateList;

	MyInFile.open("Mycomponent.vhd");
	//extract the name of the main component
	CComponent* maincomp;
	maincomp = new CComponent();
	maincomp->name = getName(MyInFile, archString);
	primInOut(MyInFile, entityString, maincomp);

	std::string tmp = archString + " " + maincomp->name;
	
	std::vector<CGate*> myVector = extractGates(MyInFile, tmp);

	assignSecInOut(maincomp, myVector);

	eraseByType(myVector, "mdff");
	//after this, the gateVector is complete
	addInOutGates(myVector, maincomp);
	//assign the remaining properties of each gate
	assignPrePostGates(myVector);

	levelize(myVector);

	makeMFFC(myVector, maincomp);

	writeCircuit(myVector, maincomp, MyOutFile);
	std::cout<<"finished writing circuit!"<<std::endl;

	// start the testscript
	system("test_file_quartus.bat");


	MyInFile.close();
	MyOutFile.close();

	return 0;
}


