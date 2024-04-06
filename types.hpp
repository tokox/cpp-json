#ifndef TOKOX_CPP_JSON_TYPES_HPP
#define TOKOX_CPP_JSON_TYPES_HPP

#include <any>
#include <string>
#include <vector>
#include <unordered_map>
#include <type_traits>

namespace tokox::json
{

class object;

enum value_type
{
	Null,
	Bool,
	Int,
	Float,
	String,
	Vector,
	Map
};

value_type any_to_value_type(const std::any& v);

template<typename T>
concept object_type = std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, bool> || std::is_same_v<T, long long int> || std::is_same_v<T, long double> || std::is_same_v<T, std::string> || std::is_same_v<T, std::vector<object>> || std::is_same_v<T, std::unordered_map<std::string, object>>;


template<value_type V>
using object_type_from_value_type = std::conditional_t<V == Null, std::nullptr_t, std::conditional_t<V == Bool, bool, std::conditional_t<V == Int, long long int, std::conditional_t<V == Float, long double, std::conditional_t<V == String, std::string, std::conditional_t<V == Vector, std::vector<object>, std::conditional_t<V == Map, std::unordered_map<std::string, object>, std::nullptr_t>>>>>>>;

template<object_type T>
value_type value_type_from_object_type();

}

#include "types.cpp"

#include "object.hpp"

#endif
