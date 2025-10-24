# ❤️ Heart Tracker

A real-time **Heart Rate Monitoring System** built using **ESP32** and a **Flask web app**.  
This project allows you to continuously monitor heart rate data from an ESP32 microcontroller and visualize it on a live web dashboard.

---

## 🚀 Features

- 📡 **ESP32 Integration** – Collects heart rate data using a pulse sensor and transmits it to the Flask server.
- 🧠 **Real-Time Data Display** – View live updates of heart rate on a clean and responsive web dashboard.
- ⚙️ **Flask Backend** – REST API endpoints handle sensor data and manage communication.
- 💾 **Data Logging** – Optionally store readings for analysis or visualization.
- 🖥️ **Web Dashboard** – Simple and elegant interface built with HTML (Flask template engine).

---

## 🧩 Project Structure

heart tracker/
│
├── app.py # Flask backend (server + API)
├── templates/
│ └── index.html # Web dashboard UI
├── arduino(esp32)/
│ └── tracker.ino # ESP32 Arduino code for heart sensor


---

## ⚙️ Hardware Requirements

- 🧠 **ESP32** development board  
- 💓 **Pulse Sensor** (e.g., PulseSensor Amped or MAX30102)  
- 🔌 USB cable for flashing and power  
- 🌐 Wi-Fi connection  

---

## 💻 Software Requirements

| Component | Version / Example |
|------------|------------------|
| Python | 3.8+ |
| Flask | 2.x |
| Arduino IDE | 1.8+ or 2.x |
| ESP32 Board Package | Latest |
| Libraries | WiFi, HTTPClient, PulseSensor (Arduino) |

---

