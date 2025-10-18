# ESP8266 Pinout Reference


## GPIO Pin Functions

| Pin | GPIO | Function | Notes |
|-----|------|----------|-------|
| D0  | 16   | GPIO     | No interrupt, no PWM, no pull-up |
| D1  | 5    | GPIO     | I2C SCL |
| D2  | 4    | GPIO     | I2C SDA, LED control |
| D3  | 0    | GPIO     | Boot mode, LED control |
| D4  | 2    | GPIO     | Boot mode, built-in LED |
| D5  | 14   | GPIO     | SPI SCK |
| D6  | 12   | GPIO     | SPI MISO |
| D7  | 13   | GPIO     | SPI MOSI |
| D8  | 15   | GPIO     | Boot mode, SPI CS |
| D9  | 3    | GPIO     | RX pin |
| D10 | 1    | GPIO     | TX pin |
| D11 | 9    | GPIO     | SPI |
| D12 | 10   | GPIO     | SPI |
| D13 | 11   | GPIO     | SPI |
| A0  | A0   | Analog   | ADC input (0-1V) |

## Workshop Pin Assignments

### LEDs
- **Red LED**: D2 (GPIO 4)
- **Green LED**: D3 (GPIO 0)

### OLED Display
- **SDA**: D2 (GPIO 4) - I2C Data
- **SCL**: D1 (GPIO 5) - I2C Clock
- **VCC**: 3.3V
- **GND**: Ground

### Power
- **3.3V**: 3.3V output
- **VIN**: 5V input (USB)
- **GND**: Ground

## Important Notes

### Boot Mode Pins
- **D3 (GPIO 0)**: Must be HIGH for normal boot
- **D4 (GPIO 2)**: Must be HIGH for normal boot
- **D8 (GPIO 15)**: Must be LOW for normal boot

### I2C Pins
- **SDA**: D2 (GPIO 4)
- **SCL**: D1 (GPIO 5)

### SPI Pins
- **MOSI**: D7 (GPIO 13)
- **MISO**: D6 (GPIO 12)
- **SCK**: D5 (GPIO 14)
- **CS**: D8 (GPIO 15)

## Circuit Connections

### LED Circuit
<img src="../circuit.svg" width="600">

```
ESP8266 D2 ----[330Ω]----[LED]----GND
ESP8266 D3 ----[330Ω]----[LED]----GND
```

## Troubleshooting

### LED Not Working
- Check pin assignment (D2 for red, D3 for green)
- Verify resistor value (220Ω)
- Check LED polarity (longer leg = positive)
- Ensure proper ground connection


### Upload Issues
- Check USB connection
- Try different USB cable
- Verify port selection

## Safety Reminders

- Always check connections before powering on
- Use proper resistor values
- Verify LED polarity
- Avoid short circuits
- Ask instructor before connecting power
