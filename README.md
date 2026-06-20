# 🤖 GenoCube PRO — AI-Powered Smart Robot (ESP32 Based)

<p align="center">
  <b>An AI-powered robotic platform built using ESP32, combining robotics, IoT, intelligent behavior, emotion simulation, and smart interaction.</b>
</p>

---

# 📖 Overview

**GenoCube PRO** is a fully custom-built AI-powered smart robotic platform built using **ESP32**, combining multiple advanced technologies into one intelligent machine.

It integrates:

* **Robotics**
* **IoT**
* **AI Behavior**
* **Emotion Simulation**
* **Obstacle Detection**
* **Visual Expressions**
* **Audio Response**
* **Web-Based Remote Control**

---

> GenoCube PRO is **not just a simple obstacle-avoiding robot car**.

It is designed as an intelligent robotic system featuring:

* 🤖 Autonomous Movement
* 👤 Human Follow Mode
* 😊 Smart Emotion System
* 📡 Sensor Fusion
* 🌈 LED Animations
* 👀 OLED Robotic Eyes
* 🔊 Sound Response
* 🧠 AI Decision Making
* 📈 AI Learning Behavior
* 🌐 Web Control Dashboard
* 🎤 Voice Command Support

---

## 🎯 Project Goal

The goal of this project was to build a robot that behaves more like a **living intelligent machine** rather than just a programmable RC robot.

---

# 🚀 Core Features

---

# 1️⃣ ESP32 Brain

The robot is fully powered by **ESP32**, which acts as the central brain of the system.

### ESP32 Handles:

* Sensor Reading
* AI Decision Making
* Motor Control
* Web Server
* Wi-Fi Communication
* LED Animations
* Emotion Engine
* Voice Command Interface

### Why ESP32?

* Powerful Dual-Core MCU
* Built-in Wi-Fi
* High Processing Speed
* Great for Real-Time Robotics

---

# 2️⃣ Autonomous Navigation (AI Auto Mode)

GenoCube PRO can move autonomously without manual control.

It continuously scans surroundings and intelligently decides:

* Move Forward
* Turn Left
* Turn Right
* Move Backward
* Stop

### Decision Making Depends On:

* Front Laser Distance Sensor
* Rear Ultrasonic Sensor
* Previous Movement Performance
* AI Scoring System

This creates **semi-intelligent navigation**.

---

# 3️⃣ Manual Mode

The robot can also be manually controlled using a **web-based control dashboard**.

### Available Controls:

* Forward
* Backward
* Left
* Right
* Stop

Manual mode provides full direct control.

---

# 4️⃣ Follow Me Mode

GenoCube PRO includes a **Human Follow Mode**.

In this mode, the robot attempts to follow a target based on:

* Bluetooth / Wi-Fi Signal Strength
* Distance Tracking
* Signal Quality Changes

### Main Goals:

* Move closer when signal weakens
* Maintain safe distance
* Stop when target is near

This enables smart follow behavior.

---

# 5️⃣ Dual Sensor Fusion System

One of the most advanced systems in GenoCube PRO.

Instead of using only one sensor, the robot uses multiple sensors together for better environmental awareness.

---

## Front Sensor: VL53L0X Laser Distance Sensor

Used as the robot’s **front eye**.

### Features:

* High Accuracy
* Fast Response
* Laser-Based Measurement
* Millimeter Precision

### Purpose:

* Detect Front Obstacles
* Measure Exact Forward Distance
* Support AI Navigation

---

## Rear Sensor: HC-SR04 Ultrasonic Sensor

Used as the robot’s **rear eye**.

### Features:

* Detects Rear Obstacles
* Useful During Reverse Movement
* Improves Navigation Safety

### Purpose:

* Prevent Collision While Moving Backward
* Improve Obstacle Avoidance

---

## Why Sensor Fusion?

Using both sensors improves:

* Accuracy
* Safety
* Reliability

**Laser Sensor → Front Vision**
**Ultrasonic Sensor → Rear Vision**

This creates near **360° obstacle awareness**.

---

# 6️⃣ Smart Emotion Engine

GenoCube PRO includes a dynamic emotion system.

Emotion changes based on:

* Distance from Objects
* Sound Intensity
* Activity Level
* Environmental Interaction

### Available Emotions:

* Happy 😊
* Curious 🤔
* Angry 😠
* Sleepy 😴
* Excited 🤩
* Sad 😢
* Scared 😨
* Love ❤️
* Party 🎉
* Focused 🧠
* Bored 😑

This makes the robot feel alive.

---

# 7️⃣ OLED Robotic Eyes

GenoCube PRO includes expressive robotic eyes using an OLED display.

### Powered By:

* SH1106 OLED
* RoboEyes Library

### Features:

* Eye Movement
* Blinking
* Emotional Expressions
* Idle Movement

### Eye Positions:

* Left
* Right
* Up
* Down
* Center

The eyes react dynamically based on robot emotion.

### Examples:

* Angry → Sharp Glare
* Sleepy → Tired Eyes
* Scared → Wide Upward Look

This creates a highly expressive robotic face.

---

# 8️⃣ 8-LED Emotion Ring

GenoCube PRO includes an **8-LED addressable emotion ring** for visual emotional feedback.

### Purpose:

* Emotion Visualization
* Ambient Lighting
* Behavior Feedback

### Emotion Colors:

* Happy → Green 🟢
* Angry → Red 🔴
* Curious → Blue 🔵
* Love → Pink 💗
* Focused → Yellow 🟡
* Bored → Orange 🟠

This provides instant emotional feedback and enhances interaction.

---

# 9️⃣ 64-LED Matrix Display

GenoCube PRO includes an **8×8 LED matrix display (64 LEDs)** for advanced visual effects.

### Display Modes:

* 🎵 Music Visualizer
* 🌊 Wave Animation
* 🔥 Fire Animation

This greatly enhances the robot’s visual aesthetics.

---

## 🌊 Matrix Wave Mode

Generates animated wave effects using mathematical sine and cosine calculations.

### Features:

* Smooth Motion
* Dynamic Flow
* Natural Wave Effect

---

## 🔥 Matrix Fire Mode

Simulates realistic fire using a heat-based algorithm.

### Fire Engine Features:

* Cooling
* Heat Rise
* Sparks
* Heat-to-Color Mapping

Creates realistic fire animation effects.

---

# 🔟 Sound Detection System

GenoCube PRO includes a microphone input system.

### Purpose:

* Detect Surrounding Sound
* React to Voice / Noise
* Trigger Dynamic Behavior

### Sound Affects:

* Emotions
* Dance Mode
* LED Visualizer

### Examples:

* Loud Sound → Excited Mode 🤩
* Very Loud Sound → Party Mode 🎉

This makes the robot more interactive with its environment.

---

# 1️⃣1️⃣ Dance Mode

The robot can dance in response to sound or music.

When music is detected, the robot performs movement patterns such as:

* Move Left
* Move Right
* Move Forward
* Move Backward

Robot dances with beat-based motion.

### Emotion Automatically Changes To:

**Excited 🤩**

This creates fun and entertaining interaction.

---

# 1️⃣2️⃣ AI Decision Engine

GenoCube PRO contains intelligent decision-making logic.

The AI continuously decides the best possible action in real time.

### Possible Decisions:

* Forward
* Left
* Right
* Backward
* Stop

### Decision Depends On:

* Front Distance
* Rear Distance
* Sound
* Learned Scores

This acts as the robot’s intelligence core.

---

# 1️⃣3️⃣ AI Learning System (Score-Based Learning)

GenoCube PRO includes a basic adaptive learning engine.

Each movement has its own score:

* Forward Score
* Left Score
* Right Score
* Backward Score
* Stop Score

---

## 📈 How Learning Works

The robot evaluates movement effectiveness.

### If movement improves path:

**Reward movement**

### If movement worsens path:

**Penalize movement**

### Example:

* Forward improves path → **+1 Score**
* Wrong turn → **-2 Score**

Over time, the robot learns:

* Which movement is better
* Which direction is safer

This creates adaptive behavior.

---

# 1️⃣4️⃣ Smooth Motor Acceleration

GenoCube PRO supports smooth acceleration control.

Instead of instant speed changes:

* Speed Gradually Increases
* Speed Gradually Decreases

### Benefits:

* Smoother Motion
* Reduced Jerk
* Better Motor Life
* Better Control

This improves overall movement quality.

---

# 1️⃣5️⃣ Motor Driver System

GenoCube PRO uses the **TB6612FNG Motor Driver** for motor control.

### Controls:

* Two DC Gear Motors
* PWM Speed Control
* Direction Control

### Pins:

* `AIN1 / AIN2`
* `BIN1 / BIN2`
* `PWMA / PWMB`
* `STBY`

This allows precise movement control.

---

# 1️⃣6️⃣ Wi-Fi Control Dashboard

GenoCube PRO hosts its own Wi-Fi access point.

### Example:

* **SSID:** GenoCube
* **Password:** 12345678

The phone connects directly to the robot.

ESP32 runs an onboard web server for communication.

---

# 1️⃣7️⃣ Web Control Interface

The robot includes a custom web-based control dashboard.

### Features:

* Mode Switching
* Movement Control
* LED Control
* Matrix Control
* Dance Control
* Voice Command Support

Accessible directly from browser.

> No app required.

---

# 1️⃣8️⃣ Voice Command Control

GenoCube PRO supports browser-based voice commands.

### Supported Commands:

* Forward
* Left
* Right
* Stop
* Manual
* Auto
* Wave
* Rainbow
* Fire
* Music ON
* Music OFF

Voice recognition is handled through the browser.

---

# 1️⃣9️⃣ Smart Rest Mode

GenoCube PRO includes an intelligent rest mode.

If active for too long:

* Robot Stops Moving
* Emotion Becomes Sleepy 😴

After rest:

* Robot resumes normal behavior

This creates realistic life-like behavior.

---

# 2️⃣0️⃣ Boot Animation

The robot plays a startup animation during power-on.

### Features:

* LED Boot Effect
* System Initialization
* Sensor Startup

This gives the robot a premium startup feel.

---

# 2️⃣1️⃣ Emotion + Motion Synchronization

One of the most advanced features of GenoCube PRO.

Robot behavior synchronizes:

* Movement
* Emotion
* Eyes
* LEDs
* Sound Response

Everything works together in real time.

### Example: Obstacle Too Close

* Emotion → Scared 😨
* Eyes → Look Up
* LEDs → Purple
* Movement → Stop / Retreat

This creates fully synchronized robotic behavior.

---

# 🔧 Hardware Used

* ESP32
* SH1106 OLED Display
* VL53L0X Laser Sensor
* HC-SR04 Ultrasonic Sensor
* TB6612FNG Motor Driver
* DC Gear Motors
* Microphone Sensor
* 8 NeoPixel LEDs
* 8×8 LED Matrix
* Battery Pack
* Chassis

## 🔌 Circuit Diagram

![Circuit Diagram](circuit.png)

---

# 💻 Software Stack

* Arduino IDE
* C++
* ESP32 Framework
* WiFi Library
* WebServer Library
* FastLED
* Adafruit GFX
* Adafruit SH110X
* Adafruit VL53L0X
* RoboEyes Library

---

# 🔮 Future Upgrades

Planned future improvements:

* Better Reinforcement Learning
* Bluetooth Tracking
* Voice Assistant Integration
* Mobile App
* Computer Vision
* Face Recognition
* Gesture Control
* Object Recognition
* Full AI Brain
* Cloud Connectivity

---

# 🎯 Project Vision

The vision behind GenoCube PRO is to build a smart robotic companion capable of:

* Understanding Surroundings
* Learning from Movement
* Reacting Emotionally
* Following Humans
* Making Intelligent Decisions

This project combines:

* Robotics
* Embedded Systems
* IoT
* AI Logic
* Human-Machine Interaction

into a single intelligent robotic platform.

---

# ✅ Final Result

GenoCube PRO is a highly advanced ESP32-based AI robot featuring:

* ✅ Autonomous Navigation
* ✅ Sensor Fusion
* ✅ Human Follow Mode
* ✅ AI Learning
* ✅ Smart Emotions
* ✅ OLED Robotic Eyes
* ✅ Sound Interaction
* ✅ LED Animations
* ✅ Voice Commands
* ✅ Wi-Fi Control Dashboard

---

> **This is not just a robot.**
> **It is an evolving intelligent machine.** 🤖
