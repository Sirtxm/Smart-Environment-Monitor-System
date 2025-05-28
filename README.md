<h1>Smart Environment Monitor</h1><br>
This Arduino-based project is a multifunctional environmental monitoring system capable of measuring temperature, humidity, and distance using various sensors. It features an LCD display for real-time data output and a servo motor that reacts based on object proximity. Users can toggle between display modes using a button with hardware interrupt support.<br>
<br>
<h2>🧩 Features</h2><br>
1.🌡️ Temperature and Humidity Monitoring using the DHT11 sensor. <br>
2.📏 Ultrasonic Distance Measurement with real-time display.<br>
3.⚙️ Servo Motor Control that responds when an object is detected within a certain range.<br>
4.📟 16x2 LCD I2C Display for visual feedback.<br>
5.🔘 Interrupt-driven Button for toggling between display modes.<br>
6.👁️ PIR Motion Sensor Support (declared but not yet implemented in logic).<br><br>

<h2>🛠 Components Used</h2>
Component	Description<br>
-Arduino Uno/Nano	Microcontroller board <br>
-DHT11	Temperature & humidity sensor<br>
-HC-SR04	Ultrasonic distance sensor<br>
-SG90 Servo Motor	For reactive control<br>
-LCD 16x2 I2C Display	Visual display output<br>
-Push Button	For mode switching<br>
-PIR Motion Sensor	(Reserved for future use)<br>
-Breadboard & Wires	For circuit assembly<br><br>

<h2>🔄 Operating Modes</h2>
MEASURE Mode<br>
1.Reads and displays humidity and temperature every second.<br>
2.Output shown on LCD and Serial Monitor.<br>
3.DISTANCE Mode<br>
4.Measures the distance using an ultrasonic sensor.<br>
5.If distance < 10 cm, servo turns to 0°; otherwise, it remains at 90°.<br>
6.Distance is shown on LCD.<br>

👉 You can toggle between these two modes using the push button (RISING edge interrupt).

