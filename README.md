# segment-tree
An implementation of Segment tree in C++ using STL, supporting any associative operation on number types
## Intricacies
- This implementation is recursive.
- It leverages std::functional in order for the user to choose which associative operation is used to construct the binary tree. (See the demo in the demos/ folder for some examples)
- Raw pointers and manual memory management are used for constructing, deconstructing and accessing the elements on the heap.
