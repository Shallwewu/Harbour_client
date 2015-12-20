#include "inspection.h"
#include <QtWidgets/QApplication>

#define _AFXDLL
#include <afxwin.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "sqlite3.h"
#include "detailcontrol.h"
#include "camera.h"
sqlite3* pdb;

extern DWORD WINAPI ICinthread(LPVOID pM);
extern DWORD WINAPI ICoutthread(LPVOID pM);
extern DWORD WINAPI OCRthread(LPVOID pM);
extern DWORD WINAPI doorControlThread(LPVOID pM);
//extern DWORD WINAPI Doorthread(LPVOID pM);

#include "registration.h"

//string AREA;
//string STATION;
//string REIP_ADDRESS;
//int REMOTEPORT;
//string UDPADDRESS;
//int UDPPORT;
//string IP_PIn;
//string IP_POut;
//string IP_DriverIn;
//string IP_TrafficIn;
//string IP_DriverOut;
//string IP_TrafficOut;

char AREA0[20];
char* AREA;
char STATION0[20];
char* STATION;
char REIP_ADDRESS[20];
int REMOTEPORT;
char UDPADDRESS[20];
int UDPPORT;
unsigned int iDevSn;
char IP_PIn[20];
char IP_POut[20];
char IP_DriverIn[20];
char IP_TrafficIn[20];
char IP_DriverOut[20];
char IP_TrafficOut[20];
wchar_t *com_ICin;
wchar_t *com_ICout;

extern People ICpp;
//char AREA1[20];
//char STATION1[20];
//char REIP_ADDRESS1[20];
//int REMOTEPORT1;
//char UDPADDRESS1[20];
//int UDPPORT1;
//char IP_PIn1[20];
//char IP_POut1[20];
//char IP_DriverIn1[20];
//char IP_TrafficIn1[20];
//char IP_DriverOut1[20];
//char IP_TrafficOut1[20];

bool config();
wchar_t * string2LPCWSTR(string strText);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	int regsucceedT = registrateT();
	int regsucceedM = registrateM();
	if ((!regsucceedT)||(!regsucceedM))
	{
		//QMessageBox::information(NULL, "Title", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		//QMessageBox::warning(NULL,"reg failed!", "Please contant Excellent to get reg information!", QMessageBox::Ok);
		QMessageBox::warning(NULL,"config failed!", "Please contact Excellent!", QMessageBox::Ok);
		exit(NULL);
	}

	config();

	Inspection w;
	w.show();

	int nResult = sqlite3_open(".//harbour_database//clientDb.db",&pdb);
	if (nResult != SQLITE_OK)
	{
		//cout<<"打开数据库失败："<<sqlite3_errmsg(pdb)<<endl;  //需要修改
		return 0;
	}
	else
	{
		//cout<<"数据库打开成功"<<endl;  //需要修改
	}

	bool ret=Clientsockini();  //初始化客户端Socket（远程访问服务器的），未连接
	cameraIni(); //网络摄像头初始化

	HANDLE OCRhandle = CreateThread(NULL, 0, OCRthread, NULL, 0, NULL); 
	HANDLE ICinhandle = CreateThread(NULL, 0, ICinthread, NULL, 0, NULL);
	HANDLE ICouthandle = CreateThread(NULL, 0, ICoutthread, NULL, 0, NULL);
	HANDLE Doorthandle = CreateThread(NULL, 0, doorControlThread, NULL, 0, NULL);
	return a.exec();
}

bool config()
{
	ifstream dataIn;
	dataIn.open("config.txt");
	string temp;
	if (dataIn.is_open())
	{
		dataIn>>temp;
		dataIn >> AREA0;
		AREA=U2G(AREA0);
		dataIn >> STATION0;
		STATION=U2G(STATION0);
		dataIn >> REIP_ADDRESS;
		dataIn >> REMOTEPORT;
		dataIn >> UDPADDRESS;
		dataIn >> UDPPORT;
		dataIn >> iDevSn;
		dataIn >> IP_PIn;
		dataIn >> IP_POut;
		dataIn >> IP_DriverIn;
		dataIn >> IP_TrafficIn;
		dataIn >> IP_DriverOut;
		dataIn >> IP_TrafficOut;
		string temp;
		dataIn >> temp;
		com_ICin = string2LPCWSTR(temp);
		dataIn >> temp;
		com_ICout = string2LPCWSTR(temp);

		dataIn.close();
	}
	else
	{
		QMessageBox::warning(NULL,"config failed!", "No configuration information!", QMessageBox::Ok);
		exit(NULL);
	}
	return true;
}

wchar_t * string2LPCWSTR(string strText)
{
	size_t size = strText.length();
	wchar_t *buffer = new wchar_t[size+1];
	MultiByteToWideChar( CP_ACP, 0, strText.c_str(), size, buffer, size * sizeof(wchar_t) );
	buffer[size] = 0;
	return buffer;
}