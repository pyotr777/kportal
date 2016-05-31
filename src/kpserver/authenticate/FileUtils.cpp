#include "FileUtils.hpp"
#include "../stdafx.h"
FileUtils::FileUtils()
{
}

bool FileUtils::IsFile(std::string path) {
    return IsFile(path.c_str());
}

bool FileUtils::IsFile(const char* path) {
    struct stat buf;
    stat(path, &buf);
    return S_ISREG(buf.st_mode);
}

bool FileUtils::IsDirectory(std::string path) {
    return IsDirectory(path.c_str());
}

bool FileUtils::IsDirectory(const char* path) {
    struct stat buf;
	if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode))
	{
		return true;
	}
	return false;
}

std::string FileUtils::GetFileName(std::string path)
{
    std::string filename = "";
    filename = path.substr(path.find_last_of(PATH_SEPARATOR) + 1, path.length());
    //std::cout << "Bp:FileName:"<< filename << std::endl;
    return filename;
}

std::string FileUtils::GetDirectoryName(std::string dir_path)
{
    std::string filename = "";
    if(dir_path.find_last_of(PATH_SEPARATOR) == dir_path.length() - 1)
    {
      filename = dir_path.substr(dir_path.find_last_of(PATH_SEPARATOR, 0, dir_path.length() - 1) + 1, dir_path.length());
    }
    else 
    {
      filename = dir_path.substr(dir_path.find_last_of(PATH_SEPARATOR) + 1, dir_path.length());
      //std::cout << "Bp:FileName:"<< filename << std::endl;
    }
    return filename;
}


std::string FileUtils::GetPathDir(std::string pathFile) {
	std::string pathDir = "";
	pathDir = pathFile.substr(0, pathFile.find_last_of(PATH_SEPARATOR));
	return pathDir;
}

std::vector<std::string> ParseDirectory(std::string path)
{
    std::vector<std::string> a;
    //StringUtil::
    return a;
}

void FileUtils::CreateDirectory(std::string path)
{
    std::vector<std::string> strs;
    StringUtils::Split(path, strs, '/');
	if(strs.size() <= 0){
		std::cout << "Strs empty!\n";
        return;
	}
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
                return;
            }
        }
        else
        {
			std::cout << "Directory exist: " << subdir << std::endl;
        }
    }
}

std::string FileUtils::ReadAllFile(std::string pathFile) {
    std::ifstream in(pathFile.c_str(), std::ios::binary);
    std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    return s;
}

std::string FileUtils::GenPathDirOfSession(std::string _conId) {

	extern std::string DEFAULT_HOME_FOLDER;
	return DEFAULT_HOME_FOLDER + "/home_session/" + _conId;
}

int FileUtils::delete_folder_tree (const char* directory_name) {
	DIR*            dp;
	struct dirent*  ep;

	dp = opendir(directory_name);

    if (dp == NULL) {
        printf ("Cannot open directory '%s'\n", directory_name);
        return -1;
    }

	while ((ep = readdir(dp)) != NULL) {
	if (strcmp(ep->d_name,".") != 0 && strcmp(ep->d_name,"..") != 0) {
		if (ep->d_type == DT_DIR) {
			printf("dir: %s\n",  ep->d_name);
			std::string path = std::string(directory_name) + "/" + ep->d_name;
				delete_folder_tree(path.c_str());
		} else {
			printf("file: %s\n",  ep->d_name);
			std::string path = std::string(directory_name) + "/" + ep->d_name;
			remove(path.c_str());
		}
	}
	}
    std::cout << "close dir after read\n";
	closedir(dp);
    std::cout << "close dir finished\n";
	return rmdir(directory_name);
}

int FileUtils::copy_file (std::string sourceFile, std::string desFile) {
	std::ifstream  src(sourceFile.c_str(), std::ios::binary);
	std::ofstream  dst(desFile.c_str(), std::ios::binary);
	dst << src.rdbuf();
	return 1;
}

//int main(int argc, char* argv[])
//{
//    std::string path = "/home/tuannp/k-portal/abc.txt";
//    if(argc > 1)
//        path = argv[1];

//    std::string filename = FileUtils::GetFileName(path);

//    return 0;
//}

