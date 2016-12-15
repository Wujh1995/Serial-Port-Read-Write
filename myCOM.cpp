// myCOM.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "serial.h"
#include <conio.h>
using namespace std;

void WriteMsg(Serial);

int main()
{
	Serial com;
	com.OpenPort();
	char buffer[100];
	unsigned int datasize = 101;
	unsigned long datalength;
	while(1)
	{
		com.Read(buffer, datasize, datalength);
		if(_kbhit())                               //判断键盘有没有被按下
		{
			_getch();
			WriteMsg(com);
			continue;
		}
		if(datalength==0) continue;
		cout<<"Datalength is ："<<datalength<<endl;
		for(unsigned int i=0; i<datalength; i++)
			cout<<buffer[i];
		cout<<endl;
	}
}

void WriteMsg(Serial com)
{
	string str_msg;
	cout<<"请写入您想发送的信息，以回车结束。最多不得超过100个字符"<<endl;
	cin>>str_msg;
	unsigned int n;
	n = str_msg.length();
	char *msg = new char [n];
	for(unsigned int i=0;i<str_msg.length();i++)
		msg[i] = str_msg[i];
	unsigned long length;
	com.Write(msg,n,length);
	delete [] msg;
}
