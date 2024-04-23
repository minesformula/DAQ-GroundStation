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

@click.command(help = "Configure API for InfluxDB writing")
@click.argument('default_user', required=True, type=click.STRING)
@click.argument('default_password', required=True, type=click.STRING)
@click.argument('default_organization', required=True, type=click.STRING)
@click.argument('default_bucket', required=True, type=click.STRING)
@click.argument('api_key', required=True, type=click.STRING)
def configure(default_user:str, default_password:str, default_organization:str, default_bucket:str, api_key:str) -> None:
    directoryName:str

    if (str(os.name) == "nt"):
        directoryName = "~mfconfig"
    else:
        directoryName = ".mfconfig"

    if (not os.path.exists(Path.home() / directoryName)):
        os.mkdir(Path.home() / directoryName)
    
    with open(Path.home() / directoryName / "influxdb_config", mode='w') as f:
        f.write(api_key + "\n" + default_user + "\n" + default_password + "\n" + default_organization + "\n" + default_bucket)
        

@click.command(help = "Enable DAQ Live Telemetry reading from a specified serial port")
@click.argument('portName', required=True, type=click.Path())
@click.option('-b', '--bucket', 'bucket', type=click.STRING)
@click.option('-o', '--organization', 'organization', type=click.STRING)
@click.option('-u', '--username', 'username', type=click.STRING)
@click.option('-p', '--password', 'password', type=click.STRING)
@click.option('-k', "--key", "key", type=click.STRING)
def live(portname, bucket:str, organization:str, username:str, password:str, key:str) -> None:

    ser: Serial
    posNames: dict = {}

    if (not bucket or not organization or not username or not password or not key):
        try:
            lines: list[str]
            if (str(os.name) == "nt"):
                with open(Path.home() / "~mfconfig" / "influxdb_config") as f:
                    lines = f.readlines()
                    
            else:
                with open(Path.home() / ".mfconfig" / "influxdb_config") as f:
                    lines = f.readlines()
            
            if not key:
                key = lines[0]
            if not username:
                username = lines[1]
            if not password:
                password = lines[2]
            if not organization:
                organization = lines[3]
            if not bucket:
                bucket = lines[4]
        except Exception as e:
            print(str(e))
            print("Error: Use configure command to add default values or use the flags")
            exit(-1)
    
    ser = Serial(portname, 57600)
    
    while(not ser.is_open):
        ser.open()

    print("Serial Acquired")

    ser.reset_input_buffer()

    client: InfluxDBClient

    client = InfluxDBClient(url="http://localhost:8086", key=key, username=username, password=password)
    write_api = client.write_api(write_options=ASYNCHRONOUS)


    while(True):
        try:
            point: list[str] = str(ser.readline().decode()).replace('\n', '').replace('\r', '').split(",")
        except:
            while(not ser.is_open):
                ser.open()
            continue

        print(point)

        try:
            if (point[0] == "0"):
                names = []
                for i in range(2,len(point)):
                    names.append(point[i])

                posNames.update({point[1]: names})

            elif (point[0] == "1" and posNames and len(point) == 11):
                if (point[1] in posNames.keys()):
                    for i in range(0,len(posNames[point[1]])):
                        p = Point(str(point[1])).tag("SensorNum", str(point[2])).field(posNames[point[1]][i], float(point[i+3]))
                        write_api.write(bucket=bucket, record=p, org=organization)
        except:
            continue

cli.add_command(configure)
cli.add_command(live)

if (__name__ == "__main__"):
    cli()