# app/routes.py

from flask import Blueprint, render_template
from sensors.sensor_data import (
    get_adxl345_data,
    get_dht22_data,
    get_mpx5700ap_data,
    get_ina219_data,
    get_ky038_data
)

# Create a Blueprint
bp = Blueprint('main', __name__)

# Define routes
@bp.route('/')
def index():
    # Render a template for the homepage
    return render_template('index.html')

@bp.route('/adxl345')
def adxl345():
    data = get_adxl345_data()
    return render_template('sensor.html', sensor_name="ADXL345 Accelerometer", data=data)

@bp.route('/dht22')
def dht22():
    data = get_dht22_data()
    return render_template('sensor.html', sensor_name="DHT22 Temperature and Humidity", data=data)

@bp.route('/mpx5700ap')
def mpx5700ap():
    data = get_mpx5700ap_data()
    return render_template('sensor.html', sensor_name="MPX5700AP Pressure Sensor", data=data)

@bp.route('/ina219')
def ina219():
    data = get_ina219_data()
    return render_template('sensor.html', sensor_name="INA219 Voltage and Current Sensor", data=data)

@bp.route('/ky038')
def ky038():
    data = get_ky038_data()
    return render_template('sensor.html', sensor_name="KY-038 Sound Sensor", data=data)

