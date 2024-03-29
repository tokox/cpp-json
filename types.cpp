#include <typeinfo>
#include <stdexcept>

namespace tokox::json
{

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
