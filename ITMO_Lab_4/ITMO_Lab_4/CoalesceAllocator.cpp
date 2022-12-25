#include "CoalesceAllocator.h"

#include <cassert>

CoalesceAllocator::CoalesceAllocator() {
#ifdef _DEBUG
	isInitialised = false;;
	isDestroyed = false;;
#endif
}

CoalesceAllocator::~CoalesceAllocator() {
#ifdef _DEBUG
	assert(isDestroyed);
#endif
}

void CoalesceAllocator::init(size_t bufferSize) {
#ifdef _DEBUG
	isInitialised = true;
#endif
	bufferSize_ = bufferSize;
	buffer_ = nullptr;
	AllocBuffer();
}

void CoalesceAllocator::destroy() {
	FreeBuffer(buffer_);
#ifdef _DEBUG
	isDestroyed = true;
#endif
}

void* CoalesceAllocator::alloc(size_t blockSize) {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	Buffer* another = buffer_;
	while (another != nullptr) {
		Block* iter = another->start;
		while (static_cast<byte*>(static_cast<void*>(iter)) <
			static_cast<byte*>(static_cast<void*>(another->start)) + bufferSize_) {
			if (iter->isFree && iter->size >= blockSize + sizeof(Block)){
				if (iter->size < 2 * sizeof(Block) + blockSize) {
					iter->isFree = false;
					return static_cast<void*>(static_cast<byte*>(static_cast<void*>(iter)) + sizeof(Block));
				}
				Block* next = static_cast<Block*>(static_cast<void*>(
					static_cast<byte*>(static_cast<void*>(iter)) + blockSize + sizeof(Block)));
				next->prevBlock = iter;
				next->isFree = true;
				next->size = iter->size - blockSize - sizeof(Block);
				iter->isFree = false;
				iter->size = blockSize;
				return static_cast<void*>(static_cast<byte*>(static_cast<void*>(iter)) + sizeof(Block));
			}
			iter = static_cast<Block*>(static_cast<void*>(static_cast<byte*>(static_cast<void*>(iter)) + iter->size + sizeof(Block)));
		}
		another = another->nextBuffer;
	}
	AllocBuffer();
	return alloc(blockSize);
}

void CoalesceAllocator::free(void* ptr) {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	Buffer* another = buffer_;
	while (another != nullptr) {
		if ( static_cast<void*>(another + bufferSize_) > ptr 
			&& ptr > static_cast<void*>(another))  {
			Block* ptr_ = static_cast<Block*>(static_cast<void*>(static_cast<byte*>(ptr) - sizeof(Block)));
			Block* next = static_cast<Block*>(static_cast<void*>(static_cast<byte*>(static_cast<void*>(ptr_)) + sizeof(Block) + ptr_->size));
			if (next->isFree) {
				ptr_->size += next->size + sizeof(Block);
				ptr_->isFree = true;
			}
			if (ptr_->prevBlock != nullptr && ptr_->prevBlock->isFree) {
				ptr_->prevBlock->size += ptr_->size + sizeof(Block);
			}
			return;
		}
		another = another->nextBuffer;
	}
	std::cout << "\nfuck, not freed :((((!!!!!\n";
}

bool CoalesceAllocator::hasPtr(void* ptr) {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	bool hasPtr = false;
	auto another = buffer_;
	while (another != nullptr) {
		if (ptr >= another->bufferStart &&
			ptr <= static_cast<void*>(static_cast<byte*>(another->bufferStart) + bufferSize_))
		{
			hasPtr = true;
			break;
		}

	}
	return hasPtr;
}

#ifdef _DEBUG

void CoalesceAllocator::dumpStat() const {
	assert(isInitialised);
	using namespace std;
	cout << "____________________________________\n";
	cout << "CoalesceAllocator report:" << endl;
	cout << "My bufferSize is: " << bufferSize_ << endl;
	auto another = buffer_;
	int buffCnt = 0;
	int freeBlocks = 0;
	int busyBlocks = 0;
	while (another != nullptr) {
		buffCnt++;
		auto iter = another->start;
		while (static_cast<byte*>(static_cast<void*>(iter)) - static_cast<byte*>(another->bufferStart) < 
			bufferSize_) {
			if (iter->isFree) freeBlocks++;
			else busyBlocks++;
			iter = static_cast<Block*>(static_cast<void*>(static_cast<byte*>(static_cast<void*>(iter)) + iter->size));
		}
		another = another->nextBuffer;
	}
	cout << "Total amount of buffers: " << buffCnt << endl;
	cout << "Amount of free blocks = " << freeBlocks << ", amount of busy blocks = " << busyBlocks << endl;
}

void CoalesceAllocator::dumpBlocks() const {
	assert(isInitialised);
	using namespace std;
	auto another = buffer_;
	cout << "____________________________________\n";
	cout << "CoalesceAllocator blocks info: " << "\n";
	int buffCnt = 0;
	while (another != nullptr) {
		cout << "Buffer " << ++buffCnt << ":\n";
		auto iter = another->start;
		while (static_cast<byte*>(static_cast<void*>(iter))
			- static_cast<byte*>(another->bufferStart) < bufferSize_) {
			cout << "Block is located at " << iter << " and has size = " << iter->size << endl;
			iter = static_cast<Block*>(static_cast<void*>(static_cast<byte*>(static_cast<void*>(iter)) + iter->size));
		}
		another = another->nextBuffer;
	}
}

#endif

void CoalesceAllocator::AllocBuffer() {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	auto buffer = static_cast<Buffer*>(VirtualAlloc(NULL, sizeof(Buffer) +
		bufferSize_ + sizeof(Block), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));
	volatile size_t s = sizeof(Buffer);
	buffer->nextBuffer = buffer_;
	buffer->bufferStart = static_cast<byte*>((void*)buffer) + sizeof(Buffer);

	Block* start_ = static_cast<Block*>(static_cast<void*>(buffer->bufferStart));
	start_->isFree = true;
	start_->prevBlock = nullptr;
	start_->size = bufferSize_;
	buffer->start = start_;

	buffer_ = buffer;
}

void CoalesceAllocator::FreeBuffer(Buffer* buffer) {
#ifdef _DEBUG
	assert(isInitialised);
#endif
	if (buffer == nullptr)
		return;
	FreeBuffer(buffer->nextBuffer);
	VirtualFree(buffer, 0, MEM_RELEASE);
}