#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <chrono>

#include "CANConfig.h"

int main(int argc, char *argv[]){
    std::string inputFile;
    std::string rawFileName = "FileRaw.csv";
    std::string processedFileName = "File.csv";

    if(argc <= 1){
        std::cout << "Program Failed: No given file location" << std::endl;
        return -1;
    } 
    if (argc >= 2){
        inputFile = argv[1];
    } 
    if (argc >= 3){
        processedFileName = rawFileName = argv[2];
        rawFileName += "Raw.csv";
        processedFileName += ".csv";
    }

    std::fstream aFile(inputFile, std::ios::in | std::ios::binary);
    uint8_t buf[sizeof(LogMsg)];

    //Note that CANID is not unique and is recorded solely to prevent negligent data loss
    std::fstream rawFile(rawFileName, std::ios::out);
    rawFile << "Timestamp_(ms),Type,Sensor_Number,CANID,BUF[0],BUF[1],BUF[2],BUF[3],BUF[4],BUF[5],BUF[6],BUF[7]\n";

    std::fstream procFile(processedFileName, std::ios::out);
    procFile << "#datatype dateTime:number,measurement,tag,double\n";
    procFile << "time,type,sensor_number,value\n";

    while (!aFile.eof()){
        LogMsg* readMsg = new LogMsg;
        uint8_t buf[sizeof(LogMsg)];


        //Converting buffer to LogMsg. Can't cast directly for memory safety. 
        //Note there are some padding zeroes for some reason
        aFile.read((char*)buf, sizeof(LogMsg));
        readMsg->type = (Sensors) buf[0];
        readMsg->sensorNum = (int)buf[1];
        readMsg->CANID = (uint16_t)buf[16] |
            (uint16_t)buf[17] << 8;
        readMsg->timestamp = (uint32_t)buf[12] | 
            (uint32_t)buf[13] << 8 | 
            (uint32_t)buf[14] << 16 | 
            (uint32_t)buf[15] << 24;

        for (unsigned int i = 0; i < 8; i++){
            readMsg->buf[i] = buf[i+2];
        }

        //TODO: Write a class to write the message raw or in csv format
        procFile << processMsgCsv(*readMsg);

        rawFile << readMsg->timestamp << "," << typeToString(readMsg->type) 
            << "," << std::to_string(readMsg->sensorNum) << "," << readMsg->CANID << ",";

        for (unsigned int i = 2; i < 2+8; i++){
            rawFile << std::to_string(buf[i]);

            if (!(i+1 >= 2+8)){
                rawFile << ",";
            }
        }

        rawFile << "\n";
    }
}