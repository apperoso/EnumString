# EnumString
## Compile-time generation of enum strings using C++23

Inspired by the magic_enum [https://github.com/Neargye/magic_enum], this code was created as a learning exercise whilst working on another project, and has been shared in the hope that others may find some of the ideas useful.

This version will convert a scoped enum into an array of strings representing each entry.

The code has been tested on Windows 11 using Visual Studio 2022 version 17.9.2. It will need some edits to support other platforms.

# Example

Create a scoped enum that satisfies the following conditions:
- Members must use the default values (i.e. start at zero, and increase in value by 1)
- The final member must be called "enumSize"

The example included in the main.cpp file is:

```cpp
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
```

Generate an enum string by calling makeEnumString: 

```cpp
auto fruitNames = EnumString::makeEnumString<Fruit>();
```

fruitNames will be an array of strings containing the names of the members of Fruit (excluding enumSize). Its type will be `std::array<std::string_view, 15>`

The code in main() demonstrates creating the enum string and printing the values:

```cpp
auto fruitNames = EnumString::makeEnumString<Fruit>();

std::println("List of fruits:\n");

for (auto const [index, name] : std::views::enumerate(fruitNames))
{
	std::println("{:2}: {}", index, name);
}
```

The output from this code is:

```cpp
List of fruits:

 0: apple
 1: banana
 2: cherry
 3: grape
 4: grapefruit
 5: kiwi
 6: lemon
 7: lime
 8: melon
 9: orange
10: pear
11: pineapple
12: plum
13: raspberry
14: strawberry
```

File `enum_string.h` contains more explanation of how the code works.

## Use of C++23

The following C++23 techniques were used in this project:

## enum_string.h

Concept `std::is_scoped_enum` to ensure only scoped enums are used to generate enum strings

`std::to_underlying` to simplify converting e.g. Fruit::enumSize to its integer representation (15)

## main.cpp

`<print>` header for `std::println()`

`std::views::enumerate` to create an index for each entry iterated in the range-based for loop
