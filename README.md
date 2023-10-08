# C++ JSON object import/export

This library contains class to represent **JSON object** and **functions to import/export it**.

## Overview

This library is **header-only** *(no precompiled files)* and **single-file** *(only one file for everything)*.
All contents are kept in **namespace `json`** as listed below.
It **does not** use any *external libraries* (**only** *standard C++ libraries*)

### Classes, constants and variables

- Class `json::object` represents **JSON object**. It is made on top of `std::variant`
- `json::object::variant` is `std::variant` used by `json::object`
- `json::object::value_type` is *an enum* representing **JSON objects types**
- Types are represented as following:

    | JSON type   | C++ type                              | `json::object::value_type` name | `json::object::value_type` value (`json::object::variant` index) |
    | :---------- | :------------------------------------ | :------------------------------ | :--------------------------------------------------------------- |
    | `Null`      | `std::monostate`                      | `Null`                          | `0`                                                              |
    | `Boolean`   | `bool`                                | `Bool`                          | `1`                                                              |
    | `Number`    | `long long int` and `long double`     | `Int` and `Float`               | `2` and `3`                                                      |
    | `String`    | `std::string`                         | `String`                        | `4`                                                              |
    | `Array`     | `std::vector<json::object>`           | `Vector`                        | `5`                                                              |
    | `Object`    | `std::map<std::string, json::object>` | `Map`                           | `6`                                                              |

- `json::import_error` is **an exception class** thrown by **import functions** (export functions don't throw (see below))
- `ERR_???` are **error messages**

- `json::float_prec` is `size_t` for **float precision**

### Functions for import:

| Function                                                                     | Description                                                                    |
| :--------------------------------------------------------------------------- | :----------------------------------------------------------------------------- |
| `json::object json::from(iterator& input_iterator, iterator input_end)`      | Main function for import. Detects what type of object it is and calls corresponding `json::TYPE_from` function. Takes iterator to current input character and input end (it may be iterator to `std::*stream`, `std::string`, `std::vector`, c-string - basically whatever you want). `iterator` is a template parameter. Returns `json::object` with imported JSON |
| `json::object json::TYPE_from(iterator& input_iterator, iterator input_end)` | Imports specific type TYPE (for every JSON type). They are called by `json::from` after type detection. They import specific type - if it isn't correct they throw `json::import_error` |
| `json::object json::from(std::string& input_string)`                         | Returns `json::from(input_string->iterator)` (first)                           |
| `json::object& json::from(json::object& obj, std::string& input_string)`     | Calls `json::from(input_string)` (above), saves result to `obj` and returns it |
| `json::object json::from(std::istream& input_stream)`                        | Returns `json::from(input_stream->iterator)` (first)                           |
| `json::object& json::from(json::object& obj, std::istream& input_stream)`    | Calls `json::from(input_stream)` (above), saves result to `obj` and returns it |
| `std::istream& operator>>(std::istream& input_stream, json::object& obj)`    | Calls `json::from(obj, input_stream)` (above) and returns `input_stream`       |

#### :bangbang: Important:

`json::number_from` is returning `json::object` containing `Int` (`long long int`) or `Float` (`long double`). It chooses **the better option**. Here is how it works:

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
| `iterator& json::to(json::object& obj, iterator& output_iterator, int tab = -1)`       | Main function for export. Checks what type of object it is and calls corresponding `json::TYPE_to` function. Takes JSON object, iterator to current output character and optional argument `tab` used for indentation (recursive objects). Negative `tab` means no indentation. `iterator` is a template parameter. Returns `output_iterator` |
| `iterator& json::TYPE_to(json::object& obj, iterator& output_iterator, int tab = -1)`  | Exports specific type TYPE (for every JSON type). They are called by `json::to` after type check. They export specific type - if it isn't correct `std::get (std::variant)` in `json::object::get_value` will throw `std::bad_variant_access` |
| `std::string& json::to(json::object& obj, std::string& output_string, int tab = -1)`   | Calls `json::to(obj, output_string->iterator, tab)` (first) and returns `output_string`                                  |
| `std::string json::to(json::object& obj, int tab = -1)`                                | Creates an instance of `std::string` and calls `json::to(obj, created_string, tab)` (above) and returns `created_string` |
| `std::ostream& json::to(json::object& obj, std::ostream& output_stream, int tab = -1)` | Calls `json::to(obj, output_stream->iterator, tab)` (first), returns `output_stream`                                     |
| `std::ostream& operator<<(std::ostream& output_stream, json::object& obj)`             | Returns `json::to(obj, output_stream)` (above)                                                                           |

#### Some useless license information

This library is licensed under the terms of [**the MIT license**](LICENSE.md). *Why did I put this here? I'm sure no one will care or even read this*
