/* file writefile
   author
   date
*/
#ifndef WRITER_H_
#define WRITER_H_

#include <cstdio>
#include <string>
#include <vector>

class Writer {
 public:
//  Writer(const std::string &filename);
  ~Writer() {
    std::fclose(file_);
  }
  bool IsOpened() {
    if (file_ == NULL) {
      return false;
    } else {
      return true;
    }
  }
  
  Writer(const std::string &filename) {
    file_ = std::fopen(filename.c_str(), "a");
  }

  int32_t Write(const char* list, int32_t list_size) {
    if (list_size == -1) {
      std::cout << "error list_size\n";
      return -1;
    }
    if (std::fwrite(list, sizeof(char), list_size, file_) == 0) {
      std::cout << "error write\n";
      return -1;
    }
    return list_size;
  }
 private:
  FILE *file_;
}; //class Writer



#endif //WRITER_H_
