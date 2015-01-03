// StringHelpers.h -- String Helpers
// Contains helper functions for string operations

#ifndef __STRINGHELPERS_H__
#define __STRINGHELPERS_H__

#include <string>

// Returns a copy of this string converted to lowercase.
std::string StringToLower(std::string str);

// Returns a copy of this string converted to uppercase.
std::string StringToUpper(std::string str);

// Performs a lowercase comparison of strings.
// The return value indicates the relation of str1 to str2 as follows:
// < 0 | str1 less than str2
// = 0 | str1 identical to str2
// > 0 | str1 greater than str2
int StringCaseCompare(std::string str1, std::string str2);

#endif
