///////////////////////////////////////////////////////////////////////////////
///
/// @file		Multiplexer.cpp
/// @brief		File for Multiplexer implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "Multiplexer.h"
#include "Module_Factory.h"

// I2C Sensor files
#include "Sensors/I2C/ADS1115.h"
#include "Sensors/I2C/AS7262.h"
#include "Sensors/I2C/AS7263.h"
#include "Sensors/I2C/AS7265X.h"
#include "Sensors/I2C/FXAS21002.h"
#include "Sensors/I2C/FXOS8700.h"
#include "Sensors/I2C/LIS3DH.h"
#include "Sensors/I2C/MB1232.h"
#include "Sensors/I2C/MMA8451.h"
#include "Sensors/I2C/MPU6050.h"
#include "Sensors/I2C/MS5803.h"
#include "Sensors/I2C/SHT31D.h"
#include "Sensors/I2C/TMP007.h"
#include "Sensors/I2C/TSL2591.h"
#include "Sensors/I2C/ZXGesture.h"
#include "Sensors/I2C/STEMMA.h"

#include <Wire.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

// Used to optimize searching for sensors:
// search addresses in array rather than 0-127
const std::array<byte, 21> Multiplexer::known_addresses =
{
	0x10, ///< ZXGESTURESENSOR
	0x11, ///< ZXGESTURESENSOR
	0x19, ///< LIS3DH
	0x1C, ///< MMA8451 / FXOS8700
	0x1D, ///< MMA8451 / FXOS8700
	0x1E, ///< FXOS8700
	0x1F, ///< FXOS8700
	0x20, ///< FXAS21002
	0x21, ///< FXAS21002
	0x29, ///< TSL2591
	0x36, ///< STEMMA
	0x40, ///< TMP007
	0x44, ///< SHT31D
	0x45, ///< SHT31D
	0x49, ///< AS726X / AS7265X
	0x68, ///< MPU6050
	0x69, ///< MPU6050
	0x70, ///< MB1232
	0x76, ///< MS5803
	0x77  ///< MS5803
};

const std::array<byte, 9>  Multiplexer::alt_addresses = {
	0x70,
	0x71,
	0x72,
	0x73,
	0x74,
	0x75,
	0x76,
	0x77,
	0x78
};

///////////////////////////////////////////////////////////////////////////////
Multiplexer::Multiplexer(
		const byte					i2c_address,
		const uint8_t				num_ports,
		const bool					dynamic_list,
		const uint					update_period
	)
	: Module("Multiplexer")
	, i2c_address(i2c_address)
	, num_ports(num_ports)
	, update_period(update_period)
	, sensors(new I2CSensor*[num_ports])
	, control_port(num_ports)
{
	// Start Multiplexer
  this->power_up();

	// Initialize array of sensors to Null pointrs
	for (auto i = 0; i < num_ports; i++) {
		sensors[i] = nullptr;
	}

	i2c_conflicts = find_i2c_conflicts();

	// Update sensor list and display   -- currently removed because Mux should be linked to DeviceManager before polling sensors
	// refresh_sensors();
	// print_state();

	print_module_label();
	LPrintln("Setup Complete");
}

///////////////////////////////////////////////////////////////////////////////
Multiplexer::Multiplexer(JsonArrayConst p)
	: Multiplexer(EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
Multiplexer::~Multiplexer()
{
	// Free any sensors
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != nullptr) {
			delete sensors[i];
		}
	}
	delete sensors;
}

///////////////////////////////////////////////////////////////////////////////
I2CSensor* Multiplexer::generate_sensor_object(const byte i2c_address, const uint8_t port)
{
		LPrintln("Adding Sensor at address:", i2c_address);
    LMark;
		switch (i2c_address) {
			case 0x10 : return new ZXGesture(i2c_address, port);	break;  // ZXGesture
			case 0x11 : return new ZXGesture(i2c_address, port);	break;	// ZXGesture
			case 0x19 : return new LIS3DH(i2c_address, port);		break;	// LIS3DH

			case 0x1C : // MMA8451 / FXOS8700
				if (i2c_0x1C == I2C_Selection::L_MMA8451)  return new MMA8451(i2c_address, port);	// MMA8451
				if (i2c_0x1C == I2C_Selection::L_FXOS8700) return new FXOS8700(i2c_address, port);	// FXOS8700
			break;

			case 0x1D : // MMA8451 / FXOS8700
				if (i2c_0x1D == I2C_Selection::L_MMA8451)  return new MMA8451(i2c_address, port);	// MMA8451
				if (i2c_0x1D == I2C_Selection::L_FXOS8700) return new FXOS8700(i2c_address, port);	// FXOS8700
			break;

			case 0x1E : return new FXOS8700(i2c_address, port);	break;		// FXOS8700
			case 0x1F : return new FXOS8700(i2c_address, port);	break;		// FXOS8700
			case 0x20 : return new FXAS21002(i2c_address, port);	break;		// FXAS21002
			case 0x21 : return new FXAS21002(i2c_address, port);	break;		// FXAS21002

			case 0x29 : // TSL2591
				if (i2c_0x29 == I2C_Selection::L_TSL2591) return new TSL2591(i2c_address, port);	// TSL2591
			break;

			case 0x36 : return new STEMMA(i2c_address, port);	break; // STEMMA
			case 0x40 : return new TMP007(i2c_address, port);	break;	// TMP007
			case 0x44 : return new SHT31D(i2c_address, port);	break;	// SHT31D
			case 0x45 : return new SHT31D(i2c_address, port);	break;	// SHT31D

			case 0x49 : // AS726X / AS7265X
				if (i2c_0x49 == I2C_Selection::L_AS7262 ) return new AS7262(i2c_address, port);		// AS7262
				if (i2c_0x49 == I2C_Selection::L_AS7263 ) return new AS7263(i2c_address, port);		// AS7263
				if (i2c_0x49 == I2C_Selection::L_AS7265X) return new AS7265X(i2c_address, port);	// AS7265X
			break;

			case 0x68 : return new MPU6050(i2c_address, port);	break;	// MPU6050
			case 0x69 : return new MPU6050(i2c_address, port);	break;	// MPU6050
			case 0x70 : return new MB1232(i2c_address, port);	break;	// MB1232
			case 0x76 : return new MS5803(i2c_address, port);	break;	// MS5803
			case 0x77 : return new MS5803(i2c_address, port);	break;	// MS5803

			default : return nullptr;
		}
}


///////////////////////////////////////////////////////////////////////////////
void Multiplexer::print_config() const
{
	Module::print_config();

	LPrint("\tI2C Address        : ");
	LPrintln_Hex(i2c_address);
	LPrintln("\tNum Ports          : ", num_ports);
	LPrintln("\tUpdate Period (ms) : ", update_period);
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::print_state() const
{
	print_module_label();
	LPrintln("Attached Sensors:");

	for (auto i = 0U; i < num_ports; i++) {
		LPrint("\tPort ", i, ": ");
    LMark;
		if (sensors[i] != nullptr) {
			LPrint_Dec_Hex(sensors[i]->get_i2c_address());
			LPrintln(" - ", sensors[i]->get_module_name() );
		} else {
			LPrintln(" -");
		}
	}
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::measure()
{
	refresh_sensors();

	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != nullptr) {
			tca_select(i);
			sensors[i]->measure();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::print_measurements() const
{
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != nullptr) {
			tca_select(i);
			sensors[i]->print_measurements();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::package(JsonObject json)
{
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != NULL) {
			tca_select(i);
			sensors[i]->package(json);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::get_sensor_list(JsonObject json)
{
	refresh_sensors();

	json["type"] = "state";
  LMark;
	JsonObject list = json.createNestedObject("MuxSensors");

	char tmp[3];
	for (auto i = 0U; i < num_ports; i++) {
    LMark;
		if (sensors[i] != NULL) {
      LMark;
			itoa(i, tmp, 10);
			list[tmp] = sensors[i]->get_module_name();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::refresh_sensors()
{
	byte previous, current;

  // update conflicts
  i2c_conflicts = find_i2c_conflicts();

	for (auto i = 0; i < num_ports; i++) {
    LMark;

		// LPrintln("TCA Port: ", i);
		// tca_select(i);

		previous = (sensors[i] != nullptr) ? sensors[i]->get_i2c_address() : 0x00;
		// LPrint("\tPrevious: 0x");
		// LPrintln_Hex(previous);

		current = get_i2c_on_port(i);

		// LPrint("Current I2C on port ", i, " : ");
		// LPrintln_Dec_Hex(current);

		// Cases:
		// No change (prev = current)
			// Do nothing
		// Sensor removed (prev ??? 0, current = 0)
			// Free removed sensor object memory
		// Sensor added (prev = 0, currect ??? 0)
			// Create object
		// Sensor switched (prev ??? current)
			// Delete old object
			// Create new object

		// Sensor removed, added, or swapped
		if (previous != current) {

			if ( (previous != 0) || (current == 0) ) {
				// Free object
				print_module_label();
				LPrintln("Free Memory of ", sensors[i]->get_module_name() );
        LMark;
				delete sensors[i];
			}

			// Create new sensor object and setup (in constructor)
			sensors[i] = generate_sensor_object(current, i);

			if (sensors[i] != nullptr) {

				if (sensors[i]->get_active()) {

					// Make sure sensor is also linked to DeviceManager
					sensors[i]->link_device_manager(device_manager);
					// device_manager->add_module(sensors[i]);


					print_module_label();
					LPrintln("Added ", sensors[i]->get_module_name() );
				} else {
					// Sensors will switch themselves to inactive if they dont
					// properly initialize.
					// If so, don't add sensor
					print_module_label();
					LPrintln(sensors[i]->get_module_name(), " failed to initialize");
          LMark;

					delete sensors[i];
					sensors[i] = nullptr;
				}

			}
		}

		// Otherwise still no sensor or sensor did not
		// change. In which case, do nothing.
	}
}

///////////////////////////////////////////////////////////////////////////////
byte Multiplexer::get_i2c_on_port(const uint8_t port) const
{
	tca_select(port);
	byte addr;

	// Iterate through known addresses try to get confirmation from sensor
	// for (auto addr = 1; addr <= 127; addr++) {
	for (const auto addr : known_addresses) {
        // if this address is on the conflict list, skip it
        if (i2c_conflict(addr) || addr == this->i2c_address) { continue; }

		Wire.beginTransmission(addr);
		byte error = Wire.endTransmission();

		if (error == 0) return addr;
	}

	return 0x00; // No sensor found
}

bool Multiplexer::i2c_conflict(byte addr) const {
    for (byte conflict : i2c_conflicts)
        if (conflict == addr)
            return true;
    return false;
}

std::vector<byte> Multiplexer::find_i2c_conflicts() {

    tca_deselect();
    std::vector<byte> i2c_conflicts_local;
    byte addr;
    // go through all the potentially conflicting sensors and find the ones that respond to blacklist them.
    for (auto j = 0; j < known_addresses.size(); j++) {
        LMark;

        addr = known_addresses[j];

        Wire.beginTransmission(addr);
        byte error = Wire.endTransmission();

        if (error == 0) {
            i2c_conflicts_local.push_back(addr);
        }
    }

    return i2c_conflicts_local;
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::tca_select(const uint8_t port) const
{
	if (port < num_ports) {
    LMark;
		Wire.beginTransmission(i2c_address);
		Wire.write(1 << port);
		Wire.endTransmission();
	}
}

///////////////////////////////////////////////////////////////////////////////
void Multiplexer::tca_deselect() const
{
  LMark;
	Wire.beginTransmission(i2c_address);
	Wire.write(0);
	Wire.endTransmission();
}

///////////////////////////////////////////////////////////////////////////////

void Multiplexer::power_up() {
  LMark;
// Begin I2C
	Wire.begin();

	Wire.beginTransmission(this->i2c_address);
	if(Wire.endTransmission() ) { //< Test on this address
		//< Test Failed
		LPrintln("Multiplexer not found on specified port. Checking alternate addresses.");
		this->active = false;

		//< Check all alternate addresses
		for(auto address : alt_addresses) {
      LMark;
			Wire.beginTransmission(address);
			if(Wire.endTransmission()) {
				continue;
			} else {
				active = true;
				this->i2c_address = address;
				LPrintln("*** Multiplexer found at: ", address, ", update your config. ***");
			}
		}
	}
	//< Test succeded
	else
		this->active = true;
}

///////////////////////////////////////////////////////////////////////////////

void Multiplexer::power_down() { }

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
