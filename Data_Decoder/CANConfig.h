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

#endif