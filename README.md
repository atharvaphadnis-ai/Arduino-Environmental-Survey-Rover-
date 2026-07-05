<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>README - Autonomous Environmental Survey Rover (AESR)</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            line-height: 1.7;
            max-width: 1000px;
            margin: 40px auto;
            padding: 20px;
            background-color: #f8fafc;
            color: #1e293b;
        }
        .container {
            background: white;
            padding: 40px;
            border-radius: 16px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.08);
        }
        h1 {
            color: #0f172a;
            border-bottom: 4px solid #3b82f6;
            padding-bottom: 10px;
            text-align: center;
            font-size: 2.2em;
        }
        h2 {
            color: #1e293b;
            margin-top: 30px;
            border-left: 6px solid #3b82f6;
            padding-left: 15px;
            background: #f1f5f9;
            line-height: 1.4;
        }
        h3 {
            color: #334155;
            margin-top: 20px;
        }
        .badge {
            background: #3b82f6;
            color: white;
            padding: 6px 14px;
            border-radius: 30px;
            font-size: 0.85em;
            display: inline-block;
            margin: 5px 0;
        }
        .highlight-box {
            background: #f1f5f9;
            padding: 20px;
            border-radius: 12px;
            border-left: 8px solid #3b82f6;
            margin: 20px 0;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
            font-size: 0.95em;
        }
        th {
            background: #1e293b;
            color: white;
            padding: 12px 15px;
            text-align: left;
        }
        td {
            border: 1px solid #e2e8f0;
            padding: 10px 15px;
            vertical-align: top;
        }
        tr:nth-child(even) {
            background: #f8fafc;
        }
        ul, ol {
            padding-left: 25px;
        }
        li {
            margin: 8px 0;
        }
        .app-screenshot {
            background: #f1f5f9;
            border-radius: 12px;
            padding: 20px;
            text-align: center;
            border: 2px dashed #94a3b8;
            margin: 20px 0;
        }
        .app-screenshot img {
            max-width: 100%;
            height: auto;
            border-radius: 8px;
            box-shadow: 0 10px 30px rgba(0,0,0,0.15);
            border: 1px solid #cbd5e1;
        }
        .caption {
            font-style: italic;
            color: #475569;
            margin-top: 8px;
            font-size: 0.95em;
        }
        footer {
            margin-top: 40px;
            padding-top: 20px;
            border-top: 2px solid #e2e8f0;
            text-align: center;
            color: #64748b;
            font-size: 0.95em;
        }
        code {
            background: #e2e8f0;
            padding: 2px 8px;
            border-radius: 6px;
            font-family: 'Courier New', monospace;
            font-size: 0.9em;
        }
    </style>
</head>
<body>
<div class="container">

    <!-- Header -->
    <h1>🤖 Autonomous Environmental Survey Rover (AESR)</h1>
    <p style="text-align: center; font-size: 1.2em; color: #475569;">
        Designed &amp; Fabricated by <strong>Atharva Phadnis</strong> · Class 8C · Vimala Kulkarni Memorial School
    </p>
    <p style="text-align: center;">
        <span class="badge">Arduino Uno R3</span>
        <span class="badge">DHT22 Sensor</span>
        <span class="badge">IR Obstacle Avoidance</span>
        <span class="badge">OLED Display</span>
        <span class="badge">MicroSD Data Logging</span>
        <span class="badge">Analytics Dashboard App</span>
    </p>

    <!-- 1. Core Objective -->
    <h2>🎯 Core Objective</h2>
    <p>
        To develop a semi-autonomous differential-drive rover capable of <strong>hyper-local environmental data acquisition</strong> 
        (temperature &amp; humidity), real-time onboard visualization via an OLED screen, and persistent time-series data storage on a microSD card. 
        The collected data is then processed by a custom-built <strong>Analytics Dashboard</strong> to compare today's temperature against yesterday's, 
        presenting the results in an easy-to-understand comparative chart.
    </p>

    <!-- 2. Working Principle (Simple Overview) -->
    <h2>⚙️ Working Principle (Simple Overview)</h2>
    <p>
        The robot has a main "brain" (Arduino Uno). It does two jobs simultaneously:
    </p>
    <ul>
        <li><strong>Job 1 – Moving Safely:</strong> It uses IR sensors (electronic eyes) to look for objects. If something is too close, the brain reverses the wheels and turns away.</li>
        <li><strong>Job 2 – Checking the Weather:</strong> It asks the DHT22 sensor for the latest temperature and humidity readings.</li>
    </ul>
    <p>
        Once the brain gets the data, it does <strong>three things</strong> instantly:
        <ol>
            <li>Shows the temperature &amp; humidity on the small OLED screen.</li>
            <li>Saves the data (with a timestamp) onto the microSD card.</li>
            <li>Sends the data via UART for potential wireless telemetry.</li>
        </ol>
        The system uses non-blocking timers, meaning the robot never stops checking for obstacles while waiting for the sensor to get ready.
    </p>

    <!-- 3. Hardware Components -->
    <h2>🛠️ Hardware Components</h2>
    <ul>
        <li><strong>Arduino Uno R3 (Brain):</strong> ATmega328P microcontroller running at 16 MHz with 32 KB Flash and 2 KB SRAM.</li>
        <li><strong>DHT22 (Weather Nose):</strong> High-accuracy capacitive sensor (±0.5°C, ±2% RH). Requires a 4.7kΩ pull-up resistor on the data line.</li>
        <li><strong>IR Obstacle Sensor (Eyes):</strong> Uses 940nm IR LED and photodiode with an LM393 comparator. Adjustable detection range (5cm – 30cm).</li>
        <li><strong>OLED Display (Screen):</strong> 128x64 pixels, SSD1306 driver, communicates via I²C protocol (uses only 2 wires).</li>
        <li><strong>MicroSD Card Module (Memory):</strong> SPI interface with onboard 3.3V regulator and level-shifting for 5V logic.</li>
        <li><strong>L298N Motor Driver (Muscle):</strong> Dual H-bridge for controlling two DC motors with PWM speed control.</li>
    </ul>

    <!-- 4. Circuit Connections (Pin Mapping) -->
    <h2>🔌 Circuit Interfacing &amp; Pin Mapping</h2>
    <p>The following table shows exactly how Atharva wired the components together to avoid conflicts (e.g., Pin 10 is reserved for the SD card CS).</p>
    <table>
        <thead>
            <tr>
                <th>Component</th>
                <th>Module Pin</th>
                <th>Arduino Pin</th>
                <th>Technical Justification (Simple)</th>
            </tr>
        </thead>
        <tbody>
            <tr><td rowspan="3"><strong>DHT22</strong></td><td>VCC</td><td>5V</td><td>Powers the sensor.</td></tr>
            <tr><td>GND</td><td>GND</td><td>Common ground.</td></tr>
            <tr><td>DATA</td><td>Digital Pin 4</td><td>Single-wire communication with pull-up resistor.</td></tr>
            <tr><td rowspan="3"><strong>IR Sensor</strong></td><td>VCC</td><td>5V</td><td>Powers the comparator circuit.</td></tr>
            <tr><td>GND</td><td>GND</td><td>Ground.</td></tr>
            <tr><td>OUT</td><td>Digital Pin 7</td><td>Digital TTL output (LOW = obstacle detected).</td></tr>
            <tr><td rowspan="4"><strong>OLED (I²C)</strong></td><td>VCC</td><td>5V</td><td>Power (board has onboard regulator).</td></tr>
            <tr><td>GND</td><td>GND</td><td>Ground.</td></tr>
            <tr><td>SDA</td><td>A4 (SDA)</td><td>Data line for I²C.</td></tr>
            <tr><td>SCL</td><td>A5 (SCL)</td><td>Clock line for I²C.</td></tr>
            <tr><td rowspan="5"><strong>SD Module (SPI)</strong></td><td>CS</td><td>Digital Pin 10</td><td><strong>Slave Select</strong> – must be set as OUTPUT.</td></tr>
            <tr><td>MOSI</td><td>Digital Pin 11</td><td>Data from Arduino to SD card.</td></tr>
            <tr><td>MISO</td><td>Digital Pin 12</td><td>Data from SD card to Arduino.</td></tr>
            <tr><td>SCK</td><td>Digital Pin 13</td><td>Clock signal for SPI.</td></tr>
            <tr><td>VCC</td><td>5V</td><td>Power for the logic shifter.</td></tr>
            <tr><td rowspan="2"><strong>L298N Driver</strong></td><td>IN1, IN2</td><td>D5, D6</td><td>Direction control for Left Motor.</td></tr>
            <tr><td>IN3, IN4</td><td>D8, D9</td><td>Direction control for Right Motor (Pin 10 avoided to prevent SPI clash).</td></tr>
        </tbody>
    </table>

    <!-- 5. Data Flow & Firmware Logic -->
    <h2>🧠 Data Flow &amp; Firmware Execution</h2>
    <ol>
        <li><strong>Startup:</strong> Initializes SPI, SD card (checks FAT format), OLED display, and sets the DHT data pin high.</li>
        <li><strong>Obstacle Polling:</strong> Reads Pin 7. If LOW, triggers a <em>reverse-then-turn</em> avoidance subroutine.</li>
        <li><strong>Reading DHT22:</strong> Every 2.5 seconds, the brain sends a START signal (18ms low, 40µs high). The sensor replies with 40 bits of data (16 bits humidity, 16 bits temperature, 8 bits checksum). The checksum validates the data.</li>
        <li><strong>Display Update:</strong> Uses the U8g2 library to render the temperature, humidity, and obstacle status onto the OLED via I²C.</li>
        <li><strong>Data Logging:</strong> Opens <code>DATALOG.CSV</code> in append mode, writes <code>[Timestamp], [Temperature], [Humidity], [Obstacle_Flag]</code>, and immediately closes the file to prevent data loss.</li>
    </ol>

    <!-- 6. APP DESCRIPTION SECTION (with image) -->
    <h2>📱 The Companion App &amp; Comparative Chart</h2>
    <p>
        Atharva developed a powerful post-processing application (either using Python with Pandas/Matplotlib or MIT App Inventor) 
        called the <strong>"Robot Data Analytics Dashboard"</strong>. This app transforms the raw CSV logs into actionable visual intelligence.
    </p>

    <!-- IMAGE PLACEMENT HERE as requested -->
    <div class="app-screenshot">
        <img src="image.png" alt="Robot Data Analytics Dashboard by Atharva Phadnis" />
        <div class="caption">
            Figure 1: The Robot Data Analytics Dashboard – Atharva's custom app for uploading CSV logs, generating comparison charts, and exporting PDF reports.
        </div>
    </div>

    <h3>How the Dashboard Works (Step-by-Step):</h3>
    <ul>
        <li><strong>Upload CSV Files:</strong> The interface allows the user to drag-and-drop <strong>File 1 (Today’s Data)</strong> and optionally <strong>File 2 (Yesterday’s Data)</strong> for comparison mode.</li>
        <li><strong>Data Parsing:</strong> The app uses regex to extract timestamps and temperature columns, converting them into standardized datetime objects.</li>
        <li><strong>Data Segregation:</strong> It dynamically subtracts a 24-hour offset to isolate "Today" and "Yesterday" datasets.</li>
        <li><strong>Data Synchronization:</strong> Since the rover samples at a fixed interval (e.g., every 5 seconds), the app normalizes the X-axis (00:00 – 23:59) and uses linear interpolation to perfectly align sampling points.</li>
        <li><strong>Chart Generation:</strong> A dual-line overlay chart is rendered. The X-axis shows the time of day (Hour:Minute), and the Y-axis shows Temperature (°C). This clearly highlights diurnal variations, cloud cover effects, or urban heat island impacts.</li>
        <li><strong>Statistics &amp; Export:</strong> The dashboard calculates the Mean, Max, and Min for both days. As seen in the screenshot, the dashboard also includes <strong>Tools &amp; Export</strong> options to generate a <strong>PDF Report</strong> and set a save folder for the analyzed data.</li>
    </ul>
    <p>
        <strong>Real-time Status:</strong> The top bar of the app displays the current live conditions (e.g., <em>24°C Cloudy</em>) and a timestamp, giving the user context while analyzing historical logs.
    </p>

    <!-- 7. Real-World Applications -->
    <h2>🌍 Real-World Applications</h2>
    <ul>
        <li><strong>🌱 Greenhouse Precision Agriculture:</strong> Patrols plant rows, logs humidity drops to optimize irrigation schedules.</li>
        <li><strong>🏛️ Museum &amp; Archive Microclimate Monitoring:</strong> Tracks thermal gradients near HVAC vents to protect sensitive artifacts from heat/humidity stress.</li>
        <li><strong>🏙️ Urban Heat Island (UHI) Study:</strong> Compares temperatures over asphalt vs. grass over consecutive days, proving how surfaces retain heat.</li>
        <li><strong>🧪 STEAM Education:</strong> Integrates mechanical engineering, electrical circuits, embedded C++ programming, and data science into one holistic class project.</li>
    </ul>

    <!-- 8. Conclusion -->
    <h2>✅ Conclusion</h2>
    <p>
        Atharva's AESR is a robust, closed-loop Data Acquisition System. By meticulously managing the SPI bus for high-throughput storage, 
        leveraging I²C for efficient display updates, and implementing strict timing protocols for the DHT22, the rover ensures high-fidelity 
        data integrity. The integration of the <strong>cross-platform comparative dashboard</strong> elevates the raw logged data into 
        actionable visual intelligence, demonstrating a sophisticated understanding of embedded systems and data analytics well beyond 
        the typical Class 8 level.
    </p>

    <!-- Footer -->
    <footer>
        <p>
            <strong>Project by:</strong> Atharva Phadnis (Class 8C) · Vimala Kulkarni Memorial School<br>
            <strong>Tech Stack:</strong> Arduino C++ · SPI / I²C Protocols · Python/App Inventor · CSV Data Parsing
        </p>
        <p style="font-size: 0.8rem; color: #94a3b8;">
            © 2026 AESR Project · Built with curiosity and engineering passion.
        </p>
    </footer>

</div>
</body>
</html>
