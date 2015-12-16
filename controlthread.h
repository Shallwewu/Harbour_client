#ifndef CONTROLTHREAD_H
#define CONTROLTHREAD_H

#include <QThread>
#include <QMessageBox> 
#include <QString>
#include <QTextCodec>
#include <time.h> 

//by Damon 20150725
#include "detailcontrol.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class controlthread : public QThread
{
	Q_OBJECT

public:
	controlthread(QObject *parent);
	~controlthread();

private:
	string otherDriverName;
	string inputLPnumber;
	int getDoorConditionFlag;
	int doorTemp;
	time_t timeBegin, timeEnd;

protected:
	void run();

signals:
	void threadtest_signal(const QString s);

	//文通刷卡机信息传递
	void peopleName_signal(const QString s);
	void peopleType_signal(const QString s);
	void peopleNo_signal(const QString s);
	void peopleShip_signal(const QString s);

	//车辆进出信息显示
	void carInformatin_signal();
	void carInformatin_signal_withPic();
	void otherDriverInf_signal();
	void snapPeopleImage_signal();
	void driverSnapShow_signal();

	//车牌信息显示
	void LPinformationShow_signal();

	private slots:
		void peopleIn_slot();
		void peopleOut_slot();
		void gateUp_slot();
		void gateDown_slot();
		void otherDriverChange_slot(const QString &otherDriver);
		void carInfRequire_slot(const QString LPnumber);
		void driverSnap_slot(const int inout);
		void getLPinformation_slot();
};

#endif // CONTROLTHREAD_H
