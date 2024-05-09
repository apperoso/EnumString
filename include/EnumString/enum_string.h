
///////////////////////////////////////////////////////////////////////////
// 
// Copyright(c) 2024, Apperoso Ltd
// All rights reserved.
//
// This source code is licensed under the BSD - style license found in the
// LICENSE file in the root directory of this source tree.
// 
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <array>
#include <string_view>
#include <source_location>
#include <algorithm>

// EnumString - Compile-time generation of enum strings using C++23
// 
// Inspired by the magic_enum [https://github.com/Neargye/magic_enum]
//
// This version will convert a scoped enum into an array of strings representing each entry
//
// The scoped enum must be defined as follows:
// - Members must use the default values (i.e. start at zero, and increase in value by 1)
// - The final member must be called "enumSize"
//
// For example: enum class Fruit { apple, banana, pear, enumSize };
//

namespace EnumString {

	// The SizedEnum concept is used to ensure the enum has an "enumSize" member as detailed above
	template<typename EnumT>
	concept SizedEnum = std::is_scoped_enum_v<EnumT> and requires(EnumT) {
		EnumT::enumSize;
	};

	// Basic implementation of a compile-time string, used to store the extracted enum names
	// Note: This version doesn't have special handling for a terminating '\0' as it is only
	// intended for use with a substring
	template <auto Size>
	struct FixedString {
		char data_[Size];

		constexpr explicit FixedString(char const* str) {
			std::copy_n(str, Size, data_);
		}

		constexpr operator std::string_view() const noexcept {
			return { data_, Size };
		}
	};

	using namespace std::string_view_literals;
	static_assert("name"sv == FixedString<4>("name"));
	static_assert("test"sv == FixedString<4>("retested"sv.substr(2,4).data()));

	// The EnumString is stored in an array, with the std::string_view referencing the FixedString data
	template<SizedEnum EnumT>
	using EnumStringArray = std::array<std::string_view, std::to_underlying(EnumT::enumSize)>;

	// n() generates a string representing its function signature, which is used to extract the enum name
	// 
	// The name is intentionally short to minimise the number of characters that need to be searched
	// 
	// Note: Ideally n() would return the extracted enum name in a FixedString here. In practice this
	//		 results in a mal-formed string, because in order to deduce the return type for the 
	//		 template, the compiler has to start with the "auto" version of the function signature string 
	//		 returned from source_location, which then creates a FixedString with a size based on this. 
	// 
	//		 However when the function is then run after template substitution, source_location generates
	//		 the correct function signature with auto replaced with "struct EnumString::FixedString<nn>"
	// 
	//		 For example: If n() is called with EnumT=Fruit::apple, then the function signature 
	//		 used to calculate the return type is:
	// 
	//			"auto __cdecl EnumString::n<Fruit::apple>(void) noexcept"
	// 
	//		 and the FixedString needs to store 55 chars.
	// 
	//		 When the actual function template runs, the signature becomes:
	// 
	//			"struct EnumString::FixedString<55> __cdecl EnumString::n<Fruit::apple>(void) noexcept"
	// 
	//		 but only the first 55 characters can be stored. 
	// 
	//		 I.e. if we compare function signatures:
	// 
	//			"auto __cdecl EnumString::n<Fruit::apple>(void) noexcept"
	//			"struct EnumString::FixedString<55> __cdecl EnumString::n<Fruit::apple>(void) noexcept"
	//
	//		 It becomes clear why the string is truncated to: "struct EnumString::FixedString<55> __cdecl EnumString::"
	//
	//		 Therefore the return type is fixed at char const*, which keeps the signature constant between the phases
	//		 described above!
	//		 The returned char const* can then be converted into a FixedString by the caller
	template <auto E>
	constexpr auto n() noexcept { return std::source_location::current().function_name(); }

	// Extract the enum name from the string returned by n() and return as a FixedString
	// The enum is located between the final ':' and subsequent '>', for example with the signature:
	//
	//		"auto __cdecl EnumString::n<Fruit::apple>(void) noexcept"
	// 
	// we extract "apple"
	//
	// Note: We assume that the use of the SizedEnum concept means we always get a valid function signature generated,
	//		 so there is no error checking provided
	template<SizedEnum EnumT, EnumT E>
	constexpr auto toEnumName() noexcept {
		constexpr std::string_view funcName{ n<E>() };

		constexpr auto start = funcName.find_last_of(':') + 1;
		constexpr auto end = funcName.find_first_of('>', start);

		constexpr auto nameView = funcName.substr(start, end-start);

		return FixedString<nameView.size()>{nameView.data()};
	}

	// Provide a storage location for each FixedString returned from toEnumName()
	template <SizedEnum EnumT, EnumT E>
	inline constexpr auto enumName = toEnumName<EnumT, E>();

	// Create the enum string array from an index sequence which represents the integer
	// value of each entry in the scoped enum (excluding the enumSize member)
	template<SizedEnum EnumT, std::size_t... I>
	constexpr auto generateEnumNames(std::index_sequence<I...>)
	{
		return EnumStringArray<EnumT>{ enumName<EnumT, static_cast<EnumT>(I)>... };
	}

	// Storage location for the final EnumArray generated by generateEnumNames()
	template<SizedEnum EnumT>
	inline constexpr auto enumNames = generateEnumNames<EnumT>(std::make_index_sequence<std::to_underlying(EnumT::enumSize)>{});

	// The entry point to generate the EnumString array
	template<SizedEnum EnumT>
	constexpr EnumStringArray<EnumT> makeEnumString()
	{
		return enumNames<EnumT>;
	}

}
