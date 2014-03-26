// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

#include "DemoManMonitor.h"

#include <iostream>

using namespace std;

DemoManMonitor::DemoManMonitor(size_t bufferSize, AudioSource* audioSource, AudioSink* audioSink, KeywordSpotter* spotter, std::vector<uint8_t>* alarmWav):
	_audioSource(audioSource),
	_audioSink(audioSink), 
	_spotter(spotter),
	_alarmWav(alarmWav),
	_buffer(bufferSize)
{}

DemoManMonitor::~DemoManMonitor() 
{}

void DemoManMonitor::update() {
	// Grab a buffer of audio.
	_audioSource->record(_buffer);
	// Look for a keyword.
	string keyword = _spotter->process(_buffer);
	if (keyword != "") {
		// Keyword was spotted, sound alarm.
		cout << "==== KEYWORD SPOTTED: " << keyword << endl;
		// Stop the recording while audio plays.
		_audioSource->pause();
		// Enable the playback sink and play audio.
		_audioSink->resume();
		_audioSink->play(*_alarmWav);
		_audioSink->pause();
		// Enable recording again.
		_audioSource->resume();
	}
}
