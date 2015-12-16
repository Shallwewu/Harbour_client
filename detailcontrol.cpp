//#include <iostream>
//#include <winsock2.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#define _AFXDLL
#include <afxwin.h>
#include <afx.h>
//#include "maincontrol.h"
#include "sqlite3.h"
#include "detailcontrol.h"
#include "camera.h"
#include <time.h>
#include "definition.h"
//#define AREA "崇明"
//#define STATION "横沙渔港1号"
//#define REIP_ADDRESS "172.16.0.100"
//#define REMOTEPORT 9999
//#define UDPADDRESS "172.16.0.106"
//#define UDPPORT 60000

int gateup=1;
int gatedown=0;
SOCKET ClientSocket;
struct sockaddr_in ReServerAddr;
//unsigned int iDevSn=423126004;  //控制器S/N
SOCKET Doorudp;  
SOCKADDR_IN dooraddr;  
//char doorbuff[64];
int flag_door=0;

extern int pupdate;
extern int cupdate;
extern People ICpp;
extern People WTpp;
extern Car Cardriver;
extern bool toOpen;
extern int toOpen_udp;		//0-for no open, 1-for open in door, 2-for open out door
extern int doorFlag_udp;	//0-for nothing, 1-for WT in, 2-for WT out, 3-for IC in, 4-for IC out
extern int typenum;
extern sqlite3* pdb;

char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len+1];
	memset(wstr, 0, len+1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len+1];
	memset(str, 0, len+1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if(wstr) delete[] wstr;
	return str;
}

/*************************************远程服务器操作***********************/
bool Clientsockini()    //socket初始化
{
	WSADATA Ws;
	if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
	{
		cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
		return FALSE;
	}
	//ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if ( ClientSocket == INVALID_SOCKET )
	//{
	//	cout<<"Create Socket Failed::"<<GetLastError()<<endl;
	//	return FALSE;
	//}

	ReServerAddr.sin_family = AF_INET;
	ReServerAddr.sin_addr.s_addr = inet_addr(REIP_ADDRESS);
	ReServerAddr.sin_port = htons(REMOTEPORT);
	memset(ReServerAddr.sin_zero, 0x00, 8);
}


bool Remotesave(string &infomation)
{
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( ClientSocket == INVALID_SOCKET )
	{
		cout<<"Create Socket Failed::"<<GetLastError()<<endl;
		return FALSE;
	}

	char SendBuffer[1024];
	string utfbuffer;
	int sendsize = infomation.length();
	memset(SendBuffer,'\0',sizeof(SendBuffer));
	memcpy(SendBuffer,infomation.c_str(),sendsize);
	utfbuffer=G2U(SendBuffer);
	//SendBuffer[sendsize]='\0';
	//SendBuffer还未赋值，且大小还未确定
	int Ret = connect(ClientSocket,(struct sockaddr*)&ReServerAddr, sizeof(ReServerAddr));
	if ( Ret == SOCKET_ERROR )
	{
		cout<<"Connect Error::"<<GetLastError()<<endl;
		return FALSE;
	}
	else
	{
		cout<<"Connect succeed!"<<endl;
	}
	Ret = send(ClientSocket, utfbuffer.c_str(), utfbuffer.length(), 0);
	closesocket(ClientSocket);
	if ( Ret == SOCKET_ERROR )
	{
		cout<<"Send Info Error::"<<GetLastError()<<endl;
		return FALSE;
	}	
}

/*****************************获取当前时间****************************/
string Getnowyear()               //获取年份
{
	string year="";
	char yc[5];
	CTime t = CTime::GetCurrentTime(); //获取系统日期

	int y=t.GetYear(); //获取年份

	itoa(y, yc, 10);

	year+=yc;

	return year;
}

string Getnowtime()               //用于存放记录的时间格式
{
	string nowtime="";
	char yc[5],mc[3],dc[3],hc[3],mmc[3];
	CTime t = CTime::GetCurrentTime(); //获取系统日期

	int y=t.GetYear(); //获取年份

	int m=t.GetMonth(); //获取当前月份

	int d=t.GetDay(); //获得几号

	int h=t.GetHour(); //获取当前为几时

	int mm=t.GetMinute(); //获取分钟

	itoa(y, yc, 10);

	nowtime+=yc;
	nowtime+="-";

	itoa(m, mc, 10);
	if(m<10)
	{
		mc[1]='\0';
		nowtime+="0";
	}
	nowtime+=mc;
	nowtime+="-";
	itoa(d, dc, 10);
	if(d<10)
	{
		dc[1]='\0';
		nowtime+="0";
	}
	nowtime+=dc;
	nowtime+=" ";
	itoa(h, hc, 10);
	if(h<10)
	{
		hc[1]='\0';
		nowtime+="0";
	}
	nowtime+=hc;
	nowtime+=":";
	itoa(mm, mmc, 10);
	if(mm<10)
	{
		mmc[1]='\0';
		nowtime+="0";
	}
	nowtime+=mmc;

	return nowtime;
}

string Obtaintime()     //用于数据库对比的时间格式
{
	string nowtime="";
	char yc[5],mc[3],dc[3],hc[3],mmc[3];
	CTime t = CTime::GetCurrentTime(); //获取系统日期

	int y=t.GetYear(); //获取年份

	int m=t.GetMonth(); //获取当前月份

	int d=t.GetDay(); //获得几号

	itoa(y, yc, 10);

	nowtime+=yc;

	itoa(m, mc, 10);
	if(m<10)
	{
		mc[1]='\0';
		nowtime+="0";
	}
	nowtime+=mc;

	itoa(d, dc, 10);
	if(d<10)
	{
		dc[1]='\0';
		nowtime+="0";
	}
	nowtime+=dc;

	return nowtime;
}

string Entiretime()
{
	string nowtime="";
	char yc[5],mc[3],dc[3],hc[3],mmc[3],secc[3];
	CTime t = CTime::GetCurrentTime(); //获取系统日期

	int y=t.GetYear(); //获取年份

	int m=t.GetMonth(); //获取当前月份

	int d=t.GetDay(); //获得几号

	int h=t.GetHour(); //获取当前为几时

	int mm=t.GetMinute(); //获取分钟

	int sec=t.GetSecond();

	itoa(y, yc, 10);

	nowtime+=yc;

	itoa(m, mc, 10);
	if(m<10)
	{
		mc[1]='\0';
		nowtime+="0";
	}
	nowtime+=mc;

	itoa(d, dc, 10);
	if(d<10)
	{
		dc[1]='\0';
		nowtime+="0";
	}
	nowtime+=dc;

	itoa(h, hc, 10);
	if(h<10)
	{
		hc[1]='\0';
		nowtime+="0";
	}
	nowtime+=hc;

	itoa(mm, mmc, 10);
	if(mm<10)
	{
		mmc[1]='\0';
		nowtime+="0";
	}
	nowtime+=mmc;

	itoa(sec, secc, 10);
	if(sec<10)
	{
		secc[1]='\0';
		nowtime+="0";
	}

	nowtime+=secc;

	return nowtime;
}
/*********************************************IC卡控制**************************************/
void ICpassstate(unsigned int state)
{
	string strsql;
	char* cErrMsg; 
	char **dbResult;
	int nRow,nColumn;

	strsql="select direction from (select * from driver_rec_table where id='";
	strsql+=G2U((ICpp.peopleinfo.number).c_str());
	strsql+="') order by date desc limit 0,1;"; //按时间排序，取出时间最近的一次
	int nRes = sqlite3_get_table(pdb,strsql.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg); 
	if (nRes != SQLITE_OK)
	{  
		cout<<"select fail: "<<cErrMsg<<endl;  
		return;
	}  
	int idex=nColumn;
	string a="";
	if(state==1)    //进门对应于登轮
	{
		if(idex==0)
		{
			ICpp.peopleinfo.inout="进门";  
		}
		else if(idex==1)
		{
			if(dbResult[idex]!=NULL)
			{
				a=U2G(dbResult[idex]);
			}
			if(a=="进门")
			{
				ICpp.peopleinfo.inout="连续两次进门";
			}
			else if(a=="出门")
			{
				ICpp.peopleinfo.inout="进门";
			}
			else if(a=="第一次非法出门")
			{
				ICpp.peopleinfo.inout="进门";
			}
			else if(a=="连续两次进门")
			{
				ICpp.peopleinfo.inout="连续两次进门";
			}
			else if(a=="连续两次出门")
			{
				ICpp.peopleinfo.inout="进门";
			}
			else
			{
				ICpp.peopleinfo.inout="进门";
			}
		}
	}

	else if(state==2)    //进门对应于登轮
	{
		if(idex==0)
		{
			ICpp.peopleinfo.inout="第一次非法出门";  
		}
		else if(idex==1)
		{
			if(dbResult[idex]!=NULL)
			{
				a=U2G(dbResult[idex]);
			}
			if(a=="进门")
			{
				ICpp.peopleinfo.inout="出门";
			}
			else if(a=="出门")
			{
				ICpp.peopleinfo.inout="连续两次出门";
			}
			else if(a=="第一次非法出门")
			{
				ICpp.peopleinfo.inout="连续两次出门";
			}
			else if(a=="连续两次进门")
			{
				ICpp.peopleinfo.inout="出门";
			}
			else if(a=="连续两次出门")
			{
				ICpp.peopleinfo.inout="连续两次出门";
			}
			else
			{
				ICpp.peopleinfo.inout="出门";
			}
		}
	}
	ICpp.peopleinfo.alarm=ICpp.peopleinfo.inout;
}

void ICsnapsave(unsigned int inorout)
{
	ICpassstate(inorout);
	if (inorout==1)
	{
//		controldoor(1);
		snap(0);
/*		ICpp.peopleinfo.inout="进"; */ 
//		toOpen = true;
//		flag_door=1;
	}
	else if(inorout==2)
	{
//		controldoor(2);
		snap(1);
//		ICpp.peopleinfo.inout="出门"; 
//		toOpen = true;
//		flag_door=2;
	}
//	ICpp.precordsave();	
}

void ICdeal(string data,int inorout)
{
	/*ICpp.peopleclear(); */  //清空变量
	ICpp.peopleinfo.number=data;
	bool Exist=ICpp.iccompare();     //IC卡信息比对
//	bool Exist=true;  //刷了就能过
	if(Exist==false)
	{
		return;
	} 
	ICpp.peopleinfo.time=Getnowtime();
	ICpp.peopleinfo.area=AREA;
	ICpp.peopleinfo.station=STATION;
	ICpp.peopleinfo.type="长期登轮证";
	ICpp.peopleinfo.stype="长期登轮证";
	toOpen_udp=inorout;
	doorFlag_udp=inorout;
	//if (inorout==1)
	//{
	//	controldoor(1);
	//	snap(0);
	//	ICpp.peopleinfo.inout="进";  
	//	toOpen = true;
	//	flag_door=1;
	//}
	//else if(inorout==2)
	//{
	//	controldoor(2);
	//	snap(1);
	//	ICpp.peopleinfo.inout="出"; 
	//	toOpen = true;
	//	flag_door=2;
	//}
//	ICsnapsave(inorout);
//	ICpp.precordsave();	
}

/**************************************文通控制**********************************************/
//void WTdeal(void)
//{
//	WTpp.peopleclear();  //人员信息清除
//	//人员信息赋值
//	pupdate=1;
//		//	char sendbuff[1024];
//		//	memcpy((void *)sendbuff,(void *)&WTpp.uipeoinfo,sizeof(a));   //结构体转换成字符流用于socket传输
//}


void confiopendoor(unsigned int inorout)     //确认开门    开门信号
{
	//if(pupdate==1)
	//{
	//	pupdate=0;     //人员信息可以不用保留了
	//	WTpp.peopleinfo.time=Getnowtime();
	//	WTpp.peopleinfo.area=AREA;
	//	if(inorout==1)
	//	{
	//		snap(0);
	//		/*WTpp.flag_snap = 1;*/
	//		WTpp.peopleinfo.inout="进";
	//	}
	//	else if(inorout==2)
	//	{
	//		snap(1);
	//		/*WTpp.flag_snap = 1;*/
	//		WTpp.peopleinfo.inout="出";
	//	}
	//	WTpp.precordsave();  //人员进出纪录存储
	//	//		Remotesave(&infomation);  //远程存储纪录
//	snapandsave(inorout);     @wu
	//}
	if(inorout==1)
	{
		//controldoor(inorout);       //开门信号
		toOpen_udp=1;
		doorFlag_udp=3;
	}
	else if(inorout==2)
	{
		toOpen_udp=2;
		doorFlag_udp=4;
	}
}

void WTpassstate(unsigned int state)
{
	string strsql;
	char* cErrMsg; 
	char **dbResult;
	int nRow,nColumn;

	strsql="select direction from (select * from driver_rec_table where id='";
	strsql+=G2U((WTpp.peopleinfo.number).c_str());
	strsql+="') order by date desc limit 0,1;"; //按时间排序，取出时间最近的一次
	int nRes = sqlite3_get_table(pdb,strsql.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg); 
	if (nRes != SQLITE_OK)
	{  
		cout<<"select fail: "<<cErrMsg<<endl;  
		return;
	}  
	int idex=nColumn;
	string a="";
	if (typenum==8)     //登轮许可证
	{
		if(state==1)    //进门对应于登轮
		{
			if(idex==0)
			{
				WTpp.peopleinfo.inout="进门";  
			}
			else if(idex==1)
			{
				if(dbResult[idex]!=NULL)
				{
					a=U2G(dbResult[idex]);
				}
				if(a=="进门")
				{
					WTpp.peopleinfo.inout="连续两次进门";
				}
				else if(a=="出门")
				{
					WTpp.peopleinfo.inout="进门";
				}
				else if(a=="第一次非法出门")
				{
					WTpp.peopleinfo.inout="进门";
				}
				else if(a=="连续两次进门")
				{
					WTpp.peopleinfo.inout="连续两次进门";
				}
				else if(a=="连续两次出门")
				{
					WTpp.peopleinfo.inout="进门";
				}
				else
				{
					WTpp.peopleinfo.inout="进门";
				}
			}
		}

		else if(state==2)    
		{
			if(idex==0)
			{
				WTpp.peopleinfo.inout="第一次非法出门";  
			}
			else if(idex==1)
			{
				if(dbResult[idex]!=NULL)
				{
					a=U2G(dbResult[idex]);
				}
				if(a=="进门")
				{
					WTpp.peopleinfo.inout="出门";
				}
				else if(a=="出门")
				{
					WTpp.peopleinfo.inout="连续两次出门";
				}
				else if(a=="第一次非法出门")
				{
					WTpp.peopleinfo.inout="连续两次出门";
				}
				else if(a=="连续两次进门")
				{
					WTpp.peopleinfo.inout="出门";
				}
				else if(a=="连续两次出门")
				{
					WTpp.peopleinfo.inout="连续两次出门";
				}
				else
				{
					WTpp.peopleinfo.inout="出门";
				}
			}
		}  
	}

	else if (typenum==6||typenum==7)
	{
		if(state==1)    
		{
			if(idex==0)
			{
				WTpp.peopleinfo.inout="第一次非法进门";  
			}
			else if(idex==1)
			{
				if(dbResult[idex]!=NULL)
				{
					a=U2G(dbResult[idex]);
				}
				if(a=="进门")
				{
					WTpp.peopleinfo.inout="连续两次进门";
				}
				else if(a=="出门")
				{
					WTpp.peopleinfo.inout="进门";
				}
				else if(a=="第一次非法进门")
				{
					WTpp.peopleinfo.inout="连续两次进门";
				}
				else if(a=="连续两次进门")
				{
					WTpp.peopleinfo.inout="连续两次进门";
				}
				else if(a=="连续两次出门")
				{
					WTpp.peopleinfo.inout="进门";
				}
				else
				{
					WTpp.peopleinfo.inout="进门";
				}
			}
		}

		else if(state==2)    //出门对应于登陆
		{
			if(idex==0)
			{
				WTpp.peopleinfo.inout="出门";  
			}
			else if(idex==1)
			{
				if(dbResult[idex]!=NULL)
				{
					a=U2G(dbResult[idex]);
				}
				if(a=="进门")
				{
					WTpp.peopleinfo.inout="出门";
				}
				else if(a=="出门")
				{
					WTpp.peopleinfo.inout="连续两次出门";
				}
				else if(a=="第一次非法进门")
				{
					WTpp.peopleinfo.inout="出门";
				}
				else if(a=="连续两次进门")
				{
					WTpp.peopleinfo.inout="出门";
				}
				else if(a=="连续两次出门")
				{
					WTpp.peopleinfo.inout="连续两次出门";
				}
				else
				{
					WTpp.peopleinfo.inout="出门";
				}
			}
		}
	}
	else if(typenum==2||typenum==5||typenum==13||typenum==17)
	{
		if(state==1)    
		{
			if(idex==0)
			{
				WTpp.peopleinfo.inout="进门";  
			}
			else if(idex==1)
			{
				if(dbResult[idex]!=NULL)
				{
					a=U2G(dbResult[idex]);
				}
				if(a=="进门")
				{
					WTpp.peopleinfo.inout="连续两次进门";
				}
				else if(a=="出门")
				{
					WTpp.peopleinfo.inout="进门";
				}
				//else if(a=="第一次非法进门")
				//{
				//	ICpp.peopleinfo.inout="连续两次进门";
				//}
				else if(a=="连续两次进门")
				{
					WTpp.peopleinfo.inout="连续两次进门";
				}
				else if(a=="连续两次出门")
				{
					WTpp.peopleinfo.inout="进门";
				}
				else
				{
					WTpp.peopleinfo.inout="进门";
				}
			}
		}

		else if(state==2)    
		{
			if(idex==0)
			{
				WTpp.peopleinfo.inout="出门";  
			}
			else if(idex==1)
			{
				if(dbResult[idex]!=NULL)
				{
					a=U2G(dbResult[idex]);
				}
				if(a=="进门")
				{
					WTpp.peopleinfo.inout="出门";
				}
				else if(a=="出门")
				{
					WTpp.peopleinfo.inout="连续两次出门";
				}
				//else if(a=="第一次非法进门")
				//{
				//	ICpp.peopleinfo.inout="出门";
				//}
				else if(a=="连续两次进门")
				{
					WTpp.peopleinfo.inout="出门";
				}
				else if(a=="连续两次出门")
				{
					WTpp.peopleinfo.inout="连续两次出门";
				}
				else
				{
					WTpp.peopleinfo.inout="出门";
				}
			}
		}
	}
	WTpp.peopleinfo.alarm=WTpp.peopleinfo.inout;
}
void snapandsave(unsigned int inorout)
{
	//if(pupdate==1)
	//{
//		pupdate=0;     //人员信息可以不用保留了
		WTpp.peopleinfo.time=Getnowtime();
		WTpp.peopleinfo.area=AREA;
		WTpp.peopleinfo.station=STATION;
		switch(typenum)
		{
			case 2:
				WTpp.peopleinfo.stype = "身份证";
				typenum=0;
				break;
			case 5:
				WTpp.peopleinfo.stype = "驾照";
				typenum=0;
				break;
			case 6:
				WTpp.peopleinfo.stype = "台湾船员登陆证";
				typenum=0;
				break;
			case 7:
				WTpp.peopleinfo.stype = "临时入境许可";
				typenum=0;
				break;
			case 8:
				WTpp.peopleinfo.stype = "临时登轮证";
				typenum=0;
				break;
			case 13:
				WTpp.peopleinfo.stype = "护照";
				typenum=0;
				break;
			case 17:
				WTpp.peopleinfo.stype = "海员证";
				typenum=0;
				break;
			default:
				typenum=0;
				break;
		}
		if(inorout==1)
		{
			snap(0);
			WTpp.flag_snap = 1;
//			WTpp.peopleinfo.inout="进";
		}
		else if(inorout==2)
		{
			snap(1);
			WTpp.flag_snap = 1;
//			WTpp.peopleinfo.inout="出";
		}
		WTpassstate(inorout);
//		WTpp.precordsave();  //人员进出纪录存储
		//		Remotesave(&infomation);  //远程存储纪录
	//}
}


void notopendoor(void)    //确认不开门     不开门信号
{
	pupdate=0;
}


/********************************车辆进出控制********************************/
int Cardeal(string &newlp)      //可用于自动识别车牌 或手动输入车牌
{
	cupdate=1;
	Cardriver.setplate(newlp);   //车牌赋值
	bool nRet=Cardriver.Getcarinfo();   //已判断有无此车牌并取信息
	if(nRet==FALSE)
	{
		return 0;
	}
	return 1;
}


bool Chosedri(string name)    //选择司机名字后的操作 包括主司机、副司机的选择  或者手动选择的司机
{
	Cardriver.choosedrina=name;
	//if((Cardriver.choosedrina!=Cardriver.firstdri.name)&&(Cardriver.choosedrina!=Cardriver.seconddri.name))
	//{
	//	bool nRet=Cardriver.Getotherdri();
	//	if(nRet==false)
	//		return FALSE;
	Cardriver.vehicle.driver=Cardriver.otherdri.name;
	Cardriver.vehicle.cardtype=Cardriver.otherdri.cardtype;
	Cardriver.vehicle.number=Cardriver.otherdri.number;
	//}
	bool nRet=Cardriver.Getotherdri();
	if(nRet==false)
		return FALSE;
	Cardriver.vehicle.driver=Cardriver.otherdri.name;
	Cardriver.vehicle.cardtype=Cardriver.otherdri.cardtype;
	Cardriver.vehicle.number=Cardriver.otherdri.number;
	return TRUE;
}

void driregister(string &name,string &type,string &number)    //获取手动添加司机信息
{
	Cardriver.vehicle.driver=name;
	Cardriver.vehicle.cardtype=type;
	Cardriver.vehicle.number=number;
}

void confiopengate()                //确认抬杆
{
	if(cupdate==1)
	{
		/*gatedown=1;*/
		cupdate=0;
		Cardriver.vehicle.time=Getnowtime();
//		Cardriver.vehicle.inout="进";   //待定
		Cardriver.vehicle.area=AREA;
		Cardriver.crecordsave();   //数据库纪录信息
		//		Remotesave(&infomation);   //远程数据库纪录信息
		//if(gateup==1)
		//{
			//controldoor(3);
		//	gateup=0;
		//}
		Cardriver.carclear();
	}
	controldoor(3);
}

void closegate()
{
	//if (gatedown==1)
	//{
	//	gatedown=0;
		controldoor(4);
	//	gateup=1;
	//}
}
/*******************************************门禁控制********************************/
void controldoor(unsigned int doorNO)  //doorNO:1（进），2（出），3（抬杆），4（降杆）
{  
	SOCKET udp;  
	SOCKADDR_IN addr;  
	char buff[64];
	memset(buff,0,sizeof(buff));
	buff[0] = 0x19;
	buff[1] = 0x40;
	memcpy(&(buff[4]),&(iDevSn), 4);
	buff[8] = (doorNO & 0xff);

	//WSADATA wsa;  
	//WSAStartup(MAKEWORD(2,2),&wsa); //initial Ws2_32.dll by a process  

	memset(&addr, 0, sizeof(addr));  

	if((udp = socket(AF_INET,SOCK_DGRAM,0)) <= 0)  
	{  
		printf("Create socket fail!\n");  
		return;  
	}  

	addr.sin_family = AF_INET;  
	addr.sin_addr.s_addr = inet_addr(UDPADDRESS);  
	addr.sin_port = htons(UDPPORT);  
	bind(udp,(struct sockaddr *)&addr,sizeof(addr));  
	/*Sleep(10);*/
	sendto(udp, buff, sizeof(buff), 0, (struct sockaddr *)&addr, sizeof(addr));  
	//Sleep(100);
	closesocket(udp);
	//	WSACleanup();   //clean up Ws2_32.dll   
}  

int doorConditionInit()
{
	memset(&dooraddr, 0, sizeof(dooraddr));  

	closesocket(Doorudp);
	if((Doorudp = socket(AF_INET,SOCK_DGRAM,0)) <= 0)  
	{  
		printf("Create socket fail!\n");  
		return -1;  
	}  

	dooraddr.sin_family = AF_INET;  
	dooraddr.sin_addr.s_addr = inet_addr(UDPADDRESS);  
	dooraddr.sin_port = htons(UDPPORT);  
	bind(Doorudp,(struct sockaddr *)&dooraddr,sizeof(dooraddr)); 
}



int getDoorCondition(unsigned int doorNO)  //查询门控板的状态，用于控制摄像头
{  

	if (doorNO != 1 && doorNO != 2)
	{
		//warning message!
		return -1;
	}

	char doorbuff[64];
	memset(doorbuff,0,sizeof(doorbuff));
	doorbuff[0] = 0x19;
	doorbuff[1] = 0x20;
	memcpy(&(doorbuff[4]),&(iDevSn), 4);

	//char doorbuff[64];
	//memset(doorbuff,0,sizeof(doorbuff));
	//doorbuff[0] = 0x19;
	//doorbuff[1] = 0x20;
	//memcpy(&(doorbuff[4]),&(iDevSn), 4);

	//memset(&dooraddr, 0, sizeof(dooraddr)); 

	//memset(buff,0,sizeof(buff));
	//buff[0] = 0x19;
	//buff[1] = 0x20;
	//memcpy(&(buff[4]),&(iDevSn), 4);
	//buff[8] = (doorNO & 0xff);

	//WSADATA wsa;  
	//WSAStartup(MAKEWORD(2,2),&wsa); //initial Ws2_32.dll by a process  
/*
	memset(&addr, 0, sizeof(addr));  

	if((udp = socket(AF_INET,SOCK_DGRAM,0)) <= 0)  
	{  
		printf("Create socket fail!\n");  
		return -1;  
	}  

	addr.sin_family = AF_INET;  
	addr.sin_addr.s_addr = inet_addr(UDPADDRESS);  
	addr.sin_port = htons(UDPPORT);  
	bind(udp,(struct sockaddr *)&addr,sizeof(addr)); */ 

	char isPassed = 'n';
	char receiveBuff[100];
	int len = sizeof(SOCKADDR);
	int nRet = 0;

	//time_t timeBegin, timeEnd;
	//timeBegin = time(NULL);
	//while (true)
	//{
		memset(receiveBuff,'\0',sizeof(receiveBuff));
		sendto(Doorudp, doorbuff, sizeof(doorbuff), 0, (struct sockaddr *)&dooraddr, sizeof(dooraddr)); 
		//Sleep(10);
		nRet = recvfrom(Doorudp, receiveBuff, sizeof(receiveBuff), 0, (struct sockaddr *)&dooraddr, &len);
		//timeEnd = time(NULL);
		if (nRet != 64 )//|| (timeEnd-timeBegin)>5)
			return -1;

		if (doorNO == 1)
			isPassed = receiveBuff[28];
		else if (doorNO == 2)
			isPassed = receiveBuff[29];

		if (isPassed == 0 )
		{
			closesocket(Doorudp);
			return 1;
		}
	//}
	//	WSACleanup();   //clean up Ws2_32.dll   
	return 0;
}  


void Getallplate(void)
{
	string strsql;
	char* cErrMsg; 
	char **dbResult;
	int nRow,nColumn;

	strsql="select WYCPH from vehicleinfo;";
	int nRes = sqlite3_get_table(pdb,strsql.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg); 
	if (nRes != SQLITE_OK)
	{  
		cout<<"select fail: "<<cErrMsg<<endl;  
		return;
	}  
	Cardriver.allplate.number=min(nRow,100);   //司机人数
	for(int q=0;q<Cardriver.allplate.number;q++)    
	{
		if (dbResult[nColumn+q]!=NULL)
		{
			Cardriver.allplate.lpnumber[q]=U2G(dbResult[nColumn+q]);
		}
	}
}