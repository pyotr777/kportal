#ifndef JSONUTILS_HPP
#define JSONUTILS_HPP

#include "../../include/libjson/libjson.h"
#include "TagKeys.hpp"
#include "StringUtils.hpp"
#include "../datalayer/image_docker.hpp"
#include "../datalayer/user.hpp"
#include "../datalayer/service.hpp"
#include "../datalayer/parameter.h"
#include "../job_manager/job.hpp"


enum Options{
  OPTS_PROVIDER_REGISTER,
  OPTS_PROVIDER
};

class JsonUtils
{
public:
  static JSONNode toJSONNode(const char* tag, const std::string valueStr);
  static JSONNode toJSONNode(const User& provider, const Options& opt);
  static JSONNode toJSONNode(ImageDocker& provider);
  static JSONNode toJSONNode(Service& service);
  static JSONNode toJSONNode(Parameter param);
  static JSONNode toJSONNode(Job job);
};

#endif // JSONUTILS_HPP
