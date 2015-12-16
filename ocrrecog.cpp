#include <stdio.h>
#define _AFXDLL
#include <afxwin.h>
//#include <windows.h>
#include "detailcontrol.h"
#include <iostream>
using namespace std;

//by Damon 20150725
#include "IDcardrecog.h"
#include "string"
#include <vector>
using std::vector;
#define  ID_PASSPORT_READER_TIMER 1001

extern People WTpp;
extern int pupdate;
int flag_ocrUpdate = 0;
int typenum=0;

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

DWORD WINAPI OCRthread(LPVOID pM)  
{  
	int nFieldNum;
	CString strUserID =_T("67665668228251036749");

	HMODULE m_hIDCard = LoadLibrary(_T("IDCard"));	

	//get the pointers of the function
	fpInitIDCard = (int (WINAPI *)(LPCTSTR, int ,LPCTSTR))GetProcAddress(m_hIDCard, "InitIDCard");
	fpFreeIDCard = (void (WINAPI *)())GetProcAddress(m_hIDCard, "FreeIDCard");
	fpSetAcquireImageType = (bool (WINAPI*)(int, int))GetProcAddress(m_hIDCard, "SetAcquireImageType");

	fpSetUserDefinedImageSize = (bool (WINAPI *)(int, int))GetProcAddress(m_hIDCard, "SetUserDefinedImageSize");
	fpSetAcquireImageResolution = (bool (WINAPI *)(int, int))GetProcAddress(m_hIDCard, "SetAcquireImageResolution");
	fpAcquireImage = (int (WINAPI *)(int))GetProcAddress(m_hIDCard, "AcquireImage");

	fpSaveImage = (int (WINAPI*)(LPCTSTR))GetProcAddress(m_hIDCard, "SaveImage");
	fpRecogIDCard = (int (WINAPI *)())GetProcAddress(m_hIDCard, "RecogIDCard");

	fpCheckDeviceOnline = (bool (WINAPI *)())GetProcAddress(m_hIDCard, "CheckDeviceOnline");
	fpGetRecogResult = (int (WINAPI *)(int, LPTSTR, int&))GetProcAddress(m_hIDCard, "GetRecogResult");
	fpGetFieldName = (int (WINAPI *)(int, LPTSTR, int&))GetProcAddress(m_hIDCard, "GetFieldName");

	fpGetVersionInfo = (void (WINAPI *)(LPCTSTR, int))GetProcAddress(m_hIDCard, "GetVersionInfo");
	fpGetCurrentDevice = (BOOL (WINAPI *)(LPCTSTR, int))GetProcAddress(m_hIDCard, "GetCurrentDevice");
	fpSaveHeadImage = (int (WINAPI *)(LPCTSTR ))GetProcAddress(m_hIDCard, "SaveHeadImage");

	fpGetButtonDownType  =( int (WINAPI *)())GetProcAddress(m_hIDCard, "GetButtonDownType");
	fpGetSubId = (int (WINAPI *)())GetProcAddress(m_hIDCard, "GetSubID");

	fpSetIDCardID = (int  (WINAPI *)(int ,int *,int ))GetProcAddress(m_hIDCard, "SetIDCardID");
	fpAddIDCardID= (int (WINAPI *)(int ,int *,int ))GetProcAddress(m_hIDCard, "AddIDCardID");
	fpRecogIDCardEx = (int (WINAPI *)(int,int))GetProcAddress(m_hIDCard, "RecogIDCardEX");
	fpGetSubId = (int (WINAPI *)())GetProcAddress(m_hIDCard, "GetSubID");
	//////////////////
	pGetGrabSignalType=(int(WINAPI *)())GetProcAddress(m_hIDCard,"GetGrabSignalType");
	pSetIDCardID = (int (WINAPI *)(int ,int *,int ))GetProcAddress(m_hIDCard, "SetIDCardID");	
	pAddIDCardID = (int (WINAPI *)(int ,int *,int ))GetProcAddress(m_hIDCard, "AddIDCardID");
	pSetSpecialAttribute=(int (WINAPI *) (int ,int ))GetProcAddress(m_hIDCard,"SetSpecialAttribute");
	pGetDeviceSN = (int(WINAPI *)(LPCTSTR,int))GetProcAddress(m_hIDCard,"GetDeviceSN");
	pSetIOStatus = (int(WINAPI *)(int,bool))GetProcAddress(m_hIDCard,"SetIOStatus");
	pQRCodeRecog = (int (WINAPI*)(LPTSTR pResult,int& nLen))GetProcAddress(m_hIDCard,"QRCodeRecog");
	//fail to get the output functions
	pRecogBusinessCard =(int (WINAPI *)(int nCardType))GetProcAddress(m_hIDCard,"RecogBusinessCard");
	pGetBusinessCardResult=(int (WINAPI *)(int nID,int nIndex,LPWSTR lpBuffer,int &nBufferLen))GetProcAddress(m_hIDCard,"GetBusinessCardResult");
	pGetBusinessCardPosition=(int (WINAPI *)(int nID,int nIndex,int &nLeft,int &nRight,int &nTop, int &nBottom))GetProcAddress(m_hIDCard,"GetBusinessCardPosition");
	pGetBusinessCardFieldName=(int (WINAPI * )(int nID,LPWSTR lpBuffer,int &nBufferLen))GetProcAddress(m_hIDCard,"GetBusinessCardFieldName");
	pGetBusinessCardResultCount=(int (WINAPI *)(int nID))GetProcAddress(m_hIDCard,"GetBusinessCardResultCount");
	pRecogBusinessImage=(int (WINAPI *)(LPCWSTR lpImageFileName,int nCardType))GetProcAddress(m_hIDCard,"RecogBusinessImage");

	if( fpInitIDCard == NULL 
		|| fpFreeIDCard == NULL
		|| fpSetAcquireImageType == NULL
		|| fpSetUserDefinedImageSize == NULL
		|| fpSetAcquireImageResolution == NULL
		|| fpAcquireImage == NULL
		|| fpSaveImage == NULL
		|| fpSaveHeadImage == NULL
		|| fpRecogIDCard == NULL
		|| fpCheckDeviceOnline == NULL
		|| fpGetRecogResult == NULL
		|| fpGetFieldName == NULL
		|| fpGetCurrentDevice == NULL
		|| fpGetVersionInfo == NULL
		|| fpGetSubId == NULL
		|| fpGetButtonDownType == NULL
		|| fpSetIDCardID == NULL
		|| fpAddIDCardID == NULL
		|| fpRecogIDCardEx == NULL
		|| fpGetSubId == NULL
		|| pGetGrabSignalType==NULL
		|| pSetIDCardID==NULL
		|| pAddIDCardID==NULL
		|| pSetSpecialAttribute==NULL
		|| pGetDeviceSN==NULL)
	{
		::FreeLibrary(m_hIDCard);
		m_hIDCard = NULL;
		return FALSE;
	}

	int nRet = fpInitIDCard(strUserID, 1,NULL);	
	if (nRet == 0)
	{
		//cout << "Init succeed" << endl;   ///////////////need modification
	}
	else
	{
		//cout<<"Init failed"<<endl;   ///////////////need modification
		return 0;
	}
	pSetSpecialAttribute(1,1);

	while(1)
	{
		Sleep(200);
		if (pGetGrabSignalType()==1)
		{
			int nSubID=0;		

			nRet = pAddIDCardID(1,&nSubID,1);
			pAddIDCardID(2,&nSubID,1);
			pAddIDCardID(3,&nSubID,1);
			pAddIDCardID(4,&nSubID,1);
			pAddIDCardID(5,&nSubID,1);
			pAddIDCardID(6,&nSubID,1);
			pAddIDCardID(7,&nSubID,1);
			pAddIDCardID(9,&nSubID,1);
			pAddIDCardID(10,&nSubID,1);
			pAddIDCardID(11,&nSubID,1);
			pAddIDCardID(12,&nSubID,1);
			pAddIDCardID(13,&nSubID,1);
			pAddIDCardID(14,&nSubID,1);
			pAddIDCardID(15,&nSubID,1);
			pAddIDCardID(16,&nSubID,1);
			pAddIDCardID(1000,&nSubID,1);
			pAddIDCardID(1001,&nSubID,1);
			pAddIDCardID(1003,&nSubID,1);
			pAddIDCardID(1004,&nSubID,1);
			pAddIDCardID(1005,&nSubID,1);
			pAddIDCardID(1107,&nSubID,1);
			pAddIDCardID(1008,&nSubID,1);
			pAddIDCardID(1009,&nSubID,1);
			pAddIDCardID(1010,&nSubID,1);	

			if (nRet == 0)
			{
				//cout << "pAddIDCardID succeed" << endl;   ///////////////need modification
			}
			else
			{
				//cout<<"pAddIDCardID failed"<<endl;   ///////////////need modification
				return 0;
			}

			int nSizeType = 21;

			nRet = fpAcquireImage(nSizeType);

			if (nRet == 0)
			{
				//cout << "fpAcquireImage succeed" << endl;   ///////////////need modification
			}
			else
			{
				//cout<<"fpAcquireImage failed"<<endl;   ///////////////need modification
				return 0;
			}
			TCHAR qresult[1000];
			memset(qresult, '\0',sizeof(qresult));
			int k;
			int length;
			k=pQRCodeRecog(qresult,length);
			if(k==0)
			{
				//the recognize result are stored in qresult(content) and length
				//analyse the content
				//cout<<"二维码识别成功！"<<endl;   ///////////////need modification

				//数据格式转换
				WTpp.peopleclear();  //人员信息清除
				pupdate=1;
				CString valueCstring = CString(qresult);
				int len = valueCstring.GetLength();   
				char* chRtn = (char*)malloc((len*2+1)*sizeof(char));//CString的长度中汉字算一个长度   
				memset(chRtn, 0, 2*len+1);  
				USES_CONVERSION;   
				strcpy((LPSTR)chRtn,OLE2A(valueCstring.LockBuffer()));  
				string szString = chRtn;

				//根据^的数量判断卡的类别
				int cardType = 0;
				for (int i = 0; i < 2*len+1; i++)
				{
					if (chRtn[i] == '^')
					{
						cardType++;
					}
				}
				//cout << cardType << endl;   ///////////////need modification
				//cout << szString << endl;   ///////////////need modification

				//解析内容
				string typeString="";
				string name;
				string number;
				string ship;
				char buffer1[1000];
				char buffer2[1000];
				string::size_type position;
				int l_string = 0;
				int count = 0;
				switch (cardType)
				{
				case 6:
					typeString ="台湾船员登陆证";    
					WTpp.peopleinfo.type = typeString;
					typenum=6;
					//cout << typeString << endl;   ///////////////need modification
					while(TRUE)
					{
						position = szString.find("^");
						if (position == szString.npos)
							break;
						count++;
						l_string = szString.copy(buffer1,position);
						buffer1[l_string] = '\0';
						l_string = szString.copy(buffer2,szString.length()-position,position+1);
						buffer2[l_string] = '\0';
						szString = buffer2;
						if (count == 1)
						{
							number = buffer1;
							WTpp.peopleinfo.number = buffer1;
							//cout << number << endl;   ///////////////need modification
						}
						else if (count == 2)
						{
							name = buffer1;
							WTpp.peopleinfo.name = buffer1;
							//cout << name << endl;   ///////////////need modification
						}
						else if (count == 5)
						{
							ship = buffer1;
							WTpp.peopleinfo.ship = ship;
							//cout << ship << endl;   ///////////////need modification
							break;
						}
					}
					break;
				case 7:
					typeString = "临时入境许可";		
					WTpp.peopleinfo.type = typeString;
					typenum=7;
					//cout << typeString << endl;   ///////////////need modification
					while(TRUE)
					{
						position = szString.find("^");
						if (position == szString.npos)
							break;
						count++;
						l_string = szString.copy(buffer1,position);
						buffer1[l_string] = '\0';
						l_string = szString.copy(buffer2,szString.length()-position,position+1);
						buffer2[l_string] = '\0';
						szString = buffer2;
						//cout << buffer1 << endl;
						//cout << buffer2 << endl;
						if (count == 1)
						{
							name = buffer1;
							WTpp.peopleinfo.name = name;
							//cout << name << endl;   ///////////////need modification
						}
						else if (count == 5)
						{
							number = buffer1;
							WTpp.peopleinfo.number = number;
							//cout << number << endl;   ///////////////need modification
						}
						else if (count == 7)
						{
							ship = buffer1;
							WTpp.peopleinfo.ship = ship;
							//cout << ship << endl;   ///////////////need modification
							break;
						}
					}
					break;
				case 8:
					typeString = "临时登轮证";
					WTpp.peopleinfo.type = typeString;
					typenum=8;
					cout << typeString << endl;
					while(TRUE)
					{
						position = szString.find("^");
						if (position == szString.npos)
							break;
						count++;
						l_string = szString.copy(buffer1,position);
						buffer1[l_string] = '\0';
						l_string = szString.copy(buffer2,szString.length()-position,position+1);
						buffer2[l_string] = '\0';
						szString = buffer2;
						//cout << buffer1 << endl;
						//cout << buffer2 << endl;
						if (count == 1)
						{
							number = buffer1;
							WTpp.peopleinfo.number = number;
							//cout << number << endl;   ///////////////need modification
						}
						else if (count == 2)
						{
							name = buffer1;
							WTpp.peopleinfo.name = name;
							//cout << name << endl;   ///////////////need modification
						}
						else if (count == 7)
						{
							ship = buffer1;
							WTpp.peopleinfo.ship = ship;
							//cout << ship << endl;   ///////////////need modification
							break;
						}
					}
					break;
				default:
					break;
				}
				flag_ocrUpdate = 1;
			}
			else
			{
				nRet = fpRecogIDCard();
				if (nRet <= 0)
				{
					//cout << "recognize failed" << endl;   ///////////////need modification
					return 0;
				}
				//int IDtype=fpGetSubId();
				//cout<<"证件识别成功！"<<endl;   ///////////////need modification
				//cout<<nRet<<endl;
				WTpp.peopleclear();  //人员信息清除
				pupdate=1;

				CONST int MAX_CH_NUM = 256;
				TCHAR szFieldValue[MAX_CH_NUM];
				int nBufLen = MAX_CH_NUM*sizeof(TCHAR);
				int nIndex = 1;
				CString strTmp=_T(""),strTmp1=_T("");
				switch (nRet)
				{
				case 2:    //身份证正面
					WTpp.peopleinfo.type = "身份证";
					typenum=2;
					while(TRUE)
					{
						memset(szFieldValue,0,nBufLen);
						//获取nIndex对应的识别内容
						nRet = fpGetRecogResult(nIndex, szFieldValue, nBufLen);
						if(nRet == 3) //获取内容失败
							break;
						//数据格式转换
						CString valueCstring = CString(szFieldValue);
						int len = valueCstring.GetLength();   
						char* chRtn = (char*)malloc((len*2+1)*sizeof(char));//CString的长度中汉字算一个长度   
						memset(chRtn, 0, 2*len+1);   
						USES_CONVERSION;   
						strcpy((LPSTR)chRtn,OLE2A(valueCstring.LockBuffer()));   

						switch (nIndex)
						{
						case 1: // name
							WTpp.peopleinfo.name = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						case 6: // number
							WTpp.peopleinfo.number = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						default: //other
							break;
						}
						++nIndex;
					}
					break;
				case 5:    //驾照
					WTpp.peopleinfo.type = "驾照";
					typenum=5;
					while(TRUE)
					{
						memset(szFieldValue,0,nBufLen);
						//获取nIndex对应的识别内容
						nRet = fpGetRecogResult(nIndex, szFieldValue, nBufLen);
						if(nRet == 3) //获取内容失败
							break;
						//数据格式转换
						CString valueCstring = CString(szFieldValue);
						int len = valueCstring.GetLength();   
						char* chRtn = (char*)malloc((len*2+1)*sizeof(char));//CString的长度中汉字算一个长度   
						memset(chRtn, 0, 2*len+1);   
						USES_CONVERSION;   
						strcpy((LPSTR)chRtn,OLE2A(valueCstring.LockBuffer()));

						switch (nIndex)
						{
						case 2: // name
							WTpp.peopleinfo.name = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						case 1: // number
							WTpp.peopleinfo.number = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						default: //other
							break;
						}
						++nIndex;
					}
					break;
				case 13:    //护照
					WTpp.peopleinfo.type = "护照";
					typenum=13;
					while(TRUE)
					{
						memset(szFieldValue,0,nBufLen);
						//获取nIndex对应的识别内容
						nRet = fpGetRecogResult(nIndex, szFieldValue, nBufLen);
						if(nRet == 3) //获取内容失败
							break;
						//数据格式转换
						CString valueCstring = CString(szFieldValue);
						int len = valueCstring.GetLength();   
						char* chRtn = (char*)malloc((len*2+1)*sizeof(char));//CString的长度中汉字算一个长度   
						memset(chRtn, 0, 2*len+1);   
						USES_CONVERSION;   
						strcpy((LPSTR)chRtn,OLE2A(valueCstring.LockBuffer()));

						switch (nIndex)
						{
						case 2: // name
							WTpp.peopleinfo.name = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						case 1: // number
							WTpp.peopleinfo.number = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						default: //other
							break;
						}
						++nIndex;
					}
					break;
				case 17:    //海员证
					WTpp.peopleinfo.type = "海员证";
					typenum=17;
					while(TRUE)
					{
						memset(szFieldValue,0,nBufLen);
						//获取nIndex对应的识别内容
						nRet = fpGetRecogResult(nIndex, szFieldValue, nBufLen);
						if(nRet == 3) //获取内容失败
							break;
						//数据格式转换
						CString valueCstring = CString(szFieldValue);
						int len = valueCstring.GetLength();   
						char* chRtn = (char*)malloc((len*2+1)*sizeof(char));//CString的长度中汉字算一个长度   
						memset(chRtn, 0, 2*len+1);   
						USES_CONVERSION;   
						strcpy((LPSTR)chRtn,OLE2A(valueCstring.LockBuffer()));

						switch (nIndex)
						{
						case 2: // name
							WTpp.peopleinfo.name = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						case 1: // number
							WTpp.peopleinfo.number = chRtn;
							//cout << chRtn << endl;   ///////////////need modification
							break;
						default: //other
							break;
						}
						++nIndex;
					}
					break;
				default:    //其他证件
					typenum=0;
					cout << "error: unknown card!!" << endl;   ///////////////need modification
					break;

				}
				flag_ocrUpdate = 1;
			}
		}
	}
	//WTpp.peopleinfo.name;
	//WTpp.peopleinfo.type;
	//WTpp.peopleinfo.number
	//WTpp.peopleinfo.ship;
	return 1;
} 