#include <fstream>
#include <iostream>
#include <string>
#include "ShipFile.h"

int main(int argc, char *argv[])
{
    // check program was started correctly
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        std::cerr << "  filename: Path to file to be shipped" << std::endl;
        return -2;
    }

    // get filename to open
    std::string filename = argv[1];

    // open file for parsing
    std::ifstream fileIn;
    fileIn.open(filename);
    shipData(fileIn);
    fileIn.close();

    return 0;
}