#ifndef SHIPFILE_H
#define SHIPFILE_H

#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <InfluxDBFactory.h>
#include <InfluxDB.h>
#include <unistd.h>

#define FILE_INFLUX_HOST "localhost"   // Host IP Address goes here
#define FILE_INFLUX_BUCKET "daqactual" // The name of the influx bucket/database goes here
#define FILE_INFLUX_USER "member"      // Username of your file bucket auth account goes here
#define FILE_INFLUX_PASS "passcode"    // Password of your file bucket auth account goes here

// #define URI "http://member:passcode@localhost:8086?db=daqactual" //Completed URI to prevent repeated memory allocation

void shipData(std::ifstream &fileIn);

void submitToInflux(std::vector<std::string> measurements, std::vector<std::string> units, std::vector<double> &values);

#endif