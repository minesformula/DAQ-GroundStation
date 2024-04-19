from serial import Serial
import click
import importlib.metadata as metadata

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import ASYNCHRONOUS

import os
from pathlib import Path


@click.group(invoke_without_command=True)
@click.option("-v", "--version", is_flag=True, help="Display Package Version")
def cli(version):
    if(version):
        click.echo("MFLiveTelemetry\nVer " + metadata.version('mflivetelemetry'))
        exit(1)

    pass

@click.command(help = "Configure API key for InfluxDB writing")
@click.argument('api_key', required=True)
def configure(api_key:str) -> None:
    directoryName:str

    if (str(os.name) == "nt"):
        directoryName = "~mfconfig"
    else:
        directoryName = ".mfconfig"

    if (not os.path.exists(Path.home() / directoryName)):
        os.mkdir(Path.home() / directoryName)
    
    with open(Path.home() / directoryName / "influxdb_config", mode='w') as f:
        f.write(api_key)
        

@click.command(help = "Enable DAQ Live Telemetry reading from a specified serial port")
@click.argument('portName', required=True, type=click.Path())
@click.option('-k', "--key", "key", type=click.STRING)
def live(portname, key:str) -> None:

    ser: Serial
    posNames: dict = {}
    key: str

    if (not key):
        try:
            if (str(os.name) == "nt"):
                with open(Path.home() / "~mfconfig" / "influxdb_config") as f:
                    key = f.readline()
            else:
                with open(Path.home() / ".mfconfig" / "influxdb_config") as f:
                    key = f.readline()
        except:
            print("Error: Use configure command to add an API key or use the -k flag")
            exit(-1)
    

    ser = Serial(portname, 57600)
    
    while(not ser.is_open):
        ser.open()

    print("Serial Acquired")

    ser.reset_input_buffer()

    client: InfluxDBClient

    client = InfluxDBClient(url="http://localhost:8086", key=key)
    write_api = client.write_api(write_options=ASYNCHRONOUS)


    while(True):
        try:
            point: list[str] = str(ser.readline().decode()).replace('\n', '').replace('\r', '').split(",")
        except:
            while(not ser.is_open):
                ser.open()
            continue

        print(point)

        if (point[0] == "0"):
            names = []
            for i in range(2,len(point)):
                names.append(point[i])

            posNames.update({point[1]: names})

        elif (point[0] == "1" and posNames and len(point) == 11):
            if (point[1] in posNames.keys()):
                for i in range(0,len(posNames[point[1]])):
                    p = Point(str(point[1])).tag("SensorNum", str(point[2])).field(posNames[point[1]][i], float(point[i+3]))
                    write_api.write(bucket="daqlive", record=p, org="minesformula")

cli.add_command(configure)
cli.add_command(live)

if (__name__ == "__main__"):
    cli()