EESchema Schematic File Version 4
LIBS:access_board-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L access_board-rescue:D D1
U 1 1 59BB21D5
P 9300 4550
F 0 "D1" H 9300 4650 50  0000 C CNN
F 1 "1n5819" H 9300 4450 50  0000 C CNN
F 2 "Diodes_THT:D_5W_P12.70mm_Horizontal" H 9300 4550 50  0001 C CNN
F 3 "" H 9300 4550 50  0001 C CNN
	1    9300 4550
	-1   0    0    1   
$EndComp
$Comp
L access_board-rescue:ESP-01v090 U2
U 1 1 59BB251E
P 5700 1650
F 0 "U2" H 5700 1550 50  0000 C CNN
F 1 "ESP-01v090" H 5700 1750 50  0000 C CNN
F 2 "ESP8266:ESP-01" H 5700 1650 50  0001 C CNN
F 3 "" H 5700 1650 50  0001 C CNN
	1    5700 1650
	-1   0    0    1   
$EndComp
$Comp
L access_board-rescue:+3.3V #PWR01
U 1 1 59BB291B
P 6450 2450
F 0 "#PWR01" H 6450 2300 50  0001 C CNN
F 1 "+3.3V" H 6450 2590 50  0000 C CNN
F 2 "" H 6450 2450 50  0001 C CNN
F 3 "" H 6450 2450 50  0001 C CNN
	1    6450 2450
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:CP C1
U 1 1 59BB2C05
P 7450 1650
F 0 "C1" H 7475 1750 50  0000 L CNN
F 1 "1000uF" H 7475 1550 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D13.0mm_P5.00mm" H 7488 1500 50  0001 C CNN
F 3 "" H 7450 1650 50  0001 C CNN
	1    7450 1650
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:+3.3V #PWR02
U 1 1 59BB2D04
P 7800 950
F 0 "#PWR02" H 7800 800 50  0001 C CNN
F 1 "+3.3V" H 7800 1090 50  0000 C CNN
F 2 "" H 7800 950 50  0001 C CNN
F 3 "" H 7800 950 50  0001 C CNN
	1    7800 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 1500 6850 1500
Wire Wire Line
	7800 1500 7800 950 
Connection ~ 7450 1500
$Comp
L access_board-rescue:GND #PWR03
U 1 1 59BB2D5B
P 7450 1800
F 0 "#PWR03" H 7450 1550 50  0001 C CNN
F 1 "GND" H 7450 1650 50  0000 C CNN
F 2 "" H 7450 1800 50  0001 C CNN
F 3 "" H 7450 1800 50  0001 C CNN
	1    7450 1800
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:GND #PWR04
U 1 1 59BB2D7E
P 4650 2000
F 0 "#PWR04" H 4650 1750 50  0001 C CNN
F 1 "GND" H 4650 1850 50  0000 C CNN
F 2 "" H 4650 2000 50  0001 C CNN
F 3 "" H 4650 2000 50  0001 C CNN
	1    4650 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 1700 6850 1700
Wire Wire Line
	6850 1700 6850 1500
Connection ~ 6850 1500
Wire Wire Line
	4750 1800 4650 1800
Wire Wire Line
	4650 1800 4650 2000
Text GLabel 6650 1800 2    60   Input ~ 0
RX
Text GLabel 4750 1500 0    60   Input ~ 0
TX
Text GLabel 4350 2350 2    60   Input ~ 0
TX
Text GLabel 3650 2450 2    60   Input ~ 0
RX
$Comp
L access_board-rescue:CONN_01X04 J5
U 1 1 59BEF50E
P 5100 4650
F 0 "J5" H 5100 4900 50  0000 C CNN
F 1 "CONN_01X04" V 5200 4650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 5100 4650 50  0001 C CNN
F 3 "" H 5100 4650 50  0001 C CNN
	1    5100 4650
	1    0    0    -1  
$EndComp
Text GLabel 4550 4600 0    60   Input ~ 0
TX
Text GLabel 4550 4500 0    60   Input ~ 0
RX
Wire Wire Line
	4550 4500 4900 4500
Wire Wire Line
	4550 4600 4900 4600
$Comp
L access_board-rescue:+3.3V #PWR05
U 1 1 59BEFBB2
P 3800 4600
F 0 "#PWR05" H 3800 4450 50  0001 C CNN
F 1 "+3.3V" H 3800 4740 50  0000 C CNN
F 2 "" H 3800 4600 50  0001 C CNN
F 3 "" H 3800 4600 50  0001 C CNN
	1    3800 4600
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:GND #PWR06
U 1 1 59BEFBD8
P 4850 5400
F 0 "#PWR06" H 4850 5150 50  0001 C CNN
F 1 "GND" H 4850 5250 50  0000 C CNN
F 2 "" H 4850 5400 50  0001 C CNN
F 3 "" H 4850 5400 50  0001 C CNN
	1    4850 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4900 4700 3800 4700
Wire Wire Line
	3800 4700 3800 4600
Wire Wire Line
	4900 4800 4850 4800
Wire Wire Line
	4850 4800 4850 5400
Text Notes 3950 4250 0    60   ~ 0
ESP8266 firmware flasher
$Comp
L access_board-rescue:CONN_01X03 J4
U 1 1 59BEFC59
P 4150 5300
F 0 "J4" H 4150 5500 50  0000 C CNN
F 1 "CONN_01X03" V 4250 5300 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.54mm" H 4150 5300 50  0001 C CNN
F 3 "" H 4150 5300 50  0001 C CNN
	1    4150 5300
	1    0    0    -1  
$EndComp
Text GLabel 3550 5200 0    60   Input ~ 0
GPIO0
Text GLabel 4500 1600 0    60   Input ~ 0
GPIO0
$Comp
L access_board-rescue:GND #PWR07
U 1 1 59BEFCF9
P 3500 5650
F 0 "#PWR07" H 3500 5400 50  0001 C CNN
F 1 "GND" H 3500 5500 50  0000 C CNN
F 2 "" H 3500 5650 50  0001 C CNN
F 3 "" H 3500 5650 50  0001 C CNN
	1    3500 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4500 1600 4750 1600
Wire Wire Line
	3550 5200 3950 5200
Wire Wire Line
	3500 5650 3500 5300
Wire Wire Line
	3500 5300 3950 5300
Text Notes 8500 3600 0    60   ~ 0
Connector for keypad
$Comp
L access_board-rescue:GND #PWR08
U 1 1 59BEFEAD
P 2950 3950
F 0 "#PWR08" H 2950 3700 50  0001 C CNN
F 1 "GND" H 2950 3800 50  0000 C CNN
F 2 "" H 2950 3950 50  0001 C CNN
F 3 "" H 2950 3950 50  0001 C CNN
	1    2950 3950
	1    0    0    -1  
$EndComp
Text Notes 950  5400 0    60   ~ 0
LEDs and keypad \nconnecter keypad end
$Comp
L access_board-rescue:arduino_mini U1
U 1 1 59D56A22
P 2950 2400
F 0 "U1" H 3450 1450 70  0000 C CNN
F 1 "arduino_nano" H 3700 1350 70  0000 C CNN
F 2 "arduino_nano:ArduinoNano" H 2950 2350 60  0001 C CNN
F 3 "" H 2950 2400 60  0001 C CNN
	1    2950 2400
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:LD1117S33TR U3
U 1 1 59D56ED7
P 5950 2750
F 0 "U3" H 5950 3000 50  0000 C CNN
F 1 "LD1117S33TR" H 5950 2950 50  0000 C CNN
F 2 "TO_SOT_Packages_THT:TO-220_Vertical" H 5950 2850 50  0001 C CNN
F 3 "" H 5950 2750 50  0001 C CNN
	1    5950 2750
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:GND #PWR09
U 1 1 59D5702E
P 5950 3000
F 0 "#PWR09" H 5950 2750 50  0001 C CNN
F 1 "GND" H 5950 2850 50  0000 C CNN
F 2 "" H 5950 3000 50  0001 C CNN
F 3 "" H 5950 3000 50  0001 C CNN
	1    5950 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 2700 6450 2700
Wire Wire Line
	6450 2700 6450 2450
Wire Wire Line
	5550 2700 5400 2700
Wire Wire Line
	5400 2700 5400 2450
Text GLabel 9700 3400 0    60   Input ~ 0
2
Text GLabel 9700 3500 0    60   Input ~ 0
3
Text GLabel 9700 3600 0    60   Input ~ 0
4
Text GLabel 9700 3700 0    60   Input ~ 0
5
Text GLabel 9700 3800 0    60   Input ~ 0
8
Text GLabel 9700 3900 0    60   Input ~ 0
9
Text GLabel 9700 4000 0    60   Input ~ 0
10
Text GLabel 9700 4100 0    60   Input ~ 0
11
Text GLabel 3650 3000 2    60   Input ~ 0
2
Text GLabel 3650 2900 2    60   Input ~ 0
3
Text GLabel 3650 2800 2    60   Input ~ 0
4
Text GLabel 3650 2700 2    60   Input ~ 0
5
Text GLabel 3650 2250 2    60   Input ~ 0
8
Text GLabel 3650 2150 2    60   Input ~ 0
9
Text GLabel 3650 2050 2    60   Input ~ 0
10
Text GLabel 3650 1950 2    60   Input ~ 0
11
$Comp
L access_board-rescue:CONN_01X08 J7
U 1 1 59D57E70
P 9900 3750
F 0 "J7" H 9900 4200 50  0000 C CNN
F 1 "CONN_01X08" V 10000 3750 50  0000 C CNN
F 2 "Connectors_Phoenix:PhoenixContact_MCV-G_08x3.81mm_Vertical" H 9900 3750 50  0001 C CNN
F 3 "" H 9900 3750 50  0001 C CNN
	1    9900 3750
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:CONN_01X03 J8
U 1 1 59D57F83
P 10000 4550
F 0 "J8" H 10000 4750 50  0000 C CNN
F 1 "CONN_01X03" V 10100 4550 50  0000 C CNN
F 2 "Connectors_Phoenix:PhoenixContact_MCV-G_03x3.81mm_Vertical" H 10000 4550 50  0001 C CNN
F 3 "" H 10000 4550 50  0001 C CNN
	1    10000 4550
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:R R3
U 1 1 59D5820E
P 9650 4450
F 0 "R3" V 9730 4450 50  0000 C CNN
F 1 "470" V 9650 4450 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 9580 4450 50  0001 C CNN
F 3 "" H 9650 4450 50  0001 C CNN
	1    9650 4450
	0    1    1    0   
$EndComp
$Comp
L access_board-rescue:GND #PWR010
U 1 1 59D585CA
P 9650 4800
F 0 "#PWR010" H 9650 4550 50  0001 C CNN
F 1 "GND" H 9650 4650 50  0000 C CNN
F 2 "" H 9650 4800 50  0001 C CNN
F 3 "" H 9650 4800 50  0001 C CNN
	1    9650 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 4500 8200 4550
Connection ~ 8850 4550
Wire Wire Line
	9800 4650 9650 4650
Wire Wire Line
	9650 4650 9650 4800
$Comp
L access_board-rescue:GND #PWR011
U 1 1 59D58716
P 8850 4850
F 0 "#PWR011" H 8850 4600 50  0001 C CNN
F 1 "GND" H 8850 4700 50  0000 C CNN
F 2 "" H 8850 4850 50  0001 C CNN
F 3 "" H 8850 4850 50  0001 C CNN
	1    8850 4850
	1    0    0    -1  
$EndComp
Text GLabel 9250 4250 1    60   Input ~ 0
LED
Text GLabel 3650 1850 2    60   Input ~ 0
LED
Wire Wire Line
	9500 4450 9250 4450
Wire Wire Line
	9250 4450 9250 4250
Wire Wire Line
	9800 4550 9450 4550
Wire Wire Line
	8200 4550 8850 4550
$Comp
L access_board-rescue:CP C2
U 1 1 59D5933F
P 8850 4700
F 0 "C2" H 8875 4800 50  0000 L CNN
F 1 "1000uF" H 8875 4600 50  0000 L CNN
F 2 "Capacitors_THT:CP_Radial_D13.0mm_P5.00mm" H 8888 4550 50  0001 C CNN
F 3 "" H 8850 4700 50  0001 C CNN
	1    8850 4700
	1    0    0    -1  
$EndComp
Text GLabel 2250 2200 0    60   Input ~ 0
OPEN
Text GLabel 2250 2300 0    60   Input ~ 0
Closed
Text GLabel 3650 1750 2    60   Input ~ 0
relay
Text GLabel 9550 2150 0    60   Input ~ 0
relay
$Comp
L access_board-rescue:+5V #PWR012
U 1 1 59D72015
P 8900 1300
F 0 "#PWR012" H 8900 1150 50  0001 C CNN
F 1 "+5V" H 8900 1440 50  0000 C CNN
F 2 "" H 8900 1300 50  0001 C CNN
F 3 "" H 8900 1300 50  0001 C CNN
	1    8900 1300
	1    0    0    -1  
$EndComp
Text GLabel 9550 2050 0    60   Input ~ 0
OPEN
Text GLabel 9550 1950 0    60   Input ~ 0
Closed
$Comp
L access_board-rescue:GND #PWR013
U 1 1 59D720EC
P 9000 2050
F 0 "#PWR013" H 9000 1800 50  0001 C CNN
F 1 "GND" H 9000 1900 50  0000 C CNN
F 2 "" H 9000 2050 50  0001 C CNN
F 3 "" H 9000 2050 50  0001 C CNN
	1    9000 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 1450 8900 1450
Wire Wire Line
	8900 1450 8900 1300
Text Notes 9100 1200 0    60   ~ 0
Relay and sensor connectors
$Comp
L access_board-rescue:R R2
U 1 1 59DC3D25
P 4250 2600
F 0 "R2" V 4330 2600 50  0000 C CNN
F 1 "2.2k" V 4250 2600 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 4180 2600 50  0001 C CNN
F 3 "" H 4250 2600 50  0001 C CNN
	1    4250 2600
	-1   0    0    1   
$EndComp
$Comp
L access_board-rescue:R R1
U 1 1 59DC3E75
P 4050 2350
F 0 "R1" V 4130 2350 50  0000 C CNN
F 1 "1k" V 4050 2350 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3980 2350 50  0001 C CNN
F 3 "" H 4050 2350 50  0001 C CNN
	1    4050 2350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4350 2350 4250 2350
Wire Wire Line
	3900 2350 3650 2350
Wire Wire Line
	4250 2450 4250 2350
Connection ~ 4250 2350
$Comp
L access_board-rescue:GND #PWR014
U 1 1 59DC404E
P 4250 2750
F 0 "#PWR014" H 4250 2500 50  0001 C CNN
F 1 "GND" H 4250 2600 50  0000 C CNN
F 2 "" H 4250 2750 50  0001 C CNN
F 3 "" H 4250 2750 50  0001 C CNN
	1    4250 2750
	1    0    0    -1  
$EndComp
Text GLabel 8200 4500 1    60   Input ~ 0
VIN
Text GLabel 5400 2450 1    60   Input ~ 0
VIN
Text GLabel 2800 1250 1    60   Input ~ 0
VIN
$Comp
L access_board-rescue:CONN_01X08 J6
U 1 1 59DD2646
P 9750 1800
F 0 "J6" H 9750 2250 50  0000 C CNN
F 1 "CONN_01X08" V 9850 1800 50  0000 C CNN
F 2 "Connectors_Phoenix:PhoenixContact_MCV-G_08x3.81mm_Vertical" H 9750 1800 50  0001 C CNN
F 3 "" H 9750 1800 50  0001 C CNN
	1    9750 1800
	1    0    0    -1  
$EndComp
Text GLabel 9550 1550 0    60   Input ~ 0
A2
Text GLabel 2250 2400 0    60   Input ~ 0
A2
$Comp
L access_board-rescue:CONN_01X01 J9
U 1 1 59DD5F50
P 7150 5500
F 0 "J9" H 7150 5600 50  0000 C CNN
F 1 "CONN_01X01" V 7250 5500 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_DIN965_Pad" H 7150 5500 50  0001 C CNN
F 3 "" H 7150 5500 50  0001 C CNN
	1    7150 5500
	1    0    0    -1  
$EndComp
$Comp
L access_board-rescue:CONN_01X01 J10
U 1 1 59DD5FB1
P 7650 5700
F 0 "J10" H 7650 5800 50  0000 C CNN
F 1 "CONN_01X01" V 7750 5700 50  0000 C CNN
F 2 "Mounting_Holes:MountingHole_3.2mm_M3_DIN965_Pad" H 7650 5700 50  0001 C CNN
F 3 "" H 7650 5700 50  0001 C CNN
	1    7650 5700
	1    0    0    -1  
$EndComp
Text Notes 3800 6350 0    60   ~ 0
http://www.electronic-discount.be/product-details/box-g311/shop.htm?lng=en (for enclosure)
Text Notes 3900 6500 0    60   ~ 0
http://support.seeedstudio.com/knowledgebase/articles/388503-what-are-the-pcb-panelization-rules
$Comp
L access_board-rescue:+5V #PWR?
U 1 1 59DE983C
P 2950 1250
F 0 "#PWR?" H 2950 1100 50  0001 C CNN
F 1 "+5V" H 2950 1390 50  0000 C CNN
F 2 "" H 2950 1250 50  0001 C CNN
F 3 "" H 2950 1250 50  0001 C CNN
	1    2950 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9550 1850 9000 1850
Wire Wire Line
	9000 1850 9000 2050
Wire Wire Line
	9550 1650 9550 1750
Connection ~ 9550 1750
Wire Wire Line
	7450 1500 7800 1500
Wire Wire Line
	6850 1500 7450 1500
Wire Wire Line
	8850 4550 9150 4550
Wire Wire Line
	4250 2350 4200 2350
Wire Wire Line
	9550 1750 9550 1850
$EndSCHEMATC
