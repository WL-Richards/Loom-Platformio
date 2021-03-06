///////////////////////////////////////////////////////////////////////////////
///
/// @file		SPI_Sensor.cpp
/// @brief		File for SPISensor implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "SPI_Sensor.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SPISensor::SPISensor(
		const char*				module_name,
		const uint8_t			num_samples 
	) 
	: Sensor(module_name, num_samples) {}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
