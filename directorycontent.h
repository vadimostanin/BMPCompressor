#ifndef DIRECTORYCONTENT_H
#define DIRECTORYCONTENT_H

#include "types_private.h"
#include <string>
#include <vector>

class DirectoryContent
{
    DirectoryContent()/* = default*/;
public:
    static DirectoryContent& Instance()
    {
        static DirectoryContent instance;
        return instance;
    }

    void applyDirectory(const std::string& directory, const std::vector<std::string>& patterns = {});
    const std::string& getDirectory() const;
    const std::vector<FileInfo>& getFiles() const;
private:
    std::string mDirectory;
    std::vector<FileInfo> mFileInfos;
};

#endif // DIRECTORYCONTENT_H
