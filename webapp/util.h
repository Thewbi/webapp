#pragma once

#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <fstream>
#include <streambuf>

// trim from start (in place)
static inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
	rtrim(s);
	return s;
}

static inline bool starts_with(const std::string data, const std::string prefix) {
	return data.rfind(prefix, 0) == 0;
}

static inline std::string readFileIntoString(const std::string filename)
{
	std::ifstream ifstream(filename);
	std::string str;

	ifstream.seekg(0, std::ios::end);
	str.reserve(ifstream.tellg());
	ifstream.seekg(0, std::ios::beg);

	str.assign((std::istreambuf_iterator<char>(ifstream)),
		std::istreambuf_iterator<char>());

	return str;
}