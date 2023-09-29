from serial import Serial
import click
import time

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

@click.command()
@click.argument('portName', required=True, type=click.Path())
def cli(portname) -> None:

    ser: Serial

    try:
        ser = Serial.open(portname, 57600)
    except:
        print("\n" + portname + " is not a valid Serial Port\n")
        exit()

    ser.read_all()

    while(ser.read() != b'!'):
        ser.write(b'?')
        ser.flush()

        time.sleep(1)

    print("\nTelemetry Signal Acquired\n")
    ser.read_all()

    ser.write(b'n')

    noTelemetry: int = 0
    client: InfluxDBClient

    while(noTelemetry <= 20):

        if(not ser.readable):
            noTelemetry += 1
            time.sleep(0.25)
        else:
            point: list[str] = str(ser.readline()).split(" ")

            bucket = "daqlive"

            client = InfluxDBClient(url="http://104.131.85.212:8086", token="wVLmmRiwtodmVZeBchKrCzFN7tMiGsS9Jp4haaTOYvS6yZ2F7fjMMSyV_3ZKpq8HjRpyCBrPeTAK9CkkHvZUQw==", org="minesformula")
            write_api = client.write_api(write_options=SYNCHRONOUS)

            #Index 0 will be the name of the related object (E.X: Engine, Suspension, etc)
            #Index 1 will be the data source (E.X: ECU, PDM, Hall_Effects_1)
            #Index 2 will be units
            #Index 3 will be the value
            p = Point(point[0]).tag("Data_Source", point[1]).field(point[2], float(point[3]))

            write_api.write(bucket=bucket, record=p)

    ser.close()
    client.close()

    print("\n\nConnection Lost: Timeout\n\n")
    exit()

if (__name__ == "__main__"):
    cli()