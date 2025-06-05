#include <iostream>
#include <climits>

#include "SegmentTree.hpp"

int main()
{
	int arr1[6] = {2, 5, 1, 4, 9, 3};

	// Segment tree for range minimum number queries
	SegmentTree<int> st1(arr1, 6, [](int a, int b){ return std::min(a, b); }, INT_MAX);

	// Test cases
	// Queries
	std::cout << "0, 1: " << st1.queryValue(0, 1) << '\n'; // 2
	std::cout << "3, 5: " << st1.queryValue(3, 5) << '\n'; // 3
	std::cout << "0, 5: " << st1.queryValue(0, 5) << '\n'; // 1

	std::cout << "Updating values...\n";

	// Updates
	st1.updateValue(1, -3); // {2, -3, 1, 4, 9, 3};
	st1.updateValue(4, -1); // {2, -3, 1, 4, -1, 3};
	st1.updateValue(5, -11); // {2, -3, 1, 4, -1, -11};

	std::cout << "New changes...\n";

	// New changes
	std::cout << "0, 1: " << st1.queryValue(0, 1) << '\n'; // -3
	std::cout << "3, 4: " << st1.queryValue(3, 4) << '\n'; // -1
	std::cout << "0, 5: " << st1.queryValue(0, 5) << '\n'; // -11
	



	int arr2[6] = { 0, 1, 3, 5, -2, 3 };

	// Segment tree for addition
	SegmentTree<int> st2(arr2, 6, [](int a, int b){ return a + b; }, 0);

	// Test cases
	// Queries
	std::cout << "0, 2: " << st2.queryValue(0, 2) << '\n'; // 4
	std::cout << "3, 4: " << st2.queryValue(3, 4) << '\n'; // 3
	std::cout << "0, 5: " << st2.queryValue(0, 5) << '\n'; // 10
	
	std::cout << "Updating values...\n";

	// Updates
	st2.updateValue(2, 7); // { 0, 1, 7, 5, -2, 3 };
	st2.updateValue(4, -5); // { 0, 1, 7, 5, -5, 3 };
	st2.updateValue(0, 12); // { 12, 1, 7, 5, -5, 3 };
	
	std::cout << "New changes...\n";

	// New changes
	std::cout << "0, 2: " << st2.queryValue(0, 2) << '\n'; // 20
	std::cout << "3, 4: " << st2.queryValue(3, 4) << '\n'; // 0
	std::cout << "0, 5: " << st2.queryValue(0, 5) << '\n'; // 23

	return 0;
}
