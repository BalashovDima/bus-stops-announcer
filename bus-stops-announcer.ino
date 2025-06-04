#define DEBUGGING
#define USE_OLED_DISPL

#define BUTTON_PIN A9
#define NUMBER_OF_BUTTONS 3

#include <AnalogKey.h>
#include <EncButton.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h> // https://github.com/Makuna/DFMiniMp3
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include "./display_7_segment.h"
#include "./coordinates.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#ifdef USE_OLED_DISPL
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif
// #define RX_GPS 15 // arduino rx pin for gps (gps's tx)
// #define TX_GPS 14 // arduino tx pin for gps (gps's rx)
#define GPSBaud 9600 // Default baud of NEO-6M is 9600
// moved to mega with it's own hardware serial so there is no need for software serial
// SoftwareSerial Serial3(RX_GPS, TX_GPS); // Create a software serial port called "Serial3"
TinyGPSPlus gps;
#define EARTH_RADIUS 6371  // Earth's radius in kilometers

int16_t sigs[NUMBER_OF_BUTTONS] = {413, 122, 0}; // array of signal value of buttons
AnalogKey<BUTTON_PIN, NUMBER_OF_BUTTONS, sigs> keys; // pin of button, number of buttons, array of signal
// Button btn(BUTTON_PIN);
VirtButton btn_1;
VirtButton btn_2;
VirtButton btn_3;


#define STARTING_VOLUME 20
class Mp3Notify; // forward declare the notify class, just the name

typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; // define a handy type using serial and our notify class
DfMp3 dfmp3(Serial2); // instance a DfMp3 object

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definitions and use these
// SoftwareSerial secondarySerial(17, 16); // RX (DF player's TX), TX (DF player's RX)
// typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
// DfMp3 dfmp3(secondarySerial);

bool increaseRadiusDistanceCheck = true;
uint16_t radiusDistanceCheck = 20;
uint16_t volume = STARTING_VOLUME;
uint8_t audioPlay = 0; // 0 play 'stop...', 1 play stop, 2 play 'next...', 3 play next stop
uint16_t trackNumber = 1;
uint16_t trackCountInFolder1;
bool secondFinishCall = false;
bool gpsInfoBigText = false;
bool showDistancesInsteadOfCord = true;
float distances[STOPS_MAX] = {0};
uint8_t index_of_shortest[STOPS_MAX]; // array of stop indexes (stop number - 1)
uint8_t lastStop = 255;
uint64_t lineChangedTime = 0;
bool checkLineSave = false;
uint64_t GPSInfoORDistanceCheckTimer = 0;
bool showGPSInfo = true;
uint8_t nextStopIndex = 0;

bool startToEnd = true;
double lastLat = 0;
double lastLng = 0;

Coordinates coordinates;

// implement a notification class,
// its member methods will get called
class Mp3Notify {
    public:
        static void PrintlnSourceAction(DfMp3_PlaySources source, const char *action) {
            if (source & DfMp3_PlaySources_Sd) {
                Serial.print("SD Card, ");
            }

            if (source & DfMp3_PlaySources_Usb) {
                Serial.print("USB Disk, ");
            }

            if (source & DfMp3_PlaySources_Flash) {
                Serial.print("Flash, ");
            }

            Serial.println(action);
        }

        static void OnError([[maybe_unused]] DfMp3 &mp3, uint16_t errorCode) {
            // see DfMp3_Error for code meaning
            Serial.println();
            Serial.print("Com Error ");
            Serial.println(errorCode);
        }

        // from wiki on library's github (important that it gets called twice)
        // A track has finished playing. There are cases where this will get called more than once for the same track. If you are using a repeating mode or random play, you will receive only one between the tracks and a double when it stops. If you play a single track, you should get called twice, one for the finish of the track, and another for the finish of the command. This is a nuance of the chip.
        static void OnPlayFinished([[maybe_unused]] DfMp3 &mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
            // stopPassedTime = millis();
            if(secondFinishCall) {
                switch(audioPlay) {
                    case 1: // play stop name
                        dfmp3.playFolderTrack16(2, coordinates.getStopAudio(index_of_shortest[0], startToEnd)); 
                        audioPlay = 2;
                        break;
                    case 2: // play 'next stop...' or 'last stop' if its the last stop
                        if(index_of_shortest[0]+1 == coordinates.getStopsNum() || index_of_shortest[0] == 0) {
                            dfmp3.playFolderTrack16(1, 3); // play 'last stop'
                            audioPlay = 0;
                            secondFinishCall = false;
                            return;
                        } else { // look for next stop. there might be cases where 'empty stop(s)' is(are) present, so code below handles it 
                            uint8_t increment = 0;
                            if(startToEnd) {
                                while(true) {
                                    increment++;
                                    nextStopIndex = index_of_shortest[0]+increment;
                                    if(coordinates.getStopAudio(nextStopIndex, startToEnd) != 0) break;
                                    else if(nextStopIndex +1 == coordinates.getStopsNum()) {
                                        dfmp3.playFolderTrack16(1, 3); // play 'last stop'
                                        audioPlay = 0;
                                        secondFinishCall = false;
                                        return;
                                    }
                                }
                            } else {
                                while(true) {
                                    increment++;
                                    nextStopIndex = index_of_shortest[0]-increment;
                                    if(coordinates.getStopAudio(nextStopIndex, startToEnd) != 0) break;
                                    else if(nextStopIndex == 0) {
                                        dfmp3.playFolderTrack16(1, 3); // play 'last stop'
                                        audioPlay = 0;
                                        secondFinishCall = false;
                                        return;
                                    }
                                }
                            }
                        }
                        dfmp3.playFolderTrack16(1, 2); // audio saying 'next stop'
                        audioPlay = 3;
                        break;
                    case 3:
                        dfmp3.playFolderTrack16(2, coordinates.getStopAudio(nextStopIndex, startToEnd));
                        audioPlay = 0;
                        break;
                    default:
                        break;
                }

                secondFinishCall = false;
            } else {
                secondFinishCall = true;
            }
        }

        static void OnPlaySourceOnline([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source) {
            PrintlnSourceAction(source, "online");
        }

        static void OnPlaySourceInserted([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source) {
            PrintlnSourceAction(source, "inserted");
        }

        static void OnPlaySourceRemoved([[maybe_unused]] DfMp3 &mp3, DfMp3_PlaySources source) {
            PrintlnSourceAction(source, "removed");
        }
};

void setup() {
    keys.setWindow(50);

    Serial.begin(115200);
    

    #ifdef USE_OLED_DISPL
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
    }

    #ifdef DEBUGGING
        display.display();
        delay(700); 
    #endif

    // Clear the buffer
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    #endif

    #ifdef DEBUGGING
        #ifdef USE_OLED_DISPL
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("initializing DF player...");
        display.display();
        #endif
        Serial.println("initializing DF player...");
    #endif

    dfmp3.begin();
    // for boards that support hardware arbitrary pins
    // dfmp3.begin(6, 5); // RX, TX

    // during development, it's a good practice to put the module
    // into a known state by calling reset().
    // You may hear popping when starting and you can remove this
    // call to reset() once your project is finalized
    dfmp3.reset();

    uint16_t version = dfmp3.getSoftwareVersion();
    uint16_t total_track_count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    trackCountInFolder1 = dfmp3.getFolderTrackCount(1);
    dfmp3.setVolume(STARTING_VOLUME);

    // show some info on dfplayer (to let know that it's working)
    #ifdef DEBUGGING
        #ifdef USE_OLED_DISPL

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("done initializing DFPlayer...");
        display.display();
        delay(500);

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("version ");
        display.print(version);

        display.setCursor(0, 8);
        display.print("tracks total: ");
        display.print(total_track_count);
        display.display();

        display.setCursor(0, 16);
        display.print("volume: ");
        display.print(volume);
        display.display();

        delay(500);
        
        #endif

        Serial.println("done initializing DFPlayer...");
        Serial.print("version ");
        Serial.println(version); 
        Serial.print("files total ");
        Serial.println(total_track_count);
        Serial.print("volume ");
        Serial.println(volume);
    #endif
    // uint16_t volume = dfmp3.getVolume();
    

    // Start the software serial port at the GPS's default baud
    Serial3.begin(GPSBaud);

    pinMode(RESET_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    resetNumber();
    showNumber(coordinates.currentRouteDispNum());
}

void loop() {
    btn_1.tick(keys.status(0));
    btn_2.tick(keys.status(1));
    btn_3.tick(keys.status(3));

    dfmp3.loop();

    if(checkLineSave) {
        if(millis() - lineChangedTime > 20000) { // remember (write to EEPROM) what route was chosen if it hasn't been changed for 20 seconds
            coordinates.rememberRoute();
            checkLineSave = false;
        }
    }

    if(btn_1.hasClicks(1)) {
        for (int i = 0; i < STOPS_MAX; ++i) {
            distances[i] = 9999;
        }
        
        coordinates.prevLine();
        checkLineSave = true;
        lineChangedTime = millis();
        showNumber(coordinates.currentRouteDispNum());
    }
    if(btn_3.hasClicks(1)) {
        for (int i = 0; i < STOPS_MAX; ++i) {
            distances[i] = 9999;
        }
        
        coordinates.nextLine();
        checkLineSave = true;
        lineChangedTime = millis();
        showNumber(coordinates.currentRouteDispNum());
    }

    if(btn_2.hasClicks(3)) {
        dfmp3.playFolderTrack16(1, 99); // in folder '01', audio '0099' which says 'audio check'
    }

    if(btn_2.hasClicks(1)) {
        Serial.println("button clicked");
        if(showDistancesInsteadOfCord) {
            gpsInfoBigText = true;
            showDistancesInsteadOfCord = false;
        } else if(gpsInfoBigText) {
            gpsInfoBigText = false;
        } else {
            showDistancesInsteadOfCord = true;
        }
    }

    if(showGPSInfo) {
        while (Serial3.available() > 0) {
            if (gps.encode(Serial3.read())) {
                // calculate distances to stops
                for (uint8_t i = 0; i < coordinates.getStopsNum(); i++) {
                    // distances[i] = calculateDistance(51.24017209067963, 33.21328523813368, coordinates.getLat(i, startToEnd), coordinates.getLng(i, startToEnd)); // for testing
                    distances[i] = calculateDistance(gps.location.lat(), gps.location.lng(), coordinates.getLat(i, startToEnd), coordinates.getLng(i, startToEnd));

                    index_of_shortest[i] = i;
                }

                // sort distances and their coordinates from smallest to largest
                for (int i = 0; i < coordinates.getStopsNum() - 1; i++) {
                    int minIndex = i;
                    for (int j = i + 1; j < coordinates.getStopsNum(); j++) {
                        if (distances[j] < distances[minIndex]) {
                            minIndex = j;
                        }
                    }
                    if (minIndex != i) {
                        // Swap distances and corresponding indexes
                        swap(distances[i], distances[minIndex]);
                        swap(index_of_shortest[i], index_of_shortest[minIndex]);
                    }
                }

                #ifdef DEBUGGING
                Serial.println("\nThe 6 shortest distances are: ");
                for(byte i = 0; i < 6; i++) {
                    Serial.print(i);
                    Serial.print(") ");
                    Serial.print(index_of_shortest[i]);
                    Serial.print(" - ");
                    Serial.println(distances[i]);
                }
                #endif

                // if within range of a specific stop and not moving then announce the stop
                if(distances[0] <= coordinates.getStopRadius(index_of_shortest[0], startToEnd) && gps.speed.kmph() <= 1) {
                    // if(millis() - stopPassedTime >= 10000) {
                    if(lastStop != index_of_shortest[0]) {
                        lastStop = index_of_shortest[0];
                        // stopPassedTime = millis();

                        audioPlay = 1;
                        dfmp3.playFolderTrack16(1, 1); // play audio that says 'stop...'
                    }
                }
                
                determineDirection();

                #ifdef USE_OLED_DISPL
                displayInfo();
                #endif
            }
        }
    } else {
        #ifdef USE_OLED_DISPL
        displayInfo();
        #endif
    }

}

#ifdef USE_OLED_DISPL
void displayInfo() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    if(showGPSInfo) {
        Serial.println("showing gps info");
        
        // if(millis() - GPSInfoORDistanceCheckTimer >= 3000) {
        //     showGPSInfo = false;
        //     GPSInfoORDistanceCheckTimer = millis();
        // }

        if (gps.location.isValid()) {
            if(showDistancesInsteadOfCord) { // show six shortest distances
                if(distances[0] <= 40) { // if very close to a stop, then show the distance to it and speed with big text size
                    display.setTextSize(2);
                    display.setCursor(0, 0);
                    display.print(index_of_shortest[0]);
                    display.print(") ");
                    display.print(distances[0]);
                    display.print("m");
                    display.setCursor(0, 17);
                    display.print(gps.speed.kmph());
                    display.print("km/h");

                    // section below doesn't print "!!" because in the main loop when announcing the stop the code updates lastStop
                    // if(distances[0] <= radiusDistanceCheck && gps.speed.kmph() <= 1) {
                    //     if (lastStop != index_of_shortest[0]) {
                    //         display.setCursor(95, 17);
                    //         display.print("!!");
                    //     }
                    // }
                    display.setCursor(105, 17);
                    if(startToEnd) {
                        display.print(">");
                    } else {
                        display.print("<");
                    }
                } else {
                    // print first six 
                    display.setTextSize(1);
                    for(byte i = 0; i < 2; i++) {
                        for (byte j = 0; j < 3; j++) {
                            byte cordNumber = j+(i*3);
                            if(cordNumber == 5) continue; // don't prit sixth coordinated to leave space for movement direction
                            display.setCursor(i * 64, j * 10);
                            display.print(index_of_shortest[cordNumber]);
                            display.print(")");
                            display.print(distances[cordNumber], 1);
                        }
                    }

                    display.setCursor(100, 24);
                    if(startToEnd) {
                        display.print("->");
                    } else {
                        display.print("<-");
                    }
                }
                
            } else if(gpsInfoBigText) { // only show latitude and longitute with big text size
                display.setTextSize(1);
                display.print("LT");
                display.setCursor(0, 20);
                display.print("LN");

                display.setTextSize(2);
                display.setCursor(13, 0);
                display.println(gps.location.lat(), 6);
                display.setCursor(13, 17);
                display.print(gps.location.lng(), 6);

                Serial.print("Latitude: ");
                Serial.println(gps.location.lat(), 6);
                Serial.print("Longitude: ");
                Serial.print(gps.location.lng(), 6);
            } else { // most robust view (time, num of sat, lat, long, speed)
                display.setTextSize(1);
                display.print("UTC ");
                display.print(gps.time.hour());
                display.print(":");
                display.print(gps.time.minute());
                display.print(" sat used: ");
                display.print(gps.satellites.value());

                display.setCursor(0, 8);
                display.print("Latitude:  ");
                display.println(gps.location.lat(), 6);
                display.setCursor(0, 16);
                display.print("Longitude: ");
                display.println(gps.location.lng(), 6);

                display.setCursor(0, 24);
                display.print("speed: ");
                display.print(gps.speed.kmph());
                display.print("km/h");
            }
        } else {
            display.setTextSize(1);
            display.println("Location not available");

            Serial.println("Location: Not Available");
        }
    } else {
        Serial.println("showing check distance");
        if(millis() - GPSInfoORDistanceCheckTimer >= 1000) {
            showGPSInfo = true;
            GPSInfoORDistanceCheckTimer = millis();
        }

        display.setTextSize(1);
        display.println("Currently check whether within:");
        display.setTextSize(3);
        display.setCursor(65, 10);
        display.print(radiusDistanceCheck);
        display.println("M");
    }

    display.display();
}
#endif

double toRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

double calculateDistance(double latA, double lonA, double latB, double lonB) {
    // Convert coordinates from degrees to radians
    latA = toRadians(latA);
    lonA = toRadians(lonA);
    latB = toRadians(latB);
    lonB = toRadians(lonB);

    // Calculate differences between latitudes and longitudes
    double deltaLat = latB - latA;
    double deltaLon = lonB - lonA;

    // Apply Haversine formula
    double a = pow(sin(deltaLat / 2), 2) + cos(latA) * cos(latB) * pow(sin(deltaLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = EARTH_RADIUS * c * 1000;

    return distance; // in meters
}

double calculateBearing(double lat1, double lon1, double lat2, double lon2) {
    lat1 = toRadians(lat1);
    lon1 = toRadians(lon1);
    lat2 = toRadians(lat2);
    lon2 = toRadians(lon2);

    double dLon = lon2 - lon1;
    double x = cos(lat2) * sin(dLon);
    double y = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);
    return atan2(x, y) * (180.0 / M_PI);
}

double bearingDifference(double bearing1, double bearing2) {
    double difference = fmod(bearing2 - bearing1 + 360.0, 360.0);
    if (difference > 180.0) {
        difference -= 360.0;
    }
    return difference;
}

// determine direction of moving (start (first stop) to end (last stop), or end to start)
void determineDirection() {
    if(gps.speed.kmph() > 10) {
        double movementBearing = calculateBearing(lastLat, lastLng, gps.location.lat(), gps.location.lng());

        uint8_t stopA = index_of_shortest[0];
        uint8_t stopB;
        if(index_of_shortest[1] == stopA + 1 || index_of_shortest[1] == stopA - 1) {
            stopB = index_of_shortest[1];
        } else if(index_of_shortest[2] == stopA + 1 || index_of_shortest[2] == stopA - 1) {
            stopB = index_of_shortest[2];
        } else return;

        double stopsBearing = calculateBearing(coordinates.getLat(stopA, startToEnd), coordinates.getLng(stopA, startToEnd), coordinates.getLat(stopB, startToEnd), coordinates.getLng(stopB, startToEnd));

        double bearingDiff = bearingDifference(movementBearing, stopsBearing);
        
        #ifdef DEBUGGING
        Serial.print("movement bearing: ");
        Serial.println(movementBearing);
        Serial.print("stops bearing: ");
        Serial.println(stopsBearing);
        Serial.print("bearing difference: ");
        Serial.println(bearingDiff);
        #endif

        if (fabs(bearingDiff) < 30.0) { // bearings match in direciton (moving from stopA to stopB)
            if(stopA < stopB) {
                startToEnd = true;
            } else {
                startToEnd = false;
            }
        } else if (fabs(bearingDiff) > 150.0) { // bearing are opposite (moving from stopB to stopA)
            if(stopA > stopB) {
                startToEnd = true;
            } else {
                startToEnd = false;
            }
        } 

        #ifdef DEBUGGING
        Serial.print("Start to end: ");
        if(startToEnd) Serial.println("true");
        else Serial.println("false");
        Serial.println("------------------------------------\n");
        #endif
    }

    lastLat = gps.location.lat();
    lastLng = gps.location.lng();
}

template <typename T>
void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}