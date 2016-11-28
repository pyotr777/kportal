#ifndef BLOCK_DATA_HPP
#define BLOCK_DATA_HPP

#define  BLOCK_SIZE	1024

class BlockData
{
	public:
		char *data1;
		char data[BLOCK_SIZE];
		int index;
		int total;
};

#endif //BLOCK_DATA_HPP
