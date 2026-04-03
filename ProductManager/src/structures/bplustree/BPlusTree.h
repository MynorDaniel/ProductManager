#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "structures/DataStructure.h"
#include <string>

class BPlusTree : public DataStructure
{
private:
	struct Node
	{
		bool leaf;
		std::string *keys;
		Node **children;
		Product **values;
		int keyCount;
		int childCount;
		int valueCount;
		int keyCapacity;
		int childCapacity;
		Node *next;
		Node *parent;

		Node(bool isLeaf, int keyCap, int childCap)
			: leaf(isLeaf),
			  keys(new std::string[keyCap]),
			  children(new Node *[childCap]),
			  values(new Product *[keyCap]),
			  keyCount(0),
			  childCount(0),
			  valueCount(0),
			  keyCapacity(keyCap),
			  childCapacity(childCap),
			  next(nullptr),
			  parent(nullptr)
		{
			for (int i = 0; i < childCap; i++)
			{
				children[i] = nullptr;
			}

			for (int i = 0; i < keyCap; i++)
			{
				values[i] = nullptr;
			}
		}

		~Node()
		{
			delete[] keys;
			delete[] children;
			delete[] values;
		}
	};

	Node *root;
	int d;
	int maxKeys;
	int maxChildren;

public:
	explicit BPlusTree(int order = 2);
	~BPlusTree() override;

	void insert(const Product &product) override;      // key = category
	void remove(const Product &product) override;      // key = barcode
	Product *search(const std::string &category) override;

	std::string listProducts() const;
	std::string toDot() const;

private:
	void destroyTree(Node *node);

	Node *findLeafForCategory(const std::string &category) const;
	Node *leftmostLeaf() const;
	Node *createNode(bool isLeaf) const;

	int findChildIndex(const Node *parent, const Node *child) const;
	void insertKeyAt(Node *node, int index, const std::string &key);
	void removeKeyAt(Node *node, int index);
	void insertValueAt(Node *node, int index, Product *product);
	void removeValueAt(Node *node, int index);
	void insertChildAt(Node *node, int index, Node *child);
	Node *removeChildAt(Node *node, int index);

	void insertInLeaf(Node *leaf, Product *product);
	void splitLeaf(Node *leaf);
	void insertInParent(Node *left, const std::string &separator, Node *right);
	void splitInternal(Node *node);

	bool findByBarcode(Node *&leafOut, int &indexOut, const std::string &barcode) const;
	void rebalanceAfterDelete(Node *node);

	bool lessProduct(const Product *a, const Product *b) const;
};

#endif // BPLUSTREE_H

