#ifndef CONFIG_H
#define CONFIG_H

// Uncomment or edit the following to customize the system

#define DEBUGGING
#define USE_OLED_DISPL

#define BUTTON_PIN A9
#define NUMBER_OF_BUTTONS 3
#define BUTTON_SIGS {480, 130, 0}
#define KEYS_WINDOW_GAP 120

// Display 7 segment pins
#define CLOCK_PIN 5
#define RESET_PIN 7

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define GPSBaud 9600 // Default baud of NEO-6M is 9600
#define EARTH_RADIUS 6371 // Earth's radius in kilometers

#define STARTING_VOLUME 20

#endif // CONFIG_H
