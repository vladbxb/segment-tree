#pragma once

template <typename T>
struct Node
{
	Node(T value) : left(nullptr), right(nullptr), value(value) {}

	Node* left;
	Node* right;
	T value;
};

template <typename Number>
class SegmentTree
{
public:
	SegmentTree(const Number* arr, size_t size);
private:
	Number* arr;
	size_t size;
	Node<Number>* root;
	void buildTree(Node<Number>* node, size_t leftIndex, size_t rightIndex);
	void deleteNode(Node<Number>* node);
};
