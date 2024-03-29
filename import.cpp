#include <sstream>
#include <stdexcept>
#include <cmath>
#include <limits>

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

}
