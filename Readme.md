# Requirements

* Quartus II
* MinGW or other C++ compiler
* Make sure quartus driver for usb cables are installed

# Preperation

1. open FaultInjectionTester.qar with Quartus
2. compile the project
3. close Quartus
4. open command window and go to folder FaultConfiguration
5. execute: g++ IRA.cpp CGate.cpp CGate.h CComponent.cpp CComponent.h -o ..\\IRA.exe (or compile with the IDE of your choice and save the .exe into root folder)
6. in command window go to folder FPGACommunication
7. g++ FaultInjectionTester.cpp PcFPGACommunication.cpp PcFPGACommunication.h -o ..\\FaultInjectionTest.exe (or compile with the IDE of your choice and save the output file as "FaultInjectionTest.exe" into root folder)
8. connect terasic DE2-115 board with USB blaster (usba to usbb) and rs232 to USB cable with your computer.

# Execution

1. start IRA.exe and enjoy.
2. eventually there is a log file

# Common Errors

* "Inconsistent set or reset compile started variable" -> clear out db and incremental_db folder in FaultInjectiontester_restored folder and recompile quartus project
* Cant find quartus_cdb ->  if quartus is not installed on "C:" you have to change the HDD constant in FPGACommunication/PcFPGACommunication.cpp and FaultConfiguration/IRA.cpp to the drive were your quartus is installed.


