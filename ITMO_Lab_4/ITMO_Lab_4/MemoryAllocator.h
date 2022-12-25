#pragma once

#include "CoalesceAllocator.h"
#include "FixedSizeAllocator.h"

constexpr int blocksPerFSAllocator = 256;
constexpr int MaxInnerAllocation = 1024 * 1024 * 10;

class MemoryAllocator
{
private:
	FixedSizeAllocator fsa16, fsa32, fsa64, fsa128, fsa256, fsa512;
	CoalesceAllocator ca;
#ifdef _DEBUG
	struct Block {
		size_t size;
		Block* nextBlock;
		void* start;
	};
	Block* startBlock;
	bool isInitialized;
	bool isDestroyed;
#endif
public:
	MemoryAllocator();
	virtual ~MemoryAllocator();

	virtual void init();
	virtual void destroy();

	virtual void* alloc(size_t size);
	virtual void free(void* ptr);

#ifdef _DEBUG

	virtual void dumpStat() const;
	virtual void dumpBlocks() const;

#endif
};

