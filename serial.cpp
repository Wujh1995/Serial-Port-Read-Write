#include "stdafx.h"
#include "serial.h"

Serial::Serial()
{
	Config.ByteSize = 8;          //8个比特数据为单位
	Config.BaudRate = CBR_9600;   //波特率默认9600
	Config.StopBits = ONESTOPBIT;  //停止位为1位
	Config.Parity = NOPARITY;      //无奇偶校验位
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
	}                              //至此，DCB已设置完毕，可调用系统函数
	if(this->Status == false)      //防止串口重复打开
	{
		this->Handle = CreateFileA(portName,     //串口名称
								GENERIC_READ | GENERIC_WRITE,  //打开串口的模式
								0,                             //不公开
								NULL,                          //默认
								OPEN_EXISTING,                 //打开
								0,                             //默认
								NULL);                         //默认
		if (GetCommState(this->Handle,&(this->Config)) == 0)          //
		{
			cout<<"Get configuration port has problem."<<endl;          
			return FALSE;
		}
		//导入数据
		this->Config.BaudRate = SerialConfig.BaudRate;
		this->Config.StopBits = SerialConfig.StopBits;
		this->Config.Parity = SerialConfig.Parity;
		this->Config.ByteSize = SerialConfig.ByteSize;
		if (SetCommState(this->Handle,&this->Config) == 0)       //Set port
		{
			cout<<"Set configuration port has problem."<<endl;
			return FALSE;
		}
		//防止超时，设置最大时间间隔
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
		cout<<"串口已打开，无需重复打开串口"<<endl;
		return false;
	}
}

/*读取串口数据，可以用以下模板，改编自百度百科
  Serial com;
  com.OpenPort(9600,"COM8");
  char InputData[100];    //每次最多读取100个字符，这个InputData就是缓冲区buffer
  unsigned int DataSize = 101;  //比 buffer 多一个结束符
  unsigned long DataLength;
  com.Read( Inputdata, DataSize, &DataLength); 
*/
bool Serial::Read(char* InputData,unsigned int DataSize,unsigned long & DataLength)
{
	if (ReadFile(this->Handle,  // handle of file to read
		InputData,               // handle of file to read 把读得的数据放到inputdata数组中
		DataSize,              // number of bytes to read 要读取多少byte数据，数据类型是DWORD，一般可以写DWORD DataSize = GetFileSize(Handle,NULL);然后再调用
		&DataLength,                 // pointer to number of bytes read 用于存放实际读取字节，数据类型是DWORD
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
			printf("Port Closeing isn't successed.\n");     //这里改过，本来是AfxMessageBox(" ")
			return FALSE;
		}    
		return TRUE;
	}
	return FALSE;
}

bool Serial::Write(char *outputData, unsigned int size, unsigned long & length)
{
	if(WriteFile(this->Handle,     //句柄
				outputData,        //要写入的东西
				size,              //要写入的字节数
				&length,           //用于存放实际写入的字节数
				NULL)==0)          //即写入失败
	{
		cout<<"写入失败咯= =||"<<endl;
		return false;
	}
	else
	{
		cout<<"写入成功，写入字节数为： "<<length<<endl;
		return true;
	}
}
