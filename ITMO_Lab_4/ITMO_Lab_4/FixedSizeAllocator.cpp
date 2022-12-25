#include "FixedSizeAllocator.h"
#include <iostream>
#include <cassert>

FixedSizeAllocator::FixedSizeAllocator() {
#ifdef _DEBUG
	isInitialised = false;
	isDestroyed = false;
#endif
}

FixedSizeAllocator::~FixedSizeAllocator() {
#ifdef _DEBUG
	assert(isDestroyed);
#endif
}

void FixedSizeAllocator::init(size_t blockSize, int blockCnt) {
#ifdef _DEBUG
	isInitialised = true;
#endif
	blockSize_ = blockSize;
	blocksCnt_ = blockCnt;
	buffer_ = nullptr;
	AllocBuffer();
}

void FixedSizeAllocator::destroy() {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	FreeBuffer(buffer_);
#ifdef _DEBUG
	isDestroyed = true;
#endif
}

void* FixedSizeAllocator::alloc() {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	Buffer* another = buffer_;
	while (another != nullptr) {
		if (another->blocksBusy < blocksCnt_) {
			return static_cast<byte*>(another->bufferStart) + another->blocksBusy++ * blockSize_;
		}
		if (another->FLH != -1) {
			void* ptr = static_cast<byte*>(another->bufferStart) + another->FLH * blockSize_;	
			another->FLH = *(static_cast<int*>(ptr));
			return ptr;
		}
		another = another->nextBuffer;
	}
	AllocBuffer();
	return alloc();
}

void FixedSizeAllocator::free(void* ptr) {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	auto another = buffer_;
	while (another != nullptr) {
		if (ptr >= another->bufferStart &&
			ptr <= static_cast<void*>(static_cast<byte*>(another->bufferStart) + blocksCnt_ * blockSize_)) 
		{
			*(static_cast<int*>(ptr)) = another->FLH;
			another->FLH = static_cast<int>((static_cast<byte*>(ptr) - static_cast<byte*>(another->bufferStart)) / blockSize_);
			return;
		}
		another = another->nextBuffer;
	}
}

bool FixedSizeAllocator::hasPtr(void* ptr) {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	bool hasPtr = false;
	auto another = buffer_;
	while (another != nullptr) {
		if (ptr >= another->bufferStart &&
			ptr <= static_cast<void*>(static_cast<byte*>(another->bufferStart) + blocksCnt_ * blockSize_)) 
		{
			hasPtr = true;
			break;
		}

	}
	return hasPtr;
}

void FixedSizeAllocator::AllocBuffer() {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	auto buffer = static_cast<Buffer*>(VirtualAlloc(NULL, sizeof(Buffer) +
		blockSize_ * blocksCnt_, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
	volatile size_t s = sizeof(Buffer);
	buffer->nextBuffer = buffer_;
	buffer->blocksBusy = 0;
	buffer->FLH = -1;
	buffer->bufferStart = static_cast<byte*>((void*)buffer) + sizeof(Buffer);
	buffer_ = buffer;
}

void FixedSizeAllocator::FreeBuffer(Buffer* buffer) {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	if (buffer == nullptr)
		return;
	FreeBuffer(buffer->nextBuffer);
	VirtualFree(buffer, 0, MEM_RELEASE);
}

#ifdef _DEBUG

void FixedSizeAllocator::dumpStat() const {
	assert(isInitialised);
	using namespace std;
	cout << "____________________________________\n";
	cout << "FixedSizeAllocator report:" << endl;
	cout << "My blockSize is: " << blockSize_ << " , My blocksCount: " << blocksCnt_ << endl;
	auto another = buffer_;
	int buffCnt = 0;
	int freeBlocks = 0;
	int busyBlocks = 0;
	while (another != nullptr) {
		buffCnt++;
		int cntFree = blocksCnt_ - another->blocksBusy;
		int iter = another->FLH;
		while (iter != -1) {
  			cntFree++;
			iter = *(static_cast<int*>(static_cast<void*>(static_cast<byte*>(another->bufferStart) + iter * blockSize_)));
		}
		freeBlocks += cntFree;
		busyBlocks += blocksCnt_ - freeBlocks;
		another = another->nextBuffer;
	}
	cout << "Total amount of buffers: " << buffCnt << endl;
	cout << "Amount of free blocks = " << freeBlocks << ", amount of busy blocks = " << busyBlocks << endl;
}

void FixedSizeAllocator::dumpBlocks() const {
	assert(isInitialised);
	using namespace std;
	auto another = buffer_;
	cout << "____________________________________\n";
	cout << "FixedSizeAllocator blocks info (my size is " << blockSize_ << "):\n";
	while (another != nullptr) {
		cout << "	Block is located at " << another << endl;
		cout << "	Block has size: " << blocksCnt_ * blockSize_ + sizeof(Buffer) << endl << endl;
		another = another->nextBuffer;
	}
}

#endif
