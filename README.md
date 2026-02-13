# 433 MHz LDR-Triggered Laser Alarm

A simple and reliable laser/beam alarm project. It detects beam interruptions using an LDR, listens for two remote commands (MUTE / RESET) via a 433 MHz receiver, and alerts with a buzzer + LED. Once the alarm is triggered (latched), it will not turn off even if the LDR returns to normal — it can only be cleared with the **RESET** command. The **MUTE** command fully silences the system and prevents new triggers.

---

## Features
- Detects beam interruption via LDR (A0) with latch logic  
- Two commands via 433 MHz remote:
  - **MUTE** → RF code: `4544259` — silences the system, prevents new triggers  
  - **RESET** → RF code: `4544268` — clears mute, resets the alarm  
- Audible/visual alarm via buzzer and LED  
- Non-blocking alarm loop (Arduino does not freeze)  
- Easy to extend (EEPROM, LCD, GSM notifications, etc.)

---

## Hardware
- Arduino Pro Mini / Nano / Uno, etc.  
- 433 MHz receiver module (DATA pin: **D2**)  
- Buzzer (passive recommended) — **D4**  
- LED (with series resistor) — **D5**  
- LDR (photoresistor) — **A0** (requires a voltage divider)  
- Fixed 10kΩ resistor (for the LDR voltage divider)  
- Antenna (10–17 cm wire) — improves RF reception

---

## Pins (default)
- `D2` — 433 MHz RX DATA (interrupt-capable pin)  
- `D4` — Buzzer  
- `D5` — LED  
- `A0` — LDR analog input

You can change these pins easily in the project; just keep the README and the constants in sync.

---

## Wiring (summary)
- 433 MHz RX: VCC — 5V, GND — GND, DATA — D2  
- Buzzer: + → D4, − → GND (note active vs passive buzzer differences)  
- LED: D5 → 220Ω → LED → GND  
- LDR: 5V — LDR — A0 — Rfixed — GND (Rfixed ~10kΩ as a starting point)

---

## Behavior / Usage
- Startup: system is **ARMED** (mute off, alarm reset).  
- If the LDR reading drops **below** the threshold `650`, the alarm **latches** and the LED + buzzer start. Even if the LDR returns to normal, the alarm stays on.  
- Remote commands:
  - Send `4544259` → **MUTE**: silences the system, stops the alarm, and blocks new triggers.  
  - Send `4544268` → **RESET**: clears mute, resets the alarm, and returns to the initial state.  
- While the alarm is latched, only **RESET** fully clears it; **MUTE** stops the alarm and prevents re-triggering.

---

## Settings / Testing
- **LDR threshold**: `650` is suggested as a starting value. Test and tune for your ambient light (try ~600–800).  
- **RF code verification**: Check the actual received value in the Serial Monitor; different remotes may send different codes/protocols. Update the RF codes if needed.  
- **Buzzer type**: `tone()` works for a passive buzzer; for an active buzzer, drive it with `HIGH/LOW`.

---

## Troubleshooting
- **RF codes are different**: Read the incoming RF value via Serial Monitor and update the MUTE/RESET codes accordingly.  
- **LDR trigger sensitivity**: The threshold must be tuned to the environment. Ensure the voltage divider (LDR + fixed resistor) is wired correctly.  
- **Receiver not working**: Is the antenna connected? VCC/GND correct? DATA on the correct (interrupt-capable) pin?  
- **Buzzer not working**: Verify active vs passive buzzer type and the correct drive method.  
- **LED too dim**: Check the series resistor value and wiring.

---

## Improvement ideas
- Small OLED/LCD for status: show `ARMED / MUTE / ALARM`  
- Store `muted` and `alarmLatched` in EEPROM to survive power loss  
- Multiple LDR zones for zone-based alarm logic  
- SMS/Push/GSM/IoT notification integration (send a notification on trigger)  
- Timer modes (night mode / day mode)

---

## Safety and Ethics
- This device is experimental; test thoroughly before using it for any critical safety purpose.  
- Listening to or retransmitting other people’s signals without permission can create ethical and/or legal issues — follow local laws.  
- Lasers or direct eye exposure can be dangerous; take eye safety seriously.

---

## License
MIT License — You may copy, modify, and distribute this project.
