# IIoT_Modbus
Design control circuits and monitor PLCs and Inverters anywhere using Webserver. Widely used in industrial electrical cabinets.

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/8c29d267-6773-45c1-a03a-440000ca269c)

Communication according to the Master-Slave principle, can connect many devices with Modbus communication:

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/8df259fc-dbf7-4f42-87de-432ed6275dba)

# Design PCB:

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/6e8000f1-acc6-47e7-84fc-f311c2b2836f)

- Stable operation, ensuring safety during regular use of the device.
- Good anti-interference ability, ensuring long-term operation.
- Operating voltage: 220VAC
- Maximum output: 20W
- 3 12VDC inputs
- 2 relay outputs 5A/250VAC

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/d1bc67c0-88c1-437a-9742-ebb6b417dcf2)

1.	Adapter AC-DC 220VAC to 12VDC
2.	Power output 5VDC
3.	Module ESP32
4.	Modbus RS-485
5.	Button reset wifi
6.	Button reset board
7.	Power 12V
8.	Led Power 
9.	Led status RS-485
10.	Led status RS-485
11.	Led trạng thái wifi
12.	Terminal Power supply 220VAC
13.	Terminal modbus RS-485
14.	Terminal output relay
15.	Terminal input 
16.	Output relay 12V/5A
17.	Sensor temperature and humidity
18.	Led board's status

# Connection
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/f72842d6-6355-46ab-a94d-64eb0ac88cb4)

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/1fcdf068-0179-401d-9db9-0e08a5d765b7)

# Webserver
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/680274ee-4d50-4835-b031-d4c809d43afe)
- Control interface and monitor the board's status:
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/c7793e3f-0eb4-4444-b0c0-4be6557f1d4f)
- PLC control and monitoring interface:
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/82e4dd94-2326-4fc1-a580-2ba61411b188)
- Inverter control and monitoring interface:
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/1f5d84d9-514b-4dfd-b739-66bb789f3637)

# Development 
- Develop more controllers that do not use WiFi but instead use LoRa waves with a transmission distance of up to 10km that works well in industry to manage more stations and other devices.
- Additional design of control software for Android and iOS, data between software and web server together.
- Design additional hardware to use backup power when there is a power outage.
- Upgrade stability, save energy, and optimize costs.
