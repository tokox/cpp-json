#include <sstream>
#include <stdexcept>
#include <cmath>
#include <limits>
#include <codecvt>
#include <locale>

namespace tokox::json
{

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
		throw parse_error(parse_error::UNEXPECTED_END);
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
object parse_null(IT& it, IT end)
{
	eat_ws(it, end);
	for (int i = 0; i < 4; i++)
	{
		if (getmv(it, end) != "null"[i])
			throw parse_error(parse_error::INCORRECT_VALUE);
	}
	return object();
}

template<class IT>
object parse_bool(IT& it, IT end)
{
	eat_ws(it, end);
	char c = getmv(it, end);
	if (c == 't')
	{
		for (int i = 1; i < 4; i++)
		{
			if (getmv(it, end) != "true"[i])
				throw parse_error(parse_error::INCORRECT_VALUE);
		}
		return object(true);
	}
	else if (c == 'f')
	{
		for (int i = 1; i < 5; i++)
		{
			if (getmv(it, end) != "false"[i])
				throw parse_error(parse_error::INCORRECT_VALUE);
		}
		return object(false);
	}
	else
		throw parse_error(parse_error::INCORRECT_VALUE);
}

template<class IT>
object parse_number(IT& it, IT end)
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
		throw parse_error(parse_error::INCORRECT_VALUE);
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
			throw parse_error(parse_error::INCORRECT_VALUE);
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
			throw parse_error(parse_error::INCORRECT_VALUE);
		try
		{
			expll = std::stoll(exp);
		}
		catch (const std::out_of_range& e)
		{
			throw parse_error(parse_error::VALUE_OUT_OF_RANGE);
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
			throw parse_error(parse_error::VALUE_OUT_OF_RANGE);
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
			throw parse_error(parse_error::VALUE_OUT_OF_RANGE);
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
object parse_string(IT& it, IT end)
{
	eat_ws(it, end);
	if (getmv(it, end) != '"')
		throw parse_error(parse_error::INCORRECT_VALUE);
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
					{
						unsigned int codepoint = 0;
						for (int i = 0; i < 4; i++)
						{
							c = getmv(it, end);
							if (c >= '0' && c <= '9')
								codepoint = codepoint * 16 + c - '0';
							else if (c >= 'A' && c <= 'F')
								codepoint = codepoint * 16 + c - 'A' + 10;
							else if (c >= 'a' && c <= 'f')
								codepoint = codepoint * 16 + c - 'a' + 10;
							else
								throw parse_error(parse_error::INCORRECT_VALUE);
						}
						result += std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().to_bytes(codepoint);
					}
					break;
				default:
					throw parse_error(parse_error::INCORRECT_VALUE);
			}
		}
		else if (c >= ' ' && c != 127) // TODO: check utf-8 too
		{
			result += c;
		}
		else
			throw parse_error(parse_error::INCORRECT_VALUE);
	}
	return object(result);
}

template<class IT>
object parse_vector(IT& it, IT end)
{
	eat_ws(it, end);
	if (getmv(it, end) != '[')
		throw parse_error(parse_error::INCORRECT_VALUE);
	object_type_from_value_type<Vector> result;
	while (get(it, end) != ']')
	{
		eat_ws(it, end);
		result.push_back(parse(it, end));
		eat_ws(it, end);
		char c;
		if ((c = get(it, end)) == ']')
			break;
		else if (c != ',')
			throw parse_error(parse_error::INCORRECT_VALUE);
		else
			mov(it);
	}
	mov(it);
	return object(result);
}

template<class IT>
object parse_map(IT& it, IT end)
{
	eat_ws(it, end);
	if (getmv(it, end) != '{')
		throw parse_error(parse_error::INCORRECT_VALUE);
	object_type_from_value_type<Map> result;
	while (get(it, end) != '}')
	{
		eat_ws(it, end);
		std::string key = parse_string(it, end);
		if (result.contains(key))
			throw parse_error(parse_error::REPEATED_MAP_KEY);
		eat_ws(it, end);
		if (getmv(it, end) != ':')
			throw parse_error(parse_error::INCORRECT_VALUE);
		eat_ws(it, end);
		object value = parse(it, end);
		result[key] = value;
		eat_ws(it, end);
		char c;
		if ((c = get(it, end)) == '}')
			break;
		else if (c != ',')
			throw parse_error(parse_error::INCORRECT_VALUE);
		else
			mov(it);
	}
	mov(it);
	return object(result);
}

template<class IT>
object parse(IT& it, IT end)
{
	eat_ws(it, end);
	switch (get(it, end))
	{
		case 'n':
			return parse_null(it, end);
		case 't':
		case 'f':
			return parse_bool(it, end);
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
			return parse_number(it, end);
		case '"':
			return parse_string(it, end);
		case '[':
			return parse_vector(it, end);
		case '{':
			return parse_map(it, end);
	}
	throw parse_error(parse_error::INCORRECT_VALUE);
}

object parse(const std::string& json_str)
{
	std::string::const_iterator jsit(json_str.begin());
	return parse(jsit, json_str.end());
}

object parse(std::istream& json_stream)
{
	std::istreambuf_iterator<char> jsit(json_stream);
	return parse(jsit, std::istreambuf_iterator<char>());
}

object& parse(object& obj, const std::string& json_str)
{
	return (obj = parse(json_str));
}

object& parse(object& obj, std::istream& json_stream)
{
	return (obj = parse(json_stream));
}

std::istream& operator>>(std::istream& json_stream, object& obj)
{
	parse(obj, json_stream);
	return json_stream;
}

}
