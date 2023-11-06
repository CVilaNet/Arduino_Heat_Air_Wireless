# Arduino_Heat_Air_Wireless
Project who catch sensor values from heat and air quality then send data through wireless between arduino, and to PC using serial port.


# 17.10.2023: First Project Components 
## calor_rf_tx.ino
Arduino heat sensor and radio frequency transmision code between arduinos uno
## receptor_rf.ino
Arduino data RF reception
## rf_rx_serial_com_json.ino
Arduino RX and convert in JSon structure in order to send data through COM serial port to PC
## SerialComJson.py
Python code serial monitor and dynamic graph visualization

# 26.10.2023: Second Project Components
## calor_com_json.ino
Arduino catch values from heat sensor, convert it in a json data structure (calor_com_json arduino code folder)
## calor_com_json_heatmap.py
Receiving json data from arduino, processing and showing in heat map