#pragma once
#include <vector>

class memory_pool
{
public:
	explicit memory_pool(unsigned int num, unsigned int size);
	~memory_pool();
	
public:
	void* Allocate();
	void Release(void* ptr);

private:
	unsigned int _total_blocks;
	unsigned int _block_size;
	std::vector<void*> _free_blocks;
	std::vector<void*> _busy_blocks;
};