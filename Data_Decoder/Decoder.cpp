#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "CANConfig.h"

int main(int argc, char *argv[]){
    std::string inputFile;
    std::string outputFile = "outFile.csv";

    if(argc <= 1){
        std::cout << "Program Failed: No given file location" << std::endl;
        return -1;
    } 
    if (argc >= 2){
        inputFile = argv[1];
    } 
    if (argc >= 3){
        outputFile = argv[2];
    }

    std::fstream aFile(inputFile, std::ios::in | std::ios::binary);
    uint8_t buf[sizeof(LogMsg)];

    std::fstream writeFile(outputFile, std::ios::out);

    //Note that CANID is not unique and is recorded solely to prevent negligent data loss
    writeFile << "Timestamp_(ms),Type,Sensor_Number,CANID,BUF[0],BUF[1],BUF[2],BUF[3],BUF[4],BUF[5],BUF[6],BUF[7]\n";

    while (!aFile.eof()){

        LogMsg* readMsg = new LogMsg;

        uint8_t buf[sizeof(LogMsg)];
        aFile.read((char*)buf, sizeof(LogMsg));


        std::cout << "MSG: ";
        for (unsigned int i = 0; i < sizeof(LogMsg); i++){
            std::cout << +buf[i] << " ";
        }
        std::cout << std::endl;

        readMsg->type = (Sensors) buf[0];
        readMsg->sensorNum = (int)buf[1];
        readMsg->CANID = (uint16_t)buf[16] |
            (uint16_t)buf[17] << 8;
        readMsg->timestamp = (uint32_t)buf[12] | 
            (uint32_t)buf[13] << 8 | 
            (uint32_t)buf[14] << 16 | 
            (uint32_t)buf[15] << 24;

        writeFile << readMsg->timestamp << "," << typeToString(readMsg->type) 
            << "," << std::to_string(readMsg->sensorNum) << "," << readMsg->CANID << ",";

        for (unsigned int i = 2; i < 2+8; i++){
            writeFile << std::to_string(buf[i]);

            if (!(i+1 >= 2+sizeof(LogMsg))){
                writeFile<< ",";
            }
        }

        writeFile << "\n";
    }
}