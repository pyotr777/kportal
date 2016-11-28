#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <string.h>
#include <vector>

#include "StringUtils.hpp"


#define PATH_SEPARATOR          "/"
#define PATH_SEPARATOR_CHAR     '/'

class FileUtils
{
public:
    FileUtils();

    static bool IsFile(std::string& path);

    static bool IsFile(const char* path);

    static bool IsDirectory(std::string& path);

    static bool IsDirectory(const char* path);

    static std::string GetFileName(std::string& path);

//    static std::string AppendPathStr(std::string& pre, std::string & post);

//    static bool p_is_a_dir_entry(char* path);

//    static bool p_is_a_file_entry(char* path);

    static void GetAllFileEntryPath(std::string& path, std::vector<std::string> & files, int & count);

    static bool CreateDirectory(std::string& path);

private:

    static void retrieve(std::string& path, std::vector<std::string> & files, int & count);

};

#endif // FILEUTILS_HPP
