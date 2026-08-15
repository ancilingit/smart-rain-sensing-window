# Smart Rain-Sensing Window System

A prototype built for **NexBuildOn Hack 2026** that automatically manages public transport bus windows during rain — protecting seats from wet-in splashes while keeping passengers in control of their own comfort.

## The Problem

On buses, an open window near an empty seat becomes a problem the moment it rains. By the time the next passenger boards, the seat is wet and unusable — and there's no automated way to prevent this today.

## Our Solution

A microcontroller-based system fitted to each window that:

- Continuously monitors for rain using a rain-sensing module
- **Light/uncertain rain** → asks the nearest passenger for permission before closing (Yes/No)
- **Heavy rain** → closes automatically without waiting, to prevent damage
- **False positives** (e.g. a splash, not real rain) are filtered out using a short sustained-reading check, so the window doesn't close unnecessarily
- The **driver** has a master override to open or close any window at any time
- A breathable mesh section remains available even when the main pane is closed, for passenger comfort

## How It Works

1. A rain sensor continuously outputs an analog reading that reflects how much water is on its sensing plate.
2. The microcontroller compares this reading against two thresholds: dry, light rain, and heavy rain.
3. Depending on the result, it either does nothing, prompts the passenger via buttons, or closes the window immediately via a servo motor.
4. The driver's override button takes priority over all other logic at any time.

## Components Used

| Component | Purpose |
|---|---|
| Rain sensor module (YL-83 + FC-37) | Detects rain and its intensity |
| Microcontroller (Arduino Uno ) | Runs the decision logic |
| SG90 micro servo | Physically closes/opens the window |
| Push buttons (x3) | Passenger Yes/No input + driver override |
| Buzzer | Alerts on automatic heavy-rain closure |
| LEDs | Window status indicator |

## Future Enhancements

- **Weather API integration**: cross-checking the physical sensor against live weather forecast data (e.g. OpenWeatherMap) to further reduce false positives and give early warnings before rain starts, once WiFi-enabled hardware (ESP32) is used
- **Driver dashboard**: a live web view showing the status of every window on the bus
- **Data logging**: tracking how often windows auto-close per route, to inform maintenance and scheduling decisions

## Repository Structure

```
/firmware   -> Arduino source code
/Smart_Bus_Window_Prototype_Document_with_Pictures -> Circuit diagram, photos, additional documentation
/demo  video -> Link to demo video
```

## Setup Instructions

1. Open `/firmware/smart_window_control.ino` in the Arduino IDE
2. Install the `Servo` library if not already available (Sketch → Include Library → Manage Libraries)
3. Wire the components as described in `/docs` (circuit diagram)
4. Select your board and port, then upload the sketch
5. Open the Serial Monitor (9600 baud) to view live sensor readings and system state

## Team

- Ancilin Angel A
- Ananthi N
- Fiona Fernando V

## Built For

NexBuildOn Hack 2026 — Round 2 Prototype Development
