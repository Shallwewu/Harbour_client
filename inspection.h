#ifndef INSPECTION_H
#define INSPECTION_H

#include <QtWidgets/QMainWindow>
#include "ui_inspection.h"
#include <QTimer>
#include <QTime>
#include <QMessageBox>

#include "controlthread.h"
#include "detailcontrol.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class Inspection : public QMainWindow
{
	Q_OBJECT

public:
	Inspection(QWidget *parent = 0);
	~Inspection();

private:
	Ui::InspectionClass ui;

public:

private:
	void widgetAbleInit();
	void peopleMessageShow();
	void peopleMessageGet();
	//ui的数据清空
	void carUIclear();
	void peopleUIclear();

	private slots: //slots fot UI
		void on_driverType(const QString &); //choose driver type
		void showTime();  //显示时间
		void on_manualOperatButton_clicked();
		void on_manualConfirmButton_clicked();
		void on_peopleInButton_clicked();
		void on_peopleOutButton_clicked();
		void on_driverPhotoButton_clicked();
		void on_driverOutPhotoButton_clicked();
		void on_gateOpenButton_clicked();
		void on_gateCloseButton_clicked();
		void on_gateOutOpenButton_clicked();
		void on_gateOutCloseButton_clicked();

private:
	bool manualOperateFlag;
	controlthread *control;

	private slots:
		void threadtest_slot(const QString s);

		//文通刷卡机信息显示
		void peopleName_slot(const QString s);
		void peopleType_slot(const QString s);
		void peopleNo_slot(const QString s);
		void peopleShip_slot(const QString s);
		void snapPeopleImage_slot();

		//车辆进出信息显示
		void carInformatin_slot();
		void carInformatin_slot_withPic();
		void otherDriverInf_slot();
		void driverSnapShow_slot();

		//显示车牌信息
		void LPinformationShow_slot();

signals:
		void peopleIn_signal();
		void peopleOut_signal();
		void gateUp_signal();
		void gateDown_signal();
		void carInfRequire_signal(const QString LPnumber);
		void driverSnap_signal(const int inout);
		void getLPinformation_signal();
};

#endif // INSPECTION_H
