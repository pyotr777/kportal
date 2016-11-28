#ifndef FILERECIEVE_HPP
#define FILERECIEVE_HPP

#include <string>
#include "IdRes.hpp"

class FileRecieve
{
public:
    FileRecieve(): _entryPath(""), _localPath(""), _realPath("") {}
    std::string getEntryPath() { return _entryPath; }
    std::string getLocalPath() { return _localPath; }
    std::string getRealPath()  { return _realPath; }
    std::string getRequestId() {return _requestId; }
    void setEntryPath(std::string entryPath) { this -> _entryPath = entryPath; }
    void setLocalPath(std::string localPath) { this -> _localPath = localPath; }
    void setRealPath(std::string realPath)   { this -> _realPath = realPath;   }
    void setRequestId(std::string requestId) { this -> _requestId = requestId; }

    bool isFinish() { return this -> _finish == IdRes::FINISH_SUCCESS; }
    void setFinish(IdRes::FinishStates finish) { this -> _finish = finish; }
    IdRes::FinishStates getFinish() { return _finish; }
private:
    std::string _entryPath;
    std::string _localPath;
    std::string _realPath;
    std::string _requestId;
    IdRes::FinishStates _finish;
};
#endif // FILERECIEVE_HPP
