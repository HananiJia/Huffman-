#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<Windows.h>
#include"FileCompress.h"
using namespace std;

static void _SetPos(int x, int y)//光标移动函数
{
	COORD position;
	position.X = x;
	position.Y = y;
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, position);
}
void InterFace()
{
	char *msg[4] = { 
		"欢迎使用定制版huffman文件压缩系统",
		"***********系统版本2.6***********",
		"                   制作者：贾浩男",
		"       按任意键继续            "
	};
	int y = 10;
	for (int i = 0; i < 3; i++)
	{
		_SetPos(40, y + 2*i);
		cout << msg[i] << endl;
		cout << endl;
	}
	for (int i = 3; i>=1; i--)
	{
		_SetPos(55, y +6);
		cout <<i<<"秒后自动跳转"<< endl;
		Sleep(1000);
	}	
}
void menu()
{
	char *mse[4] = {
		"*请输入对应的数字选项*",
		"******1.压缩文件******",
		"******2.解压文件******",
		"******0.退出系统******", 
	};
	int y = 5;
	for (int i = 0; i < 4; i++)
	{
		_SetPos(40, y + 2 * i);
		cout << mse[i] << endl;
		cout << endl;
	}
}
int main()
{
	InterFace();
	char filename[20] = { 0 };
	int begin = 0;
	int end = 0;
	system("cls");
	while (1)
	{
		menu();
		int n = 0;
		cin >> n;
		system("cls");
		switch (n)
		{
		case 0:
			exit(1);
		case 1:
		{
				  cout << "请输入要压缩的文件名" << endl;
				  cin >> filename;
				  FileCompress com1;
				  begin = GetTickCount();
				  com1.filecompress(filename);
				  cout << "压缩成功" << endl;
				  end = GetTickCount();
				  cout << "耗时" << end - begin << "ms" << endl;
				  break;
		}
		case 2:
		{
				  cout << "请输入要解压的文件名" << endl;
				  cin >> filename;
				  FileCompress com2;
				  begin = GetTickCount();
				  com2.unfilecompress(filename);
				  cout << "解压成功" << endl;
				  end = GetTickCount();
				  cout << "耗时" << end - begin << "ms" << endl;
				  break;
		}
		default:
			cout << "选项无效请重新输入" << endl;
			break;
		}
	}
	system("pause");
	return 0; 
}