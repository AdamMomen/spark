# API Specification

## Overview

This document describes the REST API endpoints for the IoT workshop dashboard. All endpoints return JSON responses and use HTTP status codes for error handling.

## Base URL

```
http://[ESP8266_IP_ADDRESS]
```

Example: `http://192.168.1.100`

## Authentication

- **OTA Updates**: Password protected (`workshop123`)
- **API Endpoints**: No authentication required for workshop

## Endpoints

### 1. System Status

**GET** `/api/status`

Returns system information and current LED states.

**Response:**
```json
{
  "wifi_connected": true,
  "uptime": 12345,
  "free_heap": 12345,
  "leds": {
    "1": false,
    "2": false
  },
  "timestamp": 1234567890
}
```

**Status Codes:**
- `200 OK`: Success
- `500 Internal Server Error`: System error

### 2. LED Control

**POST** `/api/led/{number}/toggle`

Toggle LED state (on/off).

**Parameters:**
- `number`: LED number (1 = Red, 2 = Green)

**Response:**
```json
{
  "led": 1,
  "state": true,
  "message": "LED toggled successfully",
  "timestamp": 1234567890
}
```

**Status Codes:**
- `200 OK`: Success
- `400 Bad Request`: Invalid LED number
- `500 Internal Server Error`: Hardware error

**Examples:**
```bash
# Toggle red LED (LED 1)
curl -X POST http://192.168.1.100/api/led/1/toggle

# Toggle green LED (LED 2)
curl -X POST http://192.168.1.100/api/led/2/toggle
```

### 3. LED State Control

**POST** `/api/led/{number}/state`

Set LED to specific state.

**Parameters:**
- `number`: LED number (1 = Red, 2 = Green)

**Request Body:**
```json
{
  "state": true
}
```

**Response:**
```json
{
  "led": 1,
  "state": true,
  "message": "LED state set successfully",
  "timestamp": 1234567890
}
```

**Status Codes:**
- `200 OK`: Success
- `400 Bad Request`: Invalid LED number or state
- `500 Internal Server Error`: Hardware error

### 4. Dashboard Page

**GET** `/`

Returns the main dashboard HTML page.

**Response:**
- Content-Type: `text/html`
- Returns HTML dashboard interface

**Status Codes:**
- `200 OK`: Success
- `500 Internal Server Error`: Server error

## Error Responses

All error responses follow this format:

```json
{
  "error": true,
  "message": "Error description",
  "code": 400,
  "timestamp": 1234567890
}
```

## LED Mapping

| LED Number | Color | GPIO Pin | Description |
|------------|-------|----------|-------------|
| 1          | Red   | D2       | Primary LED |
| 2          | Green | D3       | Secondary LED |


## CORS

- CORS headers not implemented
- All requests from same origin (local network)

## Example Usage

### JavaScript Fetch API

```javascript
// Get system status
fetch('/api/status')
  .then(response => response.json())
  .then(data => console.log(data));

// Toggle red LED
fetch('/api/led/1/toggle', {method: 'POST'})
  .then(response => response.json())
  .then(data => console.log(data));

// Set green LED state
fetch('/api/led/2/state', {
  method: 'POST',
  headers: {'Content-Type': 'application/json'},
  body: JSON.stringify({state: true})
})
  .then(response => response.json())
  .then(data => console.log(data));
```

### cURL Examples

```bash
# Get status
curl http://192.168.1.100/api/status

# Toggle LED 1
curl -X POST http://192.168.1.100/api/led/1/toggle

# Set LED 2 to ON
curl -X POST http://192.168.1.100/api/led/2/state \
  -H "Content-Type: application/json" \
  -d '{"state": true}'
```

## Testing

Use these tools to test your API:

1. **Browser**: Navigate to `http://[ESP_IP]/api/status`
2. **Postman**: Import API endpoints
3. **cURL**: Command line testing
4. **JavaScript**: Browser console testing

## Next Steps

1. Implement the API endpoints in your ESP8266 code
2. Test with the provided examples
3. Build your dashboard using these endpoints
4. Prepare for the creative dashboard challenge!
