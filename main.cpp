// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Main application.

#include <algorithm>
#include <csignal>
#include <cstdint>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include <wiringPi.h>
#include <softPwm.h>

#include "AlsaError.h"
#include "AlsaSink.h"
#include "AlsaSource.h"
#include "SajeMonitor.h"
#include "PocketSphinxKWS.h"

using namespace std;

#define ALARM_FILE		"alarm_movie_padded.raw"
#define RECORD_HW		"plughw:0,0"
#define PLAYBACK_HW		"plughw:1,0"
#define KEYWORD_FILE	"keywords.txt"

// #define ARDUINO_PORT	"/dev/ttyUSB0"
// #define BAUD_RATE       9600
#define QUIET_PIN       0

bool shouldRun = true;
int arduinoSerial;
int wiringPiSetupInt;

void setQuietMode(SajeMonitor& monitor, bool quietMode) {
	monitor.setQuietMode(quietMode);
	if (quietMode) {
		cout << "Quiet mode enabled." << endl;
	}
	else {
		cout << "Quiet mode disabled." << endl;
	}
}

int main(int argc, char* argv[]) {
	try {
		cout << "Demolition Man Verbal Morality Statute Monitor" << endl;
		cout << "Loading..." << endl;

		// Signal handler to catch ctrl-c in the main loop and shut down gracefully (i.e. call destructors).
		signal(SIGINT, [](int param){ shouldRun = false; });

		// Initialize wiringPi library and quiet switch input.
		wiringPiSetupInt = wiringPiSetupPhys() ;
		pinMode(QUIET_PIN, INPUT);
		bool quietSwitch = (digitalRead(QUIET_PIN) == HIGH);

        // if((arduinoSerial = serialOpen(ARDUINO_PORT, BAUD_RATE)) < 0 ) {
        //     cerr << "ERROR: Arduino Serial cannot be opened" << endl;
        //     if ( wiringPiSetupInt < 0 ) {
        //         cerr << "WiringPiSetup problem" << endl;
        //     }
    	// 	return 1;
        // } else {
        //     cout << "Arduino Serial successful!" << endl;
        // }

		// Load alarm raw audio.
		ifstream input(ALARM_FILE, ios::in | ios::binary);
		input.seekg (0, input.end);
		size_t length = input.tellg();
		input.seekg (0, input.beg);
		vector<uint8_t> alarm(length);
		input.read((char*)alarm.data(), length);

		// Initialize audio sink and source.
		AlsaSink sink;
		sink.open(PLAYBACK_HW, 44100, 1, SND_PCM_FORMAT_S16_LE);
		AlsaSource source;
		source.open(RECORD_HW, 16000, 1, SND_PCM_FORMAT_S16_LE);

		// Initialize keyword spotter.
		PocketSphinxKWS spotter;
		spotter.initialize(PocketSphinxKWS::parseConfig(argc, argv), KEYWORD_FILE);

		// Initialize main logic.
		SajeMonitor monitor(8000, &source, &sink, &spotter, &alarm, 12);
		setQuietMode(monitor, quietSwitch);

		cout << "Listening... (press Ctrl-C to stop)" << endl;

		while (shouldRun) {
			// Check quite mode switch and update state.
			bool newQuietSwitch = (digitalRead(QUIET_PIN) == HIGH);
			if (newQuietSwitch != quietSwitch) {
				quietSwitch = newQuietSwitch;
				setQuietMode(monitor, quietSwitch);
			}
			// Update main logic state.
			monitor.update();
		}
	}
	catch (AlsaError ex) {
		cerr << "ALSA ERROR " << ex.message << " (" << ex.code << ") while calling: " << ex.what() << endl;
		return 1;
	}
	catch (exception ex) {
		cerr << "ERROR: " << ex.what() << endl;
		return 1;
	}

	return 0;
}

void exiting() {
    cerr << "Ending session " << endl;
    // serialFlush(arduinoSerial);
    // serialClose(arduinoSerial);
    cerr << " " << endl;
}
