#include<iostream>
#include"Heap.h"
using namespace std;

template<class T>
struct  HuffmanTreeNode//huffman树的结点
{
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;//这里要添加父亲结点找编码的时候倒着找
	T _t;
	struct HuffmanTreeNode(const T& t)//简单的构造函数
		:_t(t),
		_left(NULL),
		_right(NULL),
		_parent(NULL)
	{

	};
};
template<class T>
struct Com//仿函数
{
	bool operator()(const HuffmanTreeNode<T>* left, const HuffmanTreeNode<T>* right) const
	{
		return left->_t < right->_t;
	}
};

template<class T>
class HuffmanTree{
public:
	HuffmanTree()
		:_root(NULL)
	{

	}
	HuffmanTree(T* arr, int n, T& invalid)
	{
		Heap<HuffmanTreeNode<T>*, Com<T>> smallheap;//创建小堆
		for (int i = 0; i < 256; i++)
		{
			if (arr[i] != invalid)//这里不能用arr[i].count!=0因为那是私有成员！！！
			{
				smallheap.Push(new HuffmanTreeNode<T>(arr[i]));
			}
		}
		while (smallheap.Size()>1)
		{
			HuffmanTreeNode<T>* left = smallheap.Top();
			smallheap.Pop();
			HuffmanTreeNode<T>* right = smallheap.Top();
			smallheap.Pop();
			HuffmanTreeNode<T>* parent = new HuffmanTreeNode<T>(left->_t + right->_t);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			smallheap.Push(parent);
		}
		_root = smallheap.Top();
	}
	HuffmanTreeNode<T>* Getroot()
	{
		return _root;
	}
private:
	HuffmanTreeNode<T>* _root;//huffman结点类型的指针
};
