#pragma once
#include "windows.h"
#include "sysinfoapi.h"

constexpr int PageSize = 4096;

class FixedSizeAllocator
{
private:
	struct Buffer {
		void* bufferStart;
		int FLH;
		int blocksBusy;
		Buffer* nextBuffer;
	};
	size_t blockSize_;
	int blocksCnt_;
	Buffer* buffer_;
#ifdef _DEBUG
	bool isInitialised;
	bool isDestroyed;
#endif
public:
	FixedSizeAllocator();
	~FixedSizeAllocator();

	void init(size_t blockSize, int blocksCnt);
	void destroy();

	void* alloc();
	void free(void *ptr);
	bool hasPtr(void* ptr);
#ifdef _DEBUG

	void dumpStat() const;
	void dumpBlocks() const;

#endif

private:
	void AllocBuffer();
	void FreeBuffer(Buffer* buffer);
};

