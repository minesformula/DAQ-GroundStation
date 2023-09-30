from serial import Serial
import click

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

@click.command()
@click.argument('portName', required=True, type=click.Path())
def cli(portname) -> None:

    ser: Serial

    ser = Serial(portname, 57600)
    
    while(not ser.is_open):
        ser.open()

    print("Serial Acquired")

    ser.reset_input_buffer()

    client: InfluxDBClient

    while(True):
        point: list[str] = str(ser.readline().decode()).split(" ")

        for a in point:
            print(a)

        bucket = "daqlive"

        client = InfluxDBClient(url="http://104.131.85.212:8086", token="wVLmmRiwtodmVZeBchKrCzFN7tMiGsS9Jp4haaTOYvS6yZ2F7fjMMSyV_3ZKpq8HjRpyCBrPeTAK9CkkHvZUQw==")
        write_api = client.write_api(write_options=SYNCHRONOUS)

        #Index 0 will be the name of the related object (E.X: Engine, Suspension, etc)
        #Index 1 will be the data source (E.X: ECU, PDM, Hall_Effects_1)
        #Index 2 will be units
        #Index 3 will be the value
        p = Point(point[0]).tag("Data_Source", point[1]).field(point[2], float(point[3]))

        write_api.write(bucket="daqlive", record=p, org="minesformula", )

if (__name__ == "__main__"):
    cli()