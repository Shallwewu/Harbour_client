#include "registration.h"
#include <QMessageBox>

//int registrate()
//{
//	bool getPasswordFlag  = false;
//
//	//open and query
//	HKEY hKEY;
//
//	DWORD cType;   
//	char ValueInfo[MAX_VALUE_NAME];  
//	DWORD ValueSize = MAX_VALUE_NAME;  
//	DWORD cchValue = MAX_VALUE_NAME;
//
//	LPCTSTR data_Set = _T("Software\\ExcellentSoft");
//	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
//	//QMessageBox::information(NULL,"error","����",QMessageBox::Ok);
//	if(ret0!=ERROR_SUCCESS)
//	{
//		cout << "the key is not exist, creat it!" << endl;
//	}
//	else
//	{
//		ret0 = RegQueryValueEx(hKEY,_T("inspection"),NULL,&cType,(LPBYTE)ValueInfo,&ValueSize);
//		if (ret0 != ERROR_SUCCESS)
//		{
//			cout << "query failed!" << endl;
//			RegCloseKey(hKEY);
//		}
//		getPasswordFlag = true;
//	}
//
//	while(!getPasswordFlag)
//	{
//		//��Ҫд�������
//		//open a txt file and read in the message
//
//
//
//
//		//UCHAR   m_name[10];
//		//memset(m_name, 0, sizeof(m_name));//������m_name����
//		//m_name[0] = 0xff;
//		//m_name[1] = 0xac;
//		//m_name[2] = 0x05;
//		//m_name[4] = 0x4e;
//		//string m_code = "ffac054e";
//		//LPBYTE b_code = m_code.c_str();
//		//char code[] = "B888E31B9FF2FFFF999920150904163709";
//		//char code[] = "B988E31B9FF2FFFF999920151004163709";
//		char code[] = "B888E31B9FF2FFFF999920151004163709";
//
//
//		//creat and write
//		HKEY hKEY_creat;
//		LPCTSTR dataSet_creat = _T("Software");
//		long ret_creat = RegOpenKeyEx(HKEY_LOCAL_MACHINE,dataSet_creat,0,KEY_READ,&hKEY_creat);
//		if(ret_creat!=ERROR_SUCCESS)
//		{
//			//��ʧ�ܣ��˳�
//			cout << "open for creat failed!" << endl;
//			getchar();
//			break;
//		}
//
//		HKEY hTempKey_creat;
//		DWORD dw_creat;
//		ret_creat = RegCreateKeyEx(hKEY_creat, _T("ExcellentSoft"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hTempKey_creat, &dw_creat);
//		if(ret_creat!=ERROR_SUCCESS)
//		{
//			//����ʧ�ܣ��˳�
//			cout << "creat failed!" << endl;
//			getchar();
//			break;
//		}
//
//		ret_creat = RegSetValueEx(hTempKey_creat, _T("inspection"), 0, REG_SZ, (const BYTE*)code, sizeof(code)+1);	//дֵ���������
//		if(ret_creat!=ERROR_SUCCESS)
//		{
//			//д��ע����Ϣʧ��
//			cout << "register failed!" << endl;
//			getchar();
//			break;
//		}
//
//		//������У���ע���븳ֵ
//		//
//		getPasswordFlag = true;
//
//
//		//�����Ƿ����ɹ�
//		ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
//		if(ret0!=ERROR_SUCCESS)
//		{
//			cout << "sorry, you can not register now!" << endl;
//			getchar();
//			break;
//		}
//
//		ret0 = RegQueryValueEx(hKEY,_T("inspection"),NULL,&cType,(LPBYTE)ValueInfo,&ValueSize);
//		if (ret0 != ERROR_SUCCESS)
//		{
//			cout << "query failed!" << endl;
//			RegCloseKey(hKEY);
//		}
//	}
//	if(cType==REG_EXPAND_SZ||cType==REG_SZ||cType==REG_MULTI_SZ)  
//	{  
//		cout << ValueInfo << endl;
//		//printf("%s",ValueInfo);  
//	}  
//	else if(cType==REG_DWORD||cType==REG_BINARY)  
//	{  
//		printf("%d/n",(long)*(short *)ValueInfo);  
//	} 
//
//	string regMessage = ValueInfo;
//	string::size_type position;
//	position = regMessage.find("FFFF9999");
//	string reg_macAddress = regMessage.substr(0, position);
//	string reg_time = regMessage.substr(position+8, regMessage.length());
//	cout << reg_macAddress.c_str() << endl << reg_time.c_str() << endl;
//	//���벢�ֽ�
//
//
//	if (getPasswordFlag)	//��ü������Mac�͵�ǰʱ�䣬��У��
//	{
//		//��ȡ��ǰʱ��
//		time_t t = time(NULL); 
//		char currentTime[64]; 
//		strftime( currentTime, sizeof(currentTime), "%Y%m%d%H%M%S", localtime(&t)); 
//		cout << currentTime << endl;
//
//		//��ȡmac��ַ
//		char macAddress[MAX_KEY_LENGTH];
//		memset(macAddress, '\0', sizeof(macAddress));
//		char macAddress_temp[8];
//
//		PIP_ADAPTER_INFO pAdapterInfo;
//		PIP_ADAPTER_INFO pAdapter=NULL;
//		DWORD dwRetVal=0;
//		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
//		ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
//		if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS){
//			GlobalFree(pAdapterInfo);
//			pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
//		}
//		if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR){
//			pAdapter=pAdapterInfo;
//			while(pAdapter){
//				if(strstr(pAdapter->Description,"PCI")>0//pAdapter->Description�а���"PCI"Ϊ����������
//					//||pAdapter->Type==71//pAdapter->Type��71Ϊ����������
//					){
//						int macLength = 0;
//						for(UINT i=0;i<pAdapter->AddressLength;i++){
//							//printf("%02X%c",pAdapter->Address[i],
//							//	i==pAdapter->AddressLength-1? '\n':'-');
//							memset(macAddress_temp, '\0', sizeof(macAddress_temp));
//							sprintf(macAddress_temp, "%02X", pAdapter->Address[i]);
//							int j = 0;
//							while (macAddress_temp[j] != 0)
//							{
//								macAddress[macLength++] = macAddress_temp[j++];
//							}
//						}
//						cout << macAddress << endl;
//				}
//				pAdapter=pAdapter->Next;
//			}
//		}
//		else
//		{
//			printf("Call to Get Adapters Information failed\n"); 
//		}
//
//		//string localMessage = macAddress;
//		//localMessage += currentTime;
//		//cout << localMessage.c_str() << endl;
//
//		string local_macAddress = macAddress;
//		string local_time = currentTime;
//
//		//��֤ע���룬��ȷ��ͨ��������ȷ�Ļ�ɾ��ԭע���벢�˳�����
//		cout << local_macAddress.compare(reg_macAddress) << endl;
//		cout << local_time.compare(reg_time) << endl;
//		if (local_macAddress.compare(reg_macAddress)!=0 || local_time.compare(reg_time)==1)
//		{
//			//ע����Ϣ����ȷ��ɾ��ԭ����Ϣ���˳�
//			LPCTSTR data_Set = _T("Software");
//			long ret_delet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
//			if(ret0!=ERROR_SUCCESS)
//			{
//				cout << "open failed!" << endl;
//				//getchar();
//				return 0;
//			}
//			ret_delet = RegDeleteKeyEx(hKEY, _T("ExcellentSoft"),KEY_WOW64_32KEY, 0);
//			if(ret0!=ERROR_SUCCESS)
//			{
//				cout << "delete failed!" << endl;
//				//getchar();
//				return 0;
//			}
//			RegCloseKey(hKEY);
//			exit(NULL);
//		}
//	}
//	else
//	{
//		cout << "error occurs in registration!" << endl;
//		exit(NULL);
//	}
//
//	getchar();
//	return 1;
//}




int registrate()
{
	bool getPasswordFlag  = false;

	//open and query
	HKEY hKEY;

	DWORD cType;   
	char ValueInfo[MAX_VALUE_NAME];  
	DWORD ValueSize = MAX_VALUE_NAME;  
	DWORD cchValue = MAX_VALUE_NAME;

	LPCTSTR data_Set = _T("Software\\ExcellentSoft");
	long ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);

	if(ret0 == ERROR_SUCCESS)
	{
		ret0 = RegQueryValueEx(hKEY,_T("inspection"),NULL,&cType,(LPBYTE)ValueInfo,&ValueSize);
		if (ret0 != ERROR_SUCCESS)
		{
			cout << "query failed!" << endl;
			RegCloseKey(hKEY);
		}
		else
			getPasswordFlag = true;
	}
	else
		cout << "open reg failed!" << endl;

	while(!getPasswordFlag)
	{
		cout << "the key is not exist, creat it!" << endl;
		//���ⲿ�ļ�����

		//char code[] = "B888E31B9FF2FFFF999920150904163709";
		//char code[] = "B988E31B9FF2FFFF999920151004163709";
		//char code[] = "B888E31B9FF2FFFF999920150804163709";
		char code[MAX_VALUE_NAME];

		ifstream regInfoIn;
		regInfoIn.open("regInfo.reg");
		if (!regInfoIn.is_open())
		{
			cout << "open reg file failed!" << endl;
			break;
		}
		regInfoIn >> code;
		regInfoIn.close();

		//creat and write
		HKEY hKEY_creat;
		LPCTSTR dataSet_creat = _T("Software");
		long ret_creat = RegOpenKeyEx(HKEY_LOCAL_MACHINE,dataSet_creat,0,KEY_READ,&hKEY_creat);
		if(ret_creat!=ERROR_SUCCESS)
		{
			//��ʧ�ܣ��˳�
			cout << "open for creat failed!" << endl;
			getchar();
			break;
		}

		HKEY hTempKey_creat;
		DWORD dw_creat;
		ret_creat = RegCreateKeyEx(hKEY_creat, _T("ExcellentSoft"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hTempKey_creat, &dw_creat);
		if(ret_creat!=ERROR_SUCCESS)
		{
			//����ʧ�ܣ��˳�
			cout << "creat failed!" << endl;
			getchar();
			break;
		}

		ret_creat = RegSetValueEx(hTempKey_creat, _T("inspection"), 0, REG_SZ, (const BYTE*)code, sizeof(code));
		if(ret_creat!=ERROR_SUCCESS)
		{
			//д��ע����Ϣʧ��
			cout << "register failed!" << endl;
			getchar();
			break;
		}

		//�����Ƿ�д��ɹ�
		ret0 = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
		if(ret0!=ERROR_SUCCESS)
		{
			cout << "sorry, you can not register now1!" << endl;
			getchar();
			break;
		}

		ret0 = RegQueryValueEx(hKEY,_T("inspection"),NULL,&cType,(LPBYTE)ValueInfo,&ValueSize);
		if (ret0 != ERROR_SUCCESS)
		{
			cout << "sorry, you can not register now2!" << endl;
			RegCloseKey(hKEY);
			break;
		}

		getPasswordFlag = true;
	}

	cout << "the local information:" << endl;
	//��ȡmac��ַ
	char macAddress[MAX_KEY_LENGTH];
	memset(macAddress, '\0', sizeof(macAddress));
	char macAddress_temp[8];

	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter=NULL;
	DWORD dwRetVal=0;
	pAdapterInfo=(IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen=sizeof(IP_ADAPTER_INFO);
	if(GetAdaptersInfo(pAdapterInfo,&ulOutBufLen)!=ERROR_SUCCESS)
	{
		GlobalFree(pAdapterInfo);
		pAdapterInfo=(IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}
	if((dwRetVal=GetAdaptersInfo(pAdapterInfo,&ulOutBufLen))==NO_ERROR)
	{
		pAdapter=pAdapterInfo;
		while(pAdapter){
			if(strstr(pAdapter->Description,"PCI")>0//pAdapter->Description�а���"PCI"Ϊ����������
				//||pAdapter->Type==71//pAdapter->Type��71Ϊ����������
				){
					int macLength = 0;
					for(UINT i=0;i<pAdapter->AddressLength;i++){
						//printf("%02X%c",pAdapter->Address[i],
						//	i==pAdapter->AddressLength-1? '\n':'-');
						memset(macAddress_temp, '\0', sizeof(macAddress_temp));
						sprintf(macAddress_temp, "%02X", pAdapter->Address[i]);
						int j = 0;
						while (macAddress_temp[j] != 0)
						{
							macAddress[macLength++] = macAddress_temp[j++];
						}
					}
					cout << macAddress << endl;
			}
			pAdapter=pAdapter->Next;
		}
	}
	else
	{
		printf("Call to Get Adapters Information failed\n"); 
	}

	//��ȡ��ǰʱ��
	time_t t = time(NULL); 
	char currentTime[64]; 
	strftime( currentTime, sizeof(currentTime), "%Y%m%d%H%M%S", localtime(&t)); 
	cout << currentTime << endl;

	string local_macAddress = macAddress;
	string local_time = currentTime;


	//У��
	if (getPasswordFlag)
	{
		cout << "the reg information:" << endl;

		if(cType==REG_EXPAND_SZ||cType==REG_SZ||cType==REG_MULTI_SZ)  
		{  
			cout << ValueInfo << endl;
			//printf("%s",ValueInfo);  
		}  
		else if(cType==REG_DWORD||cType==REG_BINARY)  
		{  
			printf("%d/n",(long)*(short *)ValueInfo);  
		} 

		//���벢�ֽ�
		//string regMessage = ValueInfo;
		string regMessage = rsa_dec(ValueInfo);
		string::size_type position;
		position = regMessage.find("FFFF9999");
		string reg_macAddress = regMessage.substr(0, position);
		string reg_time = regMessage.substr(position+8, regMessage.length());
		cout << reg_macAddress.c_str() << endl << reg_time.c_str() << endl;

		//��֤ע���룬��ȷ��ͨ��������ȷ�Ļ�ɾ��ԭע���벢�˳�����
		cout << local_macAddress.compare(reg_macAddress) << endl;
		cout << local_time.compare(reg_time) << endl;
		//if (local_macAddress.compare(reg_macAddress)!=0 || local_time.compare(reg_time)==1)	//check mac and time limit
		if (local_time.compare(reg_time)==1)	//check time limit only
		{
			//ע����Ϣ����ȷ��ɾ��ԭ����Ϣ���˳�
			LPCTSTR data_Set = _T("Software");
			long ret_delet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
			if(ret0!=ERROR_SUCCESS)
			{
				cout << "open failed!" << endl;
				//getchar();
				return 0;
			}
			ret_delet = RegDeleteKeyEx(hKEY, _T("ExcellentSoft"),KEY_WOW64_32KEY, 0);
			if(ret0!=ERROR_SUCCESS)
			{
				cout << "delete failed!" << endl;
				//getchar();
				return 0;
			}
			RegCloseKey(hKEY);
			getPasswordFlag = false;
			cout << "reg failed and delete the info!" << endl;
			//exit(NULL);
		}
	}

	if (getPasswordFlag)
		return 1;

	cout << "reg failed, please contact Excellent to get the reg info!" << endl;
	ofstream regInfoOut;
	regInfoOut.open("regRequest.reg");
	regInfoOut << local_macAddress << local_time << endl;
	regInfoOut.close();
	return 0;
}