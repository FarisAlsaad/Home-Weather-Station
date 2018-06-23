# Project: Home Weather Station

This is a DIY project that helps you build a weather station at home. It consists of two stations: The one with an LED display to show the temperature (C or F) and that other one takes measurements and sends it to the first station as well as uploading that information to a local web server though WiFi (Potentionally world-wide if you want to open a port in your router).

### Materials Needed:

 - **Arduino *Uno***
 - **NodeMCU**
 - 2 x **nRF24L01+**
 - **Breadboard** for 1st station (optional for 2nd station)
 - **DHT** Sensor
 - **7-Segment** LED Display
 - **8-Bit Shift Register** (74HC595)
 - **Push-Button** (*Breadboard friendly*)
 - **1** **k**$\Omega$ resistor

### Code Files:
-[Arduino_Temp.c](https://github.com/FarisAlsaad/Home-Weather-Station/blob/master/Arduino_Temp.c "Arduino_Temp.c"): The code to be uploaded on the Arduino Uno.

-[NodeMCU_Temp.c](https://github.com/FarisAlsaad/Home-Weather-Station/blob/master/NodeMCU_Temp.c "NodeMCU_Temp.c"): The code to be uploaded on the NodeMCU.

*Note: Some libraries might need to be installed on the Arduino IDE.

### Workflow:
Once the NodeMCU starts, it will start the local webserver displaying its IP on the Serial monitor. This IP can be typed on any web browser on the same router and it will display a welcoming message with the Temperature readings displayed. While it does that, it listens through the *nRF24L01* module for any signal comming from the Arduino's *nRF24L01*. Once it gets that request signal, it responds with the temperature reading. When the Arduino starts, it sends a request signal to the NodeMCU then listens for a reply. Once it gets that replay signal, it reads the temperature and loads it on the LED display.
### Hardware Schematics:

**Arduino:**
This is the setup for the Arduino station. 

![Arduino](https://github.com/FarisAlsaad/Home-Weather-Station/raw/master/Arduino_Circuit.png)

**NodeMCU:** 
For this setup, if you have Female-to-Female wires then you can connect the pins directly without needing a breadboard.

![NodeMCU_Circuit](https://github.com/FarisAlsaad/Home-Weather-Station/raw/master/NodeMCU_Circuit.png)

**7-Segment Display:**
This is to help visualize the connections between the LED's and the code.
![7-Seg Reference](https://github.com/FarisAlsaad/Home-Weather-Station/raw/master/7-Seg%20Reference.png)


