
///////////////////////////////////////////////////////////////////////////
// 
// Copyright(c) 2024, Apperoso Ltd
// All rights reserved.
//
// This source code is licensed under the BSD - style license found in the
// LICENSE file in the root directory of this source tree.
// 
///////////////////////////////////////////////////////////////////////////

#include "EnumString/enum_string.h"
#include <print>
#include <ranges>

enum class Fruit {
	apple,
	banana,
	cherry,
	grape,
	grapefruit,
	kiwi,
	lemon,
	lime,
	melon,
	orange,
	pear,
	pineapple,
	plum,
	raspberry,
	strawberry,

	enumSize
};

int main() {

	auto fruitNames = EnumString::makeEnumString<Fruit>();

	std::println("List of fruits:\n");

	for (auto const [index, name] : std::views::enumerate(fruitNames))
	{
		std::println("{:2}: {}", index, name);
	}
}
