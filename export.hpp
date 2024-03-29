#ifndef TOKOX_CPP_JSON_EXPORT_HPP
#define TOKOX_CPP_JSON_EXPORT_HPP

#include <sstream>
#include <stdexcept>
#include "object.hpp"

namespace tokox::json
{

template<class IT>
IT& to(const object& obj, IT& it, int tab = -1);

template<class IT>
IT& null_to(const object& obj, IT& it, int = -1)
{
	obj.get_value<Null>();
	return put(it, "null");
}

template<class IT>
IT& bool_to(const object& obj, IT& it, int = -1)
{
	return put(it, obj.get_value<Bool>() ? "true" : "false");
}

template<class IT>
IT& int_to(const object& obj, IT& it, int = -1)
{
	auto oint = obj.get_value<Int>();
	if (oint == 0)
	{
		put(it, '0');
	}
	else
	{
		std::string sint = std::to_string(oint);
		int zeros = 0, pow_10 = 1;
		while ((oint / pow_10) % 10 == 0)
		{
			zeros++;
			pow_10 *= 10;
		}
		if (zeros > 3)
		{
			sint.erase(sint.size() - zeros, zeros);
			sint += "e" + std::to_string(zeros);
		}
		put(it, sint);
	}
	return it;
}

template<class IT>
IT& float_to(const object& obj, IT& it, int = -1)
{
	size_t bufsize = float_prec + 8;
	char* buf = new char[bufsize];
	size_t needed_bufsize = snprintf(buf, bufsize, ("%." + std::to_string(float_prec) + "Lg").c_str(), obj.get_value<Float>()) + 1;
	if (needed_bufsize > bufsize)
		throw std::runtime_error("bufsize too small in json::float_to: bufsize=" + std::to_string(bufsize) + ", needed_bufsize=" + std::to_string(needed_bufsize) + " (this should never happen, please report this error!)");
	put(it, buf);
	delete[] buf;
	return it;
}

template<class IT>
IT& string_to(const object& obj, IT& it, int = -1)
{
	put(it, '"');
	for (auto& c : obj.get_value<String>())
	{
		switch (c)
		{
			case '"':
				put(it, "\\\"");
				break;
			case '\\':
				put(it, "\\\\");
				break;
			case '/':
				put(it, "\\/");
				break;
			case '\b':
				put(it, "\\b");
				break;
			case '\f':
				put(it, "\\f");
				break;
			case '\n':
				put(it, "\\n");
				break;
			case '\r':
				put(it, "\\r");
				break;
			case '\t':
				put(it, "\\t");
				break;
			default:
				put(it, c);
		}
	}
	return put(it, '"');
}

template<class IT>
IT& vector_to(const object& obj, IT& it, int tab = -1)
{
	put(it, '[');
	auto& vec = obj.get_value<Vector>();
	for (int i = 0; (size_t) i < vec.size(); i++)
	{
		if (tab >= 0)
		{
			put(it, '\n');
			put(it, '\t', tab + 1);
		}
		to(vec[i], it, tab != -1 ? tab + 1 : -1);
		if ((size_t) i + 1 != vec.size())
			put(it, ',');
	}
	if (tab >= 0 && !vec.empty())
	{
		put(it, '\n');
		put(it, '\t', tab);
	}
	return put(it, ']');
}

template<class IT>
IT& map_to(const object& obj, IT& it, int tab = -1)
{
	put(it, '{');
	auto& mp = obj.get_value<Map>();
	for (auto mit = mp.begin(); mit != mp.end();)
	{
		if (tab >= 0)
		{
			put(it, '\n');
			put(it, '\t', tab + 1);
		}
		to(object(mit->first), it, tab != -1 ? tab + 1 : -1);
		put(it, ':');
		if (tab >= 0)
			put(it, ' ');
		to(mit->second, it, tab != -1 ? tab + 1 : -1);
		mit++;
		if (mit != mp.end())
			put(it, ',');
	}
	if (tab >= 0 && !mp.empty())
	{
		put(it, '\n');
		put(it, '\t', tab);
	}
	return put(it, '}');
}

template<class IT>
IT& to(const object& obj, IT& it, int tab)
{
	switch (obj.get_value_type())
	{
		case Null:
			return null_to(obj, it, tab);
		case Bool:
			return bool_to(obj, it, tab);
		case Int:
			return int_to(obj, it, tab);
		case Float:
			return float_to(obj, it, tab);
		case String:
			return string_to(obj, it, tab);
		case Vector:
			return vector_to(obj, it, tab);
		case Map:
			return map_to(obj, it, tab);
	}
	return it;
}

std::string& to(const object& obj, std::string& out_str, int tab = -1)
{
	std::stringstream ss;
	std::ostreambuf_iterator<char> ssit(ss);
	to(obj, ssit, tab);
	return (out_str = ss.str());
}

std::string to(const object& obj, int tab = -1)
{
	std::string result;
	return to(obj, result, tab);
}

std::ostream& to(const object& obj, std::ostream& out_stream, int tab = -1)
{
	std::ostreambuf_iterator<char> osit(out_stream);
	to(obj, osit, tab);
	return out_stream;
}

std::ostream& operator<<(std::ostream& out_stream, const object& obj)
{
	return to(obj, out_stream);
}

}

#endif
