#ifndef FILENETWORKUTILS_HPP
#define FILENETWORKUTILS_HPP

#include <sys/stat.h>
#include <string>
#include <fstream>
#include "../ResponeInterface.hpp"


#define PATH_SEPARATOR  "/"

class FileNetworkUtils
{
private:
	std::string host;
	std::string port;
	std::string pathFile;
	size_t		offset;
public:
	FileNetworkUtils();
	FileNetworkUtils(std::string _host, std::string _port, std::string path_file);
	~FileNetworkUtils();
	void setOffset(size_t offset);
	ResponseCode Open();
	ResponseCode Close();
	ResponseCode GetSize(size_t &size);
	ResponseCode Read(size_t size_buf, size_t &read_length, std::string &buffer);
	ResponseCode Write(std::string buffer);
};

#endif // FILENETWORKUTILS_HPP
