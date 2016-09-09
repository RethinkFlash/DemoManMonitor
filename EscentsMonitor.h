// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Main application.

#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "AudioSink.h"
#include "AudioSource.h"
#include "KeywordSpotter.h"

class EscentsMonitor {
public:
	EscentsMonitor(size_t bufferSize,
				   AudioSource* audioSource,
				   AudioSink* audioSink,
				   KeywordSpotter* spotter,
				   std::vector<uint8_t>* alarm,
			   	   int servoPin,
				   int readyLedPin);
	~EscentsMonitor();
	void update();
    int pwmWire;

private:
	void raiseAlarm(const std::string& keyword);

	AudioSource* _audioSource;
	AudioSink* _audioSink;
	KeywordSpotter* _spotter;
	std::vector<uint8_t>* _alarm;
	std::vector<uint8_t> _buffer;
	int _arduinoSerial;
    int _servoPin;
    int _readyLedPin;
};
