import pytest
from sensors.sensor_data import SensorSystem

def test_read_all_sensors():
    system = SensorSystem()
    data = system.read_all_sensors()
    assert "ADXL345" in data
    assert "DHT22" in data
