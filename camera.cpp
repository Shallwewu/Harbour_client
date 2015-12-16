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

//�豸���߻ص�
void CALLBACK DisConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	//printf("�豸���߻ص� \n");
	if (lLoginID == lLogin_trafficIn)
	{
		if(lanalyHandle !=0 )
		{
			bool bret = CLIENT_StopLoadPic(lanalyHandle);
			if(bret)
			{
				printf("�رն��ľ��\n");
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
				printf("�رն��ľ��\n");
				lanalyHandle = 0;
			}
		}
	}
	return;
}

//��ͨ����ͷIn �¼�����
int CALLBACK AnalyzerDataCallBackIn(LONG lAnalyzerHandle, DWORD dwAlarmType, void* pAlarmInfo, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser, int nSequence, void *reserved)
{
	string traffictime = Entiretime();	

	string path = "..\\snapImage\\" + traffictime + ".jpg";
	switch(dwAlarmType)
	{
	case EVENT_IVS_TRAFFICGATE:        //��ͨ�����¼�
		{
			printf("**EVENT_IVS_TRAFFICGATE**\n");
			DEV_EVENT_TRAFFICGATE_INFO* pGateInfo = (DEV_EVENT_TRAFFICGATE_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pGateInfo->stuObject.szText,
				pGateInfo->stuFileInfo.nGroupId,
				pGateInfo->stuFileInfo.bIndex,
				pGateInfo->stuFileInfo.bCount);

			//������ɫ����������
			printf("plate color = %d,plate type = %s\n",
				pGateInfo->stuObject.rgbaMainColor,
				pGateInfo->szVehicleType);

			printf("Year=%d,Month = %d ,day = %d\n",pGateInfo->UTC.dwYear,pGateInfo->UTC.dwMonth,pGateInfo->UTC.dwDay);

			Cardriver.plate = pGateInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "����";
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

	case EVENT_IVS_TRAFFICJUNCTION:   //��ͨ·��
		{
			printf("**EVENT_IVS_TRAFFICJUNCTION** \n");
			DEV_EVENT_TRAFFICJUNCTION_INFO* pJunInfo = (DEV_EVENT_TRAFFICJUNCTION_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pJunInfo->stuObject.szText,
				pJunInfo->stuFileInfo.nGroupId,
				pJunInfo->stuFileInfo.bIndex,
				pJunInfo->stuFileInfo.bCount);

			//������ɫ����������
			printf("plate color = %s,plate type = %s\n",
				pJunInfo->stTrafficCar.szPlateColor,
				pJunInfo->stTrafficCar.szPlateType);

			printf("Year=%d,Month = %d ,day = %d\n",pJunInfo->UTC.dwYear,pJunInfo->UTC.dwMonth,pJunInfo->UTC.dwDay);

			Cardriver.plate = pJunInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "����";
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
	case EVENT_IVS_TRAFFIC_MANUALSNAP://�ֶ�ץ��
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
			Cardriver.vehicle.inout = "����";
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
			printf("*** δ�����¼�dwAlarmType =  %d ****\n",dwAlarmType);
			break;
		}
	}

	return 1;
}

//��ͨ����ͷOut �¼�����
int CALLBACK AnalyzerDataCallBackOut(LONG lAnalyzerHandle, DWORD dwAlarmType, void* pAlarmInfo, BYTE *pBuffer, DWORD dwBufSize, DWORD dwUser, int nSequence, void *reserved)
{
	string traffictime = Entiretime();	

	string path = "..\\snapImage\\" + traffictime + ".jpg";
	switch(dwAlarmType)
	{
	case EVENT_IVS_TRAFFICGATE:        //��ͨ�����¼�
		{
			printf("**EVENT_IVS_TRAFFICGATE**\n");
			DEV_EVENT_TRAFFICGATE_INFO* pGateInfo = (DEV_EVENT_TRAFFICGATE_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pGateInfo->stuObject.szText,
				pGateInfo->stuFileInfo.nGroupId,
				pGateInfo->stuFileInfo.bIndex,
				pGateInfo->stuFileInfo.bCount);

			//������ɫ����������
			printf("plate color = %d,plate type = %s\n",
				pGateInfo->stuObject.rgbaMainColor,
				pGateInfo->szVehicleType);

			printf("Year=%d,Month = %d ,day = %d\n",pGateInfo->UTC.dwYear,pGateInfo->UTC.dwMonth,pGateInfo->UTC.dwDay);

			Cardriver.plate = pGateInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "����";
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

	case EVENT_IVS_TRAFFICJUNCTION:   //��ͨ·��
		{
			printf("**EVENT_IVS_TRAFFICJUNCTION** \n");
			DEV_EVENT_TRAFFICJUNCTION_INFO* pJunInfo = (DEV_EVENT_TRAFFICJUNCTION_INFO*)pAlarmInfo;
			printf("lAnalyzerHandle=%d,plateNumber=%s,nGroupId=%d,bIndex=%d,bCount=%d\n",lAnalyzerHandle,
				pJunInfo->stuObject.szText,
				pJunInfo->stuFileInfo.nGroupId,
				pJunInfo->stuFileInfo.bIndex,
				pJunInfo->stuFileInfo.bCount);

			//������ɫ����������
			printf("plate color = %s,plate type = %s\n",
				pJunInfo->stTrafficCar.szPlateColor,
				pJunInfo->stTrafficCar.szPlateType);

			printf("Year=%d,Month = %d ,day = %d\n",pJunInfo->UTC.dwYear,pJunInfo->UTC.dwMonth,pJunInfo->UTC.dwDay);

			Cardriver.plate = pJunInfo->stuObject.szText;
			Cardriver.vehicle.picofcar = path;
			Cardriver.vehicle.spicofcar = traffictime+".jpg";
			Cardriver.vehicle.inout = "����";
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
	case EVENT_IVS_TRAFFIC_MANUALSNAP://�ֶ�ץ��
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
			Cardriver.vehicle.inout = "����";
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
			printf("*** δ�����¼�dwAlarmType =  %d ****\n",dwAlarmType);
			break;
		}
	}

	return 1;
}

//����
void CALLBACK RecConnectFunc(LONG lLoginID, char *pchDVRIP, LONG nDVRPort, DWORD dwUser)
{
	//printf("�����ɹ��ص� \n");
	if (lLoginID == lLogin_trafficIn)
	{
		lanalyHandle = CLIENT_RealLoadPictureEx(lLoginID, 0, EVENT_IVS_ALL, TRUE, AnalyzerDataCallBackIn, 0, NULL);
		if(lanalyHandle!=0)
		{
			printf("���¶����¼��ɹ�!\n");
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
			printf("���¶����¼��ɹ�!\n");
		}
		else
		{
			printf("RealPictrureEx Fail!GetLastError()= %d",CLIENT_GetLastError());
		}
	}
	return;
}


//ץͼ�ص�����
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
			Cardriver.vehicle.inout="����";
		}
		else if (ILoginID == lLogin_driverOut)
		{
			//////////////////////////////////////////////////////////���ݴ��޸�
			Cardriver.vehicle.picofdri = kfile;
			Cardriver.vehicle.spicofdri = snaptime+".jpg";
			Cardriver.flagdri_snap = 1;
			Cardriver.vehicle.inout="����";
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

//����ͷ��ʼ��
void cameraIni(void)
{
	bool binit = CLIENT_Init(DisConnectFunc,0);//��ʼ��sdk�����ö��߻ص�����,Ӧ�ó���ֻ��Ҫ����ʱ����һ��
	CLIENT_SetAutoReconnect(RecConnectFunc,0);//�豸���������ɹ��ص�
	if(binit)
	{
		CLIENT_SetSnapRevCallBack(SnapPicRet,0); //�ڶ��β������������Զ����ֵ��Ȼ����ڻص���dwUser��������
	}

	////��¼�豸
	//printf("�������豸IP:\n");
	//char sIP[255] = {0};
	//scanf("%s",sIP);
	NET_PARAM stuNetParam = {0};
	stuNetParam.nPicBufSize = 4 * 1024 * 1024;
	CLIENT_SetNetworkParam(&stuNetParam);


	NET_DEVICEINFO deviceInfo0 ={0};
	//unsigned long lLogin;
	lLogin_peopleIn = CLIENT_Login(IP_PIn,37777,"admin","admin",&deviceInfo0);//��½�豸���û�����ѡ�豸��
	if(lLogin_peopleIn == 0)
	{
		// login failed!!
	}

	NET_DEVICEINFO deviceInfo1 ={0};
	//unsigned long lLogin;
	lLogin_peopleOut = CLIENT_Login(IP_POut,37777,"admin","admin",&deviceInfo1);//��½�豸���û�����ѡ�豸��
	if(lLogin_peopleOut == 0)
	{
		// login failed!!
	}

	//������������ͷ��˾������In
	NET_DEVICEINFO deviceInfo2 ={0};
	//unsigned long lLogin;
	lLogin_driverIn = CLIENT_Login(IP_DriverIn,37777,"admin","admin",&deviceInfo2);//��½�豸���û�����ѡ�豸��
	if(lLogin_driverIn == 0)
	{
		// login failed!!
	}

	NET_DEVICEINFO deviceInfo3 ={0};
	//unsigned long lLogin;
	lLogin_trafficIn = CLIENT_Login(IP_TrafficIn,37777,"admin","admin",&deviceInfo3);//��½�豸���û�����ѡ�豸��
	if(lLogin_trafficIn == 0)
	{
		// login failed!!
	}
	//�����¼�ͼƬ��Ϣ
	lanalyHandle = CLIENT_RealLoadPictureEx(lLogin_trafficIn, 0, EVENT_IVS_ALL, TRUE, AnalyzerDataCallBackIn, 0, NULL);
	if(lanalyHandle == 0) //==0 ������Ϣʧ�ܣ�!=0 �ɹ�
	{
		printf("RealPictrureEx Fail!GetLastError()= %d",CLIENT_GetLastError());
	}
	else
	{
	}

	//������������ͷ��˾������Out
	NET_DEVICEINFO deviceInfo4 ={0};
	//unsigned long lLogin;
	lLogin_driverOut = CLIENT_Login(IP_DriverOut,37777,"admin","admin",&deviceInfo4);//��½�豸���û�����ѡ�豸��
	if(lLogin_driverOut == 0)
	{
		// login failed!!
	}

	NET_DEVICEINFO deviceInfo5 ={0};
	//unsigned long lLogin;
	lLogin_trafficOut = CLIENT_Login(IP_TrafficOut,37777,"admin","admin",&deviceInfo5);//��½�豸���û�����ѡ�豸��
	if(lLogin_trafficOut == 0)
	{
		// login failed!!
	}
	//�����¼�ͼƬ��Ϣ
	lanalyHandle = CLIENT_RealLoadPictureEx(lLogin_trafficOut, 0, EVENT_IVS_ALL, TRUE, AnalyzerDataCallBackOut, 0, NULL);
	if(lanalyHandle == 0) //==0 ������Ϣʧ�ܣ�!=0 �ɹ�
	{
		printf("RealPictrureEx Fail!GetLastError()= %d",CLIENT_GetLastError());
	}
	else
	{
	}
}

//����ͷע��
void cameraRel(void)
{
	CLIENT_Logout(lLogin_peopleIn);
	CLIENT_Logout(lLogin_peopleOut);
	CLIENT_Logout(lLogin_driverIn);
	CLIENT_Logout(lLogin_driverOut);

	//ֹͣͼƬ�¼����� ��ӦCLIENT_RealLoadPictureEx
	if(lanalyHandle!=0)
	{
		CLIENT_StopLoadPic(lanalyHandle);
		printf("ֹͣ����ͼƬ\n");
	}

	//�ǳ��豸�����ٻỰ��� CLIENT_Logout��CLIENT_Login ��Ӧ
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

//ץ����Ƭ
void snap(int inputIp)
{
	SNAP_PARAMS snapparams = {0};
	snapparams.Channel = 0; //�豸��1ͨ��
	snapparams.mode = 0;    //һ��ץһ��
	snapparams.CmdSerial = 89; // 0~255,���ڻص�����ʱ�������Ӧ
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

