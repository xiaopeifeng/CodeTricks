#include "memory_pool.h"
#include <iostream>
#include <windows.h>

MemoryPool::MemoryPool(size_t num, size_t size)
	: m_free_blocks(num)
	, m_total(num)
	, m_block_size(size)
{
	for (int i = 0; i < size; ++i)
	{
		m_buf = new unsigned char[(size + head_size) * num];
	}
	Initial();
}

MemoryPool::~MemoryPool()
{
	delete[] m_buf;
}

void MemoryPool::Initial()
{
	unsigned pos = 0;
	for (unsigned int i = 0; i < m_total; ++i)
	{
		m_buf[pos++] = 0;
		m_buf[pos++] = 0;
		m_buf[pos++] = 0;
		m_buf[pos++] = 0;

		m_buf[pos++] = 0;
		m_buf[pos++] = 0;

		m_buf[pos++] = (unsigned char) (i >> 8);
		m_buf[pos++] = (unsigned char) i;
		pos += m_block_size;
	}
}

void* MemoryPool::Alloc()
{
	if (((int)AtomDecreaseBlockNum()) >= 0)
	{
		for (size_t i = 0; i < m_total; ++i)
		{
			if (IsFree(i))
			{
				if (AtomSetBusy(i) == 0)
				{
					return m_buf + (m_block_size + head_size)*i + head_size;
				}
				else
				{
					// set busy by other threads, deal noting here.
				}
			}
		}
	}
	else
	{
		AtomIncreaseBlockNum();
		std::cout << "not enough space..." << std::endl;
		return nullptr;
	}
	return nullptr;
}

bool MemoryPool::IsFree(size_t i)
{
	return (m_buf[(head_size + m_block_size) * i] == 0);
}

void MemoryPool::Delete(void* ptr)
{
	unsigned int index = 0;
	unsigned char* head = (unsigned char*)ptr - head_size;
	index = head[6];
	index = (index << 8) + head[7];
	AtomSetFree(index);
	AtomIncreaseBlockNum();
}

int MemoryPool::GetFreeNum()
{
	return m_free_blocks;
}

unsigned int MemoryPool::AtomSetBusy(size_t i)
{
	void* ptr = m_buf + (head_size + m_block_size) * i;
	return InterlockedExchangeAdd((long*)(ptr), (long)(1));
}

unsigned int MemoryPool::AtomSetFree(size_t i)
{
	void* ptr = m_buf + (head_size + m_block_size) * i;
	return InterlockedExchangeAdd((long*)(ptr), (long)(-1));
}

//返回新值
unsigned int MemoryPool::AtomIncreaseBlockNum()
{
	return InterlockedIncrement((long*)(&m_free_blocks));
}

//返回新值
unsigned int MemoryPool::AtomDecreaseBlockNum()
{
	return InterlockedDecrement((long*)(&m_free_blocks));
}
