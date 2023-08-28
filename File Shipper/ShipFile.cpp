#include <chrono>
using namespace std::chrono;

#include "ShipFile.h"

void shipData(std::ifstream &fileIn)
{
    std::vector<std::string> measurements, units;
    std::string currString;

    auto start = high_resolution_clock::now();

    fileIn >> currString;
    fileIn >> currString;
    while (currString != "0.000")
    {

        if (currString.at(0) == '(' || currString.at(0) == '[')
        {
            units.push_back(currString);
            fileIn >> currString;
        }
        else
        {
            measurements.push_back(currString);
            fileIn >> currString;
            while (currString.at(0) != '(' && currString.at(0) != '[')
            {
                measurements[measurements.size() - 1] += ("_" + currString);
                fileIn >> currString;
            }
        }
    }

    /// std::cout << "\n\nUnit Size: " <<  units.size() << "\nMeasurements Size: " << measurements.size() << std::endl;

    // auto duration = duration_cast<seconds>(high_resolution_clock::now() - start);
    // std::cout << "First Line Complete: " << duration.count()  << "ms" << std::endl;

    std::vector<double> values;
    values.resize(units.size());

    int LinesRead = 0;
    while (!(fileIn.eof()))
    {
        /*
        for (int i = 0; i < values.size(); i++){
            values[i] = stod(currString);
            fileIn >> currString;
            //std::cout << "Values: " << currString << std::endl;
            if (fileIn.eof()){
                break;
            }
        }
        */

        values.push_back(stod(currString));
        fileIn >> currString;

        // submitToInflux(measurements, units, values);
        // std::cout << ++LinesRead << std::endl;
    }

    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    std::cout << "Full File Read: " << duration.count() << " ms" << std::endl;

    submitToInflux(measurements, units, values);
    std::cout << "Done" << std::endl;
}

void submitToInflux(const std::vector<std::string> measurements, const std::vector<std::string> units, std::vector<double> &values)
{
    // std::string URI = std::string("http://") + FILE_INFLUX_USER + std::string(":") + FILE_INFLUX_PASS + std::string("@") + FILE_INFLUX_HOST + std::string(":8086?db=") + FILE_INFLUX_BUCKET;
    std::string URI = std::string("http://minesformula:mf11-cleetus@104.131.85.212:8086?db=daqactual");
    auto start = high_resolution_clock::now();

    auto influxdb = influxdb::InfluxDBFactory::Get(URI);
    influxdb->batchOf(values.size());

    const int readValues = values.size() / measurements.size();

    for (int j = 0; j < values.size(); j = j)
    {
        for (int i = 0; i < measurements.size(); i++)
        {
            influxdb->write(influxdb::Point{units[i]}.addField(measurements[i], values[j]));
            j++;
        }

        if (values.size() > 1000)
        {
            influxdb->flushBatch();
        }
    }

    auto duration = duration_cast<seconds>(high_resolution_clock::now() - start);

    // std::cout << "Submission Time: " << duration.count() << " ms" << std::endl;
    return;
}
