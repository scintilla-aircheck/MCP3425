#pragma once

#include <Arduino.h>
#include <Wire.h>


class MCP3425
{
public:
	// MCP3425 gain setting
	enum class EGain {
		x1,
		x2,
		x4,
		x8
	};

	// MCP3425 resolution setting
	enum class EResolution {
		d12Bit,
		d14Bit,
		d16Bit
	};

	// Default constructor/destructor
	MCP3425();
	~MCP3425();

	// Default device I2C address
	static const byte I2CAddress = 0x68; // 1101 000

	byte Configure(bool continuous, EResolution resolution, EGain gain);

	double Voltage();

private:
	// Resolution and gain factors
	static const double kLSBs[3];
	static const int kPGAs[4];
};
