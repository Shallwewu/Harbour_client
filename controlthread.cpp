#include "controlthread.h"

#include "detailcontrol.h"
#include "camera.h"

extern People WTpp;
extern int flag_ocrUpdate;

extern People ICpp;
extern int flag_icUpdate;
extern int flag_door;
bool toOpen = false;

extern Car Cardriver;

extern int pupdate;

controlthread::controlthread(QObject *parent)
	: QThread(parent)
{
	getDoorConditionFlag = -1;
	timeBegin = time(NULL);
	timeEnd = time(NULL);
}

controlthread::~controlthread()
{

}

void controlthread::run()  //轮询外部硬件的触发
{
	while (true)
	{
		if (flag_icUpdate == 1)
		{
			////ic卡刷卡机测试
			//QString toshow = QString::fromStdString(ICpp.peopleinfo.number);
			//QThread::sleep(1);
			//emit threadtest_signal(toshow);
			//flag_icUpdate = 0;
		}
		if (flag_ocrUpdate == 1)
		{
			//QString toshow = QString::fromStdString(WTpp.peopleinfo.number);
			//emit threadtest_signal(toshow);

			//文通刷卡机信息传递
			//QThread::sleep(1);
			QString toshowName = QString::fromLocal8Bit(WTpp.peopleinfo.name.c_str(),-1);
			emit peopleName_signal(toshowName);
			QThread::sleep(0.2);
			QString toshowType = QString::fromStdString(WTpp.peopleinfo.type);
			emit peopleType_signal(toshowType);
			QThread::sleep(0.2);
			QString toshowNo = QString::fromLocal8Bit(WTpp.peopleinfo.number.c_str(),-1);
			emit peopleNo_signal(toshowNo);
			QThread::sleep(0.2);
			QString toshowShip = QString::fromLocal8Bit(WTpp.peopleinfo.ship.c_str(),-1);
			emit peopleShip_signal(toshowShip);
			QThread::sleep(0.2);

			flag_ocrUpdate = 0;
		}
		if (WTpp.flag_snap == 1)
		{
			WTpp.flag_snap = 0;
			emit snapPeopleImage_signal();
		}
		if (Cardriver.flagdri_snap == 1)
		{
			Cardriver.flagdri_snap = 0;
			emit driverSnapShow_signal();
		}
		if (Cardriver.flag_traffic == 1)
		{
			Cardriver.flag_traffic = 0;
			inputLPnumber = Cardriver.plate;
			Cardeal(inputLPnumber);     //可用于自动识别车牌 或手动输入车牌

			emit carInformatin_signal_withPic();
		}
		//if (getDoorConditionFlag == 1)
		//{
		//	timeEnd = time(NULL);
		//	doorTemp = getDoorCondition(1);
		//	if (doorTemp == -1 || (timeEnd - timeBegin)>10)
		//	{
		//		//error
		//		getDoorConditionFlag = 0;
		//	}
		//	else if (doorTemp == 1)
		//	{
		//		getDoorConditionFlag = 0;
		//		snapandsave(1);
		//	}
		//}
		//if (getDoorConditionFlag == 2)
		//{
		//	timeEnd = time(NULL);
		//	doorTemp = getDoorCondition(2);
		//	if (doorTemp == -1 || (timeEnd - timeBegin)>10)
		//	{
		//		//error
		//		getDoorConditionFlag = 9;
		//	}
		//	else if (doorTemp == 1)
		//	{
		//		getDoorConditionFlag = 5;
		//		snapandsave(2);
		//	}
		//test code
		//QString toshow = QString::number(getDoorConditionFlag,10);
		//sleep(1);
		//emit threadtest_signal(toshow);
		//}
	}
}

//按键响应函数
void controlthread::peopleIn_slot()
{
	//toOpen = true;
	//confiopendoor(1);
	//doorConditionInit();
	//getDoorConditionFlag = 1;
	//timeBegin = time(NULL);
	//flag_door = 3; //WT in
	if (pupdate)
	{
		if (!WTpp.WTvalid())
		{
			if (QMessageBox::warning(NULL, tr("警告！"), tr("证件超期或船已离岗。是否放行？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			{
				//confiopendoor(1);
				return;
			}
			//else
			//	return;
		}

		WTpassstate(1);
		QString warnInf = QString::fromLocal8Bit(WTpp.peopleinfo.inout.c_str(),-1);
		if (warnInf != "进门")
		{
			//QString warnInf = QString::fromStdString(WTpp.peopleinfo.inout);
			//QString warnInf = QString::fromLocal8Bit(WTpp.peopleinfo.inout.c_str(),-1);
			if (QMessageBox::warning(NULL, tr("警告！"), warnInf, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			{
				//confiopendoor(1);
				return;
			}
			//else
			//	return;
		} 
		confiopendoor(1);
	}
	else
		confiopendoor(1);
//	pupdate = 0;   
}

void controlthread::peopleOut_slot()
{
	//toOpen = true;
	//confiopendoor(2);
	//doorConditionInit();
	//getDoorConditionFlag = 2;
	//timeBegin = time(NULL);
	//flag_door = 4; //WT out
	if (pupdate)
	{
		if (!WTpp.WTvalid())
		{
			if (QMessageBox::warning(NULL, tr("警告！"), tr("证件超期或船已离岗。是否放行？"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			{
				//confiopendoor(2);
				return;
			}
			//else
			//	return;
		}

		WTpassstate(2);
		QString warnInf = QString::fromLocal8Bit(WTpp.peopleinfo.inout.c_str(),-1);
		if (warnInf != "出门")
		{
			if (QMessageBox::warning(NULL, tr("警告！"), warnInf, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
			{
				//confiopendoor(1);
				return;
			}
			//else
			//	return;
		} 
		confiopendoor(2);
	}
	else
		confiopendoor(2);
//	pupdate = 0;
}

void controlthread::driverSnap_slot(const int inout)
{
	//using the snap function to capture image
	snap(inout);
}

void controlthread::carInfRequire_slot(const QString LPnumber)
{	inputLPnumber = (const char *)LPnumber.toLocal8Bit();
Cardeal(inputLPnumber);     //可用于自动识别车牌 或手动输入车牌

emit carInformatin_signal();
}

void controlthread::gateUp_slot()
{
	confiopengate();
}

void controlthread::gateDown_slot()
{
	closegate();
}

void controlthread::getLPinformation_slot()
{
	//////////////////////////////////////////////////////////////////////////get the LP information
	Cardriver.driverclear();
	Getallplate();
	emit LPinformationShow_signal();
}

void controlthread::otherDriverChange_slot(const QString &otherDriver)
{
	otherDriverName = (const char *)otherDriver.toLocal8Bit();
	Chosedri(otherDriverName);
	emit otherDriverInf_signal();
}
