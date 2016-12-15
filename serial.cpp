#include "stdafx.h"
#include "serial.h"

Serial::Serial()
{
	Config.ByteSize = 8;          //8����������Ϊ��λ
	Config.BaudRate = CBR_9600;   //������Ĭ��9600
	Config.StopBits = ONESTOPBIT;  //ֹͣλΪ1λ
	Config.Parity = NOPARITY;      //����żУ��λ
	Status = false;
}

Serial::~Serial()
{
	Handle = NULL;
}

HANDLE Serial::GetHandle()
{
	return Handle;
}

bool Serial::GetStatus()
{
	return Status;
}

void Serial::SetStatus(bool status)
{
	Status = status;
}

bool Serial::OpenPort(int baudRate , const char * portName)
{
	DCB SerialConfig;
	SerialConfig.ByteSize = 8;
	SerialConfig.StopBits = ONESTOPBIT;
	SerialConfig.Parity = NOPARITY;
	switch(baudRate)
	{
	case 110:
		SerialConfig.BaudRate = CBR_110;
		break;
	case 300:
		SerialConfig.BaudRate = CBR_300;
		break;
	case 600:
		SerialConfig.BaudRate = CBR_600;
		break;
	case 1200:
		SerialConfig.BaudRate = CBR_1200;
		break;
	case 2400:
		SerialConfig.BaudRate = CBR_2400;
		break;
	case 4800:
		SerialConfig.BaudRate = CBR_4800;
		break;
	case 9600:
		SerialConfig.BaudRate = CBR_9600;
		break;
	case 14400:
		SerialConfig.BaudRate = CBR_14400;
		break;
	case 19200:
		SerialConfig.BaudRate = CBR_19200;
		break;
	case 38400:
		SerialConfig.BaudRate = CBR_38400;
		break;
	case 56000:
		SerialConfig.BaudRate = CBR_56000;
		break;
	case 57600:
		SerialConfig.BaudRate = CBR_57600;
		break;
	case 115200 :
		SerialConfig.BaudRate = CBR_115200;
		break;
	case 128000:
		SerialConfig.BaudRate = CBR_128000;
		break;
	case 256000:
		SerialConfig.BaudRate = CBR_256000;
		break;
	default:
		break;
	}                              //���ˣ�DCB��������ϣ��ɵ���ϵͳ����
	if(this->Status == false)      //��ֹ�����ظ���
	{
		this->Handle = CreateFileA(portName,     //��������
								GENERIC_READ | GENERIC_WRITE,  //�򿪴��ڵ�ģʽ
								0,                             //������
								NULL,                          //Ĭ��
								OPEN_EXISTING,                 //��
								0,                             //Ĭ��
								NULL);                         //Ĭ��
		if (GetCommState(this->Handle,&(this->Config)) == 0)          //
		{
			cout<<"Get configuration port has problem."<<endl;          
			return FALSE;
		}
		//��������
		this->Config.BaudRate = SerialConfig.BaudRate;
		this->Config.StopBits = SerialConfig.StopBits;
		this->Config.Parity = SerialConfig.Parity;
		this->Config.ByteSize = SerialConfig.ByteSize;
		if (SetCommState(this->Handle,&this->Config) == 0)       //Set port
		{
			cout<<"Set configuration port has problem."<<endl;
			return FALSE;
		}
		//��ֹ��ʱ���������ʱ����
		COMMTIMEOUTS comTimeOut;                   
		comTimeOut.ReadIntervalTimeout = 3;
		comTimeOut.ReadTotalTimeoutMultiplier = 3;
		comTimeOut.ReadTotalTimeoutConstant = 2;
		comTimeOut.WriteTotalTimeoutMultiplier = 3;
		comTimeOut.WriteTotalTimeoutConstant = 2;
		SetCommTimeouts(this->Handle,&comTimeOut);
		this->Status = true;
		return true;
	}
	else
	{
		cout<<"�����Ѵ򿪣������ظ��򿪴���"<<endl;
		return false;
	}
}

/*��ȡ�������ݣ�����������ģ�壬�ı��԰ٶȰٿ�
  Serial com;
  com.OpenPort(9600,"COM8");
  char InputData[100];    //ÿ������ȡ100���ַ������InputData���ǻ�����buffer
  unsigned int DataSize = 101;  //�� buffer ��һ��������
  unsigned long DataLength;
  com.Read( Inputdata, DataSize, &DataLength); 
*/
bool Serial::Read(char* InputData,unsigned int DataSize,unsigned long & DataLength)
{
	if (ReadFile(this->Handle,  // handle of file to read
		InputData,               // handle of file to read �Ѷ��õ����ݷŵ�inputdata������
		DataSize,              // number of bytes to read Ҫ��ȡ����byte���ݣ�����������DWORD��һ�����дDWORD DataSize = GetFileSize(Handle,NULL);Ȼ���ٵ���
		&DataLength,                 // pointer to number of bytes read ���ڴ��ʵ�ʶ�ȡ�ֽڣ�����������DWORD
		NULL) == 0)              // pointer to structure for data
	{
		// AfxMessageBox("Reading of serial communication has problem.");
		return FALSE;
	}
	if (DataLength > 0)
	{
		InputData[DataLength] = NULL; // Assign end flag of message.
		return TRUE;  
	}  
	return TRUE;
}

bool Serial::ClosePort()
{
	if (this->Status == TRUE)               // Port need to be open before.
	{
		this->Status = FALSE;                 // Update status
		if(CloseHandle(this->Handle) == 0)    // Call this function to close port.
		{
			printf("Port Closeing isn't successed.\n");     //����Ĺ���������AfxMessageBox(" ")
			return FALSE;
		}    
		return TRUE;
	}
	return FALSE;
}

bool Serial::Write(char *outputData, unsigned int size, unsigned long & length)
{
	if(WriteFile(this->Handle,     //���
				outputData,        //Ҫд��Ķ���
				size,              //Ҫд����ֽ���
				&length,           //���ڴ��ʵ��д����ֽ���
				NULL)==0)          //��д��ʧ��
	{
		cout<<"д��ʧ�ܿ�= =||"<<endl;
		return false;
	}
	else
	{
		cout<<"д��ɹ���д���ֽ���Ϊ�� "<<length<<endl;
		return true;
	}
}
