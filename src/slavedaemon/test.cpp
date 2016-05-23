#include <iostream>
#include "reader.h"
#include "writer.h"
int Reader::block_position_ = 1;
int main(int argc, char* argv[]) {
  Reader *x = new Reader("text_src");
  Writer *y = new Writer("text_dest");
  if (!x->IsOpened() && !y->IsOpened()) {
    std::cout << "Could not open file\n";
    return 1;
  }
  std::vector<uchar> *block = new std::vector<uchar>;
  int i = 0;
  while (!x->IsLastBlock()){
    i++;
    block->clear();
    x->Read(block);
    y->Write(*block);
  }
  x->~Reader();
  y->~Writer();
  return 0;
}
