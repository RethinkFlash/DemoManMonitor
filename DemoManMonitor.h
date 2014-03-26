// Demolition Man Verbal Morality Statute Monitor
// Created by Tony DiCola (tony@tonydicola.com)
// Released under an MIT license (http://opensource.org/licenses/MIT).

// Main application.

#pragma once

#include <cstdint>
#include <vector>

#include "AudioSink.h"
#include "KeywordSpotter.h"

class DemoManMonitor {
public:
	DemoManMonitor(AudioSink* audioSink,
		KeywordSpotter* spotter,
		std::vector<uint8_t>* alarmWav);
	~DemoManMonitor();
	void update();

private:
	AudioSink* _audioSink;
	KeywordSpotter* _spotter;
	std::vector<uint8_t>* _alarmWav;

};
