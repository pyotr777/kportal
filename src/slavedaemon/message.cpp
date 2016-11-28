#include "message.hpp"
Message::Message(){
  m_data = NULL;
}

Message::Message(Header hdr, char* data){
  m_header = hdr;
  m_data = data;
}

Message::~Message(){
//  std::cout << "Message::~Message()\n";
  if(m_data != NULL && m_header.GetDataSize() > 0){
    delete[] m_data;
  }
}

const Header* Message::GetHeader() const{
  return &m_header;
}

void Message::SetHeader(const Header &hdr){
  m_header = hdr;
}

void Message::SetData(char* buf){
  m_data = buf;
}

char* Message::GetData() const {
  return m_data;
}

int Message::SetArgs(char *args, char **argv)
{
   int count = 0;

   while (isspace(*args)) ++args;
   while (*args) {
     if (argv)
       argv[count] = args;
     while (*args && !isspace(*args))
       ++args;
     if (argv && *args)
       *args++ = '\0';
     while (isspace(*args))
       ++args;
     count++;
   }
   return count;
}

char ** Message::ParsedArgs(char *args, int *argc) {
//  std::cout << "Message::ParsedArg\n";
   char **argv = NULL;
   int    argn = 0;

   if (args && *args
    && (args = strdup(args))
    && (argn = SetArgs(args,NULL))
    && (argv = (char**)malloc((argn+1) * sizeof(char *)))) {
      *argv++ = args;
      argn = SetArgs(args,argv);
   }

   if (args && !argv) free(args);

   *argc = argn;
//   std::cout << "argc = " << *argc << std::endl;
   return argv;
}

void Message::FreeParsedArgs(char **argv)
{
  if (argv) {
    std::cout << "Message::FreeParsedArgs argv[-1],";
    free(argv[-1]);
    std::cout << "free(argv-1),";
    free(argv-1);
    std::cout << " success\n";
  }
}

void Message::ToBytes(char **bytes, unsigned int &msg_size) const {
  //std::cout <<"Message::ToBytes" << std::endl;
  Header c_hdr(m_header);
  msg_size = 0;
  char* hdr = reinterpret_cast<char*>(&c_hdr);
  msg_size = sizeof(Header) + (unsigned int)c_hdr.GetDataSize();
  //std::cout << "msg size = " << msg_size << std::endl;
  *bytes = new char[msg_size];

  memcpy(*bytes, hdr, sizeof(Header));
  if(m_data != NULL && c_hdr.GetDataSize() > 0){

    //std::cout << "copy mdata[0] " << m_data[0] << " - m_data[" << c_hdr.GetDataSize() - 1 << "] = " << m_data[ c_hdr.GetDataSize() - 1 ] << std::endl;
    memcpy(*bytes + sizeof(Header), m_data, c_hdr.GetDataSize());
    //std::cout << "copy mdata success\n";
  } else {
    //std::cout << "data size is empty.\n";
  }
}

ResultCodes Message::FromBytes(char const * buffer, unsigned int read_size){
  std::cout << "Message::FromBytes read_size = " << read_size << "\n";
  ResultCodes parse_rlt = OK;
  int type, fp, tp, ds, cmd;
  char *dt = NULL;
  int offset = 0;
  if(read_size >= sizeof(Header)) {
    // MessageTypes m_type; (4 byte)
    if (!ReadInt32(buffer, offset, &type)) {
      std::cout << "[ERR] Message type is invalid.\n";
      parse_rlt = HDR_MESSAGETYPE_INVALID;
    } else
    // int m_from_port;  (4 byte)
    if (!ReadInt32(buffer, offset += sizeof(int), &fp)) {
      std::cout << "[ERR]FromPort msg attr is invalid.\n";
      parse_rlt = HDR_FROMPORT_INVALID;
    } else
    // int m_to_port; (4 byte)
    if (!ReadInt32(buffer, offset += sizeof(int), &tp)) {
      std::cout << "[ERR] ToPort msg attr is invalid.\n";
      parse_rlt = HDR_TOPORT_INVALID;
    } else
    // int m_data_size; (4 byte)
    if (!ReadInt32(buffer, offset += sizeof(int), &ds)) {
      std::cout << "[ERR] Data size attr is invalid.\n";
      parse_rlt = HDR_DATASIZE_INVALID;
    } else
    // Commands m_command; (4 byte)
    if (!ReadInt32(buffer, offset += sizeof(int), &cmd)) {
      std::cout << "[ERR]Commands attr is invalid.\n";
      parse_rlt = HDR_COMMAND_INVALID;
    }
  } else {
    std::cout << "[ERR]Msg header missing\n";
    parse_rlt = HDR_MISSING;
  }

    int start_offset = sizeof(Header);
    //std::cout << "ReadBytes: read_size = " << read_size << ", start offset = " << start_offset << ", data_size = " << ds <<std::endl;
    if (parse_rlt == OK && read_size >= sizeof(Header) + ds) {
      try {
        dt = new char[ds];
        //std::cout << "before memcpy buffer to dt\n";
        memcpy(dt, buffer + start_offset , ds);
      } catch (int e){
        std::cout << "[ERR] Data content is invalid. Exception: e = ." << std::endl;
        //std::cout << "Parse data: Data content is invalid.\n";
        parse_rlt = DAT_INVALID;
      }
      //if (!ReadBytes(buffer, offset, cc_hdr->GetDataSize(), &dt)) {
      //  std::cout << "Parse data: Data content is invalid.\n";
      //  parse_rlt = DAT_INVALID;
      //}
    } else {
      if(ds != 0){
        std::cout << "[ERR] parse data fail or empty.\n";
        parse_rlt = DAT_INVALID;
      }
    }
    // Update attribute
    if(parse_rlt == OK){
      ///m_header = Header(*cc_hdr);
      m_header.SetType((MessageTypes)type);
      m_header.SetCommand((Commands)cmd);
      m_header.SetDataSize(ds);
      m_header.SetFromPort(fp);
      m_header.SetToPort(tp);

      if(m_data != NULL && m_header.GetDataSize() > 0){
        delete[] m_data;
        m_data = NULL;
      }
      m_data = dt;
    }
    //delete cc_hdr;

  return  parse_rlt;
}

bool Message::ReadInt32(const char * bytes, const int start_offset, int * value){
  //std::cout << "Message::ReadInt32\n";
  try {
    memcpy(value, bytes + start_offset, sizeof(int));
    //*value = (bytes[start_offset + 0] << 24) | (bytes[start_offset + 1] << 16) | (bytes[start_offset + 2] << 8) | bytes[start_offset + 3];
    //int rlt = memcpy((char*)value, bytes + start_offset, sizeof(int));
    //std::cout << "value = " << *value << std::endl;
  }catch (int e){
    std::cout << "[ERR]ReadInt32:exception e = " << e << std::endl;
    return false;
  }
  return true;
}

bool Message::ReadBytes(const char * bytes, int start_offset, unsigned int data_size, char ** data) {
  //std::cout << "ReadBytes: start offset = " << start_offset << ", data_size = " << data_size <<std::endl;
  try {
    *data = new char[data_size];
    memcpy(*data, bytes + start_offset , data_size);
    return true;
  } catch (int e){
    std::cout << "[ERR] ReadBytes:Exception: " << e << std::endl;;
    return false;
  }
}

void Message::AddArgv(const char* cmd_arg, int cmd_arg_size){
  //std::cout << " Message::AddArgv \n";
  if(cmd_arg_size <= 0)
    return;
  unsigned int cur_ds = m_header.GetDataSize();

  char * tmp = NULL;
  if(cur_ds != 0){
    tmp = new char[cur_ds + cmd_arg_size + 1];
    memcpy(tmp, m_data, cur_ds);
    tmp[cur_ds] = CMD_SEPERATOR_CHAR;
    cur_ds ++; // for seperator char
    delete[] m_data;
    m_data = NULL;
  } else {
     tmp = new char[cmd_arg_size];
  }
  memcpy(tmp + cur_ds, cmd_arg, cmd_arg_size);
  m_data = tmp;
  m_header.SetDataSize(cur_ds + cmd_arg_size);

  // Just test
//  char* data_str = new char[m_header.GetDataSize() + 1];
//  strcpy(data_str, m_data);
//  data_str[m_header.GetDataSize()] = '\0';
//  std::cout <<"m_data:" << data_str << std::endl;
//  delete[]data_str;
}

void Message::AddArgv(const std::string &cmd_arg){
  AddArgv(cmd_arg.c_str(), cmd_arg.size());
  /*std::stringstream ss;
  if(cmd_arg.size() = 0)
    ss << "\"\"";
  else  if(cmd_arg.size() = 1){
      ss << "\"" << cmd_arg << "\"";
  } else {
    if(cmd_arg[0] == '"' && cmd_arg[cmd_arg.size() - 1] == '"'){
      ss << "\"" << cmd_arg << "\"";
    }
  }
  std::string tmp = ss.str();
  //std::cout << "String before add:" << tmp << std::endl;
  AddArgv(tmp.c_str(), tmp.size());*/

}

void Message::AddArgv(int cmd_arg){
  std::stringstream ss;
  ss << cmd_arg;
  std::string cmd_arg_str = ss.str();
  AddArgv(cmd_arg_str.c_str(), cmd_arg_str.size());
}

std::vector<std::string> Message::GetCmdArgs(){
  std::cout << "Message::GetCmdArgs ";
  std::vector<std::string> argv, sub_strs;
  if(m_header.GetDataSize()<= 0){
    std::cout << " no argument.\n";
    return argv;
  }

  char* data_str = new char[m_header.GetDataSize() + 1];
  memcpy(data_str, m_data, m_header.GetDataSize());
  data_str[m_header.GetDataSize()] = '\0';
  std::cout <<"m_data:" << data_str << std::endl;

  // Parse args
  std::string data(data_str);
  Split(data,CMD_SEPERATOR_CHAR, &argv);

  std::cout << " argc=" << argv.size() << ":";
  for(unsigned int i = 0; i < argv.size(); i ++){
    std::cout << argv[i] << " ";
  }
  std::cout << std::endl;

  delete[]data_str;
  return argv;
}

