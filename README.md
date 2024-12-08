# ButtonBase Library

## Overview

This library provides a modular and extensible framework for handling button inputs in embedded systems. The design separates button handling logic into a base class (`ButtonBase`) and two derived classes (`ButtonExtended` and `ButtonClassic`) to support different hardware configurations, such as direct connections to a microcontroller or through an I/O expander like the MCP23X17.

The framework implements debouncing, click detection, double-click detection, and press duration tracking (short and long presses).

---

## Classes

### **1. ButtonBase (Abstract Base Class)**

This class encapsulates the core logic for button state tracking and provides a foundation for hardware-specific implementations.

#### **Key Features**
- **Constants**: Predefined delays for debouncing, double-clicks, and press durations.
- **State Tracking**:
  - Button press, release, short-held, long-held, and double-click detection.
- **Methods**:
  - `check(unsigned long &time_now)`: Processes button states based on the current time.
  - `is_pressed()`: Returns `true` if the button is currently pressed.
  - `was_pressed()`: Returns `true` if the button was pressed in any previous iteration and is still pressed.
  - `is_short_held()`: Returns `true` if the button is held for at least 2 seconds.
  - `is_long_held()`: Returns `true` if the button is held for at least 5 seconds.
  - `is_double_clicked()`: Returns `true` if the button was double-clicked.
  - `is_released()`: Returns `true` if the button was just released.

#### **Protected Constructor**
`ButtonBase(int port)`: Initializes the button with its hardware pin or port.

#### **Protected Virtual Method**
`virtual bool check_current_button_state()`: Must be implemented by derived classes to define how the button's current state is read from hardware.

`virtual void setup_button()`: Can be optionally implemeneted as well to define specific button setup behaviour.

---

### **2. ButtonExtended (For MCP23X17 I/O Expander)**

Handles buttons connected through an MCP23X17 I/O expander.

#### **Key Features**
- **Setup Method**:
  - `setup_button()`: Configures the MCP23X17 pin as an input with pull-up.
- **Overrides**:
  - `bool check_current_button_state()`: Reads the button state using the MCP23X17 API.

#### **Constructor**
`ButtonExtended(int p, Adafruit_MCP23X17& mcp)`: Initializes the button with the pin number and a reference to the MCP23X17 instance.

---

### **3. ButtonClassic (For Direct Connections)**

Handles buttons connected directly to the microcontroller's GPIO pins.

#### **Setup Method**:
  - `setup_button()`: Configures the _port pin as an input with pull-up.

#### **Overrides**:
- `bool check_current_button_state()`: Reads the button state using `digitalRead`.

#### **Constructor**
`ButtonClassic(int p)`: Initializes the button with the GPIO pin number.

---

## Why Separate the Base Class?

### **1. Hardware Abstraction**
The `ButtonBase` class encapsulates all hardware-agnostic logic, such as debouncing, state transitions, and timing. By making `check_current_button_state()` a virtual method, the library allows hardware-specific details to be handled by derived classes.

### **2. Extensibility**
New button configurations (e.g., SPI buttons, wireless buttons) can be supported by deriving new classes from `ButtonBase` and implementing `check_current_button_state()` accordingly.

### **3. Code Reusability**
The state management code (debouncing, double-click detection, etc.) is shared across all hardware implementations, reducing redundancy.

### **4. Cleaner Design**
Separating concerns between hardware-agnostic logic and hardware-specific behavior adheres to the Single Responsibility Principle and makes the codebase easier to maintain and extend.

---

## Usage

### **1. For Buttons with MCP23X17**
```cpp
#include <Adafruit_MCP23X17.h>
#include "button_extended.hpp"

#define addr1 0x20
Adafruit_MCP23X17 mcp;

ButtonExtended buttons[2] = {
    // Initialize first 16 buttons with mcp1
    ButtonExtended(0, mcp), 
    ButtonExtended(1, mcp)
}

constexpr int buttonCount = sizeof(buttons) / sizeof(buttons[0]);

void setup() {
    mcp.begin_I2C(addr1); // Initialize MCP23X17
    
    // Initialize all buttons
    for (int i = 0; i < buttonCount; i++) {
        buttons[i].setup_button();
    }
}

void loop() {
    auto time_now = millis(); // same time for all buttons
    for (int i = 0; i < buttonCount; i++) {
        // change the status of all buttons
        buttons[i].check(time_now);
    }

    if (button.is_pressed()) {
        // Handle press
    }

    if (button.is_double_clicked()) {
        // Handle double click
    }

    if (button.is_released()){
        // Handle release
    }
}
```
---

## Design Considerations

### **Static Assertions**
Ensures reasonable delay configurations at compile time:
- `short_held_delay < long_held_delay`
- `button_delay < 100ms`

### **Debouncing**
The `check()` method ensures state changes are valid only after a debounce period (`button_delay`).

### **Timing and Detection**
The library uses `millis()` to calculate durations for short/long holds and double-click detection. This ensures responsiveness and accuracy in state tracking.