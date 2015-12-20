#include "registration.h"
#include <QMessageBox>

int registrateT()
{
	bool getPasswordFlag  = false;

	//open and query
	HKEY hKEY;

	DWORD cType;   
	char ValueInfo[MAX_VALUE_NAME];  
	DWORD ValueSize = MAX_VALUE_NAME;  
	DWORD cchValue = MAX_VALUE_NAME;

	LPCTSTR data_Set = _T("Software\\ExcellentSoftInspectT");
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
		//从外部文件读入

		//char code[] = "B888E31B9FF2FFFF999920150904163709";
		//char code[] = "B988E31B9FF2FFFF999920151004163709";
		//char code[] = "B888E31B9FF2FFFF999920150804163709";
		char code[MAX_VALUE_NAME];

		ifstream regInfoIn;
		regInfoIn.open("regInfoT.reg");
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
			//打开失败，退出
			cout << "open for creat failed!" << endl;
			getchar();
			break;
		}

		HKEY hTempKey_creat;
		DWORD dw_creat;
		ret_creat = RegCreateKeyEx(hKEY_creat, _T("ExcellentSoftInspectT"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hTempKey_creat, &dw_creat);
		if(ret_creat!=ERROR_SUCCESS)
		{
			//创建失败，退出
			cout << "creat failed!" << endl;
			getchar();
			break;
		}

		ret_creat = RegSetValueEx(hTempKey_creat, _T("inspection"), 0, REG_SZ, (const BYTE*)code, sizeof(code));
		if(ret_creat!=ERROR_SUCCESS)
		{
			//写入注册信息失败
			cout << "register failed!" << endl;
			getchar();
			break;
		}

		//检验是否写入成功
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
	//获取mac地址
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
			if(strstr(pAdapter->Description,"PCI")>0//pAdapter->Description中包含"PCI"为：物理网卡
				//||pAdapter->Type==71//pAdapter->Type是71为：无线网卡
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

	//获取当前时间
	time_t t = time(NULL); 
	char currentTime[64]; 
	strftime( currentTime, sizeof(currentTime), "%Y%m%d%H%M%S", localtime(&t)); 
	cout << currentTime << endl;

	string local_macAddress = macAddress;
	string local_time = currentTime;


	//校验
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

		//解码并分解
		//string regMessage = ValueInfo;
		string regMessage = rsa_dec(ValueInfo);
		string::size_type position;
		position = regMessage.find("FFFF9999");
		string reg_macAddress = regMessage.substr(0, position);
		string reg_time = regMessage.substr(position+8, regMessage.length());
		cout << reg_macAddress.c_str() << endl << reg_time.c_str() << endl;

		//验证注册码，正确就通过，不正确的话删除原注册码并退出程序
		cout << local_macAddress.compare(reg_macAddress) << endl;
		cout << local_time.compare(reg_time) << endl;
		//if (local_macAddress.compare(reg_macAddress)!=0 || local_time.compare(reg_time)==1)	//check mac and time limit
		if (local_time.compare(reg_time)==1)	//check time limit only
		{
			//注册信息不正确，删除原有信息并退出
			LPCTSTR data_Set = _T("Software");
			long ret_delet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
			if(ret0!=ERROR_SUCCESS)
			{
				cout << "open failed!" << endl;
				//getchar();
				return 0;
			}
			ret_delet = RegDeleteKeyEx(hKEY, _T("ExcellentSoftInspectT"),KEY_WOW64_32KEY, 0);
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

int registrateM()
{
	bool getPasswordFlag  = false;

	//open and query
	HKEY hKEY;

	DWORD cType;   
	char ValueInfo[MAX_VALUE_NAME];  
	DWORD ValueSize = MAX_VALUE_NAME;  
	DWORD cchValue = MAX_VALUE_NAME;

	LPCTSTR data_Set = _T("Software\\ExcellentSoftInspectM");
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
		//从外部文件读入

		//char code[] = "B888E31B9FF2FFFF999920150904163709";
		//char code[] = "B988E31B9FF2FFFF999920151004163709";
		//char code[] = "B888E31B9FF2FFFF999920150804163709";
		char code[MAX_VALUE_NAME];

		ifstream regInfoIn;
		regInfoIn.open("regInfoM.reg");
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
			//打开失败，退出
			cout << "open for creat failed!" << endl;
			getchar();
			break;
		}

		HKEY hTempKey_creat;
		DWORD dw_creat;
		ret_creat = RegCreateKeyEx(hKEY_creat, _T("ExcellentSoftInspectM"), 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL, &hTempKey_creat, &dw_creat);
		if(ret_creat!=ERROR_SUCCESS)
		{
			//创建失败，退出
			cout << "creat failed!" << endl;
			getchar();
			break;
		}

		ret_creat = RegSetValueEx(hTempKey_creat, _T("inspection"), 0, REG_SZ, (const BYTE*)code, sizeof(code));
		if(ret_creat!=ERROR_SUCCESS)
		{
			//写入注册信息失败
			cout << "register failed!" << endl;
			getchar();
			break;
		}

		//检验是否写入成功
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
	//获取mac地址
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
			if(strstr(pAdapter->Description,"PCI")>0//pAdapter->Description中包含"PCI"为：物理网卡
				//||pAdapter->Type==71//pAdapter->Type是71为：无线网卡
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

	//获取当前时间
	time_t t = time(NULL); 
	char currentTime[64]; 
	strftime( currentTime, sizeof(currentTime), "%Y%m%d%H%M%S", localtime(&t)); 
	cout << currentTime << endl;

	string local_macAddress = macAddress;
	string local_time = currentTime;


	//校验
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

		//解码并分解
		//string regMessage = ValueInfo;
		string regMessage = rsa_dec(ValueInfo);
		string::size_type position;
		position = regMessage.find("FFFF9999");
		string reg_macAddress = regMessage.substr(0, position);
		string reg_time = regMessage.substr(position+8, regMessage.length());
		cout << reg_macAddress.c_str() << endl << reg_time.c_str() << endl;

		//验证注册码，正确就通过，不正确的话删除原注册码并退出程序
		cout << local_macAddress.compare(reg_macAddress) << endl;
		cout << local_time.compare(reg_time) << endl;
		//if (local_macAddress.compare(reg_macAddress)!=0 || local_time.compare(reg_time)==1)	//check mac and time limit
		if (local_macAddress.compare(reg_macAddress)!=0)	//check time limit only
		{
			//注册信息不正确，删除原有信息并退出
			LPCTSTR data_Set = _T("Software");
			long ret_delet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
			if(ret0!=ERROR_SUCCESS)
			{
				cout << "open failed!" << endl;
				//getchar();
				return 0;
			}
			ret_delet = RegDeleteKeyEx(hKEY, _T("ExcellentSoftInspectM"),KEY_WOW64_32KEY, 0);
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