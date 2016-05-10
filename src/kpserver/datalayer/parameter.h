#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <string>
//#include <vector>
enum action{
    CREAT,
    EDIT,
    DELETE,
    NOACTION
};

action convertStrToAct(std::string ac_str);
/*{
    if (ac_str.compare("newparam") == 0) {
        return CREAT;
    } else if (ac_str.compare("editparam") == 0) {
        return EDIT;
    } else if (ac_str.compare("deleteparam") == 0) {
        return DELETE;
    } else {
        return NOACTION;
    }
}
*/
class Parameter {
private:
    std::string paramId;
    std::string serviceId;
    bool        isOutPut;
    bool        isRequired;
    std::string optionName;
    std::string description;
    std::string type;
    std::string name;
    std::string defaultValue;
    action      act;


public:
    Parameter();
    Parameter(std::string id, bool output, bool require, std::string opName, std::string des, std::string t);
    ~Parameter();

    void setParamID(std::string prID);
    std::string getParamID();

    void setServiceID(std::string svID);
    std::string getServiceID();

    void setIsOutput(bool output);
    bool getIsOutput();

    void setIsRequired(bool require);
    bool getIsRequired();

    void setOptionName(std::string opName);
    std::string getOptionName();

    void setDescription(std::string des);
    std::string getDescription();

    void setType(std::string t);
    std::string getType();

    void setName(std::string na);
    std::string getName();

    void setDefaultValue(std::string dfValue);
    std::string getDefaultValue();

    void setAct(action ac);
    action getAct();

};

#endif // PARAMETER_HPP
