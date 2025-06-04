#include <iostream>
#include <climits>

#include "SegmentTree.hpp"

int main()
{
	int arr[6] = {2, 5, 1, 4, 9, 3};

	// Make a segment tree for range minimum number queries
	SegmentTree<int> st(arr, 6, [](int a, int b){ return std::min(a, b); }, INT_MAX);

	// Test cases
	std::cout << "0, 1: " << st.queryValue(0, 1) << '\n';
	std::cout << "3, 5: " << st.queryValue(3, 5) << '\n';
	std::cout << "0, 5: " << st.queryValue(0, 5) << '\n';

	return 0;
}
