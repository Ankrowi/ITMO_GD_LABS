#pragma once

#include "windows.h"
#include <iostream>

class CoalesceAllocator
{
public:
private:
	struct Block {
		size_t size;
		Block* prevBlock;
		bool isFree;
	};
	struct Buffer {
		void* bufferStart;
		Block* start;
		Buffer* nextBuffer;
	};
	Buffer* buffer_;
	size_t bufferSize_;
#ifdef _DEBUG
	bool isInitialised;
	bool isDestroyed;
#endif
public:
	CoalesceAllocator();
	~CoalesceAllocator();

	void init(size_t bufferSize);
	void destroy();

	void* alloc(size_t blockSize);
	void free(void* ptr);
	bool hasPtr(void* ptr);

#ifdef _DEBUG

	void dumpStat() const;
	void dumpBlocks() const;

#endif
private:
	void AllocBuffer();
	void FreeBuffer(Buffer* buffer);
};

