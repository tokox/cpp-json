#ifndef TOKOX_CPP_JSON_EXPORT_HPP
#define TOKOX_CPP_JSON_EXPORT_HPP

#include <sstream>
#include <stdexcept>
#include "object.hpp"

namespace tokox::json
{

template<class IT>
IT& format(const object& obj, IT& it, int tab);

std::string& format(const object& obj, std::string& out_str, int tab = -1);

std::string format(const object& obj, int tab = -1);

std::ostream& format(const object& obj, std::ostream& out_stream, int tab = -1);

std::ostream& operator<<(std::ostream& out_stream, const object& obj);


template<class IT>
IT& format_null(const object& obj, IT& it, int = -1);

template<class IT>
IT& format_bool(const object& obj, IT& it, int = -1);

template<class IT>
IT& format_int(const object& obj, IT& it, int = -1);

template<class IT>
IT& format_float(const object& obj, IT& it, int = -1);

template<class IT>
IT& format_string(const object& obj, IT& it, int = -1);

template<class IT>
IT& format_vector(const object& obj, IT& it, int tab = -1);

template<class IT>
IT& format_map(const object& obj, IT& it, int tab = -1);

}

#include "export.cpp"

#endif
