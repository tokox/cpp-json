#ifndef TOKOX_CPP_JSON_IMPORT_HPP
#define TOKOX_CPP_JSON_IMPORT_HPP

#include <stdexcept>
#include "object.hpp"

namespace tokox::json
{

class parse_error : public std::exception
{
public:
	enum error_type
	{
		INCORRECT_VALUE,
		VALUE_OUT_OF_RANGE,
		REPEATED_MAP_KEY,
		UNEXPECTED_END,
		VALUE_NOT_IMPLEMENTED
	};

	parse_error(const error_type _type): type(_type) {}

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
				return "Unknown error!";
		}
	}
private:
	error_type type;
};

constexpr const size_t float_prec = 32;

template<class IT>
object parse(IT& it, IT end);

object parse(const std::string& json_str);

object parse(std::istream& json_stream);

object& parse(object& obj, const std::string& json_str);

object& parse(object& obj, std::istream& json_stream);

std::istream& operator>>(std::istream& json_stream, object& obj);


template<class IT>
object parse_null(IT& it, IT end);

template<class IT>
object parse_bool(IT& it, IT end);

template<class IT>
object parse_number(IT& it, IT end);

template<class IT>
object parse_string(IT& it, IT end);

template<class IT>
object parse_vector(IT& it, IT end);

template<class IT>
object parse_map(IT& it, IT end);

}

#include "import.cpp"

#endif
