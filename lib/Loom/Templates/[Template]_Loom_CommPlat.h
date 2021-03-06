///////////////////////////////////////////////////////////////////////////////
///
/// @file		CommPlatTemplate.h
/// @brief		File for CommPlatTemplate definition.
/// @author		FirstName LastName
/// @date		Year
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_RADIOS
#pragma once

#include "CommPlat.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////


// ### (CommPlat) | dependencies: [] | conflicts: []
/// Class description
// ###
class CommPlatTemplate : public CommPlat
{

protected:

	// Protected members here (are inherited)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	CommPlatTemplate(
			uint			max_message_len	= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	CommPlatTemplate(JsonArrayConst p);

	/// Destructor
	virtual ~CommPlatTemplate() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		power_down() override {}
	void		power_up() override {}

	bool		receive(JsonObject json) override;
	bool		send(JsonObject json, uint16_t destination) override;
	bool		send(JsonObject json) override;
	void		broadcast(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint		get_address() override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void		set_address(uint addr) override;

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================



private:

	// Private members (are not inherited)

	// Auxiliary functions

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, CommPlatTemplate, "CommPlatTemplate");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_RADIOS
