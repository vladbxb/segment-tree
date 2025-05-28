#include "SegmentTree.hpp"

template <typename Number>
SegmentTree::SegmentTree(const Number* arr, size_t size) : arr(arr), size(size), root(nullptr)
{
	root = buildTree(0, size - 1);
}

template <typename Number>
Node<Number>* SegmentTree::buildTree(size_t leftIndex, size_t rightIndex)
{
	if (leftIndex > rightIndex)
		return nullptr;
	if (leftIndex == rightIndex)
		return new Node<Number>(arr[leftIndex]);
	size_t middleIndex = (leftIndex + rightIndex) / 2;
	Node<Number>* currentNode = new Node<Number>(0);
	currentNode->left = buildTree(leftIndex, middleIndex);
	currentNode->right = buildTree(middleIndex + 1, rightIndex);
	currentNode->value = currentNode->left.value + currentNode->right.value;
	return currentNode;
}

template <typename Number>
void SegmentTree::deleteNode(Node<Number>* node)
{
	if (!node)
		return;
	deleteNode(node->left);
	deleteNode(node->right);
	delete node;
}
