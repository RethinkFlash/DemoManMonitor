// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "EscentsMonitor.h"

#include <iostream>
#include <unistd.h>
#include <wiringPi.h>

using namespace std;

int DELAY_MS = 750;
int MIN_VAL = 25;
int MAX_VAL = 125;

EscentsMonitor::EscentsMonitor(size_t bufferSize,
							   AudioSource* audioSource,
							   AudioSink* audioSink,
							   KeywordSpotter* spotter,
							   std::vector<uint8_t>* alarm,
                               int servoPin):
	_audioSource(audioSource),
	_audioSink(audioSink),
	_spotter(spotter),
	_alarm(alarm),
	_buffer(bufferSize),
    _servoPin(servoPin)
{
    // Initialize wiringPi library.
    if (wiringPiSetup () == -1)
        exit (1) ;


    pinMode(_servoPin, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetClock(384);
    pwmSetRange(1000);
}

EscentsMonitor::~EscentsMonitor()
{}

void EscentsMonitor::update() {
	// Grab a buffer of audio.
	if (_audioSource->record(_buffer)) {
		// Look for a keyword.
		string keyword = _spotter->process(_buffer);
		if (keyword != "") {
			// Keyword was spotted, sound alarm.
			cout << "KEYWORD SPOTTED: " << keyword << endl;
			raiseAlarm(keyword);
		}
	}
	else {
		// There was a problem getting data from the microphone.
		// Print an error and try again with next update.
		cerr << "Failed to get a full buffer of microphone data!" << endl;
	}
}


void EscentsMonitor::raiseAlarm(const std::string& keyword) {

    // Stop audio recording while the alarm is raised.
	_audioSource->pause();

    /*****
    put in code to set pwm for servo
    from the python test, these pwm values where good
    2 is good for min
    6.5 is good for mid
    11.5 is good for max
    *****/

    pwmWrite(_servoPin, MAX_VAL);
    delay(2000);
    pwmWrite(_servoPin, MIN_VAL);
    delay(1000);
    pwmWrite(_servoPin, 0);

    cout << "done with servo" << endl;


	// Play audio and print the ticket at the same time.
	// Since the Pi only has one core and timing is somewhat
	// critical (for smooth audio playback), a tight loop to
	// update audio and ticket printing state will be executed.
	// _audioSink->resume();
	// _audioSink->playAsync(*_alarm);
	// bool playing = true;
	// while (playing) {
		// playing = _audioSink->asyncUpdate();
	// }
	// Stop audio playback.
	// _audioSink->pause();
	// Enable recording again.
	_audioSource->resume();
}
