#ifndef DEFAULT_CONFIG_H
#define DEFAULT_CONFIG_H

#if __has_include("config.h")
#include "config.h"
#endif

// Feature Flags
#if !defined(DEBUGGING) && !defined(DISABLE_DEBUGGING)
#define DEBUGGING
#endif

#if !defined(USE_OLED_DISPL) && !defined(DISABLE_OLED_DISPL)
#define USE_OLED_DISPL
#endif

// Arduino Pins
#ifndef BUTTON_PIN
#define BUTTON_PIN A9
#endif

#ifndef NUMBER_OF_BUTTONS
#define NUMBER_OF_BUTTONS 3
#endif

#ifndef BUTTON_SIGS
#define BUTTON_SIGS {480, 130, 0}
#endif

#ifndef KEYS_WINDOW_GAP
#define KEYS_WINDOW_GAP 120
#endif

// Display 7 segment pins
#ifndef CLOCK_PIN
#define CLOCK_PIN 5
#endif

#ifndef RESET_PIN
#define RESET_PIN 7
#endif

// Display configuration
#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 128
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 32
#endif

#ifndef OLED_RESET
#define OLED_RESET -1
#endif

#ifndef SCREEN_ADDRESS
#define SCREEN_ADDRESS 0x3C
#endif

// GPS configuration
#ifndef GPSBaud
#define GPSBaud 9600
#endif

#ifndef EARTH_RADIUS
#define EARTH_RADIUS 6371
#endif

// Audio
#ifndef STARTING_VOLUME
#define STARTING_VOLUME 20
#endif

// Special Audio Announcement
#ifndef SPECIAL_AUDIO_PIN
#define SPECIAL_AUDIO_PIN 50
#endif

#ifndef SPECIAL_AUDIO_TRIGGER_HOLD
#define SPECIAL_AUDIO_TRIGGER_HOLD true
#endif

#ifndef SPECIAL_AUDIO_HOLD_DURATION
#define SPECIAL_AUDIO_HOLD_DURATION 500
#endif

#ifndef SPECIAL_AUDIO_FOLDER
#define SPECIAL_AUDIO_FOLDER 1
#endif

#ifndef SPECIAL_AUDIO_TRACK
#define SPECIAL_AUDIO_TRACK 98
#endif

#endif // DEFAULT_CONFIG_H
