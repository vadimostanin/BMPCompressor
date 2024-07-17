#include "directorycontent.h"
#include <iostream>
#include <filesystem>
#include <glob.h>
#include <string.h>
#include <sstream>
#include <vector>

DirectoryContent::DirectoryContent() {}

void DirectoryContent::applyDirectory(const std::string& directory, const std::vector<std::string>& patterns)
{
    mFileInfos.clear();

    mDirectory = directory;

    if(false == std::filesystem::exists(std::filesystem::path(directory)))
    {
        std::stringstream ss;
        ss << "Directory does not exist" << std::endl;
        throw std::runtime_error(ss.str());
    }
    std::string glob_request = "" + directory + "/{";
    // for(auto& pattern : patterns)
    for(auto iter = patterns.begin() ; iter != patterns.end() ; ++iter)
    {
        glob_request += *iter;
        if((iter + 1) != patterns.end())
        {
            glob_request += ",";
        }
    }
    glob_request += "}";

    std::cout << "glob_request=" << glob_request << std::endl;

    glob_t glob_result = {0};

    const int return_value = glob(glob_request.c_str(), GLOB_BRACE, NULL, &glob_result);
    if(return_value != 0)
    {
        globfree(&glob_result);
        std::stringstream ss;
        ss << "Files no found at " << directory << std::endl;
        throw std::runtime_error(ss.str());
    }

    for(size_t i = 0; i < glob_result.gl_pathc; ++i)
    {
        std::cout << "glob_result.gl_pathv[i]=" << glob_result.gl_pathv[i] << std::endl;
        FileInfo fileInfo = {"", 0};
        fileInfo.name = std::filesystem::path(glob_result.gl_pathv[i]).filename();
        fileInfo.size = std::filesystem::file_size(std::filesystem::path(glob_result.gl_pathv[i]));
        mFileInfos.push_back(fileInfo);
    }

    globfree(&glob_result);
}

const std::vector<FileInfo>& DirectoryContent::getFiles() const
{
    return mFileInfos;
}

const std::string& DirectoryContent::getDirectory() const
{
    return mDirectory;
}
