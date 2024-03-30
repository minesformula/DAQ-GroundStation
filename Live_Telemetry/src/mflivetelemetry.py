from serial import Serial
import click

from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS

@click.command()
@click.argument('portName', required=True, type=click.Path())
def cli(portname) -> None:

    ser: Serial
    posNames: dict = {}
    

    ser = Serial(portname, 57600)
    
    while(not ser.is_open):
        ser.open()

    print("Serial Acquired")

    ser.reset_input_buffer()

    client: InfluxDBClient

    client = InfluxDBClient(url="http://104.131.85.212:8086", token="wVLmmRiwtodmVZeBchKrCzFN7tMiGsS9Jp4haaTOYvS6yZ2F7fjMMSyV_3ZKpq8HjRpyCBrPeTAK9CkkHvZUQw==")
    write_api = client.write_api(write_options=SYNCHRONOUS)


    while(True):
        try:
            point: list[str] = str(ser.readline().decode()).replace('\n', '').replace('\r', '').split(",")
        except:
            print("Waiting")
            while(not ser.is_open):
                ser.open()

            print("Serial Acquired")
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

if (__name__ == "__main__"):
    cli()