#pragma once

// General-purpose recursive Segment Tree implementation supporting any associative operation on number types

#include <functional>

template <typename T>
struct Node
{
	Node(const T& value) : left(nullptr), right(nullptr), value(value) {}

	Node* left;
	Node* right;
	T value;
};

template <typename T>
class SegmentTree
{
public:
	using Operation = std::function<T(const T& nr1, const T& nr2)>;
	SegmentTree(T* arr, size_t size, Operation op, const T& neutralElement);
	T queryValue(size_t leftIndex, size_t rightIndex) const;
	void updateValue(size_t valueIndex, const T& value);
	~SegmentTree();
private:
	T* arr;
	size_t size;
	Node<T>* root;
	Operation op;
	T neutralElement;
	Node<T>* buildTree(T* arr, size_t leftIndex, size_t rightIndex);
	T query(Node<T>* currentNode, size_t queryLeft, size_t queryRight, size_t leftBound, size_t rightBound) const;
	bool update(Node<T>* currentNode, size_t leftIndex, size_t rightIndex, size_t valueIndex, const T& value);
	void deleteNode(Node<T>* node);
};

template <typename T>
SegmentTree<T>::SegmentTree(T* arr, size_t size, Operation op, const T& neutralElement) : arr(arr), size(size), root(nullptr), op(op), neutralElement(neutralElement)
{
	root = buildTree(arr, 0, size - 1);
}

template <typename T>
Node<T>* SegmentTree<T>::buildTree(T* arr, size_t leftIndex, size_t rightIndex)
{
	// Pointers beyond leaf nodes should be null
	if (leftIndex > rightIndex)
		return nullptr;

	// If the bounds close in on a single array value, create a new leaf node
	if (leftIndex == rightIndex)
		return new Node<T>(arr[leftIndex]);

	// Calculate middle index with overflow protection
	size_t middleIndex = leftIndex + (rightIndex - leftIndex) / 2;

	// While traversing by recursion, create placeholder nodes
	Node<T>* currentNode = new Node<T>(neutralElement);

	// Apply divide and conquer until the tree is fully built
	currentNode->left = buildTree(arr, leftIndex, middleIndex);
	currentNode->right = buildTree(arr, middleIndex + 1, rightIndex);
	currentNode->value = op(currentNode->left->value, currentNode->right->value);

	return currentNode;
}

template <typename T>
T SegmentTree<T>::queryValue(size_t leftIndex, size_t rightIndex) const
{
	return query(root, leftIndex, rightIndex, 0, size - 1);
}

template <typename T>
T SegmentTree<T>::query(Node<T>* currentNode, size_t queryLeft, size_t queryRight, size_t leftBound, size_t rightBound) const
{
	// If the indices provided are outside of the array's possible indices, return the neutral element of the supposed monoid
	if (queryRight < leftBound || queryLeft > rightBound)
		return neutralElement;

	// If by traversing the tree the bounds arrived within the interval of the query, return the node's value
	if (queryLeft <= leftBound && rightBound <= queryRight)
		return currentNode->value;

	size_t middleIndex = (leftBound + rightBound) / 2;

	// Keep searching left and right until the queried value can be resolved
	return op(query(currentNode->left, queryLeft, queryRight, leftBound, middleIndex), query(currentNode->right, queryLeft, queryRight, middleIndex + 1, rightBound));
}

template <typename T>
void SegmentTree<T>::updateValue(size_t valueIndex, const T& value)
{
	update(root, 0, size - 1, valueIndex, value);
}

template <typename T>
bool SegmentTree<T>::update(Node<T>* currentNode, size_t leftIndex, size_t rightIndex, size_t valueIndex, const T& value)
{
	if (leftIndex > rightIndex || currentNode == nullptr)
		return false;

	// If the search closed in on the leaf node corresponding to the specified index, update the value of the node
	// and return true to indicate modification has happened
	if (leftIndex == rightIndex && leftIndex == valueIndex)
	{
		currentNode->value = value;
		return true;
	}

	size_t middleIndex = leftIndex + (rightIndex - leftIndex) / 2;

	bool modification;

	// Only traverse the path corresponding to the nodes which should change
	if (valueIndex <= middleIndex)
		modification = update(currentNode->left, leftIndex, middleIndex, valueIndex, value);
	else
		modification = update(currentNode->right, middleIndex + 1, rightIndex, valueIndex, value);

	// Only update the nodes on the path of the recursion, and the specified array's value as well
	if (modification)
	{
		currentNode->value = op(currentNode->left->value, currentNode->right->value);
		arr[valueIndex] = currentNode->value;
		return true;
	}
	return false;
}


template <typename T>
void SegmentTree<T>::deleteNode(Node<T>* node)
{
	if (!node)
		return;
	// Delete nodes in post-order as to avoid dangling pointers and deleting unallocated memory
	deleteNode(node->left);
	deleteNode(node->right);
	delete node;
}

template <typename T>
SegmentTree<T>::~SegmentTree()
{
	if (root)
		deleteNode(root);
}
