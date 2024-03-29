#ifndef TOKOX_CPP_JSON_TYPES_HPP
#define TOKOX_CPP_JSON_TYPES_HPP

#include <typeinfo>
#include <any>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
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

value_type any_to_value_type(const std::any& v)
{
	if (!v.has_value())
		return Null;
	else if (v.type() == typeid(bool))
		return Bool;
	else if (v.type() == typeid(int))
		return Int;
	else if (v.type() == typeid(float))
		return Float;
	else if (v.type() == typeid(std::string))
		return String;
	else if (v.type() == typeid(std::vector<object>))
		return Vector;
	else if (v.type() == typeid(std::map<std::string, object>))
		return Map;
	else
		throw std::runtime_error("Unknown value_type!");
}

template<typename T>
concept object_type = std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, bool> || std::is_same_v<T, long long int> || std::is_same_v<T, long double> || std::is_same_v<T, std::string> || std::is_same_v<T, std::vector<object>> || std::is_same_v<T, std::map<std::string, object>>;


template<value_type V>
using object_type_from_value_type = std::conditional_t<V == Null, std::nullptr_t, std::conditional_t<V == Bool, bool, std::conditional_t<V == Int, long long int, std::conditional_t<V == Float, long double, std::conditional_t<V == String, std::string, std::conditional_t<V == Vector, std::vector<object>, std::map<std::string, object>>>>>>>;

template<object_type T>
value_type value_type_from_object_type()
{
	if constexpr (std::is_same_v<T, std::nullptr_t>)
		return Null;
	else if constexpr (std::is_same_v<T, bool>)
		return Bool;
	else if constexpr (std::is_same_v<T, long long int>)
		return Int;
	else if constexpr (std::is_same_v<T, long double>)
		return Float;
	else if constexpr (std::is_same_v<T, std::string>)
		return String;
	else if constexpr (std::is_same_v<T, std::vector<object>>)
		return Vector;
	else if constexpr (std::is_same_v<T, std::map<std::string, object>>)
		return Map;
}

}

#include "object.hpp"

#endif
