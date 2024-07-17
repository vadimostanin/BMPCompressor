#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

std::string HumanReadableSize(uint64_t size);
std::unordered_multimap<std::string, std::string> parseArgs(int argc, char* argv[]);
std::tuple<std::string, std::vector<std::string>> extractArgs(int argc, char* argv[]);

#endif