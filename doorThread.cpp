#include <stdio.h>
#define _AFXDLL
#include <afxwin.h>
//#include <winsock2.h>
//#include <windows.h>
#include <iostream>
#include "detailcontrol.h"
//using namespace std;
#include <time.h>
#include "definition.h"
bool isOpen;	//global variable, need init every file it used

int toOpen_udp=0;		//0-for no open, 1-for open in door, 2-for open out door
int doorFlag_udp=0;	//0-for nothing, 1-for WT in, 2-for WT out, 3-for IC in, 4-for IC out

//#define UDPADDRESS "172.16.0.106"
//#define UDPPORT 60000
//extern unsigned int iDevSn;  //¿ØÖÆÆ÷S/N

DWORD WINAPI doorControlThread(LPVOID pM)
{
	unsigned int doorNo = 1;

	//socket UDP³õÊ¼»¯ 
#pragma region udp_socket_init

	SOCKET udp;  
	SOCKADDR_IN dooraddr; 
	memset(&dooraddr, 0, sizeof(dooraddr));  
	if((udp = socket(AF_INET,SOCK_DGRAM,0)) <= 0)  
	{  
		printf("Create socket fail!\n");  
		return 0;  
	} 
	dooraddr.sin_family = AF_INET;  
	dooraddr.sin_addr.s_addr = inet_addr(UDPADDRESS);
	dooraddr.sin_port = htons(UDPPORT);
	bind(udp,(struct sockaddr *)&dooraddr,sizeof(dooraddr));

	int nNetTimeout=300;//1Ãë
	setsockopt(udp, SOL_SOCKET,SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

	//open door message
	char buff_open[64];
	memset(buff_open,0,sizeof(buff_open));
	buff_open[0] = 0x19;
	buff_open[1] = 0x40;
	memcpy(&(buff_open[4]),&(iDevSn), 4);
	buff_open[8] = (doorNo & 0xff);	//choose the door No in the open step

	//ask condition message
	char buff_condition[64];
	memset(buff_condition,0,sizeof(buff_condition));
	buff_condition[0] = 0x19;
	buff_condition[1] = 0x20;
	memcpy(&(buff_condition[4]),&(iDevSn), 4);

	//receive message
	char isPassed = 'n';
	int len = sizeof(SOCKADDR);
	int nRet = 0;
	char receiveBuff[100];
	memset(receiveBuff,'\0',sizeof(receiveBuff));

	//variable init
	isOpen = false;
	toOpen_udp = 0;

	bool toAskConditon = false;
	time_t timeBegin = time(NULL);
	time_t timeEnd = time(NULL);

#pragma endregion

	//run udp
	while (true)
	{
		if (toOpen_udp == 1)
		{
			//send open in door message
			doorNo = 1;	//choose the door No in the open step	/////////////////!!!!!!!!!!!!!11
			buff_open[8] = (doorNo & 0xff);
			sendto(udp, buff_open, sizeof(buff_open), 0, (struct sockaddr *)&dooraddr, sizeof(dooraddr)); 
			//receive the Ack
			memset(receiveBuff,'\0',sizeof(receiveBuff));
			nRet = recvfrom(udp, receiveBuff, sizeof(receiveBuff), 0, (struct sockaddr *)&dooraddr, &len);
			if (nRet == 64 && receiveBuff[8])
			{
				toOpen_udp = 0;
				toAskConditon = true;
				timeBegin = time(NULL);
			}
		}
		else if (toOpen_udp == 2)
		{
			//send open out door message
			doorNo = 2;	//choose the door No in the open step	///////////////////////!!!!!!!!!!!!!!!!
			buff_open[8] = (doorNo & 0xff);
			sendto(udp, buff_open, sizeof(buff_open), 0, (struct sockaddr *)&dooraddr, sizeof(dooraddr)); 
			//receive the Ack
			memset(receiveBuff,'\0',sizeof(receiveBuff));
			nRet = recvfrom(udp, receiveBuff, sizeof(receiveBuff), 0, (struct sockaddr *)&dooraddr, &len);
			if (nRet == 64 && receiveBuff[8])
			{
				toOpen_udp = 0;
				toAskConditon = true;
				timeBegin = time(NULL);
			}
		}
		else if (toOpen_udp == 0 && toAskConditon)
		{
			timeEnd = time(NULL);
			if ((timeEnd - timeBegin) > 5)
			{
				toAskConditon = false;
			}
			//send ask condition message
			sendto(udp, buff_condition, sizeof(buff_condition), 0, (struct sockaddr *)&dooraddr, sizeof(dooraddr)); 
			//receive the Ack
			memset(receiveBuff,'\0',sizeof(receiveBuff));
			nRet = recvfrom(udp, receiveBuff, sizeof(receiveBuff), 0, (struct sockaddr *)&dooraddr, &len);
			if (nRet == 64 && ( receiveBuff[28]==0 || receiveBuff[29]==0 ))		//if Ack is right and the massage is right
			{
				//isOpen = true;
				toAskConditon = false;
				switch (doorFlag_udp)
				{
				case 1:
					//snapandsave
					ICsnapsave(1);
					doorFlag_udp = 0;
					break;
				case 2:
					//snapandsave
					ICsnapsave(2);
					doorFlag_udp = 0;
					break;
				case 3:
					//snapandsave
					snapandsave(1);
					doorFlag_udp = 0;
					break;
				case 4:
					//snapandsave
					snapandsave(2);
					doorFlag_udp = 0;
					break;
				default:
					doorFlag_udp = 0;
				}
			}
		}
		Sleep(100);
	}
	closesocket(udp);
	return 0; 
}