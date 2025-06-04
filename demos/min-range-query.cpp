#include <iostream>
#include <climits>

#include "SegmentTree.hpp"

int main()
{
	int arr[6] = {2, 5, 1, 4, 9, 3};

	// Make a segment tree for range minimum number queries
	SegmentTree<int> st(arr, 6, [](int a, int b){ return std::min(a, b); }, INT_MAX);

	// Test cases
	// Queries
	std::cout << "0, 1: " << st.queryValue(0, 1) << '\n'; // 2
	std::cout << "3, 5: " << st.queryValue(3, 5) << '\n'; // 3
	std::cout << "0, 5: " << st.queryValue(0, 5) << '\n'; // 1

	std::cout << "Updating values...\n";

	// Updates
	st.updateValue(1, -3); // {2, -3, 1, 4, 9, 3};
	st.updateValue(4, -1); // {2, -3, 1, 4, -1, 3};
	st.updateValue(5, -11); // {2, -3, 1, 4, -1, -11};

	// Querying new changes
	std::cout << "0, 1: " << st.queryValue(0, 1) << '\n'; // -3
	std::cout << "3, 4: " << st.queryValue(3, 4) << '\n'; // -1
	std::cout << "0, 5: " << st.queryValue(0, 5) << '\n'; // -11

	return 0;
}
