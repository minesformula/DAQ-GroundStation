#ifndef INFLUXDB_H
#define INFLUXDB_H

#include "influxDB_Config.h"
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

class Database
{
public:
    // Function that adds a data point given the type of data being collected.
    void createEntry(std::string type);

    // Function that modifies an existing Entry to add a value. Finds entry based on position in sensorValues array.
    void addValue(double value, int position, std::string unit);

    // Function to submit metrics to Influx
    void submitToInflux(std::vector<double> sensorValues, std::vector<std::string> units, std::vector<std::string> field);

    // Function that returns the URI
    std::string getURI();

private:
    std::vector<influxdb::Point> sensorValues;
    std::string URI = std::string("http://") + INFLUX_USER + std::string(":") + INFLUX_PASS + std::string("@") + INFLUX_HOST + std::string(":8086?db=") + INFLUX_BUCKET;
};

#endif