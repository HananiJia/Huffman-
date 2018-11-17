#pragma once
#include<iostream>
#include"HuffmanTree.h"
#include<thread>
using namespace std;

int Jude = 1;
void sign()
{
	char ch[4] = { '|', '/', '-', '\\' };// 一个\不算一个字符
	int i = 0;
	while (Jude)
	{
		printf("%c\r", ch[i % 4]);//四个字符轮流输出 \r是为了每次都在行首输出用光标移动函数太浪费了
		i++;
		fflush(stdout);//清空缓冲区
	}
}

void Progressbar(int begin,int end)//显示进度条函数
{
	//每执行完一段程序就传进来一个begin一个end
	char buff[52] = { '\0' };//最初的赋值都是0输出函数遇到0就停止输出了
	for (int i = 0; i < begin; i++)
	{
		buff[i] = '#';//给begin 的目的是为了不是每次字符串都从头开始走，这次的end就是下次的begin
	}
	int start = begin;
	while (start <= end)
	{	
		printf("[%-50s][%d]", buff, 2 * start);//显示进度条和百分比
		fflush(stdout);
		buff[start] = '#';//动态给字符串赋值#
		start++;
		buff[start] = '\0';
		Sleep(2);
	}
}
struct CharBlock{//用来存储每个字符出现的次数以及它的huffman编码
	unsigned char _ch;//字符内容
	long long _count;//每个字符出现的次数，很有可能文件很大，次数很大所以定义为longlong
	string _code;//用来存储字符对应的huffma编码

	//这里还要重载三个函数为了在创建huffman树的时候使用
	CharBlock operator+(const CharBlock& C) const
	{
		CharBlock result;
		result._count = _count + C._count;
		return result;
	}
	bool operator!=(const CharBlock& C) const
	{
		return _count != C._count;
	}
	bool operator<(const CharBlock& C) const
	{
		return _count < C._count;
	}
};
struct CountBlock{
	unsigned char _ch;
	size_t _count;
};
class FileCompress{ 
public:
	FileCompress()//构造函数
	{
		for (int i = 0; i < 256; i++)
		{
			_Info[i]._ch = i;
			_Info[i]._count = 0;

		}
	}
	void filecompress(char* Filename)//文件压缩
	{
		thread S(sign);
		thread P1(Progressbar, 0, 10);//利用多线程来同时执行两个输出语句
		FILE* fch = fopen(Filename, "rb");//这里要用二进制的格式打开文件
		unsigned char ch;
		fread(&ch, sizeof(ch), 1, fch);//从fch里边读取一个ch大小的数据写到ch那里
		while (!feof(fch))//如果读取到的不是eof文件结束符就继续读取
		{
			//用来统计各个字符串出现的个数
			_Info[ch]._count++;
			fread(&ch, sizeof(ch), 1, fch);//fread可以用来读取一个数据块
		}
		P1.join();//因为我们的代码量并不大，多线程执行顺序可能会混乱，所以这里我们加入join函数，也就是上边的P函数没执行完就不往下执行
		thread P2(Progressbar, 10, 20);//再让进度条前进20
		//统计完字符出现的次数之后开始根据字符出现的次数构建huffman树

		//开始构建huffman树
		CharBlock invalid;//无效的内容
		invalid._count = 0;//这就是没出现的字符
		HuffmanTree<CharBlock> CharTree(_Info, 256, invalid);

		//生成对应的huffman编码
		GetHuffmanCode(CharTree.Getroot());

		P2.join();//等进度条走完再去执行别的任务不然进度条会乱
		thread P3(Progressbar, 20, 30);

		//正式开始压缩
		string Name = Filename;
		size_t pos = Name.rfind(".");//找到倒数第一个.说明这部分往后是文件的后缀
		string NewName = Name.substr(0, pos) + ".jhn";
		FILE* fin = fopen(NewName.c_str(), "wb");//记得要以二进制来打开我们的文件
		CountBlock Info;//不带下划线！
		//把每个字符出现次数的数字块写到压缩文件结尾，为了解压时重建huffman树
		for (int i = 0; i < 256; i++)
		{
			if (_Info[i] != invalid)//不是空
			{
				Info._ch = _Info[i]._ch;
				Info._count = _Info[i]._count;
				fwrite(&Info, sizeof(Info), 1, fin);
			}
		}
		Info._count = -1;
		fwrite(&Info, sizeof(Info), 1, fin);
		P3.join();
		thread P4(Progressbar, 30, 40);

		//把生成的huffman编码写入文件
		fseek(fch, 0, SEEK_SET);//因为刚刚读取文件光标移动走了，现在让光标回到起始位置开始读原始文件
		char value='\0';
		int count = 0;//用来计数够八位才能开始执行写操作
		fread(&ch, sizeof(ch), 1, fch);
		while (!feof(fch))
		{
			string& code = _Info[ch]._code;
			for (size_t i = 0; i < code.size(); i++)
			{
				value <<= 1;//把value往左移动一位给下一个字节留位置
				count++;
				if (code[i] == '1')
				{
					value |= 1;//有1则变成1
				}
				//如果是0就不做处理了
				if (count == 8)//八次构成一个字节进行写操作
				{
					fwrite(&value, sizeof(value), 1, fin);
					value = 0;
					count = 0;
				}
			}
			fread(&ch, sizeof(ch), 1, fch);
		}
		if (count != 0)//也就是最后可能还剩几位不够八位没有写进去
		{
			value <<= (8 - count);//向左移动8-count也就是后边都补上0
			fwrite(&value, sizeof(value), 1, fin);
		}
		P4.join();
		thread P5(Progressbar, 40, 50);
		fclose(fin);
		fclose(fch);
		P5.join();
		Jude = 0;
		S.join();
		Jude = 1;
	}
	void unfilecompress(char* Filename)
	{
		thread S(sign);
		thread P1(Progressbar, 0, 10);
		FILE* fch = fopen(Filename, "rb");
		CountBlock Info;
		fread(&Info, sizeof(Info), 1, fch);
		while (Info._count != -1)//把对应字符串以及出现字符读取出来
		{
			if (Info._count != 0)
			{
				_Info[Info._ch]._ch = Info._ch;
				_Info[Info._ch]._count = Info._count;
				fread(&Info, sizeof(Info), 1, fch);//每次读取一个数字块的大小
			}
		}
		P1.join();
		thread P2(Progressbar, 10, 20);
		//恢复huffman树
		CharBlock invalid;
		invalid._count = 0;
		HuffmanTree<CharBlock> tree(_Info, 256, invalid);//和压缩的时候构建huffman树的过程是一样的
		//就是重新再构建一遍，这样才能恢复我们的数据

		P2.join();
		thread P3(Progressbar, 20, 35);

		//开始恢复我们的文件
		HuffmanTreeNode<CharBlock>* root = tree.Getroot();
		HuffmanTreeNode<CharBlock>* cur = root;
		int count = root->_t._count;//根节点里边的count存的是整个文件所有的字符个数
		string FileName = Filename;
		size_t pos = FileName.rfind('.');
		string NewName = FileName.substr(0, pos) + ".ujhn";
		FILE* fin = fopen(NewName.c_str(), "wb");
		unsigned char ch = getc(fch);//读取一个字符的huffman编码
		if (feof(fch))
		{
			for (size_t i = count; i > 0; i++)
			{
				ch = fputc(root->_t._ch, fin);
			}
		}
		else
		{
			while (count)
			{
				for (int i = 7; i >= 0; i--)
				{
					if ((ch&(1 << i)) == 0)//说明这一位是0要往左走
					{
						cur = cur->_left;
					}
					else
					{
						cur = cur->_right;
					}
					if ((cur->_right == NULL) && (cur->_left == NULL))//说明他是叶子结点写入他就行了
					{
						fputc(cur->_t._ch, fin);
						cur = root;
						count--;
						if (count <= 0)//代表所有字符都转化过来了退出即可。
						{
							break;
						}
					}
				}
				ch = fgetc(fch);
			}
		}
		P3.join();
		thread P4(Progressbar, 35, 50);
		fclose(fch);
		fclose(fin);
		P4.join();
		Jude = 0;
		S.join();
		Jude = 1;

	}
	void GetHuffmanCode(HuffmanTreeNode<CharBlock>* root)
	{
		if (root == NULL)
			return ;
		if ((root->_left == NULL) && (root->_right == NULL))
		{
			string HuffCode;
			HuffmanTreeNode<CharBlock>* parent = root->_parent;
			HuffmanTreeNode<CharBlock>* cur = root;
			while (parent)
			{
				if (parent->_left == cur)//左为0右为1
				{
					HuffCode.push_back('0');
				}
				if (parent->_right == cur)
				{
					HuffCode.push_back('1');
				}
				cur = parent;
				parent = cur->_parent;
			}
			//因为我们是倒着找的所以要翻转一下编码串
			reverse(HuffCode.begin(), HuffCode.end());
			_Info[root->_t._ch]._code = HuffCode;//root是huffman树的结点，结点里边的t是我们的charblock类型
		}
		GetHuffmanCode(root->_left);
		GetHuffmanCode(root->_right);
		return;
	}
private:
	CharBlock _Info[256];
};