	//all pointers of the function
#include "afxwin.h"
	int   (WINAPI *fpInitIDCard)(LPCTSTR lpUserID, int nType,LPCTSTR lpDirectory);
	void (WINAPI *fpFreeIDCard)();
	bool (WINAPI *fpSetAcquireImageType)(int nLightType,int nImageType);
	
	bool (WINAPI *fpSetUserDefinedImageSize)(int nWidth, int nHeight);
	bool (WINAPI *fpSetAcquireImageResolution)(int nResolutionX, int nResolutionY);
	bool (WINAPI *fpCheckDeviceOnline)();

	int (WINAPI *fpAcquireImage)(int nImageSizeType);
	int (WINAPI *fpSaveImage)(LPCTSTR lpFileName);
	int (WINAPI *fpRecogIDCard)();
	
	int  (WINAPI *fpSetIDCardID)(int nMainID,int nSubID[],int nSubIdCount);
	int (WINAPI *fpAddIDCardID)(int nMainID,int nSubID[],int nSubIdCount);
	int (WINAPI *fpRecogIDCardEx)(int nMainID ,int nSubID);
	int (WINAPI *fpGetSubId)();


	int (WINAPI *fpGetRecogResult)(int nIndex, LPTSTR lpBuffer, int &nBufferLen);
	int (WINAPI *fpGetFieldName)(int nIndex, LPTSTR lpBuffer, int &nBufferLen);

	BOOL (WINAPI *fpGetCurrentDevice)(LPCTSTR, int);
	void  (WINAPI *fpGetVersionInfo)(LPCTSTR ,int );
	int (WINAPI *fpGetButtonDownType)();
	int (WINAPI *fpSaveHeadImage)(LPCTSTR );
	///////////////////////////////////
	int (WINAPI *pGetGrabSignalType)();
	int (WINAPI *pSetIDCardID)(int nMainID,int nSubID[],int nSubIdCount);
	int (WINAPI *pAddIDCardID)(int nMainID,int nSubID[],int nSubIdCount);
	int (WINAPI *pSetSpecialAttribute)(int nType,int nSet);
	int (WINAPI *pGetDeviceSN)(LPCTSTR,int);
	int(WINAPI *pSetIOStatus)(int, bool);

	int (WINAPI *pRecogBusinessCard)(int nCardType);
	int (WINAPI *pGetBusinessCardResult)(int nID,int nIndex,LPWSTR lpBuffer,int &nBufferLen);
	int (WINAPI *pGetBusinessCardPosition)(int nID,int nIndex,int &nLeft,int &nRight,int &nTop, int &nBottom);
	int (WINAPI *pGetBusinessCardFieldName)(int nID,LPWSTR lpBuffer,int &nBufferLen);
	int (WINAPI *pGetBusinessCardResultCount)(int nID);
	int (WINAPI *pRecogBusinessImage)(LPCWSTR lpImageFileName,int nCardType);
	int (WINAPI *pQRCodeRecog)(LPTSTR pResult,int& nLen);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
