#ifndef KP_ENTRY_HPP
#define KP_ENTRY_HPP

#include <string>

enum  KP_EntryType
{
    LOCAL,
    REMOTE
};

class KP_Entry
{
public:
    KP_Entry(std::string jobId, std::string path, KP_EntryType type):
        _jobId(jobId),
        _path(path),
        _type(type)
    { }

    KP_Entry(std::string path, KP_EntryType type):
        _jobId(""),
        _path(path),
        _type(type)
    { }

    KP_Entry(const KP_Entry& copy)
    {
        _jobId = copy._jobId;
        _path = copy._path;
        _type = copy._type;
    }

    std::string getJobId() { return _jobId; }

    std::string getPath() { return _path; }

    KP_EntryType getType() { return _type; }

private:
    std::string _jobId;
    std::string _path;
    KP_EntryType _type;
};

#endif // KP_ENTRY_HPP
