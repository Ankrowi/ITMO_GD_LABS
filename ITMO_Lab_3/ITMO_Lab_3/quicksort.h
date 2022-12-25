#pragma once

template<typename Compare>
void quicksort(int arr[], int first, int last, Compare comp) {
	if (first >= last)
		return;
	last--;
	while (first < last) {
		
		int middle = (first + last + 1) / 2;
		if (comp(arr[last], arr[first]))
			std::swap(arr[last], arr[first]);

		if (comp(arr[middle], arr[first]))
			std::swap(arr[middle], arr[first]);

		if (comp(arr[last], arr[middle]))
			std::swap(arr[last], arr[middle]);

		int pivot = arr[middle];
		int i = first;
		int j = last;
		do {
			while (comp(arr[i], pivot) && i <= last) i++;
			while (comp(pivot, arr[j]) && j >= first) j--;
			if (i <= j) {
				std::swap(arr[i], arr[j]);
				i++;
				j--;
			}
		} while (i <= j);
		if (j - first < last - j) {
			quicksort(arr, first, j + 1, comp);
			first = j + 1;
		}
		else {
			quicksort(arr, j + 1, last + 1, comp);
			last = j;
		}
	}
}