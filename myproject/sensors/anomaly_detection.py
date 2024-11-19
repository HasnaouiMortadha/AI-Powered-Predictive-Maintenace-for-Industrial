from sklearn.ensemble import IsolationForest
import joblib

def train_model(sensor_data):
    model = IsolationForest(contamination=0.1)
    model.fit(sensor_data)
    joblib.dump(model, 'model.pkl')

def analyze_data(sensor_data):
    model = joblib.load('model.pkl')
    features = [
        sensor_data["ADXL345"]["x"],
        sensor_data["ADXL345"]["y"],
        sensor_data["DHT22"]["temperature"],
        sensor_data["INA219"]["voltage"]
    ]
    prediction = model.predict([features])
    return "Anomaly Detected" if prediction[0] == -1 else "Normal"
