#pragma once

class MemoryPool
{
private:
	enum
	{
		head_size = 8,
	};
public:
	explicit MemoryPool(size_t num, size_t size);
	~MemoryPool();

public:
	void* Alloc();
	void Delete(void* ptr);
	int GetFreeNum();

private:
	void Initial();
	bool IsFree(size_t i);				// first check m_free_blocks > 0, if true decrease with 1, then set the block busy state.

	unsigned int AtomSetBusy(size_t i);
	unsigned int AtomSetFree(size_t i);
	unsigned int AtomIncreaseBlockNum();
	unsigned int AtomDecreaseBlockNum();

private:
	int m_free_blocks;
	size_t m_total;
	size_t m_block_size;
	unsigned char* m_buf;
};