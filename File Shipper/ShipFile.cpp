#include <chrono>
using namespace std::chrono;

#include "ShipFile.h"

void shipData(std::ifstream &fileIn)
{
    std::vector<std::string> measurements;
    std::string currString;

    auto start = high_resolution_clock::now();
 
    
    std::getline(fileIn, currString);
    while (currString.find(',') != std::string::npos){
        measurements.push_back(currString.substr(0, currString.find(',')));
        currString.erase(0, currString.find(',')+1);
    }

    /// std::cout << "\n\nUnit Size: " <<  units.size() << "\nMeasurements Size: " << measurements.size() << std::endl;

    // auto duration = duration_cast<seconds>(high_resolution_clock::now() - start);
    // std::cout << "First Line Complete: " << duration.count()  << "ms" << std::endl;

    std::vector<double> values;
    values.resize(measurements.size());

    while (!fileIn.eof()){
        std::getline(fileIn, currString);
        for(int i = 0; currString.find(',') != std::string::npos; i++){
            values[i] = stod(currString.substr(0, currString.find(',')+1));
            currString.erase(0, currString.find(',')+1);
        }

        submitToInflux(measurements, values);
    }
}

void submitToInflux(std::vector<std::string> &measurements, std::vector<double> &values)
{
    // std::string URI = std::string("http://") + FILE_INFLUX_USER + std::string(":") + FILE_INFLUX_PASS + std::string("@") + FILE_INFLUX_HOST + std::string(":8086?db=") + FILE_INFLUX_BUCKET;
    std::string URI = std::string("http://minesformula:mf11-cleetus@localhost:8086?db=daqactual");
    auto start = high_resolution_clock::now();

    auto influxdb = influxdb::InfluxDBFactory::Get(URI);
    influxdb->batchOf(values.size());

    const int readValues = values.size() / measurements.size();

    for (int i = 0; i < measurements.size(); i++)
    {
        influxdb->write(influxdb::Point{measurements[i]}.addField("value", values[i]));
    }

    influxdb->flushBatch();

    auto duration = duration_cast<seconds>(high_resolution_clock::now() - start);

    //std::cout << "Submission Time: " << duration.count() << " ms" << std::endl;
    return;
}