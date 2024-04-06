#include <typeinfo>
#include <stdexcept>

namespace tokox::json
{

value_type any_to_value_type(const std::any& v)
{
	if (!v.has_value())
		return Null;
	else if (v.type() == typeid(object_type_from_value_type<Bool>))
		return Bool;
	else if (v.type() == typeid(object_type_from_value_type<Int>))
		return Int;
	else if (v.type() == typeid(object_type_from_value_type<Float>))
		return Float;
	else if (v.type() == typeid(object_type_from_value_type<String>))
		return String;
	else if (v.type() == typeid(object_type_from_value_type<Vector>))
		return Vector;
	else if (v.type() == typeid(object_type_from_value_type<Map>))
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
	else if constexpr (std::is_same_v<T, object_type_from_value_type<Int>>)
		return Int;
	else if constexpr (std::is_same_v<T, object_type_from_value_type<Float>>)
		return Float;
	else if constexpr (std::is_same_v<T, object_type_from_value_type<String>>)
		return String;
	else if constexpr (std::is_same_v<T, object_type_from_value_type<Vector>>)
		return Vector;
	else if constexpr (std::is_same_v<T, object_type_from_value_type<Map>>)
		return Map;
}

}
