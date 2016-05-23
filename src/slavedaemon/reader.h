/* file reader.h
   author
   date
*/
#ifndef READER_H_
#define READER_H_

#include <stdint.h>
#include <cstdio>
#include <vector>
#include <string>

#define BLOCK_SIZE 3  // 4kbB.
const int32_t kBytePerBlock = BLOCK_SIZE * 1024; //number byte for each block
class Reader {
 public:


  ~Reader() {
    std::fclose(file_);
  }

  bool IsOpened() {
    if (file_ == NULL) {
      return false;
    } else {
      return true;
    }
  }

  bool IsEOF() {
    if (block_position_ > numblock_) {
      return true;
    } else {
      return false;
    }
  }

  Reader(const std::string &filename) {
    file_ = std::fopen(filename.c_str(), "rb");
    if (IsOpened()) {
      block_position_ = 1;
      filesize_ = GetFileSize();
      if (filesize_ % kBytePerBlock == 0) {
        numblock_ = filesize_ / kBytePerBlock;
      } else {
        numblock_ = filesize_ / kBytePerBlock + 1;
      }
    }
  }

  int32_t Read(char **block) {
    if (block_position_ < numblock_) {
      *block = new char[kBytePerBlock];
      std::fseek(file_, kBytePerBlock * (block_position_ - 1), SEEK_SET);
      if (std::fread(*block, sizeof(char), kBytePerBlock, file_) == 0) {
        return -1;
      }
      block_position_ ++;
      return kBytePerBlock;
    } else if (block_position_ == numblock_) {
      int32_t rest_of_byte = filesize_ - (kBytePerBlock * (numblock_ - 1));
      *block = new char[rest_of_byte];
      std::fseek(file_, kBytePerBlock * (block_position_ - 1), SEEK_SET);
      if (std::fread(*block, sizeof(char), rest_of_byte, file_) == 0) {
        return -1;
      }
      block_position_ ++;
      return rest_of_byte;
    } else
      return 0;
  }

 private:
  int32_t GetFileSize() {
    std::fseek(file_, 0, SEEK_END);
    int32_t seek_end = ftell(file_);
    std::fseek(file_, 0, SEEK_SET);
    return seek_end;
  }

 private:
  FILE *file_;
  int numblock_;
  int32_t filesize_;
  int block_position_; // current block 's being read
}; //class Reader
#endif //READER_H_


