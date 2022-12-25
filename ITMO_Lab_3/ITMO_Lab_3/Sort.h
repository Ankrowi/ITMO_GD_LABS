#pragma once
#include <cassert>
static constexpr int InsertionSortThreshold = 32;

template<typename T, typename Compare>
void insertionSort(T* first, T* last, Compare comp) {
	if (first == last) return;
	for (auto i = first; i <= last; i++) {
		T ref = *i;
		T* j = i - 1;
		while (j >= first && comp(ref, *j)) {
			*(j + 1) = std::move(*j);
			j--;
		}
		*(j + 1) = std::move(ref);
	}
}


template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp)
{
	if (!first || !last) throw std::runtime_error("nullptr was passed");
	if (first >= last) return;
	last--;
	while (first < last)
	{
		if (last - first < InsertionSortThreshold)
		{
			insertionSort(first, last, comp);
			return;
		}

		T* middle = first + (last - first + 1) / 2;

		if (comp(*last, *first))
			std::swap(*last, *first);

		if (comp(*middle, *first))
			std::swap(*middle, *first);

		if (comp(*last, *middle))
			std::swap(*last, *middle);

		T pivot = *middle;

		auto i = first;
		auto j = last;

		
		do {
			while (comp(*(i), pivot)) i++;
			while (comp(pivot, *(j))) j--;
			if (i <= j) {
				std::swap(*(i), *(j));
				i++;
				j--;
				
			}
		} while (i <= j);

		if (j - first < last - j)
		{
			sort(first, j + 1, comp);
			first = j + 1;
		}
		else
		{
			sort(j + 1, last + 1, comp);
			last = j;
		}
	}
}

