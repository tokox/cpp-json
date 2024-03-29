#ifndef TOKOX_CPP_JSON_IMPORT_HPP
#define TOKOX_CPP_JSON_IMPORT_HPP

#include "object.hpp"

namespace tokox::json
{

class import_error : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};

constexpr const char* ERR_VAL = "Wrong value!";
constexpr const char* ERR_RNG = "Value out of range!";
constexpr const char* ERR_KEY = "Repeated map key!";
constexpr const char* ERR_END = "Unexpected end!";
constexpr const char* ERR_IMP = "Value not implemented!";

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
