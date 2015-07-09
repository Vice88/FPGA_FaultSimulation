#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <tchar.h>
#include <string>
#include <locale>
#include "PcFPGACommunication.h"

using namespace std;

std::wstring HDD = L"F:\\Altera";

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


void PcFPGACommunication::programmFPGA(string file)
{
    std::vector<std::wstring> foundFiles;
	std::string pgmPath;
	std::wstring name =  L"quartus_pgm.exe";
	FindFile(HDD, name, foundFiles);
	
	if(foundFiles.size() < 1) {
		std::cout << "quartus_pgm.exe could not be found in C:" << std::endl;
	}
	else if (foundFiles.size() == 1){
		std::string s( foundFiles[0].begin(), foundFiles[0].end() );
		pgmPath = s;
		std::cout << "quartus_pgm.exe found in " << pgmPath << std::endl;
	}
	
	else {
		std::cout << "choose quartus path" << std::endl;
		for (int i = 0; i < foundFiles.size(); i++){
			std::wcout << i << "  " <<  foundFiles[i] << std::endl;
		}
		int a;
		std::cin >> a; 
		std::string s( foundFiles[a].begin(), foundFiles[a].end() );
		pgmPath = s;
	}
	
	string cfgFPGA;
    const string quartusPath = "\"\"" + pgmPath + "\"";
    cfgFPGA = quartusPath + " -c \"USB-Blaster\" -m JTAG -o p;\"" + file + "\"\"";
    system(cfgFPGA.c_str());
//    system((std:string)"\"C:\\Program Files\\Altera\\14.1\\quartus\\bin64\\quartus_pgm  -c \"USB-Blaster\" -m JTAG -o p;" + file + "\"");    
}

bool PcFPGACommunication::Open(string name, size_t baudRate)
{
    DCB serialPort = { 0 };
    bool rt;
    
    serialHandle = CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE,
                              0, 0, OPEN_EXISTING, 0, 0);
    
    if(serialHandle == INVALID_HANDLE_VALUE) printf("Serial Port in use!\n");
    
    rt = SetupComm(serialHandle, 4096, 4096);
    if(rt == false) printf("Failed to initialize com parameter!\n");
    
    rt = GetCommState(serialHandle, &serialPort);
    if(rt == false) printf("Failed retrieving control settings!\n");
    
    serialPort.BaudRate = baudRate;
    serialPort.ByteSize = 8;
    serialPort.StopBits = 0;
    serialPort.Parity = 0;
    serialPort.fBinary = 1;
    rt = SetCommState(serialHandle, &serialPort);
    if(rt == false) printf("Failed setting control settings!\n");
    
    // Set timeouts
    COMMTIMEOUTS timeout = { 0 };
    timeout.ReadIntervalTimeout = MAXDWORD;
    timeout.ReadTotalTimeoutConstant = 0;
    timeout.ReadTotalTimeoutMultiplier = 0;
    timeout.WriteTotalTimeoutConstant = 0;
    timeout.WriteTotalTimeoutMultiplier = 0;

    rt = SetCommTimeouts(serialHandle, &timeout);
    if(rt == false) printf("Failed setting timeouts settings!\n");

    return rt;
}

void PcFPGACommunication::Close()
{
    CloseHandle(serialHandle);
}

int PcFPGACommunication::Send(unsigned char *data, unsigned int length)
{
    unsigned long dwNumberOfBytesWritten;
	
    bool wr = WriteFile(serialHandle, &data[0], length, &dwNumberOfBytesWritten, NULL);
    if(!wr)
    {
    	cout << "Send data: write failed!" << endl;
    	return 0;
    }
    if (dwNumberOfBytesWritten != length)
    {
    	cout << "Send data: Only " << dwNumberOfBytesWritten << " from " << length << " bytes could be transmitted!" << endl;
    	return 0;
    }
    return 1;
}

void PcFPGACommunication::Receive(unsigned char* data, size_t length)
{   
    unsigned char buf[1];
    size_t i = 0;
    clock_t time;
    DWORD dwIncommingReadSize;
    
    
    time = clock();
    while(i < length)
    {
        ReadFile(serialHandle, buf, 1, &dwIncommingReadSize, NULL);
        if(dwIncommingReadSize > 0)
        {
            data[i] = buf[0];
            i++;
            time = clock();
        }
        if(float(clock() - time)/CLOCKS_PER_SEC >= 0.5) break;
    }
    
    evalResp(data, i);
}

int PcFPGACommunication::loadRAM(const char *fileName)
{
    ifstream file;
    file.open(fileName, ios::in);
    file.seekg(0, ios::end);
    int sizeInBytes = file.tellg();
    file.seekg(0, ios::beg );
    
    unsigned char data[sizeInBytes+1];
    data[0] = 112;
    
    for(int i = 0; i < sizeInBytes; i++)
    {
        data[i+1] = file.get();
    }
    
    file.close();

    if(sizeInBytes == -1)
        return -1;
    else
    {
        if(Send(data, sizeInBytes + 1) == 0)
            return -1;        
    }
    
    return 0;
}

void PcFPGACommunication::evalTest(unsigned char* data, size_t size)
{
    bool errDetec = false;
    for (size_t j = 1; j < size; j++)
    {
        for(size_t i = 0; i < 8; i++)
        {
            if(((128>>i) & data[j]) != 0)
            {
                errDetec = true;
                printf("Fault detected: %u\n", 1 + i + (j-1)*8);
            }
        }
    }
    if (!errDetec) printf("No Fault detected!\n");
}

void PcFPGACommunication::evalResp(unsigned char* data, size_t size)
{
    switch(data[0])
    {
        case 3:
            printf("FIFO ist voll.\n");
            printf("Werte %u Bytes aus.\n\n", size - 1);
            evalTest(data, size);
            break;
        case 65:
            printf("Testsequenz fertig.\n\n");
            evalTest(data, size);
            break;
        case 66:
            printf("RAM ist leer.\n");
            break;
        case 113:
            printf("Daten wurden in den RAM geladen.\n\n");
            break;
        case 114:
            printf("RAM ist voll.\n\n");
            break;
        default:
            printf("Error.\n\n");
            break;
    }
}


