#ifndef TOKOX_CPP_JSON_EXPORT_HPP
#define TOKOX_CPP_JSON_EXPORT_HPP

#include <sstream>
#include <stdexcept>
#include "object.hpp"

namespace tokox::json
{

template<class IT>
IT& to(const object& obj, IT& it, int tab);

std::string& to(const object& obj, std::string& out_str, int tab = -1);

std::string to(const object& obj, int tab = -1);

std::ostream& to(const object& obj, std::ostream& out_stream, int tab = -1);

std::ostream& operator<<(std::ostream& out_stream, const object& obj);


template<class IT>
IT& null_to(const object& obj, IT& it, int = -1);

template<class IT>
IT& bool_to(const object& obj, IT& it, int = -1);

template<class IT>
IT& int_to(const object& obj, IT& it, int = -1);

template<class IT>
IT& float_to(const object& obj, IT& it, int = -1);

template<class IT>
IT& string_to(const object& obj, IT& it, int = -1);

template<class IT>
IT& vector_to(const object& obj, IT& it, int tab = -1);

template<class IT>
IT& map_to(const object& obj, IT& it, int tab = -1);

}

#include "export.cpp"

#endif
