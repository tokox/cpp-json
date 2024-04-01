namespace tokox::json
{

template<class IT>
IT& format_null(const object& obj, IT& it, int)
{
	obj.get_value<Null>();
	return put(it, "null");
}

template<class IT>
IT& format_bool(const object& obj, IT& it, int)
{
	return put(it, obj.get_value<Bool>() ? "true" : "false");
}

template<class IT>
IT& format_int(const object& obj, IT& it, int)
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
IT& format_float(const object& obj, IT& it, int)
{
	size_t bufsize = float_prec + 8;
	char* buf = new char[bufsize];
	size_t needed_bufsize = snprintf(buf, bufsize, ("%." + std::to_string(float_prec) + "Lg").c_str(), obj.get_value<Float>()) + 1;
	if (needed_bufsize > bufsize)
		throw std::runtime_error("bufsize too small in json::format_float: bufsize=" + std::to_string(bufsize) + ", needed_bufsize=" + std::to_string(needed_bufsize) + " (this should never happen, please report this error!)");
	put(it, buf);
	delete[] buf;
	return it;
}

template<class IT>
IT& format_string(const object& obj, IT& it, int)
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
IT& format_vector(const object& obj, IT& it, int tab)
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
		format(vec[i], it, tab != -1 ? tab + 1 : -1);
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
IT& format_map(const object& obj, IT& it, int tab)
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
		format(object(mit->first), it, tab != -1 ? tab + 1 : -1);
		put(it, ':');
		if (tab >= 0)
			put(it, ' ');
		format(mit->second, it, tab != -1 ? tab + 1 : -1);
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
IT& format(const object& obj, IT& it, int tab)
{
	switch (obj.get_value_type())
	{
		case Null:
			return format_null(obj, it, tab);
		case Bool:
			return format_bool(obj, it, tab);
		case Int:
			return format_int(obj, it, tab);
		case Float:
			return format_float(obj, it, tab);
		case String:
			return format_string(obj, it, tab);
		case Vector:
			return format_vector(obj, it, tab);
		case Map:
			return format_map(obj, it, tab);
		default:
			throw std::runtime_error("Unknown value_type");
	}
	return it;
}

std::string& format(const object& obj, std::string& out_str, int tab)
{
	std::stringstream ss;
	std::ostreambuf_iterator<char> ssit(ss);
	format(obj, ssit, tab);
	return (out_str = ss.str());
}

std::string format(const object& obj, int tab)
{
	std::string result;
	return format(obj, result, tab);
}

std::ostream& format(const object& obj, std::ostream& out_stream, int tab)
{
	std::ostreambuf_iterator<char> osit(out_stream);
	format(obj, osit, tab);
	return out_stream;
}

std::ostream& operator<<(std::ostream& out_stream, const object& obj)
{
	return format(obj, out_stream);
}

}
