# 🌿 Planto: Smart Plant Emotion Monitor

**A fusion of IoT, AI, and Fuzzy Logic to bring your plant's feelings to life.**

---

## 📌 Project Overview

Planto is an intelligent plant monitoring system designed to track environmental conditions and reflect a plant's emotional state — both physically through a device and digitally through a web app. It combines sensors, machine learning, fuzzy logic, and a visually intuitive frontend to provide plant parents with meaningful insights about their plant’s wellbeing.

---

## 🔗 GitHub Repository

Access our full project code here: https://github.com/pranay-vohra/Planto-by-InnoVibe

---

## 🔧 Hardware Components (IIoT)

Our IoT setup includes:

### 1. ESP32 Microcontroller
Acts as the central processing unit, reading sensor values and sending them to the backend server and OLED.

### 2. Sensors Used
- 🌱 **Soil Moisture Sensor** – Tracks water levels in soil  
- 🌡 **DHT Sensor** – Measures temperature and humidity  
- 💡 **LDR (Light Sensor)** – Monitors light exposure  
- 🌫 **MQ135 Air Quality Sensor** – Gauges the freshness of the environment

### 3. OLED Display
Real-time emotion feedback — "Happy", "Thirsty", or "Unhealthy" — shown via a 0.96” OLED screen directly attached to the plant pot.

### 4. Backend Server
A Python Flask server receives data via HTTP POST, evaluates it using:
- A trained ML model
- Fallback rule-based logic when ML confidence is low

---

## 🌐 Web Application Interface (React + TailwindCSS)

The frontend is developed using **React.js** with **TailwindCSS** for styling. It's a smart, visually engaging dashboard that continues the plant's emotional journey online.

### 🖼 User Interface Features

#### 🔐 **Login / Register Page**
- When the user first visits the app, they're greeted with a **Register Your Plant** form.
- Fields include:
  - 🌿 Plant Name
  - 📱 Contact Info (email/phone)
  - 📶 Wi-Fi Name
- A large, translucent background image of a plant adds aesthetic appeal.
- The form is centered, clean, and mobile-responsive.

#### 🌱 **Mood Display Section**
- A central **slider** (range 0–100) shows the current **mood level** of the plant.
- Based on slider value, dynamic **emojis** appear:
  - 🥲 (0–30): Sad
  - 😊 (31–70): Okay
  - 🥳 (71–100): Happy
- An **alert message** like _"🌵 I'm thirsty!"_ appears when the plant’s mood is poor.

#### 📊 **Dashboard Buttons**
Two action buttons appear below the mood slider:
1. **Detailed Statistics**  
   Shows sensor-based information such as:
   - Moisture Level
   - Temperature
   - Light Exposure
   - Air Quality

2. **Graph View**  
   Visualizes time-series data of plant health using interactive graphs (e.g., line/bar charts).

#### 🌿 **Aesthetic & UX**
- Translucent plant-themed backgrounds
- Rounded corners, soft animations, hover effects
- Fully mobile responsive for on-the-go plant care

---

## 🧠 Backend Logic with Fuzzy Emotion Mapping

The web app refines the basic emotion received from the ESP32 by using fuzzy logic algorithms:

- Evaluates overlapping sensor ranges instead of fixed thresholds
- Labels emotions as:
  - *Slightly Stressed*
  - *Joyful*
  - *Overstimulated*, etc.
- Visualized through historical mood charts and improvement suggestions

---

## 🔄 Complete Workflow

1. Sensors collect environment data
2. ESP32 sends it to Flask server
3. Server responds with an emotion (ML or fallback)
4. OLED displays emotion
5. Simultaneously, data is sent to web app
6. Web app uses fuzzy logic to analyze, visualize, and refine the plant’s emotional state

---

## 🎯 Why This Matters

Planto is more than a dashboard — it's an interactive plant companion. By blending real-time monitoring with intuitive emotional feedback, we’re helping users understand and bond with their plants like never before.

---

## 💡 Skills Used

- Microcontroller Programming (ESP32)
- Sensor Interfacing & Calibration
- Python Flask Backend
- Machine Learning & Rule-Based Systems
- React Frontend Development
- Fuzzy Logic Modeling
- RESTful API Communication
- Data Visualization

---
📁 Project Demonstration Media
To see Planto in action, including:

🌿 A video demonstration of the working model (hardware + OLED + plant emotions)

🖼️ Screenshots of the web app UI (form, mood slider, dashboard)

👉 Access our Google Drive Folder here:
https://drive.google.com/drive/u/0/folders/1s8tRByi5coMm8M2Gj8RS5cyJLZbfL8Sr

This media gives a full visual overview of how our system works and what users can expect in real-time.

## ✅ Conclusion

This project demonstrates the powerful synergy of IoT, AI, fuzzy systems, and UI design. Planto makes plant care more thoughtful, expressive, and connected — a leap toward emotionally-aware smart environments.

---

