#ifndef TOKOX_CPP_JSON_HPP
#define TOKOX_CPP_JSON_HPP

#include <string>
#include <vector>
#include <map>
#include <istream>
#include <ostream>
#include <any>
#include <iterator>
#include <limits>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <typeinfo>
#include <stdexcept>

namespace tokox
{

namespace json
{

class object;

enum value_type
{
	Null,
	Bool,
	Int,
	Float,
	String,
	Vector,
	Map
};

value_type any_to_value_type(const std::any& v)
{
	if (!v.has_value())
		return Null;
	else if (v.type() == typeid(bool))
		return Bool;
	else if (v.type() == typeid(int))
		return Int;
	else if (v.type() == typeid(float))
		return Float;
	else if (v.type() == typeid(std::string))
		return String;
	else if (v.type() == typeid(std::vector<object>))
		return Vector;
	else if (v.type() == typeid(std::map<std::string, object>))
		return Map;
	else
		throw std::runtime_error("Unknown value_type!");
}

template<typename T>
concept object_type = std::is_same_v<T, std::nullptr_t> || std::is_same_v<T, bool> || std::is_same_v<T, long long int> || std::is_same_v<T, long double> || std::is_same_v<T, std::string> || std::is_same_v<T, std::vector<object>> || std::is_same_v<T, std::map<std::string, object>>;


template<value_type V>
using object_type_from_value_type = std::conditional_t<V == Null, std::nullptr_t, std::conditional_t<V == Bool, bool, std::conditional_t<V == Int, long long int, std::conditional_t<V == Float, long double, std::conditional_t<V == String, std::string, std::conditional_t<V == Vector, std::vector<object>, std::map<std::string, object>>>>>>>;

template<object_type T>
value_type value_type_from_object_type()
{
	if constexpr (std::is_same_v<T, std::nullptr_t>)
		return Null;
	else if constexpr (std::is_same_v<T, bool>)
		return Bool;
	else if constexpr (std::is_same_v<T, long long int>)
		return Int;
	else if constexpr (std::is_same_v<T, long double>)
		return Float;
	else if constexpr (std::is_same_v<T, std::string>)
		return String;
	else if constexpr (std::is_same_v<T, std::vector<object>>)
		return Vector;
	else if constexpr (std::is_same_v<T, std::map<std::string, object>>)
		return Map;
}

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

namespace
{
template<class IT>
IT& mov(IT& it)
{
	it++;
	return it;
}

template<class IT>
IT& put(IT& it, char c, int t = 1)
{
	while (t-- > 0)
	{
		(*it) = c;
		it++;
	}
	return it;
}

template<class IT>
IT& put(IT& it, std::string s, int t = 1)
{
	while (t-- > 0)
	{
		for (auto& c : s)
			put(it, c);
	}
	return it;
}

template<class IT>
char get(IT& it, IT end)
{
	if (it == end)
		throw import_error(ERR_END);
	return *it;
}

template<class IT>
char getmv(IT& it, IT end)
{
	char c = get(it, end);
	mov(it);
	return c;
}

template<class IT>
void eat_ws(IT& it, IT end)
{
	while (get(it, end) == ' ' || get(it, end) == '\n' || get(it, end) == '\r' || get(it, end) == '\t')
		mov(it);
}
}

template<class IT>
object from(IT& it, IT end);

template<class IT>
object null_from(IT& it, IT end)
{
	eat_ws(it, end);
	for (int i = 0; i < 4; i++)
	{
		if (getmv(it, end) != "null"[i])
			throw import_error(ERR_VAL);
	}
	return object();
}

template<class IT>
object bool_from(IT& it, IT end)
{
	eat_ws(it, end);
	char c = getmv(it, end);
	if (c == 't')
	{
		for (int i = 1; i < 4; i++)
		{
			if (getmv(it, end) != "true"[i])
				throw import_error(ERR_VAL);
		}
		return object(true);
	}
	else if (c == 'f')
	{
		for (int i = 1; i < 5; i++)
		{
			if (getmv(it, end) != "false"[i])
				throw import_error(ERR_VAL);
		}
		return object(false);
	}
	else
		throw import_error(ERR_VAL);
}

template<class IT>
object number_from(IT& it, IT end)
{
	eat_ws(it, end);
	bool minus = false;
	std::string num;
	if (get(it, end) == '-')
	{
		minus = true;
		mov(it);
	}
	if (get(it, end) == '0')
	{
		num += '0';
		mov(it);
	}
	else if (get(it, end) >= '1' && get(it, end) <= '9')
	{
		do
			num += getmv(it, end);
		while (it != end && get(it, end) >= '0' && get(it, end) <= '9');
	}
	else
		throw import_error(ERR_VAL);
	std::string apt;
	if (it != end && get(it, end) == '.')
	{
		mov(it);
		if (get(it, end) >= '0' && get(it, end) <= '9')
		{
			do
				apt += getmv(it, end);
			while (it != end && get(it, end) >= '0' && get(it, end) <= '9');
		}
		else
			throw import_error(ERR_VAL);
	}
	std::string exp;
	long long expll;
	if (it != end && (get(it, end) == 'E' || get(it, end) == 'e'))
	{
		mov(it);
		if (get(it, end) == '+')
			mov(it);
		else if (get(it, end) == '-')
			exp += getmv(it, end);
		if (get(it, end) >= '0' && get(it, end) <= '9')
		{
			do
				exp += getmv(it, end);
			while (it != end && get(it, end) >= '0' && get(it, end) <= '9');
		}
		else
			throw import_error(ERR_VAL);
		try
		{
			expll = std::stoll(exp);
		}
		catch (const std::out_of_range& e)
		{
			throw import_error(ERR_RNG);
		}
	}
	else
	{
		exp = "0";
		expll = 0;
	}
	int max_dgt = (int) std::log10(std::numeric_limits<long long>::max()) + 1;
	if (expll + num.size() > (size_t) max_dgt || expll + num.size() < 1)
	{
		try
		{
			return object((minus ? -1 : 1) * std::stold(num + "." + apt + "e" + exp));
		}
		catch (const std::out_of_range& e)
		{
			throw import_error(ERR_RNG);
		}
	}
	if (expll > 0)
	{
		if (num.size() == 1 && num[0] == '0')
			num.clear();
		num += apt.substr(0, std::min(expll, (long long) apt.size()));
		for (int i = 0; (size_t) i < expll - apt.size(); i++)
			num += '0';
		apt.erase(0, std::min(expll, (long long) apt.size()));
	}
	else
	{
		apt.insert(0, num.substr(num.size() + expll, -expll));
		num.erase(num.size() + expll, -expll);
	}
	long long resultll;
	long double resultld;
	try
	{
		resultll = std::stoll(num);
	}
	catch (const std::out_of_range& e)
	{
		try
		{
			resultld = std::stold(num + "." + apt);
		}
		catch (const std::out_of_range& e2)
		{
			throw import_error(ERR_RNG);
		}
		return object((minus ? -1 : 1) * resultld);
	}
	if (apt.empty())
		return object((minus ? -1 : 1) * resultll);
	else if (apt[0] >= '5')
		resultll++;
	try
	{
		resultld = std::stold(num + "." + apt);
	}
	catch (const std::out_of_range& e)
	{
		return object((minus ? -1 : 1) * resultll);
	}
	if (std::llround(resultld) == resultll)
		return object((minus ? -1 : 1) * resultld);
	return object((minus ? -1 : 1) * resultll);
}

template<class IT>
object string_from(IT& it, IT end)
{
	eat_ws(it, end);
	if (getmv(it, end) != '"')
		throw import_error(ERR_VAL);
	std::string result;
	char c;
	while ((c = getmv(it, end)) != '"')
	{
		if (c == '\\')
		{
			switch (c = getmv(it, end))
			{
				case '"':
				case '\\':
				case '/':
					result += c;
					break;
				case 'b':
					result += '\b';
					break;
				case 'f':
					result += '\f';
					break;
				case 'n':
					result += '\n';
					break;
				case 'r':
					result += '\r';
					break;
				case 't':
					result += '\t';
					break;
				case 'u':
					throw import_error(ERR_IMP);
				default:
					throw import_error(ERR_VAL);
			}
		}
		else if (c >= ' ' && c < 127)
			result += c;
		else
			throw import_error(ERR_VAL);
	}
	return object(result);
}

template<class IT>
object vector_from(IT& it, IT end)
{
	eat_ws(it, end);
	if (getmv(it, end) != '[')
		throw import_error(ERR_VAL);
	std::vector<object> result;
	while (get(it, end) != ']')
	{
		eat_ws(it, end);
		result.push_back(from(it, end));
		eat_ws(it, end);
		char c;
		if ((c = get(it, end)) == ']')
			break;
		else if (c != ',')
			throw import_error(ERR_VAL);
		else
			mov(it);
	}
	mov(it);
	return object(result);
}

template<class IT>
object map_from(IT& it, IT end)
{
	eat_ws(it, end);
	if (getmv(it, end) != '{')
		throw import_error(ERR_VAL);
	std::map<std::string, object> result;
	while (get(it, end) != '}')
	{
		eat_ws(it, end);
		std::string key = string_from(it, end);
		if (result.contains(key))
			throw import_error(ERR_KEY);
		eat_ws(it, end);
		if (getmv(it, end) != ':')
			throw import_error(ERR_VAL);
		eat_ws(it, end);
		object value = from(it, end);
		result[key] = value;
		eat_ws(it, end);
		char c;
		if ((c = get(it, end)) == '}')
			break;
		else if (c != ',')
			throw import_error(ERR_VAL);
		else
			mov(it);
	}
	mov(it);
	return object(result);
}

template<class IT>
object from(IT& it, IT end)
{
	eat_ws(it, end);
	switch (get(it, end))
	{
		case 'n':
			return null_from(it, end);
		case 't':
		case 'f':
			return bool_from(it, end);
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return number_from(it, end);
		case '"':
			return string_from(it, end);
		case '[':
			return vector_from(it, end);
		case '{':
			return map_from(it, end);
	}
	throw import_error(ERR_VAL);
}

object from(const std::string& json_str)
{
	std::string::const_iterator jsit(json_str.begin());
	return from(jsit, json_str.end());
}

object from(std::istream& json_stream)
{
	std::istreambuf_iterator<char> jsit(json_stream);
	return from(jsit, std::istreambuf_iterator<char>());
}

object& from(object& obj, const std::string& json_str)
{
	return (obj = from(json_str));
}

object& from(object& obj, std::istream& json_stream)
{
	return (obj = from(json_stream));
}

std::istream& operator>>(std::istream& json_stream, object& obj)
{
	from(obj, json_stream);
	return json_stream;
}

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

}

#endif
