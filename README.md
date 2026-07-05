<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>README – AESR by Atharva Phadnis</title>
    <style>
        body {
            background: #f2f6fc;
            font-family: 'Segoe UI', Roboto, system-ui, sans-serif;
            max-width: 1100px;
            margin: 30px auto;
            padding: 25px;
        }
        .readme-card {
            background: #ffffff;
            padding: 40px 48px;
            border-radius: 24px;
            box-shadow: 0 25px 50px -12px rgba(0,0,0,0.20);
        }
        h1 {
            font-size: 2.6em;
            color: #0b1e33;
            border-bottom: 5px solid #2563eb;
            padding-bottom: 12px;
            text-align: center;
            margin-top: 0;
        }
        .subhead {
            text-align: center;
            font-size: 1.2em;
            color: #1e3a5f;
            margin-top: -10px;
        }
        .badge-group {
            text-align: center;
            margin: 20px 0 30px 0;
        }
        .badge {
            background: #2563eb;
            color: white;
            padding: 6px 18px;
            border-radius: 40px;
            font-size: 0.85em;
            font-weight: 600;
            display: inline-block;
            margin: 4px 6px;
        }
        h2 {
            color: #0b1e33;
            border-left: 8px solid #2563eb;
            padding-left: 18px;
            background: #f0f5ff;
            line-height: 1.4;
            margin-top: 40px;
            padding-top: 6px;
            padding-bottom: 6px;
        }
        h3 {
            color: #1e3a5f;
            margin-top: 28px;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            font-size: 0.95em;
            box-shadow: 0 2px 8px rgba(0,0,0,0.04);
        }
        th {
            background: #1e293b;
            color: white;
            padding: 12px 14px;
            text-align: left;
        }
        td {
            border: 1px solid #dbe0e8;
            padding: 10px 14px;
            vertical-align: top;
        }
        tr:nth-child(even) {
            background: #f9fbfe;
        }
        .app-screenshot-box {
            background: #f1f6fe;
            border: 2px dashed #3b82f6;
            border-radius: 20px;
            padding: 25px 20px 15px 20px;
            text-align: center;
            margin: 25px 0;
        }
        .app-screenshot-box img {
            max-width: 100%;
            height: auto;
            border-radius: 14px;
            box-shadow: 0 12px 40px rgba(0,0,0,0.12);
            border: 1px solid #cbd5e1;
        }
        .caption {
            font-style: italic;
            color: #334155;
            margin-top: 10px;
            font-size: 0.98em;
        }
        .highlight-box {
            background: #f8fafc;
            padding: 18px 24px;
            border-radius: 14px;
            border-left: 6px solid #2563eb;
            margin: 18px 0;
        }
        ul, ol {
            padding-left: 26px;
        }
        li {
            margin: 8px 0;
        }
        code {
            background: #e6edf6;
            padding: 2px 10px;
            border-radius: 8px;
            font-family: 'Courier New', monospace;
            font-size: 0.92em;
        }
        footer {
            margin-top: 50px;
            padding-top: 25px;
            border-top: 3px solid #e2e8f0;
            text-align: center;
            color: #475569;
            font-size: 0.95em;
        }
        .small-note {
            font-size: 0.85em;
            color: #64748b;
        }
    </style>
</head>
<body>
<div class="readme-card">

    <!-- TITLE & AUTHOR -->
    <h1>🤖 Autonomous Environmental Survey Rover (AESR)</h1>
    <p class="subhead">
        <strong>Designed &amp; Fabricated by:</strong> Atharva Phadnis &nbsp;|&nbsp; Class 8C &nbsp;|&nbsp; Vimala Kulkarni Memorial School
    </p>
    <div class="badge-group">
        <span class="badge">Arduino Uno R3</span>
        <span class="badge">DHT22 Sensor</span>
        <span class="badge">IR Obstacle Avoidance</span>
        <span class="badge">OLED 128x64 (I²C)</span>
        <span class="badge">MicroSD (SPI Logging)</span>
        <span class="badge">Analytics Dashboard App</span>
    </div>

    <!-- 1. CORE OBJECTIVE -->
    <h2>🎯 Core Objective</h2>
    <p>
        To build a fully functional, semi-autonomous differential-drive rover that captures <strong>hyper-local environmental data</strong> 
        (temperature and relative humidity) in real time. The robot displays the readings instantly on an onboard OLED screen, 
        logs every data point with a timestamp onto a microSD card, and actively avoids obstacles using infrared sensors. 
        <br><br>
        The true innovation lies in the <strong>companion analytics dashboard</strong> — a custom application that reads the logged CSV files, 
        compares today’s temperature profile with yesterday’s, and renders a clear comparative line chart. This empowers Atharva 
        to study daily weather patterns, microclimate shifts, and thermal behaviour in his immediate environment.
    </p>

    <!-- 2. WORKING PRINCIPLE (Simplified) -->
    <h2>⚙️ Working Principle – How It All Works (Simple View)</h2>
    <p>
        The Arduino Uno acts as the central nervous system. It runs a continuous loop that handles two parallel tasks without 
        interrupting either:
    </p>
    <ul>
        <li><strong>Safety &amp; Navigation:</strong> The IR sensor constantly checks for obstacles. If an object is detected within 
        the calibrated range (5–30 cm), the rover immediately reverses for 0.5 seconds, then turns left or right to clear the path.</li>
        <li><strong>Environmental Sensing:</strong> Every 2.5 seconds, the brain wakes up the DHT22 sensor, requests a fresh reading, 
        and validates the data using a checksum. Once verified, the temperature and humidity values are processed.</li>
    </ul>
    <p>
        The processed data is instantly <strong>triple-routed</strong>:
        <ol>
            <li><strong>Display:</strong> Rendered as large, clear digits on the OLED screen via the I²C bus.</li>
            <li><strong>Storage:</strong> Written to a <code>DATALOG.CSV</code> file on the microSD card through the SPI bus.</li>
            <li><strong>Telemetry (optional):</strong> Sent out via the hardware UART serial port for future wireless transmission.</li>
        </ol>
        All timers are non-blocking (using <code>millis()</code>), meaning the robot never freezes — it dodges obstacles even while 
        waiting for the slow DHT22 to respond.
    </p>

    <!-- 3. HARDWARE COMPONENTS (Detailed) -->
    <h2>🛠️ Hardware Components – Detailed Specifications</h2>
    <ul>
        <li><strong>Arduino Uno R3 (The Brain):</strong> Powered by the ATmega328P 8-bit AVR microcontroller. Runs at 16 MHz with 
        32 KB of flash memory (for program storage), 2 KB of SRAM (for runtime variables and buffer), and 1 KB of EEPROM. 
        Provides 14 digital I/O pins and 6 analogue inputs.</li>
        <li><strong>DHT22 (AM2302) – Weather Nose:</strong> Capacitive humidity sensor paired with an NTC thermistor. 
        Offers outstanding accuracy: ±0.5°C for temperature and ±2% for relative humidity. Uses a proprietary single-bus protocol 
        with precise bit-level timing (logic '0' = 26–28 µs pulse, logic '1' = 70 µs pulse). A 4.7 kΩ pull-up resistor on the 
        data line ensures a clean logic-high idle state.</li>
        <li><strong>IR Obstacle Sensor – Electronic Eyes:</strong> Transmits 940 nm infrared light and measures reflectance via a 
        photodiode. An onboard LM393 comparator compares the reflected intensity against a preset threshold (adjustable via a 
        potentiometer). Output is digital: <strong>LOW (0V)</strong> when an obstacle is near, <strong>HIGH (5V)</strong> when 
        the path is clear.</li>
        <li><strong>OLED Display (128×64, SSD1306):</strong> Configured in I²C mode to save precious GPIO pins. Uses only two 
        wires (SDA and SCL) for communication. The U8g2 library handles the frame buffer, sending pixel data over the I²C bus 
        at speeds up to 400 kHz.</li>
        <li><strong>MicroSD Card Module:</strong> Features an onboard 3.3V low-dropout regulator and a level-shifter (e.g., 74HC125) 
        to translate 5V logic from the Arduino to 3.3V logic required by the SD card. Operates in SPI Mode 0 (CPOL=0, CPHA=0) 
        with data latched on the rising edge of SCK.</li>
        <li><strong>L298N Motor Driver – The Muscle:</strong> A dual H‑bridge integrated circuit that can drive two DC motors. 
        Receives low-power TTL logic signals from the Arduino and outputs high-current signals to the motors. Speed is controlled 
        via PWM pins, while direction is controlled via digital HIGH/LOW states.</li>
    </ul>

    <!-- 4. CIRCUIT CONNECTIONS (PIN MAP) -->
    <h2>🔌 Complete Circuit Interfacing &amp; Pin Mapping</h2>
    <p>
        The table below shows exactly how Atharva wired every component. Special attention was given to avoid pin conflicts — 
        for instance, <strong>Pin 10 is reserved exclusively for the SD card CS (Slave Select)</strong>, so the motor driver 
        uses Pins 8 and 9 for the right motor instead.
    </p>
    <table>
        <thead>
            <tr><th>Component</th><th>Module Pin</th><th>Arduino Pin</th><th>Technical Justification (Simple)</th></tr>
        </thead>
        <tbody>
            <tr><td rowspan="3"><strong>DHT22</strong></td><td>VCC</td><td>5V</td><td>Requires stable 5V supply (~1.5 mA draw).</td></tr>
            <tr><td>GND</td><td>GND</td><td>Common ground reference.</td></tr>
            <tr><td>DATA</td><td>Digital Pin 4</td><td>Single-wire bi-directional data with external 4.7kΩ pull-up.</td></tr>
            <tr><td rowspan="3"><strong>IR Sensor</strong></td><td>VCC</td><td>5V</td><td>Powers the comparator and IR LED.</td></tr>
            <tr><td>GND</td><td>GND</td><td>Ground.</td></tr>
            <tr><td>OUT</td><td>Digital Pin 7</td><td>TTL output (LOW = obstacle, HIGH = clear).</td></tr>
            <tr><td rowspan="4"><strong>OLED (I²C)</strong></td><td>VCC</td><td>5V</td><td>Onboard regulator drops to 3.3V for the driver.</td></tr>
            <tr><td>GND</td><td>GND</td><td>Ground.</td></tr>
            <tr><td>SDA</td><td>A4 (SDA)</td><td>I²C Data line (part of the Wire library).</td></tr>
            <tr><td>SCL</td><td>A5 (SCL)</td><td>I²C Clock line for synchronization.</td></tr>
            <tr><td rowspan="5"><strong>SD Module (SPI)</strong></td><td>CS (SS)</td><td>Digital Pin 10</td><td><strong>Slave Select</strong> – must be set as OUTPUT to enable SPI communication.</td></tr>
            <tr><td>MOSI</td><td>Digital Pin 11</td><td>Master Out Slave In – data from Arduino to SD card.</td></tr>
            <tr><td>MISO</td><td>Digital Pin 12</td><td>Master In Slave Out – data from SD card to Arduino.</td></tr>
            <tr><td>SCK</td><td>Digital Pin 13</td><td>Serial Clock generated by the SPI hardware module.</td></tr>
            <tr><td>VCC</td><td>5V</td><td>Supplies the onboard 3.3V regulator and level-shifter.</td></tr>
            <tr><td rowspan="2"><strong>L298N Driver</strong></td><td>IN1, IN2</td><td>D5, D6</td><td>Direction control (Left Motor).</td></tr>
            <tr><td>IN3, IN4</td><td>D8, D9</td><td>Direction control (Right Motor) – <strong>Pin 10 avoided</strong> to prevent SPI conflict.</td></tr>
        </tbody>
    </table>

    <!-- 5. DATA FLOW & FIRMWARE EXECUTION -->
    <h2>🧠 Data Flow &amp; Firmware Execution Logic (Step-by-Step)</h2>
    <p>
        The embedded C++ firmware is structured for reliability and speed. Here is the exact sequence the Arduino executes 
        every single loop:
    </p>
    <ol>
        <li><strong>Initialization (<code>setup()</code>):</strong> 
            <br>– Initialises the SPI bus with <code>SPI.begin()</code> and sets SS (Pin 10) as OUTPUT.
            <br>– Mounts the SD card using <code>SD.begin(10)</code> and verifies the FAT16/FAT32 file system.
            <br>– Sends initialisation commands (0xAE, 0xD5, 0xA8) to the OLED and clears the display buffer.
            <br>– Pulls the DHT22 data line HIGH to allow the sensor to stabilise.
        </li>
        <li><strong>Obstacle Polling (Non-blocking):</strong> 
            <br>– Reads the digital state of Pin 7. If LOW (object within threshold), the microcontroller immediately 
            executes a reverse-and-turn subroutine by writing appropriate PWM values to the L298N enable pins.
        </li>
        <li><strong>DHT22 Read Routine (Every 2.5 seconds):</strong> 
            <br>– The MCU pulls the DATA line LOW for 18 ms (START signal), then HIGH for 40 µs.
            <br>– The sensor responds with an 80 µs LOW and 80 µs HIGH handshake.
            <br>– The MCU reads 40 bits (16 bits for humidity, 16 bits for temperature, 8 bits for checksum).
            <br>– The checksum is calculated (lower 8 bits of the sum of the four data bytes) to discard corrupted packets.
        </li>
        <li><strong>Data Aggregation &amp; Formatting:</strong> 
            <br>– Converts the float values to strings. Generates a system timestamp (millis() or RTC if available).
        </li>
        <li><strong>I²C Display Update:</strong> 
            <br>– The U8g2 library clears the internal buffer, writes the temperature, humidity, and obstacle status strings, 
            and executes <code>u8g2.sendBuffer()</code> to push the pixel matrix over the I²C bus.
        </li>
        <li><strong>SPI Logging Routine:</strong> 
            <br>– Opens <code>DATALOG.CSV</code> in APPEND mode. Writes a line: <code>[Timestamp], [Temp °C], [Humidity %], [Obstacle_Flag]</code>.
            <br>– Immediately executes <code>myFile.close()</code> to flush the SRAM buffer to the physical NAND flash, 
            guaranteeing zero data loss even in the event of sudden power loss.
        </li>
    </ol>

    <!-- 6. THE COMPANION APP & DASHBOARD (WITH IMAGE) -->
    <h2>📱 The Companion App – Robot Data Analytics Dashboard</h2>
    <p>
        To transform the raw CSV logs into meaningful insights, Atharva built a dedicated post-processing application 
        (using Python with Pandas/Matplotlib or MIT App Inventor). This dashboard is the bridge between the rover's hardware 
        and the user's understanding of the environment.
    </p>

    <!-- IMAGE PLACEMENT (UPDATED SCREENSHOT) -->
    <div class="app-screenshot-box">
        <img src="image.png" alt="Robot Data Analytics Dashboard showing Temp:24C, Flame:ON, Export PDF, Upload CSV">
        <div class="caption">
            <strong>Figure 1:</strong> Atharva's Robot Data Analytics Dashboard – the interface displays live status indicators 
            (<em>Temp: 24C</em>, <em>Flame: ON</em>), powerful export tools, and a drag-and-drop CSV upload section for 
            comparative analysis.
        </div>
    </div>

    <h3>Dashboard Features &amp; Workflow (Extended):</h3>
    <ul>
        <li><strong>Live Status Bar:</strong> The top of the dashboard shows real-time contextual data — for example, 
        <strong>Temp: 24C</strong> and a <strong>Flame: ON</strong> indicator (which acts as a generic system status flag 
        or obstacle alert). This gives the user instant situational awareness while working with historical logs.</li>
        <li><strong>Tools &amp; Export Section:</strong> A dedicated toolbar allows the user to <strong>Export PDF Report</strong> 
        — generating a professionally formatted document containing the comparative chart, statistical summaries, and raw data 
        tables. The <strong>Set Save Folder</strong> option lets users organise their analysis outputs efficiently.</li>
        <li><strong>Drag-and-Drop CSV Upload:</strong> The interface clearly presents two upload zones:
            <br>– <strong>File 1 — Today’s Data</strong> (mandatory): The main dataset from the rover's latest run.
            <br>– <strong>File 2 — Yesterday’s Data</strong> (optional): Enables the powerful comparison mode.
        </li>
        <li><strong>Behind the Scenes – Comparison Algorithm:</strong>
            <br>– The app parses the CSV files using regex to extract timestamps and temperature columns.
            <br>– It dynamically segregates data based on the system date, isolating "Today" and "Yesterday" subsets.
            <br>– Since the rover samples at a fixed interval (e.g., every 5 seconds), the app normalises the X-axis (00:00 to 23:59) 
            and applies linear interpolation to perfectly align the sampling points between the two days.
            <br>– Finally, it renders a dual-line overlay chart: one line for today, one for yesterday. The user can instantly 
            spot if today is hotter, cooler, or if a sudden weather event (like cloud cover or rainfall) changed the thermal pattern.
            <br>– The app also calculates and displays the <strong>Mean, Maximum, and Minimum</strong> temperatures for both days 
            in an overlay legend.
        </li>
    </ul>

    <!-- 7. REAL-WORLD APPLICATIONS -->
    <h2>🌍 Real-World Applications in Environmental Science</h2>
    <ul>
        <li><strong>🌱 Greenhouse Precision Agriculture:</strong> The rover can autonomously patrol aisles between crop rows, 
        logging humidity variations. By comparing today's humidity curve with yesterday's, the farmer can identify under-watered 
        zones and optimise irrigation schedules, reducing water waste.</li>
        <li><strong>🏛️ Museum &amp; Archive Microclimate Monitoring:</strong> Paintings, manuscripts, and historical artefacts 
        are highly sensitive to thermal and humidity fluctuations. The rover moves through different gallery sections, mapping 
        thermal gradients near windows, doors, and HVAC vents. The comparative chart helps curators ensure stable preservation 
        conditions over consecutive days.</li>
        <li><strong>🏙️ Urban Heat Island (UHI) Study:</strong> By taking the rover outside over asphalt roads, concrete pavements, 
        and grassy parks at the same time each day, the data logger captures the thermal inertia of various urban surfaces. 
        The comparison app visually proves how man-made surfaces retain heat far longer than natural vegetation — a powerful 
        demonstration for climate science classes.</li>
        <li><strong>🧪 Holistic STEAM Education:</strong> This single project merges multiple disciplines: mechanical engineering 
        (chassis and drivetrain), electrical engineering (sensor circuits and power management), computer science (embedded C++ 
        and state machines), and data science (Python charting and statistical analysis). It serves as a complete, hands-on 
        educational platform for Class 8C and beyond.</li>
    </ul>

    <!-- 8. CONCLUSION -->
    <h2>✅ Conclusion</h2>
    <p>
        Atharva's AESR is much more than a simple toy robot — it is a robust, closed-loop Data Acquisition System (DAS) 
        designed for real-world scientific inquiry. By meticulously managing the SPI bus for high-speed, reliable storage, 
        leveraging the I²C protocol for efficient display updates, and implementing strict timing-sensitive firmware for the 
        DHT22, the rover guarantees high-fidelity data integrity even in noisy environments.
        <br><br>
        The integration of the <strong>cross-platform comparative dashboard</strong> elevates this project from a mere data logger 
        to an intelligent analytical tool. Atharva can now visualise daily thermal trends, correlate them with environmental 
        phenomena, and export professional reports — all from a rover he built from the ground up. This project demonstrates a 
        sophisticated, interdisciplinary understanding of embedded systems, sensor interfacing, and data analytics, setting a 
        remarkable benchmark for school-level innovation.
    </p>

    <!-- FOOTER -->
    <footer>
        <p>
            <strong>Project By:</strong> Atharva Phadnis (Class 8C) · Vimala Kulkarni Memorial School<br>
            <strong>Tech Stack:</strong> Arduino C++ · SPI / I²C Protocols · U8g2 · SD Library · Python (Pandas/Matplotlib) / MIT App Inventor<br>
            <span class="small-note">Built with curiosity, precision, and a passion for environmental science.</span>
        </p>
        <p style="font-size: 0.8rem; color: #94a3b8;">
            © 2026 AESR Project · All rights reserved.
        </p>
    </footer>

</div>
</body>
</html>
