#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <sys/stat.h>
#include <string>
#include <fstream>


#define PATH_SEPARATOR  "/"

class FileUtils
{
public:
    FileUtils();

    static bool IsFile(std::string path);

    static bool IsFile(const char* path);

    static bool IsDirectory(std::string path);

    static bool IsDirectory(const char* path);

    static std::string GetFileName(std::string path);

    static std::string GetDirectoryName(std::string dir_path);

    static std::string GetPathDir(std::string pathFile);

    static void CreateDirectory(std::string path);

    static std::string ReadAllFile(std::string pathFile);

    static std::string GenPathDirOfSession(std::string _conId);

    static int delete_folder_tree (const char* directory_name);

    static int copy_file (std::string sourceFile, std::string desFile);
};

#endif // FILEUTILS_HPP
