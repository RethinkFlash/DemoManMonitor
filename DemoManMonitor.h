// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Main application.

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "Adafruit_Thermal.h"
#include "AudioSink.h"
#include "AudioSource.h"
#include "KeywordSpotter.h"

class DemoManMonitor {
public:
	DemoManMonitor(size_t bufferSize, 
				   Adafruit_Thermal* printer,
				   AudioSource* audioSource,
				   AudioSink* audioSink,
				   KeywordSpotter* spotter,
				   std::vector<uint8_t>* alarmWav);
	~DemoManMonitor();
	void update();

private:
	void setFineSteps();
	void raiseAlarm(const std::string& keyword);

	Adafruit_Thermal* _printer;
	AudioSource* _audioSource;
	AudioSink* _audioSink;
	KeywordSpotter* _spotter;
	std::vector<uint8_t>* _alarmWav;
	std::vector<uint8_t> _buffer;
	std::vector<std::function<void, Adafruit_Thermal*>> _ticketSteps;

};
