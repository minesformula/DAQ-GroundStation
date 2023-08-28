# DAQ
Software for physical Data Acquisition System on Mines Formula cars. Work in Progress...


## Visualizer QuickStart (Influx-CXX, Grafana and InfluxDB)

This project makes use of InfluxDB and Grafana to record and visualize data respectively. The visualizer must be installed on a Raspberry Pi 4+ and a 64 bit operating systems in order to comply with InfluxDB requirements. 

A quick installer named quickInstall.sh will be provided in the DAQ folder. This can allow you to skip the installation steps below by running 'sudo quickInstall.sh' in the DAQ folder. Though it is recommended that you follow the manual installation steps for troubleshooting.

The below installation steps assume you are using Ubuntu on a Raspberry Pi that fits the above requirements.


### Influx-CXX Installation

Run `sudo apt install cmake g++ make git curl` on your Raspberry Pi command line. This command, on success, will install the tools necessary to install Influx-CXX and it's dependencies.

Run the below command. This will install all git repos necessary and also install the required libraries if you haven't already.

```
git clone https://github.com/catchorg/Catch2
git clone https://github.com/rollbear/trompeloeil
git clone https://github.com/offa/influxdb-cxx

sudo apt update

sudo apt-get install g++
sudo apt-get install build-essential
sudo apt-get install cmake
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install libboost-all-dev
```

Once the process has completed run the below command to build the repositories. Note that the cpr library has special flags. These flags are necessary for compiling influxdb-cxx. (Note that at the time of writing InfluxDB-CXX has some issues when compiling)

```
cd Catch2
mkdir build && cd build
cmake ..
sudo make install
cd ../..

cd trompeloeil
mkdir build && cd build
cmake ..
sudo make install
cd ../..

git clone https://github.com/libcpr/cpr.git
cd cpr && mkdir build && cd build
cmake .. -DCPR_USE_SYSTEM_CURL=ON
cmake --build .
sudo cmake --install .
cd ../..

cd influxdb-cxx
mkdir build && cd build
cmake ..
sudo make install
cd ../..
```

Finally run the below command to erase the leftover files.

```
sudo rm -r Catch2
sudo rm -r trompeloeil
sudo rm -r cpr
sudo rm -r influxdb-cxx
```

Once all above process have been completed influxdb-cxx will be installed on the system. There may still be missing repositories required for the visualizer to compile.



### InfluxDB Installation

Run the below command in your Raspberry Pi command line to install the Influx CLI. This is necessary for initial setup and proper authentication control later in the process.

```
sudo apt-get install influxdb
sudo systemctl unmask influxdb
sudo systemctl start influxdb
sudo systemctl enable influxdb
```

### InfluxDB Configuration

Keep in mind that depending on how you choose to host influxdb you might have to run `sudo ufw disable`. Keep in mind this will disable your firewall allowing incoming traffic to access your server.

Once Influx CLI is installed run `influx setup` and follow the prompts to complete database setup.

After answering each prompt run `influx bucket ls` and copy the ID of the bucket that was just created via influx setup. Using your copied BUCKET_ID Run the below command replacing BUCKET_ID, USERNAME, PASSWORD and ORG_NAME with their corresponding values.
```
influx v1 auth create --write-bucket <BUCKET_ID> --read-bucket <BUCKET_ID> --username <USERNAME> --password <PASSWORD> --org <ORG_NAME>
```

This auth account will be used with influxdb-cxx to authorize the writing of information to the influx database (a database and a bucket are functionally the same in this case). Find the "influxDB_Config.h" file in the "DAQ Data Visualizer" directory and replace each of the following constants with their corresponding value. Please note that in most cases the INFLUX_HOST will be localhost since influxDB and Grafana should be working on the same device for our purposes. For the purposes of Mines Formula these values may be preset.

```
#define INFLUX_HOST "<IP_ADDRESS>" //Host IP Address goes here
#define INFLUX_BUCKET "<BUCKET_NAME>" //The name of the influx bucket/databse goes here
#define INFLUX_USER "<USERNAME>" //Username of your auth account goes here
#define INFLUX_PASS "<PASSWORD>" //Password of your auth account goes here
```

There must be no spaces in these values though if you created a value with a space it can be accounted for like "This%20is%20an%20example" remember if you can't put the url into your search bar it will not work in the program.

InfluxDB should be capable of receiving data from the submitToInflux function if it is not receiving data or the program is throwing an error (especially a 400 or 401 error) recheck the previous steps.


### Grafana Installation

Run the following command to install and start the OSS version of Grafana.
```
apt-get install wget curl gnupg2 apt-transport-https software-properties-common -y
sudo wget -q -O /usr/share/keyrings/grafana.key https://packages.grafana.com/gpg.key
echo "deb https://packages.grafana.com/oss/deb stable main" | tee -a /etc/apt/sources.list.d/grafana.list
apt-get update -y
grafana-cli plugins install pr0ps-trackmap-panel
systemctl unmask grafana-server
systemctl start grafana-server
systemctl enable grafana-server
```

Type `ss -antpl | grep 3000` to ensure that Grafana has downloaded and started correctly.


### Grafana Configuration

Once you have verified that the installation was a success quickly open a browser window (on any device) and connect port 3000 on the Raspberry Pi. For reference this will be the IP of the Raspberry Pi followed by port 3000 like so `IP_ADDRESS:PORT` (with example IP address and port 3000:`192.158.1.38:3000`). This should bring you to a Grafana UI login screen.

Once on the Grafana login screen enter the default `admin` username and `admin` password before following the prompts to set a new password for the Grafana admin account. Then click the gear on the bottom left and `add data source`. 

First select the `Flux` query language because the default influxDBquery language and Grafana 2.4(version at time of writing) don't currently work well together. Next, add a url for InfluxDB using the same `IP_ADDRESS:PORT` format you used to connect to Grafana with port 8086 instead of 3000. Since Grafana is being run on the Raspberry Pi you may use localhost in place of your IP Address (assuming that your influxDB instance is on the Raspberry Pi and not some external device). 

Once you've added the influxDB url, select basic auth and enter the username and password of the influx v1 auth account you created during the influxDB installation. 

For this next step I recommend also logging into the influxDB UI on port 8086. You should have already set a username and password during setup in the influxDB installation.

In the influxDB UI: Select `load data` and `API TOKENS` then `GENERATE API TOKEN` and `Read/Write API Token`. From here you may either give your API token Read/Write access to singular buckets or grant access to all present and future buckets. Finally save your new token and then copy it to your clipboard.

In the Grafana UI: Paste the API token you just generated into the influxDB details and then fill in your organization and default bucket names. Finally select `Save & test` to save your influxDB data source.

If this step is causing issues an admin account created in the `influx v1 shell` can be used to replace the organization (account username) and the token (account password).

It should now be possible to create a dashboard using data from your influxDB database. 

Since the query language we're using is Flux I would recommend reading about the use of flux in Grafana here: https://grafana.com/docs/grafana/latest/datasources/influxdb/influxdb-flux/

If necessary you may configure Grafana more by following the steps outlined here: https://grafana.com/docs/grafana/latest/setup-grafana/configure-grafana/

