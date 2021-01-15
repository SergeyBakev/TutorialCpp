#pragma once
#include <vector>
#include <string>
#include <string_view>

struct Algorithms
{
	using StringVector = std::vector<std::string>;
	using sstring = std::string;
	struct Vector
	{
		template<class T>
		static size_t IndexOf(const std::vector<T>& v, const T& el)
		{
			return std::distance(std::begin(v), std::find(std::begin(v), std::end(v), el));
		}

		template<class T,class Preficate>
		static size_t IndexOf(const std::vector<T>& v, const T& el, Preficate pred)
		{
			return std::distance(std::begin(v), std::find_if(std::begin(v), std::end(v), el, pred));
		}
	};

	struct String
	{
		static StringVector Split(std::string_view delim, const sstring& str)
		{
			StringVector ret;
			size_t pos = 0;
			sstring local = str;
			while ((pos = local.find(delim)) != sstring::npos)
			{
				sstring token = local.substr(0, pos);
				ret.push_back(token);
				local.erase(0, pos + delim.length());

			}
			ret.push_back(local);
			return ret;
		}
	};
};