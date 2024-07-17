#include "helpers.h"
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <sstream>
#include <cmath>

std::string HumanReadableSize(uint64_t size)
{
    double mantissa = size;
    std::stringstream ss;
    uint i = 0;
    for (; mantissa >= 1024.; mantissa /= 1024., ++i);
    ss << std::ceil(mantissa * 10.) / 10. << "BKMGTPE"[i];
    if(i > 0)
    {
        (ss << "B (" << size << ')');
        return ss.str();
    }
    return ss.str();
}

std::unordered_multimap<std::string, std::string> parseArgs(int argc, char* argv[])
{
    std::unordered_multimap<std::string, std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-' && i + 1 < argc) {
            std::string key = arg.substr(1);
            if(i+1 >= argc || argv[i+1][0] == '-')
                continue;
            std::string value = argv[++i];
            args.emplace(key, value);
        }
    }
    return args;
}

std::tuple<std::string, std::vector<std::string>> extractArgs(int argc, char* argv[])
{
    std::string directory;
    std::vector<std::string> extentions;
    {
        const auto args = parseArgs(argc, argv);

        if (args.find("dir") != args.end())
        {
            directory = args.find("dir")->second;
            std::cout << "Directory: " << directory << std::endl;
        }
        else
        {
            std::cout << "Directory is not provided!" << std::endl;
        }
        if (args.find("ext") != args.end())
        {
            const auto its = args.equal_range("ext");
            for(auto it = its.first; it != its.second; ++it)
            {
                std::cout << it->first << '\t' << it->second << std::endl;
                extentions.push_back(it->second);
            }
            // directory = args.find("dir");
            std::cout << "Directory: " << directory << std::endl;
        }
        else
        {
            std::cout << "Directory is not provided!" << std::endl;
        }
    }
    if(directory.empty())
    {
        char cwd[PATH_MAX] = {0};
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Current working dir: %s\n", cwd);
            directory = cwd;
        } else {
            perror("getcwd() error");
            return std::tuple<std::string, std::vector<std::string>>();
        }
    }
    return std::make_tuple(directory, extentions);
}
