// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "SajeMonitor.h"

#include <iostream>
#include <unistd.h>

using namespace std;

SajeMonitor::SajeMonitor(size_t bufferSize,
							   AudioSource* audioSource,
							   AudioSink* audioSink,
							   KeywordSpotter* spotter,
							   std::vector<uint8_t>* alarm,
							   std::function<void(bool enable)> servo):
	_audioSource(audioSource),
	_audioSink(audioSink),
	_spotter(spotter),
	_alarm(alarm),
	_buffer(bufferSize),
	_quietMode(false),
	_servo(servo)
{}

SajeMonitor::~SajeMonitor()
{}

void SajeMonitor::update() {
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


void SajeMonitor::raiseAlarm(const std::string& keyword) {
	// Don't play audio or print ticket while in quiet mode.
	if (_quietMode) {
		return;
	}
	// Turn the servo on.
	_servo(true);
	// Stop audio recording while the alarm is raised.
	_audioSource->pause();
	// Play audio and print the ticket at the same time.
	// Since the Pi only has one core and timing is somewhat
	// critical (for smooth audio playback), a tight loop to
	// update audio and ticket printing state will be executed.
	_audioSink->resume();
	_audioSink->playAsync(*_alarm);
	bool playing = true;
	while (playing) {
		playing = _audioSink->asyncUpdate();
	}
	// Stop audio playback.
	_audioSink->pause();
	// Enable recording again.
	_audioSource->resume();
	// Turn the servo off.
	_servo(false);
}

void SajeMonitor::setQuietMode(bool quietMode) {
	_quietMode = quietMode;
}
