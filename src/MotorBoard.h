/**********************************************************************
DCC++ BASE STATION FOR ESP32

COPYRIGHT (c) 2017 Mike Dunston

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses
**********************************************************************/

#ifndef _MOTORBOARD_H_
#define _MOTORBOARD_H_

#include <vector>
#include <ArduinoJson.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include "DCCppProtocol.h"
#include "SignalGenerator.h"

enum MOTOR_BOARD_TYPE { ARDUINO_SHIELD, POLOLU, BTS7960B_5A, BTS7960B_10A };

class GenericMotorBoard {
public:
	GenericMotorBoard(adc1_channel_t, uint8_t, uint16_t, uint32_t, String);
	void powerOn(bool=true);
	void powerOff(bool=true, bool=false);
	void showStatus();
	void check();
	bool isOn() {
		return _state;
	}
	bool isOverCurrent() {
		return _triggered;
	}
	const String getName() {
		return _name;
	}
	const adc1_channel_t getADC1Channel() {
		return _senseChannel;
	}
	uint16_t getLastRead() {
		return _current;
	}
	const uint32_t getMaxMilliAmps() {
		return _maxMilliAmps;
	}
	float getCurrentDraw() {
		return (float)((_current * _maxMilliAmps) / 4096.0f);
	}
private:
	const String _name;
	const adc1_channel_t _senseChannel;
	const uint8_t _enablePin;
	const uint32_t _maxMilliAmps;
	const uint32_t _triggerValue;
	uint32_t _current;
	uint32_t _lastCheckTime;
	bool _state;
	bool _triggered;
	uint8_t _triggerClearedCountdown;
	uint8_t _triggerRecurrenceCount;
};

class MotorBoardManager {
public:
	static GenericMotorBoard *registerBoard(adc1_channel_t, uint8_t, MOTOR_BOARD_TYPE, String);
	static GenericMotorBoard *getBoardByName(String);
	static std::vector<String> getBoardNames();
	static uint8_t getMotorBoardCount();
	static void check();
	static void powerOnAll();
	static bool powerOn(const String);
	static void powerOffAll();
	static bool powerOff(const String);
	static int getLastRead(const String);
	static void showStatus();
	static void getState(JsonArray &);
};

class CurrentDrawCommand : public DCCPPProtocolCommand {
public:
	void process(const std::vector<String>);
	String getID() {
    return "c";
  }
};

class PowerOnCommand : public DCCPPProtocolCommand {
public:
	void process(const std::vector<String>);
	String getID() {
    return "1";
  }
};

class PowerOffCommand : public DCCPPProtocolCommand {
public:
	void process(const std::vector<String>);
	String getID() {
    return "0";
  }
};

#endif
