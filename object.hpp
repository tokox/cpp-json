#ifndef TOKOX_CPP_JSON_OBJECT_HPP
#define TOKOX_CPP_JSON_OBJECT_HPP

#include "types.hpp"

namespace tokox::json
{

class object
{
public:
	object(): actual_object()
	{}

	object(const object& o): actual_object(o.get_any())
	{}

	object(const std::any& v): actual_object(v)
	{}

	template<object_type T>
	object(const T& v) : actual_object(v)
	{
		if constexpr (std::is_same_v<T, std::nullptr_t>)
			this->actual_object.reset();
	}

	object& operator=(const object& o)
	{
		this->actual_object = o.get_any();
		return (*this);
	}

	object& operator=(const std::any& v)
	{
		this->actual_object = v;
		return (*this);
	}

	template<object_type T>
	object& operator=(const T& v)
	{
		if constexpr (std::is_same_v<T, std::nullptr_t>)
			this->actual_object.reset();
		else
			this->actual_object = v;
		return (*this);
	}
	const std::any& get_any() const
	{
		return actual_object;
	}

	operator std::any() const
	{
		return this->get_any();
	}

	value_type get_value_type() const
	{
		return any_to_value_type(this->actual_object);
	}

	operator value_type() const
	{
		return this->get_value_type();
	}

	template<object_type T>
	T& get_value()
	{
		return *std::any_cast<T>(&this->actual_object);
	}

	template<object_type T>
	const T& get_value() const
	{
		return *std::any_cast<T>(&this->actual_object);
	}

	template<object_type T>
	operator T() const
	{
		return this->get_value<T>();
	}

	template<value_type V>
	auto& get_value()
	{
		return this->get_value<object_type_from_value_type<V>>();
	}

	template<value_type V>
	const auto& get_value() const
	{
		return this->get_value<object_type_from_value_type<V>>();
	}

private:
	std::any actual_object;
};

}


#endif
