# IIoT_Modbus
Thiết kế mạch điều khiển và giám sát các PLC và Biến tần ở mọi nơi bằng Webserver. Ứng dụng rộng rãi trong các tủ điện công nghiệp.

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/8c29d267-6773-45c1-a03a-440000ca269c)

Giao tiếp theo nguyên tắc Master - Slave, có thể kết nối nhiều thiết bị có truyền thông Modbus:

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/8df259fc-dbf7-4f42-87de-432ed6275dba)

Bản thiết kế PCB:

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/6e8000f1-acc6-47e7-84fc-f311c2b2836f)

-	Hoạt động ổn định, đảm bảo an toàn trong quá trình sử dụng thiết bị một cách thường xuyên.
-	Khả năng chống nhiễu tốt, đảm bảo cho một quá trình hoạt động lâu dài.
-	Điện áp hoạt động: 220VAC
-	Công xuất tối đa: 20W
-	3 ngõ vào 12VDC
-	2 ngõ ra relay 5A/250VAC

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/d1bc67c0-88c1-437a-9742-ebb6b417dcf2)

1.	Nguồn AC-DC 220VAC sang 12VDC
2.	Nguồn output 5VDC
3.	Module ESP32
4.	Modbus RS-485
5.	Nút nhấn reset wifi
6.	Nút nhấn reset board
7.	Nguồn cách ly 12V
8.	Led nguồn 
9.	Led trạng thái RS-485
10.	Led trạng thái RS-485
11.	Led trạng thái wifi
12.	Terminal nguồn cấp 220VAC
13.	Terminal modbus RS-485
14.	Terminal output relay
15.	Terminal input 
16.	Output relay cuộn coil 12V/5A
17.	Cảm biến nhiệt độ và độ ẩm
18.	Led trạng thái của mạch

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/f72842d6-6355-46ab-a94d-64eb0ac88cb4)

![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/1fcdf068-0179-401d-9db9-0e08a5d765b7)

Có thể kết nối với Webserver ở cả Global và máy Local:
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/680274ee-4d50-4835-b031-d4c809d43afe)
- Giao diện điều khiển và giám sát trạng thái của board:
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/c7793e3f-0eb4-4444-b0c0-4be6557f1d4f)
- Giao diện điều khiển và giám sát PLC:
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/82e4dd94-2326-4fc1-a580-2ba61411b188)
- Giao diện điều khiển và giám sát Biến Tần:
![image](https://github.com/thanhtam18/IIoT_Modbus/assets/140053842/1f5d84d9-514b-4dfd-b739-66bb789f3637)

Hướng phát triển:
- Phát triển thêm các bộ điều khiển phụ không sử dụng WiFi mà thay vào đó là sử dụng sóng Lora với khoảng cách truyền lên đến 10km hoạt động tốt trong công nghiệp để quản lí thêm nhiều trạm, nhiều thiết bị khác.
- Thiết kế thêm các phần mềm điều khiển cho android và IOS, dữ liệu giữa các phần mềm và webserver đồng bộ với nhau.
- Thiết kế thêm phần cứng sử dụng nguồn dự phòng khi có sự cố mất điện.
- Nâng cấp độ ổn định, tiết kiệm năng lượng, tối ưu hoá chi phí.
