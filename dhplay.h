/************************************************************************/
/* dhplay   版本:  3.34.0 			                                    */
/************************************************************************/

#ifndef _DHPLAY_H
#define _DHPLAY_H

#if (defined(WIN32) || defined(WIN64))
	#ifdef dhplay_EXPORTS
		#define PLAYSDK_API __declspec(dllexport)
	#else
		#define PLAYSDK_API __declspec(dllimport)
	#endif
	#define CALLMETHOD __stdcall
	#define CALLBACK   __stdcall
#else
	#define CALLMETHOD
	#define CALLBACK
	#define PLAYSDK_API extern "C"
#endif

#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#else
#define BOOL		int
#define BYTE		unsigned char
#define PBYTE		BYTE*
#define LPBYTE		BYTE*
#define LONG		long
#define DWORD		unsigned long
#define WORD		unsigned short
#define COLORREF	DWORD
#define HDC			void*
#define HWND		void*
#define LPSTR		char*
#define UINT		unsigned int
#define TRUE		1
#define FALSE		0

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef struct tagRECT
{
	LONG left;
	LONG top;
	LONG right;
	LONG bottom;
}RECT;

#endif

#ifdef	__cplusplus
extern "C" {
#endif

/************************************************************************/
/* 宏定义			                                                    */
/************************************************************************/

#define  FUNC_MAX_PORT					501		//最大通道号

#define MIN_WAVE_COEF					-100	//PLAY_AdjustWaveAudio使用
#define MAX_WAVE_COEF					100		//PLAY_AdjustWaveAudio使用

#define TIMER_1							1		//多媒体定时器
#define TIMER_2							2		//线程定时器

#define BUF_VIDEO_SRC					1		//视频源缓冲
#define BUF_AUDIO_SRC					2		//音频源缓冲
#define BUF_VIDEO_RENDER				3		//解码后视频数据缓冲
#define BUF_AUDIO_RENDER				4		//解码后音频数据缓冲

#define MAX_DISPLAY_WND					4

#define DISPLAY_NORMAL					1		//正常分辨率显示
#define DISPLAY_QUARTER					2		//1/4分辨率显示

#define MAX_DIS_FRAMES					50		//播放缓冲区最大缓冲帧数
#define MIN_DIS_FRAMES					6		//播放缓冲区最小缓冲帧数

#define BY_FRAMENUM						1		//按帧号方式(PLAY_GetKeyFramePos使用)
#define BY_FRAMETIME					2		//按时间方式(PLAY_GetKeyFramePos使用)

#define SOURCE_BUF_MAX					1024*100000	//数据流缓冲区最大长度
#define SOURCE_BUF_MIN					1024*50		//数据流缓冲区最小长度

#define STREAME_REALTIME				0		//实时流模式
#define STREAME_FILE					1		//文件流模式

#define T_AUDIO16						101		//16位音频数据类型
#define T_AUDIO8						100		//8位音频数据类型

#define T_UYVY							1		//UYVY类型的YUV数据,现在不支持.
#define T_IYUV							3		//IYUV(I420)类型YUV数据
#define T_RGB32							7		//RGB32类型

#define SUPPORT_DDRAW					1		//支持DIRECTDRAW,如果不支持,则播放器不能工作
#define SUPPORT_BLT						2		//显卡支持BLT操作,如果不支持,则播放器不能工作
#define SUPPORT_BLTFOURCC				4		//显卡BLT支持颜色转换,如果不支持,播放器会使用软件方式作RGB转换
#define SUPPORT_BLTSHRINKX				8		//显卡BLT支持X轴缩小,如果不支持,系统使用软件方式转换
#define SUPPORT_BLTSHRINKY				16		//显卡BLT支持Y轴缩小,如果不支持,系统使用软件方式转换
#define SUPPORT_BLTSTRETCHX				32		//显卡BLT支持X轴放大,如果不支持,系统使用软件方式转换
#define SUPPORT_BLTSTRETCHY				64		//显卡BLT支持Y轴放大,如果不支持,系统使用软件方式转换
#define SUPPORT_SSE						128		//CPU支持SSE指令,Intel Pentium3以上支持SSE指令
#define SUPPORT_MMX						256		//CPU支持MMX指令集

#define PLAY_CMD_GetTime				1		//编码中时间信息
#define PLAY_CMD_GetFileRate			2		//帧率信息
#define PLAY_CMD_GetMediaInfo			3		//媒体信息
#define PLAY_CMD_GetRenderNum			4		//当前要渲染的帧号
#define PLAY_CMD_GetRenderTime			5		//当前要渲染的时间
#define PLAY_CMD_GetSrcTime				6

#define AVI_MEDIACHANGE_FRAMERATE		1		//帧率改变
#define AVI_MEDIACHANGE_RESOLUTION		2		//分辨率改变

#define WATERMARK_DATA_TEXT				0		//文字
#define WATERMARK_DATA_JPEG_BMP			1		//JPEG或者BMP图片
#define WATERMARK_DATA_FRAMEDATA		3		//帧数据

#define	DH_PLAY_NOERROR					0		//没有错误
#define DH_PLAY_PARA_OVER				1		//输入参数非法
#define DH_PLAY_ORDER_ERROR				2		//调用顺序不对
#define DH_PLAY_TIMER_ERROR				3		//多媒体时钟设置失败
#define DH_PLAY_DEC_VIDEO_ERROR			4		//视频解码失败
#define DH_PLAY_DEC_AUDIO_ERROR			5		//音频解码失败
#define DH_PLAY_ALLOC_MEMORY_ERROR		6		//分配内存失败
#define DH_PLAY_OPEN_FILE_ERROR			7		//文件操作失败
#define DH_PLAY_CREATE_OBJ_ERROR		8		//创建线程事件等失败
#define DH_PLAY_CREATE_DDRAW_ERROR		9		//创建directDraw失败
#define DH_PLAY_CREATE_OFFSCREEN_ERROR	10		//创建后端缓存失败
#define DH_PLAY_BUF_OVER				11		//缓冲区满,输入流失败
#define DH_PLAY_CREATE_SOUND_ERROR		12		//创建音频设备失败
#define DH_PLAY_SET_VOLUME_ERROR		13		//设置音量失败
#define DH_PLAY_SUPPORT_FILE_ONLY		14		//只能在播放文件时才能使用
#define DH_PLAY_SUPPORT_STREAM_ONLY		15		//只能在播放流时才能使用
#define DH_PLAY_SYS_NOT_SUPPORT			16		//系统不支持,解码器只能工作在Pentium 3以上
#define DH_PLAY_FILEHEADER_UNKNOWN		17		//没有文件头
#define DH_PLAY_VERSION_INCORRECT		18		//解码器和编码器版本不对应
#define DH_PLAY_INIT_DECODER_ERROR		19		//初始化解码器失败
#define DH_PLAY_CHECK_FILE_ERROR		20		//文件太短或码流无法识别
#define DH_PLAY_INIT_TIMER_ERROR		21		//初始化多媒体时钟失败
#define DH_PLAY_BLT_ERROR				22		//位拷贝失败
#define DH_PLAY_UPDATE_ERROR			23		//显示overlay失败
#define DH_PLAY_MEMORY_TOOSMALL			24		//缓冲太小

/************************************************************************/
/* 结构体	                                                            */
/************************************************************************/

typedef enum __tPicFormats
{
	PicFormat_BMP = 0,				        //BMP32类型
    PicFormat_JPEG,							//JPEG类型
	PicFormat_JPEG_70,						//70%质量的JPEG类型
	PicFormat_JPEG_50,						//50%质量的JPEG类型
	PicFormat_JPEG_30,						//30%质量的JPEG类型
	PicFormat_JPEG_10,						//10%质量的JPEG类型
	PicFormat_BMP24,                        //BMP24类型
} tPicFormats;

typedef struct
{
	LONG			nFilePos;				//指定帧在文件中的偏移位置
	LONG			nFrameLen;				//帧长度
	LONG			nFrameNum;				//帧序号
	LONG			nFrameTime;				//帧时间
	LONG			nErrorFrameNum;			//保留,暂无使用
	SYSTEMTIME*		pErrorTime;				//保留,暂无使用
	LONG			nErrorLostFrameNum;		//保留,暂无使用
	LONG			nErrorFrameSize;		//保留,暂无使用
}FRAME_POS,*PFRAME_POS;

typedef struct
{
	LONG			nWidth;					//画面宽,单位像素.如果是音频数据则为0.
	LONG			nHeight;				//画面高,如果是音频数据则为0
	LONG			nStamp;					//时标信息,单位毫秒
	LONG			nType;					//视频帧类型,T_AUDIO16,T_RGB32,T_IYUV
	LONG			nFrameRate;				//编码时产生的图像帧率
}FRAME_INFO;

typedef struct
{
	#define FRAME_TYPE_VIDEO 0				//视频帧
	#define FRAME_TYPE_AUDIO 1				//音频帧
	int				nFrameType;				//视频帧类型,见上面定义
	int				nFrameSeq;				//帧序号
	int				nStamp;					//时标信息,单位毫秒
	int				nWidth;					//画面宽,单位像素.如果是音频数据则为0.
	int 			nHeight;				//画面高,如果是音频数据则为0
	int				nFrameRate;				//编码时产生的图像帧率
	int				nChannels;				//音频通道数
	int				nBitPerSample;			//音频采样位数
	int				nSamplesPerSec;			//音频采样频率
	int				nReserved[64];			//保留字段
}FRAME_INFO_EX;

typedef struct 
{
	int				nFrameType;					//帧类型,定义见FRAME_INFO_EX里nFrameType字段
	
	void*			pAudioData;				//音频数据,如果是音频帧
	int				nAudioDataLen;			//音频数据长度

	void*			pVideoData[3];			//分别表示视频的YUV三个分量
	int				nStride[3];				//分别表示YUV三个分量的跨距
	int				nWidth[3];				//分别表示YUV三个分量的宽度
	int				nHeight[3];				//分别表示YUV三个分量的高度
	int				nReserved[64];			//保留字段
}FRAME_DECODE_INFO;

typedef struct 
{
	int			lWidth;					//画面宽,单位像素.
	int			lHeight;				//画面高
	int			lFrameRate;				//帧率
	int			lChannel;				//音频通道数
	int			lBitPerSample;			//音频采样位数
	int			lSamplesPerSec;			//音频采样频率
}MEDIA_INFO;

typedef struct 
{
	char*			pDataBuf;				//帧数据
	LONG			nSize;					//帧数据长度
	LONG			nFrameNum;				//帧序号
	BOOL			bIsAudio;				//是否音频帧
	LONG			nReserved;				//保留
}FRAME_TYPE;

typedef struct 
{
	char*	pHead;
	int		nLen;
	char*	pBody;
	int		nBodyLen;
	
	int		nRet;				//0: decode 1://don't decode
	char	reserved[128];
}DemuInfoEx;

typedef struct
{
	int type;			// 1:VIDEO, 2:AUDIO, 3:DATA
	int subtype;		// I Frame, BP Frame, PCM8, MS-ADPCM etc.
	int encode;			// MPEG4, H264, STDH264
	int sequence;		// I帧间隔里面的序号
	
	// 图像大小信息
	int width;
	int height;
	
	// 帧率信息
	int rate;
	
	// 时间信息
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int secode;
	LONG timestamp;
	
	int channels;
	int bitspersample;
	int samplespersecond;
}DEMUX_INFO;

/************************************************************************/
/* 接口		                                                            */
/************************************************************************/

//------------------------------------------------------------------------
// 函数: PLAY_OpenFile
// 描述: 打开播放文件，用于在播放文件之前调用. 文件不能超过4G或小于4K
// 参数: nPort,通道号
//		 sFileName,文件名
// 返回: BOOL,成功返回TRUE,失败返回FALSE
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OpenFile(LONG nPort,LPSTR sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_CloseFile
// 描述: 关闭文件, 在PLAY_Stop后调用
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CloseFile(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_OpenStream
// 描述: 打开流播放
// 参数: nPort,通道号
//		 pFileHeadBuf,目前不使用,填NULL.
//		 nSize,目前不适用,填0.
//		 nBufPoolSize,设置播放器中存放数据流的缓冲区大小.
//			范围是[SOURCE_BUF_MIN,SOURCE_BUF_MAX].一般设为900*1024，如果数
//			据送过来相对均匀,可调小该值,如果数据传输不均匀,可增大该值.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OpenStream(LONG nPort,PBYTE pFileHeadBuf,DWORD nSize,DWORD nBufPoolSize);

//------------------------------------------------------------------------
// 函数: PLAY_CloseStream
// 描述: 关闭流
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CloseStream(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_Play
// 描述: 开始播放.如果已经播放,改变当前播放状态为正常速度播放.
// 参数: nPort,通道号
//		 hWnd,播放窗口句柄
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Play(LONG nPort, HWND hWnd);

//------------------------------------------------------------------------
// 函数: PLAY_Stop
// 描述: 停止播放
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Stop(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_InputData
// 描述: 输入数据流,PLAY_Play之后使用
// 参数: nPort,通道号
//		 pBuf,缓冲区地址
//		 nSize,缓冲区大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.如失败,一般是缓冲区已满,用户可暂停
//			输入，一段时间之后再输入流，确保播放库不丢失数据。
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_InputData(LONG nPort,PBYTE pBuf,DWORD nSize);

//------------------------------------------------------------------------
// 函数: PLAY_Pause
// 描述: 暂停或者恢复播放
// 参数: nPort,通道号
//		 nPause,TRUE暂停,FLASE恢复
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Pause(LONG nPort,DWORD nPause);

//------------------------------------------------------------------------
// 函数: PLAY_Fast
// 描述: 播放速度分为九级,播放速度分别为每秒播放1,3,6,12,25,50,75,100,125帧
//		 图像.每次调用播放速度提升一级,最多调用4次,要恢复正常播放调用
//		 PLAY_Play,从当前位置开始正常播放.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Fast(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_Slow
// 描述: 每次调用播放速度降一级,最多调用4次,要恢复正常播放调用PLAY_Play.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_Slow(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_OneByOne
// 描述: 单桢播放,要恢复正常播放调用PLAY_ Play.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OneByOne(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlayPos
// 描述: 设置文件播放指针的相对位置(百分比)
// 参数: nPort,通道号
//		 fRelativePos,文件长度的百分比
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlayPos(LONG nPort,float fRelativePos);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlayDirection
// 描述: 设置播放方向
// 参数: nPort,通道号
//		 emDirection,播放方向：0，向前，1，向后
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlayDirection(LONG nPort, DWORD emDirection);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayPos
// 描述: 获取文件播放指针的相对位置(百分比)
// 参数: nPort,通道号
// 返回: float,文件长度的相对位置,范围0-100%
//------------------------------------------------------------------------
PLAYSDK_API float CALLMETHOD PLAY_GetPlayPos(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetFileEndMsg
// 描述: 设置文件结束时要发送的消息
// 参数: nPort,端口号
//       hWnd,消息发送窗口
//       nMsg,用户自定义的输入的消息，当播放到文件结束时用户在hWnd窗口过程中
//			收到这个消息。此消息中的wParam或者lParam都可以获得nPort的值.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFileEndMsg(LONG nPort,HWND hWnd,UINT nMsg);

//------------------------------------------------------------------------
// 函数: PLAY_SetVolume
// 描述: 设置音量,设置的音量是指声卡输出的音量,会影响其他的声音应用.
// 参数: nPort,通道号
//		 nVolume,音量的值,范围[0,0xFFFF]
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetVolume(LONG nPort,WORD nVolume);

//------------------------------------------------------------------------
// 函数: PLAY_StopSound
// 描述: 关闭独占方式声音
// 参数: 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopSound();

//------------------------------------------------------------------------
// 函数: PLAY_PlaySound
// 描述: 独占方式打开声音,同一时刻只能有一路声音,如果现在已经有声音打开，则
//		 自动关闭原来已经打开的声音.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_PlaySound(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetFileTime
// 描述: 获取文件总时间
// 参数: nPort,通道号
// 返回: 文件总的时间长度值,单位秒.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetFileTime(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayedTime
// 描述: 获取当前文件播放时间
// 参数: nPort,通道号
// 返回: 文件当前播放的时间,单位秒
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetPlayedTime(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayedFrames
// 描述: 获取已播放的桢数
// 参数: nPort,通道号
// 返回: 已经解码的视频帧数
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetPlayedFrames(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecodeCallBack
// 描述: 设置解码回调,替换播放器中的显示部分,由用户自己控制显示,该函数在
//			PLAY_Play之前调用,在PLAY_Stop时自动失效,下次调用PLAY_Play之前
//			需要重新设置.解码部分不控制速度,只要用户从回调函数中返回,解码器
//			就会解码下一部分数据.适用于只解码不显示的情形。
// 参数: nPort,通道号
//		 cbDec,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pFrameDecodeInfo,解码后的音视频数据
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			pUser,用户自定义参数
//		 pUser,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fCBDecode)(LONG nPort, FRAME_DECODE_INFO* pFrameDecodeInfo, FRAME_INFO_EX* pFrameInfo, void* pUser);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecodeCallBack(LONG nPort, fCBDecode cbDec, void* pUser);

//------------------------------------------------------------------------
// 函数: PLAY_SetDisplayCallBack
// 描述: 设置视频抓图回调函数.如果要停止回调,可以把回调函数指针设为NULL,该
//			函数可以在任何时候调用
// 参数: nPort,通道号
//		 DisplayCBFun,抓图回调函数,可以为NULL,其参数含义如下
//			nPort,通道号
//			pBuf,返回图像数据
//			nSize,返回图像数据大小
//			nWidth,画面宽,单位像素
//			nHeight,画面高
//			nStamp,时标信息，单位毫秒
//			nType,数据类型,T_RGB32,T_UYVY,详见宏定义说明
//			nReceaved,对应用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDisplayCBFun)(LONG nPort,char * pBuf,LONG nSize,LONG nWidth,LONG nHeight,LONG nStamp,LONG nType, void* pReserved);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDisplayCallBack(LONG nPort, fDisplayCBFun DisplayCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_ConvertToBmpFile
// 描述: 图像格式转为BMP格式.
// 参数: pBuf,图像数据指针
//		 nSize,图像数据大小
//		 nWidth,图像宽度
//		 nHeight,图像高度
//		 nType,数据类型.T_RGB32,T_UYVY等.
//		 sFileName,要保存的文件名.最好以BMP作为文件扩展名.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ConvertToBmpFile(char * pBuf,LONG nSize,LONG nWidth,LONG nHeight,LONG nType, char *sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_ConvertToJpegFile
// 描述: 图像格式转为JPEG格式.
// 参数: pYUVBuf,图像数据指针
//		 nWidth,图像宽度
//		 nHeight,图像高度
//		 YUVtype,YUV数据类型,如T_YV12,T_UYVY
//		 quality,图片压缩质量,范围(0, 100].
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ConvertToJpegFile(char *pYUVBuf, LONG nWidth, LONG nHeight, int YUVtype, int quality, char *sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_GetFileTotalFrames
// 描述: 获取文件总帧数
// 参数: nPort,通道号
// 返回: DWORD,文件中的总帧数
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetFileTotalFrames(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetCurrentFrameRate
// 描述: 获取当前帧率
// 参数: nPort,通道号
// 返回: DWORD,当前码流中编码时的帧率值
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetCurrentFrameRate(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPlayedTimeEx
// 描述: 获取文件当前播放时间
// 参数: nPort,通道号
// 返回: DWORD,文件当前播放的时间,单位毫秒.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetPlayedTimeEx(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlayedTimeEx
// 描述: 设置文件当前播放时间.
// 参数: nPort,通道号
//		 nTime,设置文件播放位置到指定时间,单位毫秒.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlayedTimeEx(LONG nPort,DWORD nTime);

//------------------------------------------------------------------------
// 函数: PLAY_GetCurrentFrameNum
// 描述: 获取当前播放的帧序号,PLAY_GetPlayedFrames是总共解码的帧数.如果文件
//			播放位置不被改变，那么这两个函数的返回值应该非常接近，除非码流丢
//			失数据.
// 参数: nPort,通道号
// 返回: 当前播放的帧序号.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetCurrentFrameNum(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetStreamOpenMode
// 描述: 设置流播放模式,必须在播放之前设置.
// 参数: nPort,通道号
//		 nMode,STREAME_REALTIME实时模式(默认);STREAME_FILE文件模式.实时模
//			式,适合播放网络实时数据;文件模式,适合用户把文件数据用流方式输入.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetStreamOpenMode(LONG nPort,DWORD nMode);

//------------------------------------------------------------------------
// 函数: PLAY_GetSdkVersion
// 描述: 获取播放库SDK版本号,次版本号和补丁号
// 参数: 
// 返回: DWORD,高16位表示当前的主版本号;9~16位表示次版本号;1~8位表示次补丁号;
//			如:返回值0x00030107表示:主版本号是3,次版本号是1,补丁号是7.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetSdkVersion();

//------------------------------------------------------------------------
// 函数: PLAY_GetLastError
// 描述: 获取错误码
// 参数: nPort,通道号
// 返回: DWORD,获得当前错误的错误码.请参见错误码宏定义.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetLastError(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_RefreshPlay
// 描述: 刷新显示.当用户暂停时如果刷新了窗口,则窗口中的图像因为刷新而消失,
//			此时调用这个接口可以重新把图像显示出来.只有在暂停和单帧播放时才
//			执行,其它情况会直接返回.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_RefreshPlay(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPictureSize
// 描述: 获取原始图像大小,根据此大小来设置显示窗口的区域,可以不用显卡做缩放
//			工作,对于那些不支持硬件缩放的显卡来说非常有用.
// 参数: nPort,通道号
//		 pWidth,输出参数,返回图片宽度
//		 pHeight,输出参数,返回图片高度
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPictureSize(LONG nPort,LONG *pWidth,LONG *pHeight);

//------------------------------------------------------------------------
// 函数: PLAY_SetPicQuality
// 描述: 设置图像质量,当设置成高质量时画面效果好,但CPU利用率高.在支持多路播放时，
//			可以设为低质量,以降低CPU利用率.当某路放大播放时将该路设置成高质量，
//			以达到好的画面效果.
// 参数: nPort,通道号
//		 bHighQuality,为TRUE(1)时图像高质量,为FALSE(0)时低质量(默认值).
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPicQuality(LONG nPort,BOOL bHighQuality);

//------------------------------------------------------------------------
// 函数: PLAY_PlaySoundShare
// 描述: 以共享方式播放声音,播放本路声音而不去关闭其他路的声音.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_PlaySoundShare(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_StopSoundShare
// 描述: 关闭声音(共享方式).在同一个进程中,所有通道必须使用相同的方式播放或关闭声音.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopSoundShare(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetStreamOpenMode
// 描述: 获取流播放模式
// 参数: nPort,通道号
// 返回: LONG,STREAME_REALTIME或STREAME_FILE.
//------------------------------------------------------------------------
PLAYSDK_API LONG CALLMETHOD PLAY_GetStreamOpenMode(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetVolume
// 描述: 获取音量,这里的音量是指声卡输出的音量.
// 参数: nPort,通道号
// 返回: WORD,音量值
//------------------------------------------------------------------------
PLAYSDK_API WORD CALLMETHOD PLAY_GetVolume(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetPictureQuality
// 描述: 获取图像质量
// 参数: nPort,通道号
//		 bHighQuality,输出参数,TRUE表示高质量,FALSE表示低质量.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPictureQuality(LONG nPort,BOOL *bHighQuality);

//------------------------------------------------------------------------
// 函数: PLAY_GetSourceBufferRemain
// 描述: 获取流播放模式下源缓冲区剩余数据大小
// 参数: nPort,通道号
// 返回: DWORD,当前源缓冲的大小单位BYTE(字节).
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetSourceBufferRemain(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_ResetSourceBuffer
// 描述: 清空流播放模式下源缓冲区的剩余数据
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ResetSourceBuffer(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_OneByOneBack
// 描述: 单帧回退,此接口只支持文件播放,必须在文件索引生成之后才能调用.每调
//			用一次倒退一帧
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_OneByOneBack(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetCurrentFrameNum
// 描述: 设置文件当前帧播放帧号,此函数必须在文件索引生成之后才能调用.
// 参数: nPort,通道号
//		 nFrameNum,帧序号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetCurrentFrameNum(LONG nPort,DWORD nFrameNum);

//------------------------------------------------------------------------
// 函数: PLAY_GetKeyFramePos
// 描述: 查找指定位置之前的关键帧位置,图像解码必须从关键帧开始，如果用户保存
//			的文件不是从关键帧开始的,那么倒下一个关键帧之前的数据会被忽略.如
//			果用户要截取文件中的一段数据,则应该考虑从关键帧开始截取.结束位置
//			则关系不大,最多丢失3帧数据.在文件索引建立完全的前提下,与
//			PLAY_GetNextKeyFramePos联合使用,可以用来实现剪辑录像文件,剪辑精度
//			与关键帧间隔有关.
// 参数: nPort,通道号
//		 nValue,当前位置,可以是时间或帧号,类型由nType指定.
//		 nType,指定nValue的类型.如果nType是BY_FRAMENUM,则nValue表示帧号;
//			如果nType是BY_FRAMTIME,则nValue表示时间,单位ms.
//		 pFramePos,查找到的关键帧的文件位置信息结构指针,详见PFRAME_POS.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetKeyFramePos(LONG nPort,DWORD nValue, DWORD nType, PFRAME_POS pFramePos);

//------------------------------------------------------------------------
// 函数: PLAY_GetNextKeyFramePos
// 描述: 查找指定位置之后的关键帧位置
// 参数: nPort,通道号
//		 nValue,当前位置,可以是时间或帧号,类型由nType指定.
//		 nType,指定nValue的类型.如果nType是BY_FRAMENUM,则nValue表示帧号;
//			如果nType是BY_FRAMTIME,则nValue表示时间,单位ms.
//		 pFramePos,查找到的关键帧的文件位置信息结构指针,详见PFRAME_POS.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetNextKeyFramePos(LONG nPort,DWORD nValue, DWORD nType, PFRAME_POS pFramePos);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecCBStream
// 描述: 设置解码回调流类型,在PLAY_Play之前调用.
// 参数: nPort,通道号
//		 nStream,1 视频流;2 音频流;3 复合流.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecCBStream(LONG nPort,DWORD nStream);

//------------------------------------------------------------------------
// 函数: PLAY_SetDisplayRegion
// 描述: 设置或增加显示区域,可以做局部放大显示.
// 参数: nPort,通道号
//		 nRegionNum,显示区域序号,0~(MAX_DISPLAY_WND-1),如果为0,则将设置的区域显示在主窗口中.
//		 pSrcRect,局部显示区域
//		 hDestWnd,显示窗口句柄
//		 bEnable,打开(设置)或关闭显示区域
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDisplayRegion(LONG nPort,DWORD nRegionNum, RECT *pSrcRect, HWND hDestWnd, BOOL bEnable);


//------------------------------------------------------------------------
// 函数: PLAY_GetRefValue
// 描述: 获取文件索引,以便下次打开同一个文件时直接使用这个信息.必须在索引建成后才能获得信息.
// 参数: nPort,通道号
//		 pBuffer,索引信息
//		 pSize,输入pBuffer的大小,输出索引信息大小.可以在第一次指定pSize=0,pBuffer=NULL,从
//			pSize的返回值获得需要的缓冲区大小.然后分配足够的缓冲,再调用一次.	 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetRefValue(LONG nPort,BYTE *pBuffer, DWORD *pSize);

//------------------------------------------------------------------------
// 函数: PLAY_SetRefValue
// 描述: 设置文件索引,索引信息及其长度必须准确.如果已经有了文件索引信息,可以
//			不再调用生成索引的回调函数PLAY_SetFileRefCallBackEx,直接输入索引信息.
// 参数: nPort,通道号
//		 pBuffer,索引信息
//		 nSize,索引信息的长度
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetRefValue(LONG nPort,BYTE *pBuffer, DWORD nSize);

//------------------------------------------------------------------------
// 函数: PLAY_RigisterDrawFun
// 描述: 注册一个回调函数,获得当前表面的device context,你可以在这个DC上画图(或文字),
//			就好像在窗口的客户区DC上绘图,但这个DC不是窗口客户区的DC,而是DirectDraw里
//			的Off-Screen表面的DC.注意,如果是使用overlay表面,这个接口无效,可以直接在
//			窗口上绘图,只要不是透明色就不会被覆盖.
// 参数: nPort,通道号
//		 DrawFun,画图回调函数,其参数含义如下:
//			nPort,通道号
//			hDc,OffScreen表面设备上下文.
//			pUserData,用户数据,就是上面输入的用户数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDrawCBFun)(LONG nPort,HDC hDc, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_RigisterDrawFun(LONG nPort, fDrawCBFun DrawCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_ResetBuffer
// 描述: 清空指定缓冲区的剩余数据
// 参数: nPort,通道号
//		 nBufType,如下:
//			BUF_VIDEO_SRC 1 视频源缓冲
//			BUF_AUDIO_SRC 2 音频源缓冲
//			BUF_VIDEO_RENDER 3 解码后视频数据缓冲
//			BUF_AUDIO_RENDER 4 解码后音频数据缓冲
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ResetBuffer(LONG nPort,DWORD nBufType);

//------------------------------------------------------------------------
// 函数: PLAY_GetBufferValue
// 描述: 获取指定缓冲区的大小
// 参数: nPort,通道号
//		 nBufType,如下:
//			BUF_VIDEO_SRC 1 视频源缓冲
//			BUF_AUDIO_SRC 2 音频源缓冲
//			BUF_VIDEO_RENDER 3 解码后视频数据缓冲
//			BUF_AUDIO_RENDER 4 解码后音频数据缓冲
// 返回: DWORD,根据不同参数返回缓冲区值,源缓冲区返回byte,解码后缓冲区返回帧数.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_GetBufferValue(LONG nPort,DWORD nBufType);

//------------------------------------------------------------------------
// 函数: PLAY_AdjustWaveAudio
// 描述: 调整WAVE波形,可以改变声音的大小.它和PLAY_SetVolume的不同在于,它是调整
//			声音数据,只对该路其作用,而PLAY_SetVolume是调整声卡音量,对整个系统起作用.
// 参数: nPort,通道号
//		 nCoefficient,调整的参数,范围从MIN_WAVE_COEF到MAX_WAVE_COEF,0是不调整
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_AdjustWaveAudio(LONG nPort,LONG nCoefficient);

//------------------------------------------------------------------------
// 函数: PLAY_SetAudioCallBack
// 描述: 音频解码后的WAVE数据回调.
// 参数: nPort,通道号
//		 funAudio,音频解码回调函数,其参数含义如下
//			nPort,通道号
//			pAudioBuf,wave格式音频数据
//			nSize,音频数据长度
//			nStamp,时标(ms)
//			nType,音频类型T_AUDIO16, 采样率8000，单声道，每个采样点16位表示
//			pUserData,用户自定义数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK * fAudioCBFun)(LONG nPort, char * pAudioBuf, LONG nSize, LONG nStamp, LONG nType, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetAudioCallBack(LONG nPort, fAudioCBFun AudioCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetEncTypeChangeCallBack
// 描述: 设置图像分辨率改变通知回调,打开文件前使用.
// 参数: nPort,通道号
//		 funEncChange,回调函数
//			nPort,通道号
//			pUserData,用户自定义数据
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void(CALLBACK *fEncChangeCBFun)(LONG nPort, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetEncTypeChangeCallBack(LONG nPort, fEncChangeCBFun EncChangeCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetColor
// 描述: 设置视频参数
// 参数: nPort,通道号
//		 nRegionNum,显示区域,参考PLAY_SetDisplayRegion;如果只有一个显示区域(通常情况)设为0.
//		 nBrightness,亮度,默认64;范围0-128.
//		 nContrast,对比度,默认64;范围0-128.
//		 nSaturation,饱和度,默认64;范围0-128.
//		 nHue,色调,默认64;范围0-128.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetColor(LONG nPort, DWORD nRegionNum, int nBrightness, int nContrast, int nSaturation, int nHue);

//------------------------------------------------------------------------
// 函数: PLAY_GetColor
// 描述: 获取视频参数
// 参数: nPort,通道号
//		 nRegionNum,显示区域,参考PLAY_SetDisplayRegion;如果只有一个显示区域(通常情况)设为0.
//		 pBrightness,亮度,默认64;范围0-128.
//		 pContrast,对比度,默认64;范围0-128.
//		 pSaturation,饱和度,默认64;范围0-128.
//		 pHue,色调,默认64;范围0-128.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetColor(LONG nPort, DWORD nRegionNum, int *pBrightness, int *pContrast, int *pSaturation, int *pHue);

//------------------------------------------------------------------------
// 函数: PLAY_SetEncChangeMsg
// 描述: 设置分辨率改变通知消息
// 参数: nPort,通道号
//		 hWnd,消息发送的窗口
//		 nMsg,用户输入的消息,当解码时编码格式发生改变时用户在hWnd窗口过程中收到这个消息.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetEncChangeMsg(LONG nPort,HWND hWnd,UINT nMsg);

//------------------------------------------------------------------------
// 函数: PLAY_CatchPic
// 描述: 抓图,将BMP图片保存为指定的文件.PLAY_SetDisplayCallBack设置的视频数
//			据回调函数,只有在有视频数据解码出来时才调用,并由用户处理视频数
//			据(如抓图),如果不断有解码的数据,就不断调用这个回调函数.而PLAY_
//			CatchPic一次只抓一幅图,并能在暂停和单帧播放时实现抓图.建议:如果
//			用户想实现抓图(一次抓一幅图),调用PLAY_CatchPic,而如果想得到一段
//			时间内的视频数据,调用PLAY_SetDisplayCallBack.
// 参数: nPort,通道号
//		 sFileName,文件名称
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CatchPic(LONG nPort,char* sFileName) ;

//------------------------------------------------------------------------
// 函数: PLAY_CatchPicEx
// 描述: PLAY_CatchPic的扩展接口,用法请参详PLAY_CatchPic.可指定图片格式
// 参数: nPort,通道号
//		 sFileName,文件名称
//		 ePicfomat,图片格式,见tPicFormats.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CatchPicEx(LONG nPort,char* sFileName,tPicFormats ePicfomat);

//------------------------------------------------------------------------
// 函数: PLAY_SetFileEndCallBack
// 描述: 设置文件结束回调
// 参数: nPort,通道号
//		 pFileEnd,回调函数指针,其参数含义如下:
//			nPort,通道号
//			pUserData,用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK *fpFileEndCBFun)(DWORD nPort, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFileEndCallBack(LONG nPort, fpFileEndCBFun pFileEndCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_StartDataRecord
// 描述: 开始流数据录像,只对流模式有用,在PLAY_Play之后调用.
// 参数: nPort,通道号
//		 sFileName,录像文件名,如果文件名中有不存在的文件夹,就创建该文件夹.
//		 idataType,0表示原始视频流;1表示转换成AVI格式;2表示转换成ASF格式;3分段保存文件
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StartDataRecord(LONG nPort, char *sFileName, int idataType);

//------------------------------------------------------------------------
// 函数: PLAY_WriteData
// 描述: 保存原始码流
// 参数: nPort,通道号
//		 pBuf,流数据
//		 nSize,大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_WriteData(LONG nPort, PBYTE pBuf,DWORD nSize);

//------------------------------------------------------------------------
// 函数: PLAY_StopDataRecord
// 描述: 停止流数据录像
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopDataRecord(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetPlaySpeed
// 描述: 改变图像播放的速度,
// 参数: nPort,通道号
//		 fCoff,播放速度,范围[1/64~64.0],小于1表示慢放，大于1表示正放.
//			   当播放速度较快时可能会抽帧播放.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetPlaySpeed(LONG nPort, float fCoff);

//------------------------------------------------------------------------
// 函数: PLAY_SetDemuxCallBack
// 描述: 源数据分析完的数据回调
// 参数: nPort,通道号
//		 DecCBFun,分析数据回调指针
//			nPort,通道号
//			pBuf,数据指针
//			nSize,数据长度
//			pParam,帧信息  指向DEMUX_INFO这个结构体 
//			nReserved,保留
//			pUserData,用户自定义数据
//		 pUserData,用户自定义数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDemuxDecCBFun)(LONG nPort,char * pBuf,	LONG nSize,void * pParam,void* pReserved, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDemuxCallBack(LONG nPort, fDemuxDecCBFun DecCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_QueryInfo
// 描述: 目前实现了对当前时间和帧率信息的查询.
// 参数: nPort,通道号
//		 cmdType,指定状态查询指令,如下
//			PLAY_CMD_GetTime   获取编码中时间信息,单位ms.
//			PLAY_CMD_GetFileRate  获取帧率信息
//			PLAY_CMD_GetMediaInfo  获取媒体信息,信息结构为 MEDIA_INFO
//		 buf,存放信息的缓冲
//		 buflen,缓冲长度
//		 returnlen,获取的信息的有效数据长度
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_QueryInfo(LONG nPort , int cmdType, char* buf, int buflen, int* returnlen);

//------------------------------------------------------------------------
// 函数: PLAY_OpenAudioRecord
// 描述: 打开音频采集功能.
// 参数: pProc,音频采集数据回调指针,其参数含义如下:
//			pDataBuffer,回调数据指针
//			DataLength,回调数据长度
//			pUserData,用户数据
//		 nBitsPerSample,表示每个采样所需要的位数
//		 nSamplesPerSec,采样率
//		 nLength,数据缓冲的长度
//		 nReserved,保留
//		 pUserData,用户自定义数据
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK *pCallFunction)(LPBYTE pDataBuffer, DWORD DataLength, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_OpenAudioRecord(pCallFunction pProc, LONG nBitsPerSample, LONG nSamplesPerSec, LONG nLength, LONG nReserved, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_CloseAudioRecord
// 描述: 关闭音频采集功能
// 参数: 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CloseAudioRecord();

//------------------------------------------------------------------------
// 函数: PLAY_SetWaterMarkCallBack
// 描述: 设置水印数据回调
// 参数: nPort,通道号
//		 pFunc,水印信息获取回调函数,其参数含义如下:
//			buf,水印数据buffer指针
//			key,区分不同水印信息
//			len,缓冲的最大长度
//			reallen,缓冲的实际长度
//			type,数值范围[0,3],含义如下
//				0  I帧帧数据水印信息
//				1  帧水印
//				2  水印校验 
//				3  智能分析帧
//		 pUserData,用户自定义参数 
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef int (CALLBACK* GetWaterMarkInfoCallbackFunc)(char* buf, LONG key, LONG len, LONG reallen, LONG type, void* pUserData);	
PLAYSDK_API BOOL CALLMETHOD PLAY_SetWaterMarkCallBack(LONG nPort, GetWaterMarkInfoCallbackFunc pFunc, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_CreateFile
// 描述: 打开文件并自动分配通道号
// 参数: sFileName,文件名
// 返回: DWORD,失败返回0,否则返回通道号.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_CreateFile(LPSTR sFileName);

//------------------------------------------------------------------------
// 函数: PLAY_DestroyFile
// 描述: 关闭播放文件并释放自动分配的通道号.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_DestroyFile(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_CreateStream
// 描述: 打开流接口并自动分配通道号
// 参数: nBufPoolSize,置播放器中存放数据流的缓冲区大小,范围是[SOURCE_BUF_MIN,SOURCE_BUF_MAX].
// 返回: DWORD,失败返回0,否则返回通道号.
//------------------------------------------------------------------------
PLAYSDK_API DWORD CALLMETHOD PLAY_CreateStream(DWORD nBufPoolSize);

//------------------------------------------------------------------------
// 函数: PLAY_DestroyStream
// 描述: 关闭数据流,并释放自动分配的通道号.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_DestroyStream(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_GetFreePort
// 描述: 获取空闲通道号,上限为501.与PLAY_ReleasePort匹对使用.
// 参数: plPort,输出参数,返回获取的通道号.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetFreePort(LONG *plPort);

//------------------------------------------------------------------------
// 函数: PLAY_ReleasePort
// 描述: 释放通道号,与PLAY_RealsePort匹对使用.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_ReleasePort(LONG lPort);

//------------------------------------------------------------------------
// 函数: PLAY_VerticalSyncEnable
// 描述: 垂直同步显示开关.此接口需在PLAY_Play之后调用,重新播放时需重新设置.
//			在播放动态图像出现断层时,可以使用此接口打开垂直同步功能.
// 参数: nPort,通道号
//		 bEnable,TRUE打开垂直同步;FALSE关闭垂直同步.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API	BOOL CALLMETHOD PLAY_VerticalSyncEnable(LONG nPort, BOOL bEnable);

//------------------------------------------------------------------------
// 函数: PLAY_GetPicBMP
// 描述: 抓取BMP图像
// 参数: nPort,通道号
//		 pBmpBuf,存放BMP图像数据的缓冲地址,由用户分配,不得小于bmp 图像大小,
//			推荐大小:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
//			其中w和h分别为图像宽高.
//		 dwBufSize,缓冲区大小
//		 pBmpSize,获取到的实际bmp 图像大小
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPicBMP(LONG nPort, PBYTE pBmpBuf, DWORD dwBufSize, DWORD* pBmpSize);

//------------------------------------------------------------------------
// 函数: PLAY_GetPicBMPEx
// 描述: 抓取BMP图像
// 参数: nPort,通道号
//		 pBmpBuf,存放BMP图像数据的缓冲地址,由用户分配,不得小于bmp 图像大小,
//			推荐大小:sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+w*h*4,
//			其中w和h分别为图像宽高.
//		 dwBufSize,缓冲区大小
//		 pBmpSize,获取到的实际bmp 图像大小
//		 nWidth,指定的bmp的宽
//		 nHeight,指定的bmp的高
//		 nRgbType,指定RGB格式 0：RGB32;1：RGB24;
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPicBMPEx(LONG nPort, PBYTE pBmpBuf, DWORD dwBufSize, DWORD* pBmpSize, LONG nWidth, LONG nHeight, int nRgbType);

//------------------------------------------------------------------------
// 函数: PLAY_GetPicJPEG
// 描述: 抓取JPEG图像.
// 参数: nPort,通道号
//		 pJpegBuf,存放JPEG图像数据的缓冲地址,由用户分配,不得小于JPEG图像大小,
//			推荐大小:w*h*3/2,其中w和h分别为图像宽高.
//		 dwBufSize,缓冲区大小.
//		 pJpegSize,获取到的实际JPEG图像大小.
//		 quality,JPEG图像的压缩质量,取值范围为(0,100].
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetPicJPEG(LONG nPort, PBYTE pJpegBuf, DWORD dwBufSize, DWORD* pJpegSize, int quality);

//------------------------------------------------------------------------
// 函数: PLAY_SetVisibleDecodeCallBack
// 描述: 解码回调.与PLAY_SetDecodeCallBack基本相同,不同的是解码回调的同时可以
//			显示视频,建议不要在回调函数里面做长时间的逻辑处理,以免增加显示的延时.
// 参数: nPort,通道号
//		 cbDec,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pFrameDecodeInfo,解码后的音视频数据
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			pUser,用户自定义参数
//		 pUser,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
PLAYSDK_API BOOL CALLMETHOD PLAY_SetVisibleDecodeCallBack(LONG nPort, fCBDecode cbDec, void* pUser);

//------------------------------------------------------------------------
// 函数: PLAY_RigisterDrawFunEx
// 描述: 注册一个回调函数,获得当前表面的上下文(HDC),这个DC不是窗口客户区的DC,
//			而是DirectDraw里的Off-Screen表面的DC.注意.如果是使用overlay表面,
//			这个接口无效,overlay方式可以直接在窗口上绘图,只要不是透明色就不会被覆盖.
// 参数: nPort,通道号
//		 nReginNum,显示区域序号,范围[0,(MAX_DISPLAY_WND-1)].如果nRegionNum为0,
//			则将设置的区域显示在主窗口中.
//		 DrawFunEx,画图回调函数,其参数含义如下:
//			nPort,通道号
//			hDc,Off-Screen表面的设备上下文(DC)
//			pUserData,用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDrawCBFunEx)(LONG nPort,LONG nReginNum,HDC hDc, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_RigisterDrawFunEx(LONG nPort, LONG nReginNum, fDrawCBFunEx DrawFunEx, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_CatchResizePic
// 描述: 抓图,可选择格式并指定宽高.
// 参数: nPort,通道号
//		 sFileName,文件名称
//		 lTargetWidth,指定的图像宽度
//		 lTargetHeight,指定的图像高度
//		 ePicfomat,图片格式,详见tPicFomats
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_CatchResizePic(LONG nPort, char* sFileName, LONG lTargetWidth, LONG lTargetHeight, tPicFormats ePicfomat);

//------------------------------------------------------------------------
// 函数: PLAY_GetRealFrameBitRate
// 描述: 获取视频实时码率
// 参数: nPort,通道号
//		 pBitRate,输出参数,返回视频码率.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetRealFrameBitRate(LONG nPort, double* pBitRate);

//------------------------------------------------------------------------
// 函数: PLAY_StartAVIResizeConvert
// 描述: 开始AVI数据录像,可指定视频宽高.只对流模式有用,在PLAY_Play之后调用.
// 参数: nPort,通道号
//		 sFileName,录像文件名,如果文件名中有不存在的文件夹,就创建该文件夹.
//		 lWidth,视频格式宽度
//		 lHeight,视频格式高度
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StartAVIResizeConvert(LONG nPort, char *sFileName, LONG lWidth, LONG lHeight);

//------------------------------------------------------------------------
// 函数: PLAY_StopAVIResizeConvert
// 描述: 停止AVI数据录像,与PLAY_StartAVIResizeConvert匹对使用.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopAVIResizeConvert(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetFileRefCallBackEx
// 描述: 设置建立索引回调,以返回索引创建情况.
// 参数: nPort,通道号
//		 pFileRefDoneEx,回调函数指针,其参数含义如下:
//			nPort,通道号
//			bIndexCreated,索引创建标志,TRUE索引创建成功;FALSE失败.
//			pUserData,用户自定义参数
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK *fpFileRefDoneCBFunEx)(DWORD nPort, BOOL bIndexCreated, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetFileRefCallBackEx(LONG nPort, fpFileRefDoneCBFunEx pFileRefDoneCBFunEx, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_StartAVIConvert
// 描述: 开始AVI转换并设置AVI转换状态回调.
// 参数: nPort,通道号
//		 pAVIFunc,回调函数,其参数含义如下:
//			nPort,通道号
//			lMediaChangeType,AVI_MEDIACHANGE_FRAMERATE表示帧率改变;AVI_MEDIACHANGE_RESOLUTION表示分辨率改变
//			lUser,用户自定义参数
//			pbIfContinue,TRUE继续转换;FALSE 停止转换
//			sNewFileName,如果继续转换,新的录像文件名
//		 lUser,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* AVIConvertCallback)(LONG nPort, LONG lMediaChangeType, void* pUserData, BOOL *pbIfContinue, char *sNewFileName);
PLAYSDK_API BOOL CALLMETHOD PLAY_StartAVIConvert(LONG nPort, char *sFileName, AVIConvertCallback pAVIFunc, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_StopAVIConvert
// 描述: 停止AVI转换.
// 参数: nPort,通道号
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_StopAVIConvert(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetWaterMarkCallBackEx
// 描述: 设置水印数据回调.
// 参数: nPort,通道号
//		 pFunc,函数指针,其参数含义如下:
//			nPort,通道号
//			buf,水印数据缓冲
//			lTimeStamp,水印的时间戳
//			lInfoType,水印信息类型,有三种类型,WATERMARK_DATA_TEXT,
//				WATERMARK_DATA_JPEG_BMP,WATERMARK_DATA_FRAMEDATA
//		 len,缓冲的最大长度
//		 reallen,缓冲的实际长度
//		 lCheckResult,1没有错误;2水印错误;3帧数据错误;4帧号错误
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef int (CALLBACK* GetWaterMarkInfoCallbackFuncEx)(LONG nPort, char* buf, LONG lTimeStamp, LONG lInfoType, LONG len, LONG reallen, LONG lCheckResult, void* pUserData);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetWaterMarkCallBackEx(LONG nPort, GetWaterMarkInfoCallbackFuncEx pFunc, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetAudioRecScaling
// 描述: 设置音频采集数据缩放比例
// 参数: fRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetAudioRecScaling(float fRatio);

//------------------------------------------------------------------------
// 函数: PLAY_GetAudioRecScaling
// 描述: 获取音频采集数据缩放比例
// 参数: pfRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetAudioRecScaling(float* pfRatio);

//------------------------------------------------------------------------
// 函数: PLAY_SetAudioRenderScaling
// 描述: 设置音频解码缩放比例
// 参数: nPort,通道号
//		 fRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetAudioRenderScaling(LONG nPort, float fRatio);

//------------------------------------------------------------------------
// 函数: PLAY_GetAudioRenderScaling
// 描述: 获取音频解码缩放比例
// 参数: nPort,通道号
//		 pfRatio,缩放比例.大于0小于1为音频缩小;1为原始音频;大于1为音频放大.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_GetAudioRenderScaling(LONG nPort, float* pfRatio);

//------------------------------------------------------------------------
// 函数: PLAY_SetRotateAngle
// 描述: 图像翻转
// 参数: nrotateType,翻转类型,范围[0,3]. 0不旋转;1旋转90度;2旋转180度;3旋转270度.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetRotateAngle(LONG nPort , int nrotateType);

//------------------------------------------------------------------------
// 函数: PLAY_SetDelayTime
// 描述: 设置延迟时间
// 参数: nDelay,延迟时间，缓冲多少时间开始播放，缓冲小于此值开始稍微慢放
//       nThreshold,阀值时间，到达阀值开始稍微快放
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDelayTime(LONG nPort, int nDelay, int nThreshold);

//------------------------------------------------------------------------
// 函数: PLAY_BackOne
// 描述: 与PLAY_OneByOneBack重复，请使用PLAY_OneByOneBack
// 参数: 
// 返回: 
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_BackOne(LONG nPort);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecCallBack(建议使用PLAY_SetDecodeCallBack)
// 描述: 设置解码回调,替换播放器中的显示部分,由用户自己控制显示,该函数在
//			PLAY_Play之前调用,在PLAY_Stop时自动失效,下次调用PLAY_Play之前
//			需要重新设置.解码部分不控制速度,只要用户从回调函数中返回,解码器
//			就会解码下一部分数据.适用于只解码不显示的情形。
// 参数: nPort,通道号
//		 DecCBFun,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pBuf,解码后的音视频数据
//			nSize,解码后的音视频数据pBuf的长度
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			nReserved1,保留参数
//			nReserved2,保留参数	
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fDecCBFun)(LONG nPort,char * pBuf,LONG nSize,FRAME_INFO * pFrameInfo, void* pUserData, LONG nReserved2);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecCallBack(LONG nPort, fDecCBFun DecCBFun);

//------------------------------------------------------------------------
// 函数: PLAY_SetDecCallBackEx(建议使用PLAY_SetDecodeCallBack)
// 描述: 设置解码回调,替换播放器中的显示部分,由用户自己控制显示,该函数在
//			PLAY_Play之前调用,在PLAY_Stop时自动失效,下次调用PLAY_Play之前
//			需要重新设置.解码部分不控制速度,只要用户从回调函数中返回,解码器
//			就会解码下一部分数据.适用于只解码不显示的情形。
// 参数: nPort,通道号
//		 DecCBFun,解码回调函数指针,不能为NULL.回调函数参数含义如下:
//			nPort,通道号
//			pBuf,解码后的音视频数据
//			nSize,解码后的音视频数据pBuf的长度
//			pFrameInfo,图像和声音信息,请参见FRAME_INFO结构体
//			nReserved1,用户自定义参数
//			nReserved2,保留参数	
//		 pUserData,用户自定义参数
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
PLAYSDK_API BOOL CALLMETHOD PLAY_SetDecCallBackEx(LONG nPort, fDecCBFun DecCBFun, void* pUserData);

//------------------------------------------------------------------------
// 函数: PLAY_SetVisibleDecCallBack(建议使用PLAY_SetVisibleDecodeCallBack)
// 描述: 解码回调.与PLAY_SetDecCallBackEx基本相同,不同的是解码回调的同时可以
//			显示视频,建议不要在回调函数里面做长时间的逻辑处理,以免增加显示的延时.
// 参数: nPort,通道号
//		 DecCBFun,回调函数指针,不能为NULL,其参数含义如下:
//			nPort,通道号
//			pBuf,解码后的音视频数据
//			nSize,数据长度
//			pFrameInfo,帧信息,详见FRAME_INFO结构体.
//			pUserData,用户自定义参数
//			pReserved1,保留参数.
//		 pUserData,用户自定义参数.
// 返回: BOOL,成功返回TRUE,失败返回FALSE.
//------------------------------------------------------------------------
typedef void (CALLBACK* fVisibleDecCBFun)(LONG nPort,char * pBuf,LONG nSize,FRAME_INFO * pFrameInfo, void* pUserData, LONG nReserved1);
PLAYSDK_API BOOL CALLMETHOD PLAY_SetVisibleDecCallBack(LONG nPort, fVisibleDecCBFun DecCBFun, void* pUserData);
#ifdef __cplusplus
}
#endif

#endif
