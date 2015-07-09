#ifndef PCFPGACOMMUNICATION_H
#define	PCFPGACOMMUNICATION_H

#include<windows.h>
#include <string>

class PcFPGACommunication {
public:
    void programmFPGA(std::string file);
    bool Open(std::string name, size_t baudRate);
    void Close();
    int Send(unsigned char *data, unsigned int length);
    void Receive(unsigned char *data, size_t length);
    int loadRAM(const char *fileName);
    void evalTest(unsigned char* data, size_t size);
    void evalResp(unsigned char* data, size_t size);
    
private:
    HANDLE serialHandle;

};

#endif	/* PCFPGACOMMUNICATION_H */

