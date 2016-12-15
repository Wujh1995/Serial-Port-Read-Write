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
	HANDLE Handle;  //���
	DCB Config;    //�����������
	bool Status;    //��ǰ״̬

public:
	Serial();
	~Serial();
	void SetStatus(bool status);   //����״̬
	bool GetStatus();              //��ȡ״̬��Ϣ
	HANDLE GetHandle();            //��ȡ���
	bool OpenPort(int baudRate = 9600 , const char * portName="COM8");  //�ò����ʺʹ������ƴ򿪴��ڣ�Ĭ�ϲ�����9600��Ĭ�ϴ���COM8����Ϊ�ҵĵ���û������׵Ķ˿���COM8��
	bool Read(char* InputData,unsigned int DataSize,unsigned long &DataLength);
	bool ClosePort();
	bool Write(char *outputData,unsigned int size, unsigned long & length);
};