///////////////////////////////////////////////////////////////////////////////
///
/// @file		FXOS8700.cpp
/// @brief		File for FXOS8700 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "FXOS8700.h"
#include "Module_Factory.h"

#include <Adafruit_Sensor.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
FXOS8700::FXOS8700(
		const byte		i2c_address,
		const uint8_t	mux_port
	)
	: I2CSensor("FXOS8700", i2c_address, mux_port)
	, inst_FXOS8700(Adafruit_FXOS8700(0x8700A, 0x8700B))
{
  LMark;
	bool setup = inst_FXOS8700.begin(ACCEL_RANGE_4G);

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
FXOS8700::FXOS8700(JsonArrayConst p)
	: FXOS8700(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void FXOS8700::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tAX: ", accel[0], " m/s^2");
	LPrintln("\tAY: ", accel[1], " m/s^2");
	LPrintln("\tAZ: ", accel[2], " m/s^2");
	LPrintln("\tMX: ", mag[0], " uT");
	LPrintln("\tMY: ", mag[1], " uT");
	LPrintln("\tMZ: ", mag[2], " uT");
}

///////////////////////////////////////////////////////////////////////////////
void FXOS8700::measure()
{
	sensors_event_t aevent, mevent;
  LMark;
	inst_FXOS8700.getEvent(&aevent, &mevent);

	accel[0] = aevent.acceleration.x;
	accel[1] = aevent.acceleration.y;
	accel[2] = aevent.acceleration.z;

	mag[0] = mevent.magnetic.x;
	mag[1] = mevent.magnetic.y;
	mag[2] = mevent.magnetic.z;
}

///////////////////////////////////////////////////////////////////////////////
void FXOS8700::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);

	data["ax"] = accel[0];
	data["ay"] = accel[1];
	data["az"] = accel[2];

	data["mx"] = mag[0];
	data["my"] = mag[1];
	data["mz"] = mag[2];
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
