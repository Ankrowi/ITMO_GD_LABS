#include "MemoryAllocator.h"

#include <cassert>
#include <iostream>
#include <string>

MemoryAllocator::MemoryAllocator() {
#ifdef _DEBUG
	isInitialized = false;
	isDestroyed = false;
#endif
}

MemoryAllocator::~MemoryAllocator() {
#ifdef _DEBUG
	assert(isDestroyed);
#endif
}

void MemoryAllocator::init() {
	fsa16.init(16, blocksPerFSAllocator);
	fsa32.init(32, blocksPerFSAllocator);
	fsa64.init(64, blocksPerFSAllocator);
	fsa128.init(128, blocksPerFSAllocator);
	fsa256.init(256, blocksPerFSAllocator);
	fsa512.init(512, blocksPerFSAllocator);

	ca.init(MaxInnerAllocation);
#ifdef _DEBUG
	isInitialized = true;
	startBlock = nullptr;
#endif
}

void MemoryAllocator::destroy() {
	fsa16.destroy();
	fsa32.destroy();
	fsa64.destroy();
	fsa128.destroy();
	fsa256.destroy();
	fsa512.destroy();

	ca.destroy();
#ifdef _DEBUG
	isDestroyed = true;
#endif
}

void* MemoryAllocator::alloc(size_t size) {
#ifdef _DEBUG
	assert(isInitialized);
#endif
	if (size <= 16)
		return fsa16.alloc();
	if (size <= 32)
		return fsa32.alloc();
	if (size <= 64)
		return fsa64.alloc();
	if (size <= 128)
		return fsa128.alloc();
	if (size <= 256)
		return fsa256.alloc();
	if (size <= 512)
		return fsa512.alloc();
	if (size <= MaxInnerAllocation)
		return ca.alloc(size);
#ifndef _DEBUG
	return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
	Block* block_ = static_cast<Block*>(VirtualAlloc(NULL, sizeof(Block) +
		size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
	block_->size = size;
	block_->nextBlock = startBlock;
	block_->start = static_cast<void*>(static_cast<byte*>(static_cast<void*>(block_)) + sizeof(Block));
	startBlock = block_;
	return block_->start;
#endif
}

void MemoryAllocator::free(void* ptr) {
#ifdef _DEBUG
	assert(isInitialized);
#endif
	if (fsa16.hasPtr(ptr))
		return fsa16.free(ptr);
	if (fsa32.hasPtr(ptr))
		return fsa32.free(ptr);
	if (fsa64.hasPtr(ptr))
		return fsa64.free(ptr);
	if (fsa128.hasPtr(ptr))
		return fsa128.free(ptr);
	if (fsa256.hasPtr(ptr))
		return fsa256.free(ptr);
	if (fsa512.hasPtr(ptr))
		return fsa512.free(ptr);
	if (ca.hasPtr(ptr))
		return ca.free(ptr);

#ifdef _DEBUG
	Block* prev = nullptr;
	Block* iter = startBlock;
	while (iter->start != ptr && iter != nullptr) {
		prev = iter;
		iter = iter->nextBlock;
	}
	assert(iter != nullptr);
	if (prev != nullptr)
		prev->nextBlock = iter->nextBlock;
	else startBlock = iter->nextBlock;
	VirtualFree(iter, 0, MEM_RELEASE);
#else
	VirtualFree(ptr, 0, MEM_RELEASE);
#endif
}

#ifdef _DEBUG

void MemoryAllocator::dumpStat() const {
	using std::cout;
	using std::endl;

	assert(isInitialized);
	cout << "DumpStat from FSA: \n\n";
	fsa16.dumpStat();
	fsa32.dumpStat();
	fsa64.dumpStat();
	fsa128.dumpStat();
	fsa256.dumpStat();
	fsa512.dumpStat();
	cout << "DumpStat from CA: \n\n";
	ca.dumpStat();

	cout << "DumpStat from OS allocated memory: \n\n";
	auto iter = startBlock;
	int cnt = 0;
	std::string report = "";
	while (iter != nullptr) {
		cnt++;
		iter = iter->nextBlock;
	}
	cout << "Blocks taken from OS: " << cnt << endl;

}

void MemoryAllocator::dumpBlocks() const {
	using std::cout;
	using std::endl;

	assert(isInitialized);
	cout << "DumpBlocks from FSA: \n\n";
	fsa16.dumpBlocks();
	fsa32.dumpBlocks();
	fsa64.dumpBlocks();
	fsa128.dumpBlocks();
	fsa256.dumpBlocks();
	fsa512.dumpBlocks();
	cout << "DumpBlocks from CA: \n\n";
	ca.dumpBlocks();

	cout << "DumpBlocks from OS allocated memory: \n\n";
	auto iter = startBlock;
	int cnt = 0;
	while (iter != nullptr) {
		cout << "Block " << ++cnt << " is located " << iter->start << " and has size = " << iter->size << endl;
		iter = iter->nextBlock;
	}
}

#endif