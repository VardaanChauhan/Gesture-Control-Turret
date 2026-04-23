# Gesture-Controlled Turret System

## Overview
This project implements a gesture-based remote turret prototype, where turret orientation is controlled using real-time hand motion data. The system translates human gestures into precise angular movement commands for a pan-tilt mechanism.

The objective is to design a low-latency, intuitive human-machine interface (HMI) applicable to defense, surveillance, and robotic targeting systems.

## Problem Statement
Traditional control systems such as joysticks and manual interfaces introduce:
- Operator fatigue  
- Limited intuitiveness  
- Slower response in dynamic environments  

This project explores gesture recognition as a control paradigm to improve:
- Reaction time  
- Control naturalness  
- Operator efficiency  

## System Architecture

The system consists of two main subsystems:

### 1. Gesture Acquisition Unit
- Motion sensor captures hand orientation  
- Data is processed into control signals (pitch and yaw)  

### 2. Turret Control Unit
- Microcontroller interprets incoming data  
- Drives servo motors for directional control  

**Data Flow:**
Hand Motion → Sensor → Microcontroller → Communication → Turret MCU → Servo Actuation

## Hardware Components
- Microcontroller (Arduino or STM32)  
- IMU Sensor (MPU6050 or equivalent)  
- Servo Motors (pan-tilt mechanism)  
- Wireless Module (NRF, Bluetooth, or RF)  
- Power Supply System  

## Software Implementation

### Embedded Layer
- Written in C++ 
- Sensor data acquisition via I2C  
- Real-time mapping of orientation to servo angles  

### Control Logic
- Angle normalization  
- Noise filtering (basic smoothing by implementing ded-zones)  
- Threshold-based motion control  

### Communication
- Done via ESP-now 
- Packetized control signals  


## Key Features
- Real-time gesture-to-motion mapping  
- Wireless control interface  
- Modular architecture (sensor and actuation separation)  
- Scalable for advanced targeting systems  


## Results
- Smooth pan-tilt control with minimal latency  
- Stable operation under continuous motion input  
- Reliable gesture tracking within operational range  

## Limitations
- Basic filtering, susceptible to noise  
- No advanced stabilization such as Kalman filtering  
- Limited range depending on communication module  

## Conclusion
This project demonstrates a functional prototype of gesture-driven control systems, bridging human intent with electromechanical actuation. It highlights key principles used in modern defense and robotic platforms.

## Disclaimer
This project is a prototype for educational and research purposes only and does not include any weaponized system.
