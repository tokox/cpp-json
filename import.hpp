#ifndef TOKOX_CPP_JSON_IMPORT_HPP
#define TOKOX_CPP_JSON_IMPORT_HPP

#include <stdexcept>
#include "object.hpp"

namespace tokox::json
{

enum error_type
{
	INCORRECT_VALUE,
	VALUE_OUT_OF_RANGE,
	REPEATED_MAP_KEY,
	UNEXPECTED_END,
	VALUE_NOT_IMPLEMENTED
};

class import_error : public std::exception
{
public:
	import_error(const error_type _type): type(_type) {}

	const char* what() const noexcept override
	{
		switch (type)
		{
			case INCORRECT_VALUE:
				return "Incorrect value!";
			case VALUE_OUT_OF_RANGE:
				return "Value out of range!";
			case REPEATED_MAP_KEY:
				return "Repeated map key!";
			case UNEXPECTED_END:
				return "Unexpected end!";
			case VALUE_NOT_IMPLEMENTED:
				return "Value not implemented!";
			default:
				throw std::runtime_error("Unknown error_type!");
		}
	}
private:
	error_type type;
};

constexpr const size_t float_prec = 32;

template<class IT>
object from(IT& it, IT end);

object from(const std::string& json_str);

object from(std::istream& json_stream);

object& from(object& obj, const std::string& json_str);

object& from(object& obj, std::istream& json_stream);

std::istream& operator>>(std::istream& json_stream, object& obj);


template<class IT>
object null_from(IT& it, IT end);

template<class IT>
object bool_from(IT& it, IT end);

template<class IT>
object number_from(IT& it, IT end);

template<class IT>
object string_from(IT& it, IT end);

template<class IT>
object vector_from(IT& it, IT end);

template<class IT>
object map_from(IT& it, IT end);

}

#include "import.cpp"

#endif
