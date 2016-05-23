#include "FileUtils.hpp"

#define PATH_SEPARATOR          "/"
#define PATH_SEPARATOR_CHAR     '/'

FileUtils::FileUtils()
{
}

bool FileUtils::IsFile(std::string& path) {
    return IsFile(path.c_str());
}

bool FileUtils::IsFile(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

bool FileUtils::IsDirectory(std::string& path) {
    return IsDirectory(path.c_str());
}

bool FileUtils::IsDirectory(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISDIR(buf.st_mode);
}

std::string FileUtils::GetFileName(std::string& path)
{
    std::string filename = "";
    filename = path.substr(path.find_last_of(PATH_SEPARATOR) + 1, path.length());
    //std::cout << "Bp:FileName:"<< filename << std::endl;
    return filename;
}

//std::string FileUtils::AppendPathStr(std::string& pre, std::string & post)
//{
//    std::string tmp = "";
//    if(pre.size() > 0)
//    {

//    }
//    else
//    {

//    }
//}

//bool FileUtils::p_is_a_dir_entry(char* path)
//{
//    //p_status ret = P_FALSE;
//    bool ret = false;
//    struct stat statbuf;
//    if (stat(path, &statbuf) == 0)
//        if (S_ISDIR(statbuf.st_mode))
//            //ret = P_TRUE;
//            ret = true;
//    return ret;
//}

//bool FileUtils::p_is_a_file_entry(char* path)
//{
//    //p_status ret = P_FALSE;
//    bool ret = false;
//    struct stat statbuf;
//    if (stat(path, &statbuf) == 0)
//        if (!S_ISDIR(statbuf.st_mode))
//            //ret = P_TRUE;
//            ret = true;
//    return ret;
//}

void FileUtils::GetAllFileEntryPath(std::string& path, std::vector<std::string> & files, int & count)
{
    files.clear();
    count = 0;
    retrieve(path, files, count);
}

void FileUtils::retrieve(std::string& path, std::vector<std::string> & files, int & count)
{
    //std::cout << "Bp: retrieve: Path:" << path <<std::endl;
    struct dirent **name_list;
    char *rel_name = NULL;
    if(IsFile(path))
    {
        files.push_back(path);
        count ++;
        return;
    }

    long n = scandir(path.c_str(), &name_list, 0, alphasort);
    if (n <= 0)
        return;
    while (n--)
    {
        rel_name = name_list[n] -> d_name;
        if (!strncmp(".", rel_name, 1) || !strncmp ("..", rel_name, 2))
            continue;

        std::string tmp2 = path;

        if(path.size() > 0)
        {
            if(path.data()[path.size() - 1] == PATH_SEPARATOR_CHAR)
            {
                tmp2 += std::string(rel_name);
            }
            else
            {
                tmp2 += std::string(PATH_SEPARATOR) + std::string(rel_name);
            }
        }


        if(IsDirectory(tmp2))
        {
            retrieve(tmp2, files, count);
            delete [] name_list[n];
            continue;
        }
        else if(IsFile(tmp2))
        {
            files.push_back(tmp2);
            count ++;
        }
        delete [] name_list[n];
    }
    delete name_list;
}

bool FileUtils::CreateDirectory(std::string& path)
{
    std::vector<std::string> strs;
    StringUtils::Split(path, strs, '/');
    if(strs.size() <= 0)
        return false;
    std::string subdir = "";
    for(int i = 0 ; i < strs.size(); i ++)
    {
        subdir += std::string(PATH_SEPARATOR) + strs[i];

        if(!IsDirectory(subdir))
        {
            int status = mkdir(subdir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            //int status = mkdir(subdir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP);

            if(status != 0)
            {
                std::cout << "makedir: " << subdir << " fail\n";
                return false;
            }
        }
        else
        {
            //std::cout << "Directory exist!" << std::endl;
        }
    }
    return true;
}

