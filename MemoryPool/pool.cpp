#include "pool.h"
#include <algorithm>

memory_pool::memory_pool(unsigned int num, unsigned int size)
	: _total_blocks(num)
	, _block_size(size)
{
	for (auto i = 0; i < num; ++i)
	{
		_free_blocks.push_back(new unsigned char[size]);
	}
}

memory_pool::~memory_pool()
{
	if (_free_blocks.size() != 0)
	{
		for (auto iter = _free_blocks.begin(); iter != _free_blocks.end(); ++iter)
		{
			delete *iter;
		}
	}
	if (_busy_blocks.size() != 0)
	{
		for (auto iter = _busy_blocks.begin(); iter != _busy_blocks.end(); ++iter)
		{
			delete *iter;
		}
	}
}

void* memory_pool::Allocate()
{
	if (_free_blocks.size())
	{
		void* ptr = _free_blocks[_free_blocks.size() - 1];
		_busy_blocks.push_back(ptr);
		_free_blocks.erase(_free_blocks.end() - 1, _free_blocks.end());
		return ptr;
	}
}

void memory_pool::Release(void* ptr)
{
	memset(ptr, 0, _block_size);
	_free_blocks.push_back(ptr);
	_busy_blocks.erase(
		std::remove(_busy_blocks.begin(), _busy_blocks.end(), ptr), _busy_blocks.end());
}