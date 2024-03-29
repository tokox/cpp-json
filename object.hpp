#ifndef TOKOX_CPP_JSON_OBJECT_HPP
#define TOKOX_CPP_JSON_OBJECT_HPP

#include "types.hpp"

namespace tokox::json
{

class object
{
public:
	object();

	object(const object& o);

	object(const std::any& v);

	template<object_type T>
	object(const T& v);


	object& operator=(const object& o);

	object& operator=(const std::any& v);

	template<object_type T>
	object& operator=(const T& v);


	const std::any& get_any() const;

	operator std::any() const;


	value_type get_value_type() const;

	operator value_type() const;


	template<object_type T>
	T& get_value();

	template<object_type T>
	const T& get_value() const;

	template<object_type T>
	operator T() const;


	template<value_type V>
	auto& get_value();

	template<value_type V>
	const auto& get_value() const;

private:
	std::any actual_object;
};

}

#include "object.cpp"


#endif
