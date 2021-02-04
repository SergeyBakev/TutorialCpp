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

		static std::vector<std::wstring> Split(std::wstring_view delim, const std::wstring& str)
		{
			std::vector<std::wstring> ret;
			size_t pos = 0;
			std::wstring local = str;
			while ((pos = local.find(delim)) != sstring::npos)
			{
				std::wstring token = local.substr(0, pos);
				ret.push_back(token);
				local.erase(0, pos + delim.length());

			}
			ret.push_back(local);
			return ret;
		}

		// trim from start (in place)
		static inline void ltrim_local(std::string& s)
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !std::isspace(ch);
				}));
		}

		// trim from start (in place)
		static inline void ltrim_local(std::wstring& s)
		{
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
				return !std::isspace(ch);
				}));
		}

		// trim from end (in place)
		static inline void rtrim_local(std::string& s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch);
				}).base(), s.end());
		}

		static inline void rtrim_local(std::wstring& s)
		{
			s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
				return !std::isspace(ch);
				}).base(), s.end());
		}

		// trim from both ends (in place)
		static inline void trim_local(std::string& s)
		{
			ltrim_local(s);
			rtrim_local(s);
		}

		static inline void trim_local(std::wstring& s)
		{
			ltrim_local(s);
			rtrim_local(s);
		}

		// trim from start (copying)
		static inline std::string ltrim(std::string s) 
		{
			ltrim_local(s);
			return s;
		}

		static inline std::wstring ltrim(std::wstring s)
		{
			ltrim_local(s);
			return s;
		}

		// trim from end (copying)
		static inline std::string rtrim(std::string s) 
		{
			rtrim_local(s);
			return s;
		}

		// trim from end (copying)
		static inline std::wstring rtrim(std::wstring s)
		{
			rtrim_local(s);
			return s;
		}

		// trim from both ends (copying)
		static inline std::string trim(std::string s)
		{
			trim_local(s);
			return s;
		}

		static inline std::wstring trim(std::wstring s)
		{
			trim_local(s);
			return s;
		}
	};
};