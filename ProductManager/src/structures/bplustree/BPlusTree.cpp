#include "BPlusTree.h"

#include <map>
#include <sstream>
#include <vector>

namespace
{
	std::string escapeDot(const std::string &value)
	{
		std::string escaped;
		escaped.reserve(value.size());
		for (char ch : value)
		{
			if (ch == '\\' || ch == '"')
			{
				escaped.push_back('\\');
			}
			escaped.push_back(ch);
		}
		return escaped;
	}

	std::string productLabel(const Product *product, const std::string &key)
	{
		if (product == nullptr)
		{
			return "category: " + escapeDot(key) + "\\nProducto nulo";
		}

		std::ostringstream label;
		label << "category: " << escapeDot(key)
			  << "\\nname: " << escapeDot(product->name)
			  << "\\nbarcode: " << escapeDot(product->barcode)
			  << "\\nexpiry: " << escapeDot(product->expiry_date)
			  << "\\nbrand: " << escapeDot(product->brand)
			  << "\\nprice: " << product->price
			  << "\\nstock: " << product->stock;
		return label.str();
	}
}

BPlusTree::BPlusTree(int order)
	: root(nullptr), d(order < 1 ? 1 : order), maxKeys((2 * (order < 1 ? 1 : order)) + 1), maxChildren((2 * (order < 1 ? 1 : order)) + 2)
{
}

BPlusTree::~BPlusTree()
{
	destroyTree(root);
	root = nullptr;
}

BPlusTree::Node *BPlusTree::createNode(bool isLeaf) const
{
	return new Node(isLeaf, maxKeys, maxChildren);
}

void BPlusTree::destroyTree(Node *node)
{
	if (node == nullptr)
	{
		return;
	}

	if (node->leaf)
	{
		for (int i = 0; i < node->valueCount; i++)
		{
			delete node->values[i];
			node->values[i] = nullptr;
		}
	}
	else
	{
		for (int i = 0; i < node->childCount; i++)
		{
			destroyTree(node->children[i]);
		}
	}

	delete node;
}

bool BPlusTree::lessProduct(const Product *a, const Product *b) const
{
	if (a->category != b->category)
	{
		return a->category < b->category;
	}
	return a->barcode < b->barcode;
}

int BPlusTree::findChildIndex(const Node *parent, const Node *child) const
{
	for (int i = 0; i < parent->childCount; i++)
	{
		if (parent->children[i] == child)
		{
			return i;
		}
	}
	return -1;
}

void BPlusTree::insertKeyAt(Node *node, int index, const std::string &key)
{
	for (int i = node->keyCount; i > index; i--)
	{
		node->keys[i] = node->keys[i - 1];
	}
	node->keys[index] = key;
	node->keyCount++;
}

void BPlusTree::removeKeyAt(Node *node, int index)
{
	for (int i = index; i + 1 < node->keyCount; i++)
	{
		node->keys[i] = node->keys[i + 1];
	}
	node->keyCount--;
}

void BPlusTree::insertValueAt(Node *node, int index, Product *product)
{
	for (int i = node->valueCount; i > index; i--)
	{
		node->values[i] = node->values[i - 1];
	}
	node->values[index] = product;
	node->valueCount++;
}

void BPlusTree::removeValueAt(Node *node, int index)
{
	for (int i = index; i + 1 < node->valueCount; i++)
	{
		node->values[i] = node->values[i + 1];
	}
	node->valueCount--;
}

void BPlusTree::insertChildAt(Node *node, int index, Node *child)
{
	for (int i = node->childCount; i > index; i--)
	{
		node->children[i] = node->children[i - 1];
	}
	node->children[index] = child;
	node->childCount++;
}

BPlusTree::Node *BPlusTree::removeChildAt(Node *node, int index)
{
	Node *removed = node->children[index];
	for (int i = index; i + 1 < node->childCount; i++)
	{
		node->children[i] = node->children[i + 1];
	}
	node->childCount--;
	return removed;
}

BPlusTree::Node *BPlusTree::findLeafForCategory(const std::string &category) const
{
	Node *current = root;
	while (current != nullptr && !current->leaf)
	{
		int i = 0;
		while (i < current->keyCount && category >= current->keys[i])
		{
			i++;
		}
		current = current->children[i];
	}
	return current;
}

BPlusTree::Node *BPlusTree::leftmostLeaf() const
{
	Node *current = root;
	while (current != nullptr && !current->leaf)
	{
		current = current->children[0];
	}
	return current;
}

void BPlusTree::insert(const Product &product)
{
	Product *newProduct = new Product(product);

	if (root == nullptr)
	{
		root = createNode(true);
		insertKeyAt(root, 0, newProduct->category);
		insertValueAt(root, 0, newProduct);
		return;
	}

	Node *leaf = findLeafForCategory(newProduct->category);
	insertInLeaf(leaf, newProduct);

	if (leaf->keyCount > 2 * d)
	{
		splitLeaf(leaf);
	}
}

void BPlusTree::insertInLeaf(Node *leaf, Product *product)
{
	int pos = 0;
	while (pos < leaf->valueCount && lessProduct(leaf->values[pos], product))
	{
		pos++;
	}

	insertValueAt(leaf, pos, product);
	insertKeyAt(leaf, pos, product->category);
}

void BPlusTree::splitLeaf(Node *leaf)
{
	Node *right = createNode(true);
	right->parent = leaf->parent;

	const int splitPoint = d;
	const int moved = leaf->keyCount - splitPoint;

	for (int i = 0; i < moved; i++)
	{
		right->keys[i] = leaf->keys[splitPoint + i];
		right->values[i] = leaf->values[splitPoint + i];
	}
	right->keyCount = moved;
	right->valueCount = moved;

	leaf->keyCount = splitPoint;
	leaf->valueCount = splitPoint;

	right->next = leaf->next;
	leaf->next = right;

	const std::string separator = right->keys[0];
	insertInParent(leaf, separator, right);
}

void BPlusTree::insertInParent(Node *left, const std::string &separator, Node *right)
{
	if (left == root)
	{
		Node *newRoot = createNode(false);
		insertKeyAt(newRoot, 0, separator);
		insertChildAt(newRoot, 0, left);
		insertChildAt(newRoot, 1, right);

		left->parent = newRoot;
		right->parent = newRoot;
		root = newRoot;
		return;
	}

	Node *parent = left->parent;
	int idx = findChildIndex(parent, left);
	insertKeyAt(parent, idx, separator);
	insertChildAt(parent, idx + 1, right);
	right->parent = parent;

	if (parent->keyCount > 2 * d)
	{
		splitInternal(parent);
	}
}

void BPlusTree::splitInternal(Node *node)
{
	Node *right = createNode(false);
	right->parent = node->parent;

	const int promoteIndex = d;
	const std::string promoteKey = node->keys[promoteIndex];

	int rightKeyCount = node->keyCount - promoteIndex - 1;
	for (int i = 0; i < rightKeyCount; i++)
	{
		right->keys[i] = node->keys[promoteIndex + 1 + i];
	}
	right->keyCount = rightKeyCount;

	int rightChildCount = node->childCount - (promoteIndex + 1);
	for (int i = 0; i < rightChildCount; i++)
	{
		right->children[i] = node->children[promoteIndex + 1 + i];
		right->children[i]->parent = right;
	}
	right->childCount = rightChildCount;

	node->keyCount = promoteIndex;
	node->childCount = promoteIndex + 1;

	insertInParent(node, promoteKey, right);
}

Product *BPlusTree::search(const std::string &category)
{
	Node *leaf = findLeafForCategory(category);
	while (leaf != nullptr)
	{
		for (int i = 0; i < leaf->keyCount; i++)
		{
			if (leaf->keys[i] == category)
			{
				return leaf->values[i];
			}
			if (leaf->keys[i] > category)
			{
				return nullptr;
			}
		}

		if (leaf->next == nullptr || leaf->next->keyCount == 0 || leaf->next->keys[0] > category)
		{
			break;
		}
		leaf = leaf->next;
	}

	return nullptr;
}

bool BPlusTree::findByBarcode(Node *&leafOut, int &indexOut, const std::string &barcode) const
{
	Node *leaf = leftmostLeaf();
	while (leaf != nullptr)
	{
		for (int i = 0; i < leaf->valueCount; i++)
		{
			Product *value = leaf->values[i];
			if (value != nullptr && value->barcode == barcode)
			{
				leafOut = leaf;
				indexOut = i;
				return true;
			}
		}
		leaf = leaf->next;
	}

	leafOut = nullptr;
	indexOut = -1;
	return false;
}

void BPlusTree::remove(const Product &product)
{
	if (root == nullptr || product.barcode.empty())
	{
		return;
	}

	Node *leaf = nullptr;
	int index = -1;
	if (!findByBarcode(leaf, index, product.barcode))
	{
		return;
	}

	delete leaf->values[index];
	removeValueAt(leaf, index);
	removeKeyAt(leaf, index);

	if (leaf == root)
	{
		if (leaf->keyCount == 0)
		{
			delete root;
			root = nullptr;
		}
		return;
	}

	rebalanceAfterDelete(leaf);
}

void BPlusTree::rebalanceAfterDelete(Node *node)
{
	const int minLeafKeys = d;
	const int minInternalKeys = d;

	if (node == root)
	{
		if (!node->leaf && node->keyCount == 0)
		{
			Node *newRoot = node->children[0];
			newRoot->parent = nullptr;
			delete node;
			root = newRoot;
		}
		return;
	}

	Node *parent = node->parent;
	int idx = findChildIndex(parent, node);

	if (node->leaf)
	{
		if (node->keyCount >= minLeafKeys)
		{
			if (idx > 0 && node->keyCount > 0)
			{
				parent->keys[idx - 1] = node->keys[0];
			}
			return;
		}

		Node *left = (idx > 0) ? parent->children[idx - 1] : nullptr;
		Node *right = (idx + 1 < parent->childCount) ? parent->children[idx + 1] : nullptr;

		if (left != nullptr && left->keyCount > minLeafKeys)
		{
			insertKeyAt(node, 0, left->keys[left->keyCount - 1]);
			insertValueAt(node, 0, left->values[left->valueCount - 1]);
			left->valueCount--;
			left->keyCount--;
			parent->keys[idx - 1] = node->keys[0];
			return;
		}

		if (right != nullptr && right->keyCount > minLeafKeys)
		{
			insertKeyAt(node, node->keyCount, right->keys[0]);
			insertValueAt(node, node->valueCount, right->values[0]);
			removeKeyAt(right, 0);
			removeValueAt(right, 0);
			parent->keys[idx] = right->keys[0];
			return;
		}

		if (left != nullptr)
		{
			for (int i = 0; i < node->keyCount; i++)
			{
				left->keys[left->keyCount + i] = node->keys[i];
				left->values[left->valueCount + i] = node->values[i];
			}
			left->keyCount += node->keyCount;
			left->valueCount += node->valueCount;
			left->next = node->next;

			removeKeyAt(parent, idx - 1);
			removeChildAt(parent, idx);
			delete node;
		}
		else if (right != nullptr)
		{
			for (int i = 0; i < right->keyCount; i++)
			{
				node->keys[node->keyCount + i] = right->keys[i];
				node->values[node->valueCount + i] = right->values[i];
			}
			node->keyCount += right->keyCount;
			node->valueCount += right->valueCount;
			node->next = right->next;

			removeKeyAt(parent, idx);
			removeChildAt(parent, idx + 1);
			delete right;
		}

		rebalanceAfterDelete(parent);
		return;
	}

	if (node->keyCount >= minInternalKeys)
	{
		return;
	}

	Node *left = (idx > 0) ? parent->children[idx - 1] : nullptr;
	Node *right = (idx + 1 < parent->childCount) ? parent->children[idx + 1] : nullptr;

	if (left != nullptr && left->keyCount > minInternalKeys)
	{
		insertKeyAt(node, 0, parent->keys[idx - 1]);
		parent->keys[idx - 1] = left->keys[left->keyCount - 1];
		left->keyCount--;

		Node *movedChild = left->children[left->childCount - 1];
		left->childCount--;
		insertChildAt(node, 0, movedChild);
		movedChild->parent = node;
		return;
	}

	if (right != nullptr && right->keyCount > minInternalKeys)
	{
		insertKeyAt(node, node->keyCount, parent->keys[idx]);
		parent->keys[idx] = right->keys[0];
		removeKeyAt(right, 0);

		Node *movedChild = removeChildAt(right, 0);
		insertChildAt(node, node->childCount, movedChild);
		movedChild->parent = node;
		return;
	}

	if (left != nullptr)
	{
		left->keys[left->keyCount] = parent->keys[idx - 1];
		left->keyCount++;
		for (int i = 0; i < node->keyCount; i++)
		{
			left->keys[left->keyCount + i] = node->keys[i];
		}
		left->keyCount += node->keyCount;

		for (int i = 0; i < node->childCount; i++)
		{
			left->children[left->childCount + i] = node->children[i];
			node->children[i]->parent = left;
		}
		left->childCount += node->childCount;

		removeKeyAt(parent, idx - 1);
		removeChildAt(parent, idx);
		delete node;
		rebalanceAfterDelete(parent);
		return;
	}

	if (right != nullptr)
	{
		node->keys[node->keyCount] = parent->keys[idx];
		node->keyCount++;
		for (int i = 0; i < right->keyCount; i++)
		{
			node->keys[node->keyCount + i] = right->keys[i];
		}
		node->keyCount += right->keyCount;

		for (int i = 0; i < right->childCount; i++)
		{
			node->children[node->childCount + i] = right->children[i];
			right->children[i]->parent = node;
		}
		node->childCount += right->childCount;

		removeKeyAt(parent, idx);
		removeChildAt(parent, idx + 1);
		delete right;
		rebalanceAfterDelete(parent);
	}
}

std::string BPlusTree::listProducts() const
{
	std::ostringstream oss;
	Node *leaf = leftmostLeaf();

	while (leaf != nullptr)
	{
		for (int i = 0; i < leaf->valueCount; i++)
		{
			Product *product = leaf->values[i];
			if (product != nullptr)
			{
				oss << product->name << ", "
					<< product->barcode << ", "
					<< product->category << ", "
					<< product->expiry_date << ", "
					<< product->brand << ", "
					<< product->price << ", "
					<< product->stock << "\n";
			}
		}
		leaf = leaf->next;
	}

	return oss.str();
}

std::string BPlusTree::toDot() const
{
	std::ostringstream dot;
	dot << "digraph BPlusTree {\n";
	dot << "  rankdir=TB;\n";
	dot << "  node [shape=record, fontsize=10];\n";

	if (root == nullptr)
	{
		dot << "  empty [label=\"Arbol B+ vacio\"];\n";
		dot << "}\n";
		return dot.str();
	}

	std::map<const Node *, int> ids;
	int nextId = 0;

	std::vector<Node *> stack;
	stack.push_back(root);

	while (!stack.empty())
	{
		Node *node = stack.back();
		stack.pop_back();
		if (node == nullptr || ids.find(node) != ids.end())
		{
			continue;
		}

		int id = nextId++;
		ids[node] = id;

		dot << "  n" << id << " [label=\"";
		for (int i = 0; i < node->keyCount; i++)
		{
			if (i > 0)
			{
				dot << "|";
			}

			if (node->leaf)
			{
				dot << productLabel(node->valueCount > i ? node->values[i] : nullptr, node->keys[i]);
			}
			else
			{
				dot << escapeDot(node->keys[i]);
			}
		}
		dot << "\"];\n";

		if (!node->leaf)
		{
			for (int i = node->childCount - 1; i >= 0; i--)
			{
				if (node->children[i] != nullptr)
				{
					stack.push_back(node->children[i]);
				}
			}
		}
	}

	stack.push_back(root);
	while (!stack.empty())
	{
		Node *node = stack.back();
		stack.pop_back();
		if (node == nullptr)
		{
			continue;
		}

		if (!node->leaf)
		{
			for (int i = 0; i < node->childCount; i++)
			{
				if (node->children[i] != nullptr)
				{
					dot << "  n" << ids[node] << " -> n" << ids[node->children[i]] << ";\n";
					stack.push_back(node->children[i]);
				}
			}
		}
	}

	dot << "  edge [style=dashed, color=gray];\n";
	for (Node *leaf = leftmostLeaf(); leaf != nullptr && leaf->next != nullptr; leaf = leaf->next)
	{
		dot << "  n" << ids[leaf] << " -> n" << ids[leaf->next] << ";\n";
	}

	dot << "}\n";
	return dot.str();
}

