def preprocess_data(data):
    # Example preprocessing: normalize values
    data["ADXL345"]["x"] /= 256.0
    data["ADXL345"]["y"] /= 256.0
    data["DHT22"]["temperature"] = max(0, data["DHT22"]["temperature"])
    return data
