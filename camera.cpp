//#include "stdafx.h"
#include <Windows.h>
#include <conio.h>
#include "dhnetsdk.h"
#include <stdio.h>
#include "dhplay.h"
#include <string>
#include "detailcontrol.h"
#include "definition.h"
//#define IP_PIn "172.16.0.102"
//#define IP_POut "172.16.0.103"
//#define IP_Driver "172.16.0.104"
//#define IP_Traffic "172.16.0.105"

extern People ICpp;
extern People WTpp;
extern Car Cardriver;
extern int pupdate;

unsigned long lLogin_peopleIn;
unsigned long lLogin_peopleOut;
unsigned long lLogin_driverIn;
unsigned long lLogin_trafficIn;
unsigned long lLogin_driverOut;
unsigned long lLogin_trafficOut;
FILE *pfile;
long lanalyHandle;

//设备断线回调
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	//printf("设备断线回调 \n");
	if (lLoginID == lLogin_trafficIn)
	{
		if(lanalyHandle !=0 )
		{
			bool bret = CLIENT_StopLoadPic(lanalyHandle);
			if(bret)
			{
				printf("关闭订阅句柄\n");
				lanalyHandle = 0;
			}
		}
	}
	if (lLoginID == lLogin_trafficOut)
	{
		if(lanalyHandle !=0 )
		{
			bool bret = CLIENT_StopLoadPic(lanalyHandle);
			if(bret)
			{
				printf("关闭订阅句柄\n");
				lanalyHandle = 0;
			}
		}
	}
	return;
}

//交通摄像头In 事件触发
int CALLBACK AnalyzerDataCallBackIn(LONG lAnalyzerHandle, DWORD dwAlarmType, void* pAlarmInfo, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser, int nSequence, void *reserved)
{
	string traffictime = Entiretime();	

	string path = "..\\snapImage\\" + traffictime + ".jpg";
	switch(dwAlarmType)
	{
	case EVENT_IVS_TRAFFICGATE:        //交通卡口事件
		{
			printf("**EVENT_IVS_TRAFFICGATE**\n");
			DEV_EVENT_TRAFFICGATE_INFO* pGateInfo = (DEV_EVENT_TRAFFICGATE_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pGateInfo->stuObject.szText,
				pGateInfo->stuFileInfo.nGroupId,
				pGateInfo->stuFileInfo.bIndex,
				pGateInfo->stuFileInfo.bCount);

			//车牌颜色、车辆类型
			printf("plate color = %d,plate type = %s\n",
				pGateInfo->stuObject.rgbaMainColor,
				pGateInfo->szVehicleType);

			printf("Year=%d,Month = %d ,day = %d\n",pGateInfo->UTC.dwYear,pGateInfo->UTC.dwMonth,pGateInfo->UTC.dwDay);

			Cardriver.plate = pGateInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "进门";
			Cardeal(Cardriver.plate);
			FILE* pFile = fopen(path.c_str(), "w+b");
			if (pFile)
			{
				fwrite(pBuffer, dwBufSize, 1, pFile);
				fclose(pFile);
			}
			Cardriver.flag_traffic=1;

		}
		break;

	case EVENT_IVS_TRAFFICJUNCTION:   //交通路口
		{
			printf("**EVENT_IVS_TRAFFICJUNCTION** \n");
			DEV_EVENT_TRAFFICJUNCTION_INFO* pJunInfo = (DEV_EVENT_TRAFFICJUNCTION_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pJunInfo->stuObject.szText,
				pJunInfo->stuFileInfo.nGroupId,
				pJunInfo->stuFileInfo.bIndex,
				pJunInfo->stuFileInfo.bCount);

			//车牌颜色、车牌类型
			printf("plate color = %s,plate type = %s\n",
				pJunInfo->stTrafficCar.szPlateColor,
				pJunInfo->stTrafficCar.szPlateType);

			printf("Year=%d,Month = %d ,day = %d\n",pJunInfo->UTC.dwYear,pJunInfo->UTC.dwMonth,pJunInfo->UTC.dwDay);

			Cardriver.plate = pJunInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "进门";
			Cardeal(Cardriver.plate);
			FILE* pFile = fopen(path.c_str(), "w+b");
			if (pFile)
			{
				fwrite(pBuffer, dwBufSize, 1, pFile);
				fclose(pFile);
			}
			Cardriver.flag_traffic=1;
		}
		break;
	case EVENT_IVS_TRAFFIC_MANUALSNAP://手动抓拍
		{
			printf("**EVENT_IVS_TRAFFIC_MANUALSNAP** \n");
			DEV_EVENT_TRAFFIC_MANUALSNAP_INFO* pManInfo = (DEV_EVENT_TRAFFIC_MANUALSNAP_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pManInfo->stuObject.szText,
				pManInfo->stuFileInfo.nGroupId,
				pManInfo->stuFileInfo.bIndex,
				pManInfo->stuFileInfo.bCount);

			printf("Year=%d,Month = %d ,day = %d\n",pManInfo->UTC.dwYear,pManInfo->UTC.dwMonth,pManInfo->UTC.dwDay);

			Cardriver.plate = pManInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "进门";
			Cardeal(Cardriver.plate);
			FILE* pFile = fopen(path.c_str(), "w+b");
			if (pFile)
			{
				fwrite(pBuffer, dwBufSize, 1, pFile);
				fclose(pFile);
			}
			Cardriver.flag_traffic=1;
		}
		break;

	default:
		{
			printf("*** 未解析事件dwAlarmType =  %d ****\n",dwAlarmType);
			break;
		}
	}

	return 1;
}

//交通摄像头Out 事件触发
int CALLBACK AnalyzerDataCallBackOut(LONG lAnalyzerHandle, DWORD dwAlarmType, void* pAlarmInfo, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser, int nSequence, void *reserved)
{
	string traffictime = Entiretime();	

	string path = "..\\snapImage\\" + traffictime + ".jpg";
	switch(dwAlarmType)
	{
	case EVENT_IVS_TRAFFICGATE:        //交通卡口事件
		{
			printf("**EVENT_IVS_TRAFFICGATE**\n");
			DEV_EVENT_TRAFFICGATE_INFO* pGateInfo = (DEV_EVENT_TRAFFICGATE_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pGateInfo->stuObject.szText,
				pGateInfo->stuFileInfo.nGroupId,
				pGateInfo->stuFileInfo.bIndex,
				pGateInfo->stuFileInfo.bCount);

			//车牌颜色、车辆类型
			printf("plate color = %d,plate type = %s\n",
				pGateInfo->stuObject.rgbaMainColor,
				pGateInfo->szVehicleType);

			printf("Year=%d,Month = %d ,day = %d\n",pGateInfo->UTC.dwYear,pGateInfo->UTC.dwMonth,pGateInfo->UTC.dwDay);

			Cardriver.plate = pGateInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "出门";
			Cardeal(Cardriver.plate);
			FILE* pFile = fopen(path.c_str(), "w+b");
			if (pFile)
			{
				fwrite(pBuffer, dwBufSize, 1, pFile);
				fclose(pFile);
			}
			Cardriver.flag_traffic=1;

		}
		break;

	case EVENT_IVS_TRAFFICJUNCTION:   //交通路口
		{
			printf("**EVENT_IVS_TRAFFICJUNCTION** \n");
			DEV_EVENT_TRAFFICJUNCTION_INFO* pJunInfo = (DEV_EVENT_TRAFFICJUNCTION_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pJunInfo->stuObject.szText,
				pJunInfo->stuFileInfo.nGroupId,
				pJunInfo->stuFileInfo.bIndex,
				pJunInfo->stuFileInfo.bCount);

			//车牌颜色、车牌类型
			printf("plate color = %s,plate type = %s\n",
				pJunInfo->stTrafficCar.szPlateColor,
				pJunInfo->stTrafficCar.szPlateType);

			printf("Year=%d,Month = %d ,day = %d\n",pJunInfo->UTC.dwYear,pJunInfo->UTC.dwMonth,pJunInfo->UTC.dwDay);

			Cardriver.plate = pJunInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "出门";
			Cardeal(Cardriver.plate);
			FILE* pFile = fopen(path.c_str(), "w+b");
			if (pFile)
			{
				fwrite(pBuffer, dwBufSize, 1, pFile);
				fclose(pFile);
			}
			Cardriver.flag_traffic=1;
		}
		break;
	case EVENT_IVS_TRAFFIC_MANUALSNAP://手动抓拍
		{
			printf("**EVENT_IVS_TRAFFIC_MANUALSNAP** \n");
			DEV_EVENT_TRAFFIC_MANUALSNAP_INFO* pManInfo = (DEV_EVENT_TRAFFIC_MANUALSNAP_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pManInfo->stuObject.szText,
				pManInfo->stuFileInfo.nGroupId,
				pManInfo->stuFileInfo.bIndex,
				pManInfo->stuFileInfo.bCount);

			printf("Year=%d,Month = %d ,day = %d\n",pManInfo->UTC.dwYear,pManInfo->UTC.dwMonth,pManInfo->UTC.dwDay);

			Cardriver.plate = pManInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "出门";
			Cardeal(Cardriver.plate);
			FILE* pFile = fopen(path.c_str(), "w+b");
			if (pFile)
			{
				fwrite(pBuffer, dwBufSize, 1, pFile);
				fclose(pFile);
			}
			Cardriver.flag_traffic=1;
		}
		break;

	default:
		{
			printf("*** 未解析事件dwAlarmType =  %d ****\n",dwAlarmType);
			break;
		}
	}

	return 1;
}

//重连
void CALLBACK RecConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	//printf("重连成功回调 \n");
	if (lLoginID == lLogin_trafficIn)
	{
		lanalyHandle = CLIENT_RealLoadPictureEx(lLoginID, 0, EVENT_IVS_ALL, TRUE, AnalyzerDataCallBackIn, 0, NULL);
		if(lanalyHandle!=0)
		{
			printf("重新订阅事件成功!\n");
		}
		else
		{
			printf("RealPictrureEx Fail!GetLastError()= %d",CLIENT_GetLastError());
		}
	}
	if (lLoginID == lLogin_trafficOut)
	{
		lanalyHandle = CLIENT_RealLoadPictureEx(lLoginID, 0, EVENT_IVS_ALL, TRUE, AnalyzerDataCallBackOut, 0, NULL);
		if(lanalyHandle!=0)
		{
			printf("重新订阅事件成功!\n");
		}
		else
		{
			printf("RealPictrureEx Fail!GetLastError()= %d",CLIENT_GetLastError());
		}
	}
	return;
}


//抓图回调函数
void CALLBACK SnapPicRet(LLONG ILoginID, BYTE *pBuf, UINT RevLen, UINT EncodeType, DWORD CmdSerial, LDWORD dwUser)
{
	string snaptime = Entiretime();
	printf("snapPic SaveTo d:\\test*.jpg; CmdSerial = %d \n",CmdSerial);	

	string kfile = "..\\snapImage\\" + snaptime + ".jpg";
	FILE *pFile = fopen(kfile.c_str(),"w+b");
	if(pFile)
	{
		fwrite(pBuf, RevLen, 1, pFile);
		fclose(pFile);
		if (ILoginID == lLogin_peopleIn)
		{
			ICpp.peopleinfo.pic = kfile;
			WTpp.peopleinfo.pic = kfile;
			ICpp.peopleinfo.spic = snaptime+".jpg";
			WTpp.peopleinfo.spic = snaptime+".jpg";
			WTpp.flag_snap = 1;
		} 
		else if (ILoginID == lLogin_peopleOut)
		{
			ICpp.peopleinfo.pic = kfile;
			WTpp.peopleinfo.pic = kfile;
			ICpp.peopleinfo.spic = snaptime+".jpg";
			WTpp.peopleinfo.spic = snaptime+".jpg";
			WTpp.flag_snap = 1;
		}
		else if (ILoginID == lLogin_driverIn)
		{
			Cardriver.vehicle.picofdri = kfile;
			Cardriver.vehicle.spicofdri = snaptime+".jpg";
			Cardriver.flagdri_snap = 1;
			Cardriver.vehicle.inout="进门";
		}
		else if (ILoginID == lLogin_driverOut)
		{
			//////////////////////////////////////////////////////////内容待修改
			Cardriver.vehicle.picofdri = kfile;
			Cardriver.vehicle.spicofdri = snaptime+".jpg";
			Cardriver.flagdri_snap = 1;
			Cardriver.vehicle.inout="出门";
		}
		if (pupdate == 1)
		{
			WTpp.precordsave();
			pupdate = 0;
		}
		else
		{
			ICpp.precordsave();
		}
	}
}

//摄像头初始化
void cameraIni(void)
{
	bool binit = CLIENT_Init(DisConnectFunc,0);//初始化sdk，设置断线回调函数,应用程序只需要启动时调用一次
	CLIENT_SetAutoReconnect(RecConnectFunc,0);//设备离线重连成功回调
	if(binit)
	{
		CLIENT_SetSnapRevCallBack(SnapPicRet,0); //第二次参数可以输入自定义的值，然后会在回调里dwUser参数返回
	}

	////登录设备
	//printf("请输入设备IP:\n");
	//char sIP[255] = {0};
	//scanf("%s",sIP);
	NET_PARAM stuNetParam = {0};
	stuNetParam.nPicBufSize = 4 * 1024 * 1024;
	CLIENT_SetNetworkParam(&stuNetParam);


	NET_DEVICEINFO deviceInfo0 ={0};
	//unsigned long lLogin;
	lLogin_peopleIn = CLIENT_Login(IP_PIn,37777,"admin","admin",&deviceInfo0);//登陆设备，用户需另选设备。
	if(lLogin_peopleIn == 0)
	{
		// login failed!!
	}

	NET_DEVICEINFO deviceInfo1 ={0};
	//unsigned long lLogin;
	lLogin_peopleOut = CLIENT_Login(IP_POut,37777,"admin","admin",&deviceInfo1);//登陆设备，用户需另选设备。
	if(lLogin_peopleOut == 0)
	{
		// login failed!!
	}

	//车辆进门摄像头和司机拍照In
	NET_DEVICEINFO deviceInfo2 ={0};
	//unsigned long lLogin;
	lLogin_driverIn = CLIENT_Login(IP_DriverIn,37777,"admin","admin",&deviceInfo2);//登陆设备，用户需另选设备。
	if(lLogin_driverIn == 0)
	{
		// login failed!!
	}

	NET_DEVICEINFO deviceInfo3 ={0};
	//unsigned long lLogin;
	lLogin_trafficIn = CLIENT_Login(IP_TrafficIn,37777,"admin","admin",&deviceInfo3);//登陆设备，用户需另选设备。
	if(lLogin_trafficIn == 0)
	{
		// login failed!!
	}
	//订阅事件图片消息
	lanalyHandle = CLIENT_RealLoadPictureEx(lLogin_trafficIn, 0, EVENT_IVS_ALL, TRUE, AnalyzerDataCallBackIn, 0, NULL);
	if(lanalyHandle == 0) //==0 订阅消息失败，!=0 成功
	{
		printf("RealPictrureEx Fail!GetLastError()= %d",CLIENT_GetLastError());
	}
	else
	{
	}

	//车辆进门摄像头和司机拍照Out
	NET_DEVICEINFO deviceInfo4 ={0};
	//unsigned long lLogin;
	lLogin_driverOut = CLIENT_Login(IP_DriverOut,37777,"admin","admin",&deviceInfo4);//登陆设备，用户需另选设备。
	if(lLogin_driverOut == 0)
	{
		// login failed!!
	}

	NET_DEVICEINFO deviceInfo5 ={0};
	//unsigned long lLogin;
	lLogin_trafficOut = CLIENT_Login(IP_TrafficOut,37777,"admin","admin",&deviceInfo5);//登陆设备，用户需另选设备。
	if(lLogin_trafficOut == 0)
	{
		// login failed!!
	}
	//订阅事件图片消息
	lanalyHandle = CLIENT_RealLoadPictureEx(lLogin_trafficOut, 0, EVENT_IVS_ALL, TRUE, AnalyzerDataCallBackOut, 0, NULL);
	if(lanalyHandle == 0) //==0 订阅消息失败，!=0 成功
	{
		printf("RealPictrureEx Fail!GetLastError()= %d",CLIENT_GetLastError());
	}
	else
	{
	}
}

//摄像头注销
void cameraRel(void)
{
	CLIENT_Logout(lLogin_peopleIn);
	CLIENT_Logout(lLogin_peopleOut);
	CLIENT_Logout(lLogin_driverIn);
	CLIENT_Logout(lLogin_driverOut);

	//停止图片事件订阅 对应CLIENT_RealLoadPictureEx
	if(lanalyHandle!=0)
	{
		CLIENT_StopLoadPic(lanalyHandle);
		printf("停止订阅图片\n");
	}

	//登出设备，销毁会话句柄 CLIENT_Logout跟CLIENT_Login 对应
	if(lLogin_trafficIn!=0)
	{
		CLIENT_Logout(lLogin_trafficIn);
		printf("dev logout \n");
	}
	if(lLogin_trafficOut!=0)
	{
		CLIENT_Logout(lLogin_trafficOut);
		printf("dev logout \n");
	}
	CLIENT_Cleanup();
}

//抓拍照片
void snap(int inputIp)
{
	SNAP_PARAMS snapparams = {0};
	snapparams.Channel = 0; //设备第1通道
	snapparams.mode = 0;    //一次抓一张
	snapparams.CmdSerial = 89; // 0~255,用于回调返回时跟命令对应
	unsigned long ILoginID;
	switch (inputIp)
	{
	case 0: //people in
		ILoginID = lLogin_peopleIn;
		break;
	case 1: //people out
		ILoginID = lLogin_peopleOut;
		break;
	case 2: //driver in
		ILoginID = lLogin_driverIn;
		break;
	case 3: //driver out
		ILoginID = lLogin_driverOut;
		break;
	default:
		//error
		return;
		break;
	}
	BOOL b = CLIENT_SnapPicture(ILoginID, snapparams);
	if (!b)
	{
		printf("snap one failed!\n");
	}
	else
	{
		printf("Snap Succed!\n");
	}
	return;
}

