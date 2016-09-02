#include "MCP3425.h"


const double MCP3425::kLSBs[] = { 1000, 250.0, 62.5 };	// (in microvolts)

const int MCP3425::kPGAs[] = { 1, 2, 4, 8 };


MCP3425::MCP3425() {}


MCP3425::~MCP3425() {}


byte MCP3425::Configure(bool continuous, EResolution resolution, EGain gain)
{
	// Compile configuration byte
	byte config = (1 << 7) | (continuous << 4) | ((byte)resolution << 2) | (byte)gain;

	// Write to MCP3425 configuration register
	Wire.beginTransmission(I2CAddress);
	Wire.write(config);
	return Wire.endTransmission();
}


double MCP3425::Voltage()
{
	// TODO: Non-cont. voltage measurement
	//bool ready = false;
	//while (!ready) {}

	// Request data frame from MCP3425
	Wire.requestFrom((int)I2CAddress, 3);

	// Read incoming bytes
	byte high_byte, low_byte, config;
	high_byte = Wire.read();
	low_byte = Wire.read();
	config = Wire.read();

	// Parse configuration byte
	byte gain = config & true;
	byte res = (config >> 2) & 3;
	//bool cont = (bool)((config >> 4) & 3);
	bool ready = !(bool)((config >> 7) & 3);

	// Mask excess sign bits
	byte hb_mask = 0xF0 << (res * 2); // 0b11110000
	high_byte &= ~hb_mask;

	// Calculate data
	if (ready)
	{
		// Determine LSB & PGA factors
		double lsb = kLSBs[res];
		int pga = kPGAs[gain];

		// Calculate voltage
		int raw_adc = (high_byte * 256) + low_byte;
		double voltage = (raw_adc * lsb) / (pga * pow10(6));
		return voltage;
	}
	else
		return 0.0;
}
