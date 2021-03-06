///////////////////////////////////////////////////////////////////////////////

// This example has a few different configurations provided, but only one can
// be used at a time.

// The purpose of each config is as follows:

// - config.h          : A basic analog and digital logging to Max
// - config_neopixel.h : Use to control a neopixel on pins A0, A1, or A2
// - config_relay.h    : Use to control a relay on pin 10
// - config_servo.h    : Use to control servos using Adafruit fetherwing
//							servo controller V2
// - config_stepper.h  : Use to control steppers using Adafruit featherwing
//							stepper controller V2

// To switch between which configuration will be loaded to the device, change
//	the '#include "config.h"' line in the SimpleMax.ino sketch file to to
//	configuration you want to use.

// You can combine modules from the different configurations as hardware permits.

// See the documentation (https://openslab-osu.github.io/Loom/html/index.html)
//	for available options for modules.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In config, instance number is number to set ModuleCore to in Max MSP
// Include configuration
const char* json_config =
#include "config.h"
;

// You may change what you have enabled depending on which config you are using
// In Tools menu, set:
// Internet  > WiFi
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Feather{};


void setup()
{
	Feather.begin_LED();
	Feather.begin_serial(false);
	Feather.parse_config(json_config);
	Feather.print_config(true);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
	Feather.measure();
	Feather.package();
	Feather.display_data();
	getMaxPub(Feather).publish();
	getMaxSub(Feather).subscribe();
	Feather.pause();
}
