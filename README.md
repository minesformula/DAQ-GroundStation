# DAQ-Groundstation
Utilities for ground station side DAQ interfaces.


## Visualizer QuickStart (Grafana and InfluxDB)

This project makes use of InfluxDB and Grafana to record and visualize data respectively. Both of these software
packages have been integrated with docker. On first use the web servers will have to be configured but they should
run normally after the initial setup. 

## Setup

1. Go to the [Docker Desktop](https://docs.docker.com/desktop/) page and follow their installation steps for your OS
2. Open Docker desktop and leave it running in the background
3. Clone this repository onto your computer
4. In powershell/terminal, navigate to the project's root directory and run `docker-compose up -d`
5. In your browser type `localhost:8086` and follow the prompts. Copy the API token somewhere
6. Go to `localhost:3000` and login using User:MFadmin Pass:MFadmin
7. Navigate to "Data Sources" and click "Add Data Source"
8. Select InfluxDB and choose the Flux query language
9. Set URL as `http://influxdb:8086`
10. Enter your InfluxDB username and password under basic auth
11. In InfluxDB details. Enter the organization you set for influx then set the default bucket to whatever 
you set in the config
12. Adjust minimum update time to 1s and max series to 1000000000
13. Copy the API token InfluxDB produced for you into the token field

## Usage

Any data pushed to InfluxDB may be queried using the [Flux query language](https://docs.influxdata.com/flux/v0/).
Within this project there are a few packages which can help with pushing data such as the [Live Telemetry](Live_Telemetry)
python script. Note all of these packages need an API token to be configured, new API tokens can be generated in
the InfluxDB GUI located at `localhost:8086`.