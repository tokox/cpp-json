namespace tokox::json
{

object::object(): actual_object()
{}

object::object(const object& o): actual_object(o.get_any())
{}

object::object(const std::any& v): actual_object(v)
{}


template<object_type T>
object::object(const T& v) : actual_object(v)
{
	if constexpr (std::is_same_v<T, std::nullptr_t>)
		this->actual_object.reset();
}


object& object::operator=(const object& o)
{
	this->actual_object = o.get_any();
	return (*this);
}

object& object::operator=(const std::any& v)
{
	this->actual_object = v;
	return (*this);
}

template<object_type T>
object& object::operator=(const T& v)
{
	if constexpr (std::is_same_v<T, std::nullptr_t>)
		this->actual_object.reset();
	else
		this->actual_object = v;
	return (*this);
}
const std::any& object::get_any() const
{
	return actual_object;
}

object::operator std::any() const
{
	return this->get_any();
}

value_type object::get_value_type() const
{
	return any_to_value_type(this->actual_object);
}

object::operator value_type() const
{
	return this->get_value_type();
}

template<object_type T>
T& object::get_value()
{
	return *std::any_cast<T>(&this->actual_object);
}

template<object_type T>
const T& object::object::get_value() const
{
	return *std::any_cast<T>(&this->actual_object);
}

template<object_type T>
object::operator T() const
{
	return this->get_value<T>();
}

template<value_type V>
auto& object::get_value()
{
	return this->get_value<object_type_from_value_type<V>>();
}

template<value_type V>
const auto& object::get_value() const
{
	return this->get_value<object_type_from_value_type<V>>();
}

}
