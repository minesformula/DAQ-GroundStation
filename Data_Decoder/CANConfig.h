#ifndef CANCONFIG_H
#define CANCONFIG_H

#include <stdint.h>

enum class Sensors:uint8_t {
        UNKNOWN,
        ECU_RPM_WATERTEMP,
        ECU_OILTEMP,
        ECU_PUMPS_FAN,
        ECU_VOLTAGE,
        ECU_GEAR
};

struct LogMsg {
    Sensors type = Sensors::UNKNOWN;
    uint8_t sensorNum;
    uint8_t buf[8];
    uint32_t timestamp;
    uint16_t CANID;
};

std::string typeToString(Sensors type){
    if (type == Sensors::UNKNOWN) return "UNKNOWN";
    else if (type == Sensors::ECU_RPM_WATERTEMP) return "ECU_RPM_WATERTEMP";
    else if (type == Sensors::ECU_OILTEMP) return "ECU_OILTEMP";
    else if (type == Sensors::ECU_PUMPS_FAN) return "ECU_PUMPS_FAN";
    else if (type == Sensors::ECU_VOLTAGE) return "ECU_VOLTAGE";
    else if (type == Sensors::ECU_GEAR) return "ECU_GEAR";
    else return "UNSPECIFIED:" + std::to_string((uint8_t)(type));
}

const long int start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

std::string processMsgCsv(LogMsg msg){
    std::string retval = "";

    if (msg.type == Sensors::ECU_RPM_WATERTEMP){
        retval = std::to_string(start+msg.timestamp) + ",ECU_RPM," 
            + std::to_string(msg.sensorNum) + "," + std::to_string(msg.buf[7]*100) + "\n";

        retval += std::to_string(start+msg.timestamp) + ",ECU_WATERTEMP," + std::to_string(msg.sensorNum) + "," 
            + std::to_string(msg.buf[3]) + "\n";
    }
    else if (msg.type == Sensors::ECU_OILTEMP){

    }
    else if (msg.type == Sensors::ECU_PUMPS_FAN) {
        retval = std::to_string(start+msg.timestamp) + ",ECU_FUELPUMP," + std::to_string(msg.sensorNum) + "," 
            + std::to_string(msg.buf[0]) + "\n";

        retval += std::to_string(start+msg.timestamp) + ",ECU_WATERPUMP," + std::to_string(msg.sensorNum) + ","
            + std::to_string(msg.buf[1]) + "\n"; 

        retval += std::to_string(start+msg.timestamp) + ",ECU_FAN," + std::to_string(msg.sensorNum) + "," 
            + std::to_string(msg.buf[3]) + "\n";

    }
    else if (msg.type == Sensors::ECU_VOLTAGE && msg.buf[0] == 48) {
        retval += std::to_string(start+msg.timestamp) + "," + typeToString(msg.type) + "," 
            + std::to_string(msg.sensorNum) + "," + std::to_string(msg.buf[2]*0.1216) + "\n";
        std::cout << "Number: " << msg.buf[2]*0.1216 << "\n";
    }
    else if (msg.type == Sensors::ECU_GEAR) {
        retval += std::to_string(start+msg.timestamp) + ",ECU_NEUTRAL," 
            + std::to_string(msg.sensorNum) + "," + std::to_string(msg.buf[4]) + "\n";
    }
    
    return retval;
}

#endif