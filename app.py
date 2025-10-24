from flask import Flask, request, jsonify, render_template
from flask_cors import CORS
import random
import time
import threading

app = Flask(__name__)
CORS(app)

# Simulated sensor data that changes over time
sensor_data = {
    "temp": 36.5,
    "pulse": 75,
    "ecg": 512
}

def simulate_sensor_data():
    """Function to simulate changing sensor values"""
    while True:
        # Simulate realistic vital signs
        sensor_data["temp"] = round(random.uniform(36.0, 37.5), 1)  # Normal body temperature
        sensor_data["pulse"] = random.randint(60, 100)  # Normal heart rate
        sensor_data["ecg"] = random.randint(400, 600)   # Simulated ECG values
        
        print(f"Simulated data - Temp: {sensor_data['temp']}°C, Pulse: {sensor_data['pulse']} BPM, ECG: {sensor_data['ecg']}")
        time.sleep(2)  # Update every 2 seconds

# Start simulation in background thread
simulation_thread = threading.Thread(target=simulate_sensor_data, daemon=True)
simulation_thread.start()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/update-data', methods=['POST'])
def update_data():
    """This endpoint would normally receive data from ESP32"""
    try:
        sensor_data["temp"] = float(request.form.get("temp", 0))
        sensor_data["pulse"] = int(request.form.get("pulse", 0))
        sensor_data["ecg"] = int(request.form.get("ecg", 0))
        print(f"Received data - Temp: {sensor_data['temp']}°C, Pulse: {sensor_data['pulse']} BPM, ECG: {sensor_data['ecg']}")
        return "Data received", 200
    except Exception as e:
        return str(e), 400

@app.route('/sensor-data', methods=['GET'])
def get_sensor_data():
    """Frontend fetches data from this endpoint"""
    return jsonify(sensor_data)

@app.route('/test')
def test():
    """Test endpoint to check if server is running"""
    return "Flask server is running! Current data: " + str(sensor_data)

if __name__ == '__main__':
    print("Starting LiveVitals server...")
    print("Open your browser and go to: http://localhost:5000")
    print("Login with: patient1@gmail.com / 12345")
    app.run(host="0.0.0.0", port=5000, debug=True)