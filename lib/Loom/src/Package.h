///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Package.h
/// @brief		File for functions to package data into JsonObject,
///				and extracting data from it
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "Macros.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
/// Add an list of alternating keys and values to a json data object
/// under the specified module
JsonObject get_module_data_object(JsonObject json, const char* module_name);

///////////////////////////////////////////////////////////////////////////////
/// Add timestamp to a Json object
void package_json_timestamp(JsonObject json, const char* date, const char* time);

///////////////////////////////////////////////////////////////////////////////
/// Convert data in key values in arrays in ojects to
/// keys and values in single object 'flatObj'
void flatten_json_data_object(JsonObject json);

///////////////////////////////////////////////////////////////////////////////
/// Convert data in key values in arrays in ojects to
/// alternating keys and values in array 'flatArray'
void flatten_json_data_array(JsonObject json);

//////////////////////////////////////////////////////////////////////////////
JsonArray add_config_temp(JsonObject json, const char* module_name);

///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom
