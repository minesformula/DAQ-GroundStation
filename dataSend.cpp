// Relies on https://github.com/offa/influxdb-cxx
// Relies on c++17

#include "include/dataSend.h"

void Database::createEntry(std::string type)
{
    sensorValues.push_back(influxdb::Point{type});
}

void Database::addValue(double val, int pos, std::string unit)
{
    sensorValues[pos].addField(unit, val)
}

void Database::submitToInflux(std::vector<double> sensorValues, std::vector<std::string> units, std::vector<std::string> field)
{

    usleep(1000); // For testing. Delete upon recieving access to actual sensor data.

    auto influxdb = influxdb::InfluxDBFactory::Get(URI);
    influxdb->batchOf(1000);

    for (int i = 0; i < sensorValues.size(), i++)
    {
        influxdb->write(sensorValues[i]);
    }

    influxdb->flushBatch();
}

string Database::getURI()
{
    return URI;
}