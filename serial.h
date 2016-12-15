#include <iostream>
#include <Windows.h>
#include <WinNT.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

class Serial
{
private:
	HANDLE Handle;  //句柄
	DCB Config;    //具体参数设置
	bool Status;    //当前状态

public:
	Serial();
	~Serial();
	void SetStatus(bool status);   //设置状态
	bool GetStatus();              //获取状态信息
	HANDLE GetHandle();            //获取句柄
	bool OpenPort(int baudRate = 9600 , const char * portName="COM8");  //用波特率和串口名称打开串口，默认波特率9600，默认串口COM8（因为我的电脑没插键鼠套的端口是COM8）
	bool Read(char* InputData,unsigned int DataSize,unsigned long &DataLength);
	bool ClosePort();
	bool Write(char *outputData,unsigned int size, unsigned long & length);
};