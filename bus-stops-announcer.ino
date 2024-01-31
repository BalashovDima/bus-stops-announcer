#define BUTTON_PIN 9

#include <EncButton.h>
#include <SoftwareSerial.h>
#include <DFMiniMp3.h> // https://github.com/Makuna/DFMiniMp3

Button btn(BUTTON_PIN);


class Mp3Notify; // forward declare the notify class, just the name

// typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; // define a handy type using serial and our notify class
// DfMp3 dfmp3(Serial1); // instance a DfMp3 object

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definitions and use these
SoftwareSerial secondarySerial(5, 4); // RX (DF player's TX), TX (DF player's RX)
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
DfMp3 dfmp3(secondarySerial);

bool increaseVolume = true;
uint16_t volume;
uint16_t trackNumber = 1;
uint16_t trackCountInFolder1;
bool secondFinishCall = false;

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

        static void OnPlayFinished([[maybe_unused]] DfMp3 &mp3, [[maybe_unused]] DfMp3_PlaySources source, uint16_t track) {
            if(secondFinishCall) {
                trackNumber += 1;
                if(trackNumber > trackCountInFolder1) trackNumber = 1;

                dfmp3.playFolderTrack16(1, trackNumber);

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
    btn.setBtnLevel(HIGH);

    Serial.begin(57600);

    Serial.println("initializing DF player...");
    dfmp3.begin();
    // for boards that support hardware arbitrary pins
    // dfmp3.begin(6, 5); // RX, TX

    // during development, it's a good practice to put the module
    // into a known state by calling reset().
    // You may hear popping when starting and you can remove this
    // call to reset() once your project is finalized
    dfmp3.reset();

    uint16_t version = dfmp3.getSoftwareVersion();
    Serial.print("version ");
    Serial.println(version);

    // uint16_t volume = dfmp3.getVolume();
    volume = dfmp3.getVolume();
    Serial.print("volume was ");
    Serial.println(volume);
    Serial.println("now it's set to 15");
    dfmp3.setVolume(15);
    volume = 15;

    uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
    Serial.print("files total ");
    Serial.println(count);

    trackCountInFolder1 = dfmp3.getFolderTrackCount(1);
    Serial.print("tracks in folder '01'");
    Serial.println(trackCountInFolder1);

    Serial.println("starting playing track 0001 in folder 01...");
    dfmp3.playFolderTrack16(1, 1); // start playing track '0001.....' in folder '01'
}

void loop() {
    btn.tick();
    dfmp3.loop();

    if(btn.hasClicks(1)) {
        if(dfmp3.getStatus().state == DfMp3_StatusState_Paused) {
            dfmp3.start();
            Serial.println("playing resumed");
        } else {
            dfmp3.pause();
            Serial.println("playing paused");
        }
    }

    if(btn.hasClicks(2)) {
        trackNumber++;
        if(trackNumber > trackCountInFolder1) trackNumber = 1;
        dfmp3.playFolderTrack16(1, trackNumber);
        Serial.println("next track");
    }

    if(btn.hasClicks(3)) {
        trackNumber--;
        if(trackNumber == 0) trackNumber = trackCountInFolder1;
        dfmp3.playFolderTrack16(1, trackNumber);
        Serial.println("prev track");
    }

    if(btn.step()) {
        if(increaseVolume) {
            dfmp3.increaseVolume();
            Serial.print("Volume increased, current is ");
            // Serial.println(dfmp3.getVolume());
            volume++;
            Serial.println(volume);
        } else {
            dfmp3.decreaseVolume();
            Serial.print("Volume decreased, current is ");
            // Serial.println(dfmp3.getVolume());
            volume--;
            Serial.println(volume);
        }
    }

    if(btn.releaseStep()) {
        increaseVolume = !increaseVolume;
    }

}