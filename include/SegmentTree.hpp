#pragma once

// General-purpose recursive Segment Tree implementation supporting any associative operation and preferred type

#include <functional>

template <typename T>
struct Node
{
	Node(T value) : left(nullptr), right(nullptr), value(value) {}

	Node* left;
	Node* right;
	T value;
};

template <typename T>
class SegmentTree
{
public:
	using Operation = std::function<T(const T& nr1, const T& nr2)>;
	SegmentTree(const T* arr, size_t size, Operation op, const T& neutralElement);
	T queryValue(size_t leftIndex, size_t rightIndex) const;
	void updateValue(size_t valueIndex, const T& value);
	~SegmentTree();
private:
	const T* arr;
	size_t size;
	Node<T>* root;
	Operation op;
	T neutralElement;
	Node<T>* buildTree(const T* arr, size_t leftIndex, size_t rightIndex);
	T query(Node<T>* currentNode, size_t queryLeft, size_t queryRight, size_t leftBound, size_t rightBound) const;
	bool update(Node<T>* currentNode, size_t leftIndex, size_t rightIndex, size_t valueIndex, const T& value);
	void deleteNode(Node<T>* node);
};

template <typename T>
SegmentTree<T>::SegmentTree(const T* arr, size_t size, Operation op, const T& neutralElement) : arr(arr), size(size), root(nullptr), op(op), neutralElement(neutralElement)
{
	root = buildTree(arr, 0, size - 1);
}

template <typename T>
Node<T>* SegmentTree<T>::buildTree(const T* arr, size_t leftIndex, size_t rightIndex)
{
	if (leftIndex > rightIndex)
		return nullptr;
	if (leftIndex == rightIndex)
		return new Node<T>(arr[leftIndex]);

	// Calculate middle index with overflow protection
	size_t middleIndex = leftIndex + (rightIndex - leftIndex) / 2;
	Node<T>* currentNode = new Node<T>(neutralElement);

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
	// If the indices provided are outside of the array's possible indexes, return the neutral element of the supposed monoid
	if (queryRight < leftBound || queryLeft > rightBound)
		return neutralElement;

	if (queryLeft <= leftBound && rightBound <= queryRight)
		return currentNode->value;

	size_t middleIndex = (leftBound + rightBound) / 2;

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
	if (leftIndex == rightIndex && leftIndex == valueIndex)
	{
		currentNode->value = value;
		return true;
	}

	size_t middleIndex = leftIndex + (rightIndex - leftIndex) / 2;

	bool modification;

	if (valueIndex <= middleIndex)
		modification = update(currentNode->left, leftIndex, middleIndex, valueIndex, value);
	else
		modification = update(currentNode->right, middleIndex + 1, rightIndex, valueIndex, value);

	// Only update the nodes on the path of the recursion
	if (modification)
	{
		currentNode->value = op(currentNode->left, currentNode->right);
		return true;
	}
	return false;
}


template <typename T>
void SegmentTree<T>::deleteNode(Node<T>* node)
{
	if (!node)
		return;
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
