<img width="807" height="450" alt="image" src="https://github.com/user-attachments/assets/f9d3e769-16cb-4471-aeaa-a3d0955d4b14" />
PROJECT: AUTONOMOUS ENVIRONMENTAL SURVEY ROVER (AESR)<br>
Designed & Fabricated by: Atharva Phadnis<br>
Class: 8C<br>
School: Vimala Kulkarni Memorial School<br>
<br>
<br>
================================================================================<br>
CORE OBJECTIVE<br>
================================================================================<br>


To build a fully functional, semi-autonomous differential-drive rover that captures hyper-local environmental data (temperature and relative humidity) in real time. The robot displays the readings instantly on an onboard OLED screen, logs every data point with a timestamp onto a microSD card, and actively avoids obstacles using infrared sensors.<br>
<br>
The true innovation lies in the companion analytics dashboard — a custom application that reads the logged CSV files, compares today's temperature profile with yesterday's, and renders a clear comparative line chart. This empowers Atharva to study daily weather patterns, microclimate shifts, and thermal behaviour in his immediate environment.<br>
<br>
<br>
================================================================================<br>
WORKING PRINCIPLE – HOW IT ALL WORKS (SIMPLE VIEW)<br>
================================================================================<br>
The Arduino Uno acts as the central nervous system. It runs a continuous loop that handles two parallel tasks without interrupting either:<br>
<br>
* Safety & Navigation: The IR sensor constantly checks for obstacles. If an object is detected within the calibrated range (5–30 cm), the rover immediately reverses for 0.5 seconds, then turns left or right to clear the path.<br>
<br>
* Environmental Sensing: Every 2.5 seconds, the brain wakes up the DHT22 sensor, requests a fresh reading, and validates the data using a checksum. Once verified, the temperature and humidity values are processed.<br>
<br>
The processed data is instantly triple-routed:<br>
1. Display: Rendered as large, clear digits on the OLED screen via the I²C bus.<br>
2. Storage: Written to a DATALOG.CSV file on the microSD card through the SPI bus.<br>
3. Telemetry (optional): Sent out via the hardware UART serial port for future wireless transmission.<br>
<br>
All timers are non-blocking (using millis()), meaning the robot never freezes — it dodges obstacles even while waiting for the slow DHT22 to respond.<br>
<br>
<br>
================================================================================<br>
HARDWARE COMPONENTS – DETAILED SPECIFICATIONS<br>
================================================================================<br>
1. Arduino Uno R3 (The Brain)<br>
   Powered by the ATmega328P 8-bit AVR microcontroller. Runs at 16 MHz with 32 KB of flash memory (for program storage), 2 KB of SRAM (for runtime variables and buffer), and 1 KB of EEPROM. Provides 14 digital I/O pins and 6 analogue inputs.<br>
<br>
2. DHT22 (AM2302) – Weather Nose<br>
   Capacitive humidity sensor paired with an NTC thermistor. Offers outstanding accuracy: ±0.5°C for temperature and ±2% for relative humidity. Uses a proprietary single-bus protocol with precise bit-level timing (logic '0' = 26–28 µs pulse, logic '1' = 70 µs pulse). A 4.7 kΩ pull-up resistor on the data line ensures a clean logic-high idle state.<br>
<br>
3. IR Obstacle Sensor – Electronic Eyes<br>
   Transmits 940 nm infrared light and measures reflectance via a photodiode. An onboard LM393 comparator compares the reflected intensity against a preset threshold (adjustable via a potentiometer). Output is digital: LOW (0V) when an obstacle is near, HIGH (5V) when the path is clear.<br>
<br>
4. OLED Display (128×64, SSD1306)<br>
   Configured in I²C mode to save precious GPIO pins. Uses only two wires (SDA and SCL) for communication. The U8g2 library handles the frame buffer, sending pixel data over the I²C bus at speeds up to 400 kHz.<br>
<br>
5. MicroSD Card Module<br>
   Features an onboard 3.3V low-dropout regulator and a level-shifter (e.g., 74HC125) to translate 5V logic from the Arduino to 3.3V logic required by the SD card. Operates in SPI Mode 0 (CPOL=0, CPHA=0) with data latched on the rising edge of SCK.<br>
<br>
6. L298N Motor Driver – The Muscle<br>
   A dual H‑bridge integrated circuit that can drive two DC motors. Receives low-power TTL logic signals from the Arduino and outputs high-current signals to the motors. Speed is controlled via PWM pins, while direction is controlled via digital HIGH/LOW states.<br>
<br>
<br>
================================================================================<br>
COMPLETE CIRCUIT INTERFACING & PIN MAPPING<br>
================================================================================<br>
The following connections were made exactly as listed. Special attention was given to avoid pin conflicts — Pin 10 is reserved exclusively for the SD card CS (Slave Select), so the motor driver uses Pins 8 and 9 for the right motor instead.<br>
<br>
DHT22:<br>
  VCC  -> 5V<br>
  GND  -> GND<br>
  DATA -> Digital Pin 4 (with 4.7kΩ pull-up resistor)<br>
<br>
IR Sensor:<br>
  VCC  -> 5V<br>
  GND  -> GND<br>
  OUT  -> Digital Pin 7<br>
<br>
OLED (I²C):<br>
  VCC  -> 5V<br>
  GND  -> GND<br>
  SDA  -> A4 (SDA)<br>
  SCL  -> A5 (SCL)<br>
<br>
MicroSD Module (SPI):<br>
  CS   -> Digital Pin 10 (Slave Select – must be OUTPUT)<br>
  MOSI -> Digital Pin 11<br>
  MISO -> Digital Pin 12<br>
  SCK  -> Digital Pin 13<br>
  VCC  -> 5V<br>
<br>
L298N Motor Driver:<br>
  IN1, IN2 -> D5, D6 (Left Motor direction)<br>
  IN3, IN4 -> D8, D9 (Right Motor direction – Pin 10 avoided to prevent SPI clash)<br>
<br>
<br>
================================================================================<br>
DATA FLOW & FIRMWARE EXECUTION LOGIC (STEP-BY-STEP)<br>
================================================================================<br>
The embedded C++ firmware is structured for reliability and speed. Here is the exact sequence the Arduino executes every single loop:<br>
<br>
1. Initialization (setup()):<br>
   – Initialises the SPI bus with SPI.begin() and sets SS (Pin 10) as OUTPUT.<br>
   – Mounts the SD card using SD.begin(10) and verifies the FAT16/FAT32 file system.<br>
   – Sends initialisation commands (0xAE, 0xD5, 0xA8) to the OLED and clears the display buffer.<br>
   – Pulls the DHT22 data line HIGH to allow the sensor to stabilise.<br>
<br>
2. Obstacle Polling (Non-blocking):<br>
   – Reads the digital state of Pin 7. If LOW (object within threshold), the microcontroller immediately executes a reverse-and-turn subroutine by writing appropriate PWM values to the L298N enable pins.<br>
<br>
3. DHT22 Read Routine (Every 2.5 seconds):<br>
   – The MCU pulls the DATA line LOW for 18 ms (START signal), then HIGH for 40 µs.<br>
   – The sensor responds with an 80 µs LOW and 80 µs HIGH handshake.<br>
   – The MCU reads 40 bits (16 bits for humidity, 16 bits for temperature, 8 bits for checksum).<br>
   – The checksum is calculated (lower 8 bits of the sum of the four data bytes) to discard corrupted packets.<br>
<br>
4. Data Aggregation & Formatting:<br>
   – Converts the float values to strings. Generates a system timestamp (millis() or RTC if available).<br>
<br>
5. I²C Display Update:<br>
   – The U8g2 library clears the internal buffer, writes the temperature, humidity, and obstacle status strings, and executes u8g2.sendBuffer() to push the pixel matrix over the I²C bus.<br>
<br>
6. SPI Logging Routine:<br>
   – Opens DATALOG.CSV in APPEND mode. Writes a line: [Timestamp], [Temp °C], [Humidity %], [Obstacle_Flag].<br>
   – Immediately executes myFile.close() to flush the SRAM buffer to the physical NAND flash, guaranteeing zero data loss even in the event of sudden power loss.<br>
<br>
<br>
================================================================================<br>
THE COMPANION APP – ROBOT DATA ANALYTICS DASHBOARD<br>
================================================================================<br>
To transform the raw CSV logs into meaningful insights, Atharva built a dedicated post-processing application (using Python with Pandas/Matplotlib or MIT App Inventor). This dashboard is the bridge between the rover's hardware and the user's understanding of the environment.<br>
<br>
[IMAGE: Robot Data Analytics Dashboard – showing Temp: 24C, Flame: ON, Export PDF Report, Upload CSV Files, File 1 – Today's Data, and File 2 – Yesterday's Data (optional)]<br>
<br>
Dashboard Features & Workflow (Extended):<br>
<br>
* Live Status Bar: The top of the dashboard shows real-time contextual data — for example, "Temp: 24C" and a "Flame: ON" indicator (which acts as a generic system status flag or obstacle alert). This gives the user instant situational awareness while working with historical logs.<br>
<br>
* Tools & Export Section: A dedicated toolbar allows the user to Export PDF Report — generating a professionally formatted document containing the comparative chart, statistical summaries, and raw data tables. The Set Save Folder option lets users organise their analysis outputs efficiently.<br>
<br>
* Drag-and-Drop CSV Upload: The interface clearly presents two upload zones:<br>
  – File 1 — Today's Data (mandatory): The main dataset from the rover's latest run.<br>
  – File 2 — Yesterday's Data (optional): Enables the powerful comparison mode.<br>
<br>
* Behind the Scenes – Comparison Algorithm:<br>
  – The app parses the CSV files using regex to extract timestamps and temperature columns.<br>
  – It dynamically segregates data based on the system date, isolating "Today" and "Yesterday" subsets.<br>
  – Since the rover samples at a fixed interval (e.g., every 5 seconds), the app normalises the X-axis (00:00 to 23:59) and applies linear interpolation to perfectly align the sampling points between the two days.<br>
  – Finally, it renders a dual-line overlay chart: one line for today, one for yesterday. The user can instantly spot if today is hotter, cooler, or if a sudden weather event (like cloud cover or rainfall) changed the thermal pattern.<br>
  – The app also calculates and displays the Mean, Maximum, and Minimum temperatures for both days in an overlay legend.<br>
<br>
<br>
================================================================================<br>
REAL-WORLD APPLICATIONS IN ENVIRONMENTAL SCIENCE<br>
================================================================================<br>
1. Greenhouse Precision Agriculture<br>
   The rover can autonomously patrol aisles between crop rows, logging humidity variations. By comparing today's humidity curve with yesterday's, the farmer can identify under-watered zones and optimise irrigation schedules, reducing water waste.<br>
<br>
2. Museum & Archive Microclimate Monitoring<br>
   Paintings, manuscripts, and historical artefacts are highly sensitive to thermal and humidity fluctuations. The rover moves through different gallery sections, mapping thermal gradients near windows, doors, and HVAC vents. The comparative chart helps curators ensure stable preservation conditions over consecutive days.<br>
<br>
3. Urban Heat Island (UHI) Study<br>
   By taking the rover outside over asphalt roads, concrete pavements, and grassy parks at the same time each day, the data logger captures the thermal inertia of various urban surfaces. The comparison app visually proves how man-made surfaces retain heat far longer than natural vegetation — a powerful demonstration for climate science classes.<br>
<br>
4. Holistic STEAM Education<br>
   This single project merges multiple disciplines: mechanical engineering (chassis and drivetrain), electrical engineering (sensor circuits and power management), computer science (embedded C++ and state machines), and data science (Python charting and statistical analysis). It serves as a complete, hands-on educational platform for Class 8C and beyond.<br>
<br>
<br>
================================================================================<br>
CONCLUSION<br>
================================================================================<br>
Atharva's AESR is much more than a simple toy robot — it is a robust, closed-loop Data Acquisition System (DAS) designed for real-world scientific inquiry. By meticulously managing the SPI bus for high-speed, reliable storage, leveraging the I²C protocol for efficient display updates, and implementing strict timing-sensitive firmware for the DHT22, the rover guarantees high-fidelity data integrity even in noisy environments.<br>
<br>
The integration of the cross-platform comparative dashboard elevates this project from a mere data logger to an intelligent analytical tool. Atharva can now visualise daily thermal trends, correlate them with environmental phenomena, and export professional reports — all from a rover he built from the ground up. This project demonstrates a sophisticated, interdisciplinary understanding of embedded systems, sensor interfacing, and data analytics, setting a remarkable benchmark for school-level innovation.<br>
<br>
<br>
--------------------------------------------------------------------------------<br>
Project By: Atharva Phadnis (Class 8C) · Vimala Kulkarni Memorial School<br>
Tech Stack: Arduino C++ · SPI / I²C Protocols · U8g2 · SD Library · Python (Pandas/Matplotlib) / MIT App Inventor<br>
Built with curiosity, precision, and a passion for environmental science.<br>
© 2026 AESR Project · All rights reserved.<br>
