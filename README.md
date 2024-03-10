# C++ JSON object import/export

This library contains class to represent **JSON object** and **functions to import/export it**.

## Overview

This library is **header-only** *(no precompiled files)* and **single-file** *(only one file for everything)*.
All contents are kept in **namespace `tokox::json`** as listed below.
It **does not** use any *external libraries* (**only** *standard C++ libraries*).
Test program called `test.cpp` is just reading and writing same one JSON object.

### Classes, constants and variables

- Class `object` represents **JSON object**. It is made on top of `std::variant`
- `object::variant` is `std::variant` used by `object`
- `object::value_type` is *an enum* representing **JSON objects types**
- Types are represented as following:

    | JSON type   | C++ type                              | `object::value_type` name | `object::value_type` value (`object::variant` index) |
    | :---------- | :------------------------------------ | :------------------------ | :--------------------------------------------------- |
    | `Null`      | `std::monostate`                      | `Null`                    | `0`                                                  |
    | `Boolean`   | `bool`                                | `Bool`                    | `1`                                                  |
    | `Number`    | `long long int` and `long double`     | `Int` and `Float`         | `2` and `3`                                          |
    | `String`    | `std::string`                         | `String`                  | `4`                                                  |
    | `Array`     | `std::vector<object>`                 | `Vector`                  | `5`                                                  |
    | `Object`    | `std::map<std::string, object>`       | `Map`                     | `6`                                                  |

- `import_error` is **an exception class** thrown by **import functions** (export functions don't throw (see below))
- `ERR_???` are **error messages**

- `float_prec` is `size_t` for **float precision**

### Functions for import:

| Function                                                                     | Description                                                                    |
| :--------------------------------------------------------------------------- | :----------------------------------------------------------------------------- |
| `object from(iterator& input_iterator, iterator input_end)`                  | Main function for import. Detects what type of object it is and calls corresponding `TYPE_from` function. Takes iterator to current input character and input end (it may be iterator to `std::*stream`, `std::string`, `std::vector`, c-string - basically whatever you want). `iterator` is a template parameter. Returns `object` with imported JSON |
| `object TYPE_from(iterator& input_iterator, iterator input_end)`             | Imports specific type TYPE (for every JSON type). They are called by `from` after type detection. They import specific type - if it isn't correct they throw `import_error` |
| `object from(std::string& input_string)`                                     | Returns `from(input_string->iterator)` (first)                                 |
| `object& from(object& obj, std::string& input_string)`                       | Calls `from(input_string)` (above), saves result to `obj` and returns it       |
| `object from(std::istream& input_stream)`                                    | Returns `from(input_stream->iterator)` (first)                                 |
| `object& from(object& obj, std::istream& input_stream)`                      | Calls `from(input_stream)` (above), saves result to `obj` and returns it       |
| `std::istream& operator>>(std::istream& input_stream, object& obj)`          | Calls `from(obj, input_stream)` (above) and returns `input_stream`             |

#### :bangbang: Important:

`number_from` is returning `object` containing `Int` (`long long int`) or `Float` (`long double`). It chooses **the better option**. Here is how it works:

| Condition for number                                                                                     | `Int` (`long long int`) or `Float` (`long double`) |
| :------------------------------------------------------------------------------------------------------- | :------------------------------------------------- |
| doesn't fit in `long long int` (less than one or more than `std::numeric_limits<long long int>::max()`)  | `Float` (`long double`)                            |
| fits in `long long int` and is an integer                                                                | `Int` (`long long int`)                            |
| fits in `long long int` and `long double` loses precision (is less precise than rounded `long long int`) | `Int` (`long long int`)                            |
| Well, `long long int` isn't any better than `long double` there! (Other)                                 | `Float` (`long double`)                            |

You can always **cast it** to the other one **after import**.

### Functions for export:

| Function                                                                               | Description |
| :------------------------------------------------------------------------------------- | :---------- |
| `iterator& to(object& obj, iterator& output_iterator, int tab = -1)`                   | Main function for export. Checks what type of object it is and calls corresponding `TYPE_to` function. Takes JSON object, iterator to current output character and optional argument `tab` used for indentation (recursive objects). Negative `tab` means no indentation. `iterator` is a template parameter. Returns `output_iterator` |
| `iterator& TYPE_to(object& obj, iterator& output_iterator, int tab = -1)`              | Exports specific type TYPE (for every JSON type). They are called by `to` after type check. They export specific type - if it isn't correct `std::get (std::variant)` in `object::get_value` will throw `std::bad_variant_access` |
| `std::string& to(object& obj, std::string& output_string, int tab = -1)`               | Calls `to(obj, output_string->iterator, tab)` (first) and returns `output_string` |
| `std::string to(object& obj, int tab = -1)`                                            | Creates an instance of `std::string` and calls `to(obj, created_string, tab)` (above) and returns `created_string` |
| `std::ostream& to(object& obj, std::ostream& output_stream, int tab = -1)`             | Calls `to(obj, output_stream->iterator, tab)` (first), returns `output_stream` |
| `std::ostream& operator<<(std::ostream& output_stream, object& obj)`                   | Returns `to(obj, output_stream)` (above) |

#### Some useless license information

This library is licensed under the terms of [**the MIT license**](LICENSE.md). *Why did I put this here? I'm sure no one will care or even read this*
