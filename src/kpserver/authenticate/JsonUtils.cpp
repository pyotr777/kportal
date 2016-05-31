#include "JsonUtils.hpp"

/*
 * Create a JSONNode from string unicode
 * Args:
 *      tag: Tag key
 *      valueStr: value string
 *      node: output node
 * Return: (0: success, > 0 false)
 */
JSONNode JsonUtils::toJSONNode(const char* tag, const std::string valueStr)
{

  std::cout << "JsonUtils::toJSONNode ";
    JSONNode node;
    std::string value = valueStr;
    StringUtils::convertStandardStr(value);
    std::string nodeStr = std::string("{\"") + tag + "\" : \"" + value + "\"}";
    try {
        node = libjson::parse(nodeStr);
    } catch(const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << '\n';
    }
    return node;
}

/*
 * Create a JSONNode from User for Options
 * Args:
 *      user: info
 *      node: output node
 * Return: JSONNode result
 */
JSONNode JsonUtils::toJSONNode(const User& user, const Options& opt ){
  std::cout << "JsonUtils::toJSONNode ";

  JSONNode item(JSON_NODE), addressNode, descriptNode, publicKeyNode, kdeskAccNode, timeUsableNode;
  std::string email, phone, publickey, kdeskacc; double usableTime = 0, usageTime = 0;
	switch(opt){
    case OPTS_PROVIDER:
      /// Email
      email = user.getEmail();
      std::cout << "email " << email;
      item.push_back(JSONNode(TAG_EMAIL_STR, email));

      /// Phone
      phone = user.getPhone();

      std::cout << ", phone " << phone;
      item.push_back(JSONNode(TAG_PHONE_STR, phone));

      /// Address
      addressNode = toJSONNode(TAG_ADDRESS_STR, user.getAddress());
      item.push_back(*(addressNode.begin()));

      /// Description
      descriptNode = toJSONNode(TAG_DESCRIPTION_STR, user.getDescription());
      item.push_back(*(descriptNode.begin()));

      std::cout << "JsonUtils::toJSONNode getPublicKey";
      /// Public key
      publickey = user.getPublicKey();
      publicKeyNode = toJSONNode(TAG_PUBLICKEY_STR, publickey);
      item.push_back(*(publicKeyNode.begin()));
      std::cout << "JsonUtils::toJSONNode getKdeskAcc";
      /// Kdesk account
      kdeskacc = user.getKdeskAcc();
      item.push_back(JSONNode(TAG_KDESKACC_STR, kdeskacc));

      /// Time usage
      std::cout << "JsonUtils::toJSONNode getTimeUsage";
      usageTime = user.getTimeUsage();
      item.push_back(JSONNode(TAG_TIMEUSAGE_STR, usageTime));

      /// Time usable
      std::cout << "JsonUtils::toJSONNode getTimeUsable";
      usableTime = user.getTimeUsable();
      item.push_back(JSONNode(TAG_TIMEUSABLE_STR, usableTime));

      break;
    case OPTS_PROVIDER_REGISTER:
      /// Email
      email = user.getEmail();
      item.push_back(JSONNode(TAG_EMAIL_STR, email));

      /// Phone
      phone = user.getPhone();
      item.push_back(JSONNode(TAG_PHONE_STR, phone));

      /// Address
      addressNode = toJSONNode(TAG_ADDRESS_STR, user.getAddress());
      item.push_back(*(addressNode.begin()));

      /// Description
      descriptNode = toJSONNode(TAG_DESCRIPTION_STR, user.getDescription());
      item.push_back(*(descriptNode.begin()));

      /// Kdesk account
      kdeskacc = user.getKdeskAcc();
      item.push_back(JSONNode(TAG_KDESKACC_STR, kdeskacc));
		break;
	default:
		break;
  }
  std::cout << " finish\n";
    return item;
}

/*
 * Create a JSONNode from string ImageDocker
 * Args:
 *      image: info
 *      node: output node
 * Return: JSONNode result
 */
JSONNode JsonUtils::toJSONNode(ImageDocker& image)
{
    JSONNode item(JSON_NODE);
    std::string id;
    int isPublic = 0;

	/// image
    id = image.getImageId();
    item.push_back(JSONNode(TAG_IMAGE_STR, id));

    /// IsPublic
    isPublic = (int)image.getIsPublic();
    item.push_back(JSONNode(TAG_IS_PUBLIC_STR, isPublic));

    /// Name
	JSONNode nameNode = toJSONNode(TAG_NAME_STR, image.getImageName());
    item.push_back(*(nameNode.begin()));

	/// Description
	JSONNode descriptNode = toJSONNode(TAG_DESCRIPTION_STR, image.getComment());
	item.push_back(*(descriptNode.begin()));

    return item;
}

/*
 * Create JSONNode from KP_Service
 */
JSONNode JsonUtils::toJSONNode(Service& service)
{
    JSONNode item(JSON_NODE);
    //std::string value;
    //n.push_back(JSONNode(TAG_SERVICE_STR, service.serviceId));
    //n.push_back(JSONNode(TAG_PROVIDER_STR, service.providerId));

    std::cout << "serviceId = " << service.getServiceID() << std::endl;
    item.push_back(JSONNode(TAG_SERVICE_STR, service.getServiceID()));

    //name
    std::cout << "servicename = " << service.getServiceName() << std::endl;
    JSONNode jSon_ServiceName = toJSONNode(TAG_SERVICE_NAME_STR, service.getServiceName());
    item.push_back(*(jSon_ServiceName.begin()));

/*
    JSONNode jSon_ServiceName;
    value = service.getServiceName();

    StringUtils::convertStandardStr(value);
    string strJsonServiceName = string("{\"") + TAG_SERVICE_NAME_STR + "\" : \"" + value + "\"}";
    try {
        jSon_ServiceName = libjson::parse(strJsonServiceName);
    } catch(const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << '\n';
    }
    item.push_back(*(jSon_ServiceName.begin()));
    //item.push_back(JSONNode(TAG_SERVICE_NAME_STR, service.serviceName));
*/
    //image
    std::cout << "ImageId = " << service.getImageId() << std::endl;
    item.push_back(*(toJSONNode(TAG_IMAGE_STR, service.getImageId()).begin()));

    //producer
    std::cout << "Producer = " << service.getProducer() << std::endl;
    item.push_back(*(toJSONNode(TAG_PRODUCER_STR, service.getProducer()).begin()));
    /*
    JSONNode jSon_Producer;
    value = service.getProducer();
    StringUtils::convertStandardStr(value);
    string std_jSon_Producer = string("{\"") + TAG_PRODUCER_STR + "\" : \"" + value + "\"}";
    try {
        jSon_Producer = libjson::parse(std_jSon_Producer);
    } catch(const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << '\n';
    }
    item.push_back(*(jSon_Producer.begin()));
    */

    /// Status
    std::cout << "Status = " << service.getStatus() << std::endl;
    item.push_back(JSONNode(TAG_STATUS_STR, service.getStatus()));

    // max number of node
    item.push_back(JSONNode(TAG_NUMBER_OF_NODES_STR, service.getNumberOfNode()));

    //description
    std::cout << "description = " << service.getDescription() << std::endl;
    item.push_back(*(toJSONNode(TAG_DESCRIPTION_STR, service.getDescription()).begin()));
    /*
    JSONNode jSon_Des;
    value = service.getDescription();
    StringUtils::convertStandardStr(value);
    string str_jSon_Des = string("{\"") + TAG_DESCRIPTION_STR + "\" : \"" + value + "\"}";
    try {
        jSon_Des = libjson::parse(str_jSon_Des);
    } catch(const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << '\n';
    }
    item.push_back(*(jSon_Des.begin()));
    //item.push_back(JSONNode(TAG_DESCRIPTION_STR, service.description));
*/
    /// Path
    std::cout << "Path = " << service.getPathExcuteFile() << std::endl;
    item.push_back(*(toJSONNode(TAG_PATH_EXCUTE_FILE_STR, service.getPathExcuteFile()).begin()));

    /// ShPath
    item.push_back(*(toJSONNode(TAG_PATH_SH_FILE_STR, service.getPathShFile()).begin()));

    // Stage-in dirs
    JSONNode stginArr(JSON_ARRAY);
    stginArr.set_name(TAG_STAGEINDIRS_STR);
    for(unsigned int i = 0; i < service.getStageinDirs()->size(); i ++){
      std::string stgin = service.getStageinDirs()->at(i);
      stginArr.push_back(toJSONNode(TAG_PATH_STR, stgin));
    }
    item.push_back(stginArr);

    // Stage-out dirs
    JSONNode stgoutArr(JSON_ARRAY);
    stgoutArr.set_name(TAG_STAGEOUTDIRS_STR);
    for(unsigned int i = 0; i < service.getStageoutDirs()->size(); i ++){
      std::string stgout = service.getStageoutDirs()->at(i);
      stgoutArr.push_back(toJSONNode(TAG_PATH_STR, stgout));
      //stgoutArr.push_back(*(toJSONNode(TAG_PATH_STR, stgout).begin()));
    }
    item.push_back(stgoutArr);


//    JSONNode jSon_Path;
//    value = service.getPathExcuteFile();
//    StringUtils::convertStandardStr(value);
//    string str_jSon_Path = string("{\"") + TAG_PATH_EXCUTE_FILE_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon_Path = libjson::parse(str_jSon_Path);
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }
//    item.push_back(*(jSon_Path.begin()));
    //item.push_back(JSONNode(TAG_PATH_EXCUTE_FILE_STR, service.pathExcuteFile));

    //
    item.push_back(JSONNode(TAG_NUMBER_OF_USER_STR, service.getNumberOfUser()));
    //
    item.push_back(JSONNode(TAG_NUMBER_OF_WAIT_USER_STR, service.getNumberOfWaitUser()));

    /// Provider
    std::cout << "Provider = " << service.getProvider() << std::endl;
    item.push_back(*(toJSONNode(TAG_PROVIDER_STR, service.getProvider()).begin()));
//    JSONNode jSon_Prov;
//    value = service.getProvider();
//    StringUtils::convertStandardStr(value);
//    string str_jSon_Prov = string("{\"") + TAG_PROVIDER_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon_Prov = libjson::parse(str_jSon_Prov);
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }
//    item.push_back(*(jSon_Prov.begin()));
    //item.push_back(JSONNode(TAG_PROVIDER_STR, service.providerId));

    // max elapse time
    item.push_back(JSONNode(TAG_MAX_ESLAPSED_TIME_STR, service.getMaxElapseTime()));

    return item;
}

/*
 * Create JSONNode from Parameter
 */
JSONNode JsonUtils::toJSONNode(Parameter param)
{
    JSONNode item(JSON_NODE);
    //std::string value;
    //n.push_back(JSONNode(TAG_SERVICE_STR, service.serviceId));
    //n.push_back(JSONNode(TAG_PROVIDER_STR, service.providerId));

    //id
    item.push_back(JSONNode(TAG_ID_STR, param.getParamID()));
//    JSONNode jSon_ParamId;
//    value = param.getParamID();
//    convertStandardStr(value);
//    std::string strjSon_ParamId = string("{\"") + TAG_ID_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon_ParamId = libjson::parse(strjSon_ParamId);
//        item.push_back(*(jSon_ParamId.begin()));
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }

    //item.push_back(JSONNode(TAG_ID_STR, ((Parameter )*it).getParamID()));

    //service id
    item.push_back(JSONNode(TAG_SERVICE_STR, param.getServiceID()));

    //name
    item.push_back(*(toJSONNode(TAG_PARAMETER_NAME_STR, param.getName()).begin()));
//    JSONNode jSon_ParamName;
//    value = param.getName();
//    convertStandardStr(value);
//    string strjSon_ParamName = string("{\"") + TAG_PARAMETER_NAME_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon_ParamName = libjson::parse(strjSon_ParamName);
//        item.push_back(*(jSon_ParamName.begin()));
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }

    //item.push_back(JSONNode(TAG_PARAMETER_NAME_STR, param.getName()));

    //
    item.push_back(*(toJSONNode(TAG_DESCRIPTION_STR, param.getDescription()).begin()));
//    JSONNode jSon;
//    value = param.getDescription();
//    convertStandardStr(value);
//    string strjSon = string("{\"") + TAG_DESCRIPTION_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon = libjson::parse(strjSon);
//        item.push_back(*(jSon.begin()));
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }

    //item.push_back(JSONNode(TAG_DESCRIPTION_STR, ((Parameter )*it).getDescription()));
    //

    item.push_back(JSONNode(TAG_IS_OUTPUT_STR, param.getIsOutput()));

    item.push_back(JSONNode(TAG_IS_REQUIRED_STR, param.getIsRequired()));

    //

    item.push_back(*(toJSONNode(TAG_OPTION_NAME_STR, param.getOptionName()).begin()));
//    value = param.getOptionName();
//    convertStandardStr(value);
//    strjSon = string("{\"") + TAG_OPTION_NAME_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon = libjson::parse(strjSon);
//        item.push_back(*(jSon.begin()));
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }
    //item.push_back(JSONNode(TAG_OPTION_NAME_STR, ((Parameter )*it).getOptionName()));

    //
    item.push_back(*(toJSONNode(TAG_TYPE_STR, param.getType()).begin()));
//    value = param.getType();
//    convertStandardStr(value);
//    strjSon = string("{\"") + TAG_TYPE_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon = libjson::parse(strjSon);
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }
//    item.push_back(*(jSon.begin()));

    //item.push_back(JSONNode(TAG_TYPE_STR, ((Parameter )*it).getType()));

    //
    item.push_back(*(toJSONNode(TAG_DEFAULT_VALUE_STR, param.getDefaultValue()).begin()));
//    value = param.getDefaultValue();
//    convertStandardStr(value);
//    strjSon = string("{\"") + TAG_DEFAULT_VALUE_STR + "\" : \"" + value + "\"}";
//    try {
//        jSon = libjson::parse(strjSon);
//        item.push_back(*(jSon.begin()));
//    } catch(const std::invalid_argument& e) {
//        std::cerr << "Invalid argument: " << e.what() << '\n';
//    }
    //item.push_back(JSONNode(TAG_DEFAULT_VALUE_STR, ((Parameter )*it).getDefaultValue()));

    /*
    item.push_back(JSONNode(TAG_ID_STR, param.getParamID()));
    item.push_back(JSONNode(TAG_SERVICE_STR, param.getServiceID()));
    item.push_back(JSONNode(TAG_PARAMETER_NAME_STR, param.getName()));
    item.push_back(JSONNode(TAG_DESCRIPTION_STR, param.getDescription()));
    */
    return item;
}

/*
 * Create JSONNode from KP_Job
 */
JSONNode JsonUtils::toJSONNode(Job job)
{
    JSONNode n(JSON_NODE);
    n.push_back(JSONNode(TAG_ID_STR, job.getId()));
    return n;
}

