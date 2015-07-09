#include <stdio.h>
#include <stdlib.h>

#include "PcFPGACommunication.h"


std::string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
    return std::string( buffer ).substr( 0, pos);
}


int main(int argc, char** argv) {
	bool eRR = false;

	std::string comPort;
	std::string sofFile;
	std::string patternFile;

	size_t rcvBytes;

	// Check argc
	if (argc == 1) {
		std::string path = ExePath();
		comPort = "COM1";
		sofFile = path + "\\FaultInjectionTester_restored\\output_files\\FaultInjectionTester.sof";
		patternFile = path + "\\TestSequenz.txt";
		rcvBytes = 300;
	} else if (argc == 5) {
		comPort = argv[1];
		sofFile = argv[2];
		patternFile = argv[3];
		rcvBytes = atoi(argv[4]);
	} else {
		eRR = true;
	}

	if (!eRR) {
		PcFPGACommunication a;
		a.programmFPGA(sofFile);

		 if(a.Open(comPort, 115200))
		 {
		 	 unsigned char Test[rcvBytes];
		 	 unsigned char Load[1] = {64};

		 	 a.loadRAM(patternFile.c_str());
		 	 a.Receive(Test, 1);
		 	 a.Send(Load, 1);
		 	 a.Receive(Test, rcvBytes);
		 	 a.Close();
		 }
	} else
		printf("Wrong number of parameter!\n");
}
