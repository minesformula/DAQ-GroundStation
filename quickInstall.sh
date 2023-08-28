#!/bin/bash

cd ~

sudo apt update

sudo apt-get install g++
sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libboost-all-dev


git clone https://github.com/catchorg/Catch2
cd Catch2
mkdir build && cd build
cmake ..
sudo make install
cd ~

git clone https://github.com/rollbear/trompeloeil
cd trompeloeil
mkdir build && cd build
cmake ..
sudo make install
cd ~

git clone https://github.com/libcpr/cpr.git
cd cpr && mkdir build && cd build
cmake .. -DCPR_USE_SYSTEM_CURL=ON
cmake --build .
sudo cmake --install .
cd ~

git clone https://github.com/offa/influxdb-cxx
cd influxdb-cxx
mkdir build && cd build
cmake ..
sudo make install
cd ~

sudo rm -r Catch2
sudo rm -r trompeloeil
sudo rm -r cpr
sudo rm -r influxdb-cxx

sudo apt-get install influxdb
sudo systemctl unmask influxdb
sudo systemctl start influxdb
sudo systemctl enable influxdb

