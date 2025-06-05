#pragma once

// General-purpose recursive Segment Tree implementation supporting any associative operation on number types, with persistence (versioning)

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
	SegmentTree(const T* arr, size_t size, Operation op, const T& neutralElement);
	T queryValue(size_t leftIndex, size_t rightIndex) const;
	void updateValue(size_t valueIndex, const T& value);
	void setVersion(size_t versionIndex);
	~SegmentTree();
private:
	const T* arr;
	size_t size;
	std::vector<Node<T>*> roots;
	Operation op;
	T neutralElement;
	size_t versionIndex;
	Node<T>* buildTree(T* arr, size_t leftIndex, size_t rightIndex);
	T query(Node<T>* currentNode, size_t queryLeft, size_t queryRight, size_t leftBound, size_t rightBound) const;
	Node<T>* update(Node<T>* currentNode, size_t leftIndex, size_t rightIndex, size_t valueIndex, const T& value);
	void deleteNode(Node<T>* node);
};

template <typename T>
SegmentTree<T>::SegmentTree(const T* arr, size_t size, Operation op, const T& neutralElement) : arr(arr), size(size), op(op), neutralElement(neutralElement), versionIndex(0)
{
	roots.push_back(buildTree(arr, 0, size - 1));
}

template <typename T>
Node<T>* SegmentTree<T>::buildTree(const T* arr, size_t leftIndex, size_t rightIndex)
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
	return query(roots[versionIndex], leftIndex, rightIndex, 0, size - 1);
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

	size_t middleIndex = leftBound + (rightBound - leftBound) / 2;

	// Keep searching left and right until the queried value can be resolved
	return op(query(currentNode->left, queryLeft, queryRight, leftBound, middleIndex), query(currentNode->right, queryLeft, queryRight, middleIndex + 1, rightBound));
}

template <typename T>
void SegmentTree<T>::updateValue(size_t valueIndex, const T& value)
{
	roots.push_back(update(roots[versionIndex++], 0, size - 1, valueIndex, value));
}

template <typename T>
Node<T>* SegmentTree<T>::update(Node<T>* currentNode, size_t leftIndex, size_t rightIndex, size_t valueIndex, const T& value)
{
	if (leftIndex > rightIndex || currentNode == nullptr)
		return nullptr;

	// If the search closed in on the leaf node corresponding to the specified index, update the value of the node
	// and return true to indicate modification has happened
	if (leftIndex == rightIndex && leftIndex == valueIndex)
	{
		Node<T>* newNode = new Node<T>(value);
		return newNode;
	}

	size_t middleIndex = leftIndex + (rightIndex - leftIndex) / 2;

	Node<T>* newLeftNode = currentNode->left;
	Node<T>* newRightNode = currentNode->right;

	// Only traverse the path corresponding to the nodes which should change
	if (valueIndex <= middleIndex)
		newLeftNode = update(currentNode->left, leftIndex, middleIndex, valueIndex, value);
	else
		newRightNode = update(currentNode->right, middleIndex + 1, rightIndex, valueIndex, value);

	// Only update the nodes on the path of the recursion, and the specified array's value as well
	T newValue = op(newLeftNode->value, newRightNode->value);
	Node<T>* newNode = new Node<T>(newValue);
	newNode->left = newLeftNode;
	newNode->right = newRightNode;
	return newNode;
}

template <typename T>
void SegmentTree<T>::setVersion(size_t versionIndex)
{
	if (versionIndex => 0 && versionIndex <= roots.size() - 1)
	{
		this->versionIndex = versionIndex;
		return;
	}
	// If provided an invalid version, fall back to latest version
	versionIndex = roots.size() - 1;
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
	// Set node to nullptr after deletion as to prevent dangling pointers for other versions
	node = nullptr;
}

template <typename T>
SegmentTree<T>::~SegmentTree()
{
	for (auto it = roots.begin(); it != roots.end(); ++it)
		if (*it)
			deleteNode(*it);
}
