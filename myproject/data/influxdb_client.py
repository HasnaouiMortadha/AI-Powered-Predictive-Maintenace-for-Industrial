from influxdb_client import InfluxDBClient, Point

class InfluxDBManager:
    def __init__(self, url, token, org, bucket):
        self.client = InfluxDBClient(url=url, token=token)
        self.bucket = bucket
        self.org = org

    def log_data(self, data):
        for sensor, values in data.items():
            point = Point(sensor)
            for key, value in values.items():
                point = point.field(key, value)
            self.client.write_api().write(bucket=self.bucket, org=self.org, record=point)
