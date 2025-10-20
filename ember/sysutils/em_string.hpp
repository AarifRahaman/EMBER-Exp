#ifndef __EMBER_STRING_HPP__
#define __EMBER_STRING_HPP__

namespace ember {

const char hierseparator[] = ".";

const char* strcat(const char* str_a, const char* str_b);

const std::string hiercat(const std::string base, const char* suffix);

static constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

}

#endif