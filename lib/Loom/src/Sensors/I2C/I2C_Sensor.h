///////////////////////////////////////////////////////////////////////////////
///
/// @file		I2C_Sensor.h
/// @brief		File for I2CSensor definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "../Sensor.h"
#include <Wire.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Abstract root for I2C sensor modules.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_i2_c_sensor.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#i2c-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class I2CSensor : public Sensor
{
protected:
	
	/// The sensor's I2C address.
	/// If the sensor supports mutliple, make sure this matches
	/// the current configuration of the i2c address 
	const byte		i2c_address;	

	/// Used with multiplexer, keep track of port it is on
	const uint8_t	port_num;		

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	i2c_address		The i2c address of the sensor 
	/// @param[in]	mux_portx		The port of the sensor if used with a multiplexer, 255 if not 
	/// @param[in]	num_samples		The number of samples to take and average
	I2CSensor(
			const char*				module_name,
			const byte				i2c_address,
			const uint8_t			mux_port	= 255,	// Default to 255 (not on mux)
			const uint8_t			num_samples	= 1		 
		);

	/// Destructor
	virtual ~I2CSensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the sensor's I2C address
	/// @return	The sensor's I2C address
	byte			get_i2c_address() const { return i2c_address; }; 	

private:

	/// Allow Multiplexer to access private methods of I2C sensor modules
	friend class Multiplexer;

};

///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS

