//#include <windows.h>
#define _AFXDLL
#include <afxwin.h>
//#include "stdafx.h"
#include <stdio.h>   
#include <iostream>
#include "detailcontrol.h"
#include "definition.h"

//��Ϊ����5����Ϊ����6
//#define ICIN COM6
//#define ICOUT COM5

extern People ICpp;

//by Damon 20150725
int flag_icUpdate = 0;

DWORD WINAPI ICinthread(LPVOID pM)     //����
{
	HANDLE hModem;

	bool hascard;

	//hModem = CreateFile(L"COM1",GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,0);
	hModem = CreateFile(com_ICin,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,0);

	BOOL istrue;

	string cardnum;

	SetupComm(hModem,200,200);
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���
	istrue=SetCommTimeouts(hModem,&TimeOuts);
	if(istrue)
	{
		cout<<"SetCommTimeouts succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"SetCommTimeouts failed"<<endl;   ///////////////need modification
	}
	DCB dcb;
	GetCommState(hModem,&dcb);
	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	//	dcb.fBinary = TRUE;
	//	dcb.fParity = FALSE;
	istrue=SetCommState(hModem,&dcb);
	//cout<<istrue<<endl;   ///////////////need modification
	if(istrue)
	{
		//cout<<"SetCommState succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"SetCommState failed"<<endl;   ///////////////need modification
	}

	istrue=PurgeComm(hModem,PURGE_TXCLEAR|PURGE_RXCLEAR);
	if(istrue)
	{
		//cout<<"PurgeComm succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"PurgeComm failed"<<endl;   ///////////////need modification
	}	
	istrue=SetCommMask(hModem,EV_RXCHAR|EV_RXFLAG);
	if(istrue)
	{
		//cout<<"SetCommMask succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"SetCommMask failed"<<endl;   ///////////////need modification
	}	 

	OVERLAPPED m_osRead;
	memset(&m_osRead,0,sizeof(OVERLAPPED));
	m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	COMSTAT ComStat;
	DWORD dwErrorFlags;

	//char str[100];
	//memset(str,'\0',100);
	DWORD dwBytesRead=1000;//��ȡ���ֽ���
	DWORD BytesRead = 0;
	BOOL bReadStat; 
	int count=0;
	//cout << "reading begin" << endl;   ///////////////need modification
	char str[1000];
	memset(str,'\0',1000);
	int amount=0;
	while(1)
	{
		//char str[1000];
		//char str[1000];
		//memset(str,'\0',1000);
		char strtemp[1000];
		memset(strtemp,'\0',1000);
		dwBytesRead=1000;
		ClearCommError(hModem,&dwErrorFlags,&ComStat);
		dwBytesRead=min(dwBytesRead, (DWORD)ComStat.cbInQue);
		bReadStat=ReadFile(hModem,strtemp,dwBytesRead,&BytesRead,&m_osRead); // read data
		if (BytesRead>0)
		{
			if (amount+BytesRead<1000)
			{
				for (int i=0;i<BytesRead;i++)
				{
					str[amount+i]=strtemp[i];
				}
			}
		}
		amount+=BytesRead;
		/*cout << BytesRead << endl;*/
		if(!bReadStat)
		{
			if(GetLastError()==ERROR_IO_PENDING)
				//GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж�����
			{
				WaitForSingleObject(m_osRead.hEvent,2000);
				//ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ2����
				//�����ڶ�����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
			}
		}

		PurgeComm(hModem, PURGE_TXABORT|
			PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		if(BytesRead==0)
		{
			if (amount>0)
			{
				string b="                  ";   //18λ
				int j=0;
				while(j<amount)
				{
					if(str[j]=='i')
					{
						count=count+1;
						if(count==2)
							break ;
					}
					j++;
				}
				if(count==2&&((j+19)<=amount))
				{
					for(int k=0;k<=17;k++)
					{
						b[k]=str[j+k+7];
					}
					string peoname="";
					if((j+36)<=amount)
					{
						for(int k=0;k<10;k++)
						{
							if(str[j+26+k]=='~')
								break;
							peoname+=str[j+26+k];
						}
					}
					string c="    ";
					int count1=0;
					j=0;                           
					while(j<amount)
					{
						if (str[j]=='~')             //ȡ��Ч�����
						{
							count1=count1+1;
							if(count1==5)
								break ;
						}
						j++;
					}
					if(count1==5&&((j+5)<=amount))
					{
						for(int k=0;k<=3;k++)
						{
							c[k]=str[j+k+1];
						}
					}
					ICpp.peopleclear(); 
					ICpp.peopleinfo.validdate=c;
					ICpp.peopleinfo.name=peoname;
					cardnum=b;
					//cout<<"���ţ�"<<endl;   ///////////////need modification
					//cout<<cardnum<<endl;    ///////////////need modification
					ICdeal(cardnum,1);
					flag_icUpdate = 1;
				}
				count=0;
				amount=0;
				memset(str,'\0',1000);
			}
		}
		Sleep(300);
	}
} 


DWORD WINAPI ICoutthread(LPVOID pM)    //����
{
	HANDLE hModem;

	bool hascard;

	hModem = CreateFile(com_ICout,GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED,0);

	BOOL istrue;

	string cardnum;

	SetupComm(hModem,200,200);
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ
	TimeOuts.ReadIntervalTimeout=MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�
	//�������Ƿ������Ҫ����ַ���
	istrue=SetCommTimeouts(hModem,&TimeOuts);
	if(istrue)
	{
		//cout<<"SetCommTimeouts succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"SetCommTimeouts failed"<<endl;   ///////////////need modification
	}
	DCB dcb;
	GetCommState(hModem,&dcb);
	dcb.DCBlength = sizeof(dcb);
	dcb.BaudRate = 9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	//	dcb.fBinary = TRUE;
	//	dcb.fParity = FALSE;
	istrue=SetCommState(hModem,&dcb);
	//cout<<istrue<<endl;   ///////////////need modification
	if(istrue)
	{
		//cout<<"SetCommState succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"SetCommState failed"<<endl;   ///////////////need modification
	}

	istrue=PurgeComm(hModem,PURGE_TXCLEAR|PURGE_RXCLEAR);
	if(istrue)
	{
		//cout<<"PurgeComm succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"PurgeComm failed"<<endl;   ///////////////need modification
	}	
	istrue=SetCommMask(hModem,EV_RXCHAR|EV_RXFLAG);
	if(istrue)
	{
		//cout<<"SetCommMask succeed"<<endl;   ///////////////need modification
	}
	else
	{
		//cout<<"SetCommMask failed"<<endl;   ///////////////need modification
	}	 

	OVERLAPPED m_osRead;
	memset(&m_osRead,0,sizeof(OVERLAPPED));
	m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);

	COMSTAT ComStat;
	DWORD dwErrorFlags;

	//char str[100];
	//memset(str,'\0',100);
	DWORD dwBytesRead=1000;//��ȡ���ֽ���
	DWORD BytesRead = 0;
	BOOL bReadStat; 
	int count=0;
	//cout << "reading begin" << endl;   ///////////////need modification
	char str[1000];
	memset(str,'\0',1000);
	int amount=0;
	while(1)
	{
		//char str[1000];
		//char str[1000];
		//memset(str,'\0',1000);
		char strtemp[1000];
		memset(strtemp,'\0',1000);
		dwBytesRead=1000;
		ClearCommError(hModem,&dwErrorFlags,&ComStat);
		dwBytesRead=min(dwBytesRead, (DWORD)ComStat.cbInQue);
		bReadStat=ReadFile(hModem,strtemp,dwBytesRead,&BytesRead,&m_osRead); // read data
		if (BytesRead>0)
		{
			if (amount+BytesRead<1000)
			{
				for (int i=0;i<BytesRead;i++)
				{
					str[amount+i]=strtemp[i];
				}
			}
		}
		amount+=BytesRead;
		/*cout << BytesRead << endl;*/
		if(!bReadStat)
		{
			if(GetLastError()==ERROR_IO_PENDING)
				//GetLastError()��������ERROR_IO_PENDING,�����������ڽ��ж�����
			{
				WaitForSingleObject(m_osRead.hEvent,2000);
				//ʹ��WaitForSingleObject�����ȴ���ֱ����������ɻ���ʱ�Ѵﵽ2����
				//�����ڶ�����������Ϻ�m_osRead��hEvent�¼����Ϊ���ź�
			}
		}

		PurgeComm(hModem, PURGE_TXABORT|
			PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
		if(BytesRead==0)
		{
			if (amount>0)
			{
				string b="                  ";   //18λ
				int j=0;
				while(j<amount)
				{
					if(str[j]=='i')
					{
						count=count+1;
						if(count==2)
							break ;
					}
					j++;
				}
				if(count==2&&((j+19)<=amount))
				{
					for(int k=0;k<=17;k++)
					{
						b[k]=str[j+k+7];
					}
					string peoname="";
					if((j+36)<=amount)
					{
						for(int k=0;k<10;k++)
						{
							if(str[j+26+k]=='~')
								break;
							peoname+=str[j+26+k];
						}
					}
					string c="    ";
					int count1=0;
					j=0;                           
					while(j<amount)
					{
						if (str[j]=='~')             //ȡ��Ч�����
						{
							count1=count1+1;
							if(count1==5)
								break ;
						}
						j++;
					}
					if(count1==5&&((j+5)<=amount))
					{
						for(int k=0;k<=3;k++)
						{
							c[k]=str[j+k+1];
						}
					}
					ICpp.peopleclear(); 
					ICpp.peopleinfo.validdate=c;
					ICpp.peopleinfo.name=peoname;
					cardnum=b;
					//cout<<"���ţ�"<<endl;   ///////////////need modification
					//cout<<cardnum<<endl;    ///////////////need modification
					ICdeal(cardnum,2);
					flag_icUpdate = 1;
				}
				count=0;
				amount=0;
				memset(str,'\0',1000);
			}
		}
		Sleep(300);
	}
} 