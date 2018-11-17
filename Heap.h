#include<iostream>
#include<vector>
using namespace std;


template<class T,class compare>
class Heap{
public:
	Heap()
	{

	}
	Heap(T* a, int n)
	{
		_arr.reserve(n);//给私有成员数组定义大小
		for (int i = 0; i < n; i++)
		{
			_arr.push_back(a[i]);
		}
		for (int i = (_arr.size() - 2) / 2; i >= 0; i--)//从最后一个非叶子结点开始向下建堆调整
		{
			Adjustdown(i);
		}
	}
	void Adjustdown(int root)//向下调整从root为根节点往下都是小堆
	{
		compare Com;
		int parent = root;
		int child = root * 2 + 1;//先让孩子都是左孩子
		while(child < _arr.size())
		{
			if ((child + 1 < _arr.size()) && Com(_arr[child + 1], _arr[child]))//如果右子树没出界并且比左子树大就让右子树和父亲操作
			{
				//if里边的判断条件千万不能写反
				child++;
			}
			if (Com(_arr[child], _arr[parent]))
			{
				swap(_arr[child], _arr[parent]);//如果符合条件说明孩子要比父亲小，那就得让孩子和父亲换位置
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	void Adjustup(int root)//在每次push一个元素的时候要进行向上调整
	{
		compare Com;
		int child = root;
		int parent = (child - 1) / 2;
		while (parent >= 0)
		{
			//这时候我们的堆已经是成型了就不用比较了
			if (Com(_arr[child], _arr[parent]))
			{
				swap(_arr[child], _arr[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;//如果发现当前孩子比父亲大就不用走了，因为已经建成小堆孩子肯定要比父亲大。
			}
		}
	}
	void Push(T x)
	{
		_arr.push_back(x);
		Adjustup(_arr.size() - 1);
	}
	void Pop()
	{
		//这里要注意我们pop的是我们的堆顶元素也就是最小的那个但是vector接口pop的时候是出后边那个
		swap(_arr[0], _arr[_arr.size() - 1]);//先交换把最后一个放到第一个，然后让vector出元素
		_arr.pop_back();
		Adjustdown(0);//然后从0开始重新调整新的堆
	}
	bool Empty()
	{
		return _arr.empty();
	}
	size_t Size()
	{
		return _arr.size();
	}
	T& Top()
	{
		return _arr[0];
	}
private: 
	vector<T> _arr;
};