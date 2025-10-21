# ESP32 LDR Web Monitor

A simple ESP32 project that reads light intensity from an LDR sensor and displays the data on a web interface in real time.

## 🔧 Features
- Reads analog values from an LDR (photoresistor)
- Serves a local web page using the ESP32's internal web server
- Displays real-time light intensity data
- Built with Arduino IDE

## 🧰 Hardware
- ESP32 Dev Module  
- LDR (photoresistor)  
- 100kΩ potenciometer to calibrate (voltage divider)

## 💻 Software
- Arduino IDE  
- ESP32 Core (version 3.0.7 recommended)

## ⚙️ Setup
1. Connect the LDR and potenciometer(pull down) as a voltage divider to an analog pin (e.g., GPIO 32).  
2. Open the project in Arduino IDE.  
3. Select your ESP32 board and port.  
4. Upload the sketch.  
5. Open the Serial Monitor to get the IP address.  
6. Access that IP in your browser to view the web interface.

## 📸 Preview
<img width="1364" height="598" alt="image" src="https://github.com/user-attachments/assets/bd98d48d-63da-499f-a655-eeb37b0cb7ed" />
<img width="1358" height="599" alt="image" src="https://github.com/user-attachments/assets/badea2af-65cb-4a54-8a93-95e1132874a0" />
<img width="1363" height="595" alt="image" src="https://github.com/user-attachments/assets/adfdc60f-9df4-4858-9f90-625017dd5aa1" />




## 📄 License
MIT License
