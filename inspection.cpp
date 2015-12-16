#include "inspection.h"

extern People ICpp;
extern People WTpp;
extern Car Cardriver;

Inspection::Inspection(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->showFullScreen();

	//控制模块线程
	control = new controlthread(this);
	connect(control, SIGNAL(threadtest_signal(const QString )), this, SLOT(threadtest_slot(const QString)));

	//文通信息显示
	connect(control, SIGNAL(peopleName_signal(const QString )), this, SLOT(peopleName_slot(const QString)));
	connect(control, SIGNAL(peopleType_signal(const QString )), this, SLOT(peopleType_slot(const QString)));
	connect(control, SIGNAL(peopleNo_signal(const QString )), this, SLOT(peopleNo_slot(const QString)));
	connect(control, SIGNAL(peopleShip_signal(const QString )), this, SLOT(peopleShip_slot(const QString)));
	connect(control, SIGNAL(snapPeopleImage_signal()), this, SLOT(snapPeopleImage_slot()));

	//车辆进出信息显示
	connect(control, SIGNAL(carInformatin_signal( )), this, SLOT(carInformatin_slot( )));
	connect(control, SIGNAL(carInformatin_signal_withPic( )), this, SLOT(carInformatin_slot_withPic( )));
	connect(control, SIGNAL(otherDriverInf_signal( )), this, SLOT(otherDriverInf_slot( )));
	connect(control, SIGNAL(driverSnapShow_signal( )), this, SLOT(driverSnapShow_slot( )));

	//按键响应
	connect(this, SIGNAL(peopleIn_signal()), control, SLOT(peopleIn_slot()));
	connect(this, SIGNAL(peopleOut_signal()), control, SLOT(peopleOut_slot()));
	connect(this, SIGNAL(gateUp_signal()), control, SLOT(gateUp_slot()));
	connect(this, SIGNAL(gateDown_signal()), control, SLOT(gateDown_slot()));
	connect(this, SIGNAL(carInfRequire_signal(const QString)), control, SLOT(carInfRequire_slot(const QString)));
	connect(this, SIGNAL(driverSnap_signal(const int)), control, SLOT(driverSnap_slot(const int)));

	//获取车牌信息
	connect(this, SIGNAL(getLPinformation_signal()), control, SLOT(getLPinformation_slot()));
	connect(control, SIGNAL(LPinformationShow_signal()), this, SLOT(LPinformationShow_slot()));


	control->start();


	///////////////////////////////////////////////////////about ui
	//the background setting
	QPixmap pixmap = QPixmap("./images/background3.png").scaled(this->size());
	QPalette palette;
	palette.setBrush(this->backgroundRole(), QBrush(pixmap));
	//palette.setBrush(QPalette::Background, QBrush(pixmap));
	this->setPalette(palette);
	//this->setAutoFillBackground(true);

	//loga 
	QImage img;
	img.load("./images/slogan3.png");
	QImage loga = img.scaled(ui.logaLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	ui.logaLabel->setPixmap(QPixmap::fromImage(loga));

	//digital clock
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
	timer->start(500);    //启动定时器

	//choose license plat
	ui.LPcomboBox->setEditable(true);

	//choose the driver
	ui.car_comboBox->addItem("");
	ui.car_comboBox->addItem("主司机");
	ui.car_comboBox->addItem("副司机");
	ui.car_comboBox->addItem("其他司机");
	connect(ui.car_comboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(on_driverType(const QString &)));
	connect(ui.odcomboBox, SIGNAL(currentIndexChanged(const QString &)), control, SLOT(otherDriverChange_slot(const QString &)));
	//other driver choose
	ui.odcomboBox->setEditable(true);


	//关于一些控件是否可编辑
	widgetAbleInit();

	//others
	manualOperateFlag = false;

}

Inspection::~Inspection()
{

}

//initial the able/disable of widget
void Inspection::widgetAbleInit()
{
	ui.LPcomboBox->setEnabled(false);
	ui.LPEdit->setEnabled(false);
	ui.companyEdit->setEnabled(false);
	ui.carTypeEdit->setEnabled(false);
	ui.car_comboBox->setEnabled(false);

	ui.zdNameEdit->setEnabled(false);
	ui.zdGenderEdit->setEnabled(false);
	ui.zdIDEdit->setEnabled(false);
	ui.zdDLEdit->setEnabled(false);

	ui.fdNameEdit->setEnabled(false);
	ui.fdGenderEdit->setEnabled(false);
	ui.fdIDEdit->setEnabled(false);
	ui.fdDLEdit->setEnabled(false);

	ui.odcomboBox->setEnabled(false);
	ui.odGenderEdit->setEnabled(false);
	ui.odIDEdit->setEnabled(false);
	ui.odDLEdit->setEnabled(false);

	ui.nameEdit->setEnabled(false);
	ui.certificateTypeEdit->setEnabled(false);
	ui.certificateNoEdit->setEnabled(false);
	ui.validPeriodEdit->setEnabled(false);
	ui.shipEdit->setEnabled(false);

	ui.manualConfirmButton->setEnabled(false);
	ui.driverPhotoButton->setEnabled(false);
	ui.driverOutPhotoButton->setEnabled(false);

	//set the front
	ui.LPcomboBox->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.LPEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.companyEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.carTypeEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.car_comboBox->setFont(QFont("Timers" , 11 ,  QFont::Bold));

	ui.zdNameEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.zdGenderEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.zdIDEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.zdDLEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));

	ui.fdNameEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.fdGenderEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.fdIDEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.fdDLEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));

	ui.odcomboBox->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.odGenderEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.odIDEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.odDLEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));

	ui.nameEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.certificateTypeEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.certificateNoEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.validPeriodEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.shipEdit->setFont(QFont("Timers" , 11 ,  QFont::Bold));

	ui.gateOpenButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.gateCloseButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.gateOutOpenButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.gateOutCloseButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.driverPhotoButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.driverOutPhotoButton->setFont(QFont("Timers" , 11 , QFont::Bold));
	ui.manualOperatButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.manualConfirmButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.peopleInButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.peopleOutButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
	ui.quitButton->setFont(QFont("Timers" , 11 ,  QFont::Bold));
}

//show the current time
void Inspection::showTime()
{
	QTime time = QTime::currentTime();
	QString txtTime = time.toString("hh:mm:ss");
	ui.clock->display(txtTime);
}

//choose driver type
void Inspection::on_driverType(const QString &driverType)
{
	int index = ui.car_comboBox->currentIndex();
	//QString s = QString::number(index, 10);
	//QMessageBox::information(NULL, tr("Warning!"), s);
	switch (index)
	{
	case 1:
		{ 
			ui.odcomboBox->setEnabled(false);
			ui.odGenderEdit->setEnabled(false);
			ui.odIDEdit->setEnabled(false);
			ui.odDLEdit->setEnabled(false);
			Cardriver.vehicle.driver=Cardriver.firstdri.name;
			Cardriver.vehicle.cardtype=Cardriver.firstdri.cardtype;
			Cardriver.vehicle.number=Cardriver.firstdri.number;
			break; 
		}
	case 2:
		{ 
			ui.odcomboBox->setEnabled(false);
			ui.odGenderEdit->setEnabled(false);
			ui.odIDEdit->setEnabled(false);
			ui.odDLEdit->setEnabled(false);
			Cardriver.vehicle.driver=Cardriver.seconddri.name;
			Cardriver.vehicle.cardtype=Cardriver.seconddri.cardtype;
			Cardriver.vehicle.number=Cardriver.seconddri.number;
			break; 
		}
	case 3:
		{ 
			ui.odcomboBox->setEnabled(true);
			ui.odGenderEdit->setEnabled(true);
			ui.odIDEdit->setEnabled(true);
			ui.odDLEdit->setEnabled(true);
			break; 
		}
		break;
	}
}


//手动添加或者取消手动添加
void Inspection::on_manualOperatButton_clicked()
{
	if (manualOperateFlag == false)
	{
		manualOperateFlag = true;
		ui.manualOperatButton->setText("取消手动输入");
		ui.manualConfirmButton->setEnabled(true);

		ui.LPcomboBox->setEnabled(true);
		ui.LPEdit->setEnabled(true);
		ui.driverPhotoButton->setEnabled(false);
		ui.driverOutPhotoButton->setEnabled(false);
		carUIclear();
		//ui.companyEdit->setEnabled(true);
		//ui.carTypeEdit->setEnabled(true);

		//add the existing LPs
		emit getLPinformation_signal();
	}
	else
	{
		manualOperateFlag = false;
		ui.manualOperatButton->setText("手动添加");
		ui.manualConfirmButton->setEnabled(false);

		ui.LPcomboBox->setEnabled(false);
		ui.LPEdit->setEnabled(false);
		//ui.companyEdit->setEnabled(false);
		//ui.carTypeEdit->setEnabled(false);

		ui.LPcomboBox->clear();
		ui.LPEdit->setText("");
		ui.companyEdit->setText("");
		ui.carTypeEdit->setText("");
	}
}
//显示车牌下拉框信息
void Inspection::LPinformationShow_slot()
{
	ui.LPcomboBox->clear();	//////////////////////////////////////////////////////////////////////////error
	ui.LPcomboBox->addItem("");
	QString toshow;
	//for (int i = 0; i < **; i++)
	//{
	//	toshow = QString::fromLocal8Bit(Cardriver.alldriname.driname[i].c_str(), -1);
	//	ui.LPcomboBox->addItem(toshow);
	//}
}
//确认手动添加的数据
void Inspection::on_manualConfirmButton_clicked()
{
	manualOperateFlag = false;
	ui.manualOperatButton->setText("手动添加");
	ui.manualConfirmButton->setEnabled(false);

	ui.LPcomboBox->setEnabled(false);
	ui.LPEdit->setEnabled(false);
	ui.car_comboBox->setEnabled(true);
	ui.car_comboBox->clear();
	ui.car_comboBox->addItem("");
	ui.car_comboBox->addItem("主司机");
	ui.car_comboBox->addItem("副司机");
	ui.car_comboBox->addItem("其他司机");
	ui.driverPhotoButton->setEnabled(true);
	ui.driverOutPhotoButton->setEnabled(true);
	//ui.companyEdit->setEnabled(false);
	//ui.carTypeEdit->setEnabled(false);

	//ui.LPEdit->setText("");
	//ui.companyEdit->setText("");
	//ui.carTypeEdit->setText("");

	emit carInfRequire_signal(ui.LPcomboBox->currentText());
	//emit carInfRequire_signal(ui.LPEdit->text());	//////////////////////////////////////////////////////////////////////////error
}

//人员进出信息复制
void Inspection::peopleMessageShow()
{
	string teststring = "黄晓明";
	QString test = QString::fromStdString(teststring);
	ui.nameEdit->setText(test);
	ui.certificateTypeEdit->setText("登船证");
	ui.certificateNoEdit->setText("987456321");
	ui.validPeriodEdit->setText("20150721-20160101");
	ui.shipEdit->setText("黑珍珠号");

	QImage img;
	img.load("../images/people.jpg");
	QImage peopleInImage = img.scaled(ui.ppictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	QGraphicsScene* scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(peopleInImage));
	ui.ppictureView->setScene(scene);
}

//人员进出信息获取
void Inspection::peopleMessageGet()
{

}

//清空车辆进出的界面
void Inspection::carUIclear()
{
	ui.LPcomboBox->clear();
	ui.LPcomboBox->setEditText("");
	ui.LPEdit->clear();
	ui.companyEdit->clear();
	ui.carTypeEdit->clear();
	//ui.car_comboBox->setItem("");
	ui.car_comboBox->setEnabled(false);
	//picture clear
	ui.dpictureView->setScene(NULL);
	ui.cpictureView->setScene(NULL);

	ui.zdNameEdit->clear();
	ui.zdGenderEdit->clear();
	ui.zdIDEdit->clear();
	ui.zdDLEdit->clear();

	ui.fdNameEdit->clear();
	ui.fdGenderEdit->clear();
	ui.fdIDEdit->clear();
	ui.fdDLEdit->clear();

	ui.odcomboBox->clear();
	ui.odcomboBox->setEnabled(false);
	ui.odGenderEdit->clear();
	ui.odIDEdit->clear();
	ui.odDLEdit->clear();
}

//清空人员进出的界面
void Inspection::peopleUIclear()
{
	ui.nameEdit->setText("");
	ui.certificateTypeEdit->setText("");
	ui.certificateNoEdit->setText("");
	ui.validPeriodEdit->setText("");
	ui.shipEdit->setText("");
	ui.ppictureView->setScene(NULL);
}


//进门响应函数
void Inspection::on_peopleInButton_clicked()
{
	emit peopleIn_signal();

	//QImage img;
	//img.load(QString::fromLocal8Bit(WTpp.peopleinfo.pic.c_str(),-1));
	//QImage peopleInImage = img.scaled(ui.ppictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	//QGraphicsScene* scene = new QGraphicsScene;
	//scene->addPixmap(QPixmap::fromImage(peopleInImage));
	//ui.ppictureView->setScene(scene);
	//peopleUIclear();
}
//出门响应函数
void Inspection::on_peopleOutButton_clicked()
{
	emit peopleOut_signal();
	//peopleUIclear();

	//ui.nameEdit->setText("");
	//ui.certificateTypeEdit->setText("");
	//ui.certificateNoEdit->setText("");
	//ui.validPeriodEdit->setText("");
	//ui.shipEdit->setText("");
	//ui.ppictureView->setScene(NULL);//clear the graphic view
}

//司机进门拍照响应函数
void Inspection::on_driverPhotoButton_clicked()
{
	emit driverSnap_signal(2);
	//QImage img;
	//img.load("../images/people.jpg");
	//QImage driverPicture = img.scaled(ui.dpictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	//QGraphicsScene* scene = new QGraphicsScene;
	//scene->addPixmap(QPixmap::fromImage(driverPicture));
	//ui.dpictureView->setScene(scene);
}

//司机出门拍照响应函数
void Inspection::on_driverOutPhotoButton_clicked()
{
	emit driverSnap_signal(3);
	//QImage img;
	//img.load("../images/people.jpg");
	//QImage driverPicture = img.scaled(ui.dpictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	//QGraphicsScene* scene = new QGraphicsScene;
	//scene->addPixmap(QPixmap::fromImage(driverPicture));
	//ui.dpictureView->setScene(scene);
}

//进门抬杆响应函数
void Inspection::on_gateOpenButton_clicked()
{
	//应该对所有要上交的信息做校验
	//int driver = ui.car_comboBox->currentIndex();
	//QString toshow = QString::number(driver, 10);
	//ui.LPEdit->setText(toshow);
	string tempName;
	if (ui.car_comboBox->currentIndex() == 3)
	{
		tempName =(const char *) ui.odcomboBox->currentText().toLocal8Bit();
		int i = 0;
		for(i = 0; i < Cardriver.alldriname.number; i++)
		{
			if ( tempName == Cardriver.alldriname.driname[i])
			{
				break;
			}
		}
		if (i == Cardriver.alldriname.number)
		{
			Cardriver.vehicle.driver = tempName;
			Cardriver.vehicle.cardtype = (const char *) ui.odIDEdit->text().toLocal8Bit();
			Cardriver.vehicle.number = (const char *) ui.odDLEdit->text().toLocal8Bit();
		}
	}
	emit gateUp_signal();
	ui.car_comboBox->clear();
	ui.car_comboBox->setEnabled(false);
	//ui.odcomboBox->clear();
	ui.odcomboBox->setEnabled(false);
	ui.odGenderEdit->setEnabled(false);
	ui.odIDEdit->setEnabled(false);
	ui.odDLEdit->setEnabled(false);
	ui.driverPhotoButton->setEnabled(false);
	ui.driverOutPhotoButton->setEnabled(false);
	//carUIclear();
}

//进门放杆响应函数
void Inspection::on_gateCloseButton_clicked()
{
	carUIclear();
	emit gateDown_signal();
}

//出门抬杆响应函数
void Inspection::on_gateOutOpenButton_clicked()
{
	//应该对所有要上交的信息做校验
	//int driver = ui.car_comboBox->currentIndex();
	//QString toshow = QString::number(driver, 10);
	//ui.LPEdit->setText(toshow);
	string tempName;
	if (ui.car_comboBox->currentIndex() == 3)
	{
		tempName =(const char *) ui.odcomboBox->currentText().toLocal8Bit();
		int i = 0;
		for(i = 0; i < Cardriver.alldriname.number; i++)
		{
			if ( tempName == Cardriver.alldriname.driname[i])
			{
				break;
			}
		}
		if (i == Cardriver.alldriname.number)
		{
			Cardriver.vehicle.driver = tempName;
			Cardriver.vehicle.cardtype = (const char *) ui.odIDEdit->text().toLocal8Bit();
			Cardriver.vehicle.number = (const char *) ui.odDLEdit->text().toLocal8Bit();
		}
	}
	emit gateUp_signal();
	ui.car_comboBox->clear();
	ui.car_comboBox->setEnabled(false);
	//ui.odcomboBox->clear();
	ui.odcomboBox->setEnabled(false);
	ui.odGenderEdit->setEnabled(false);
	ui.odIDEdit->setEnabled(false);
	ui.odDLEdit->setEnabled(false);
	ui.driverPhotoButton->setEnabled(false);
	ui.driverOutPhotoButton->setEnabled(false);
	//carUIclear();
}

//出门放杆响应函数
void Inspection::on_gateOutCloseButton_clicked()
{
	carUIclear();
	emit gateDown_signal();
}


//message exchange
void Inspection::threadtest_slot(const QString s)
{
	ui.companyEdit->setText(s);

	QImage img;
	img.load(QString::fromLocal8Bit(ICpp.peopleinfo.pic.c_str(),-1));
	QImage peopleInImage = img.scaled(ui.ppictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	QGraphicsScene* scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(peopleInImage));
	ui.ppictureView->setScene(scene);
}

//文通刷卡机信息显示
void Inspection::peopleName_slot(const QString s)
{
	ui.nameEdit->setText(s);

	//QImage img;
	//img.load(QString::fromLocal8Bit(WTpp.peopleinfo.pic.c_str(),-1));
	//QImage peopleInImage = img.scaled(ui.ppictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	//QGraphicsScene* scene = new QGraphicsScene;
	//scene->addPixmap(QPixmap::fromImage(peopleInImage));
	//ui.ppictureView->setScene(scene);
}
void Inspection::peopleType_slot(const QString s)
{
	ui.certificateTypeEdit->setText(s);
}
void Inspection::peopleNo_slot(const QString s)
{
	ui.certificateNoEdit->setText(s);
}
void Inspection::peopleShip_slot(const QString s)
{
	ui.shipEdit->setText(s);
}
void Inspection::snapPeopleImage_slot()
{
	QImage img;
	img.load(QString::fromLocal8Bit(WTpp.peopleinfo.pic.c_str(),-1));
	QImage peopleInImage = img.scaled(ui.ppictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	QGraphicsScene* scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(peopleInImage));
	ui.ppictureView->setScene(scene);
}

//车辆进出信息显示
void Inspection::carInformatin_slot()
{
	QString toshow = QString::fromLocal8Bit(Cardriver.vedetail.company.c_str(), -1);
	ui.companyEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.vedetail.cartype.c_str(), -1);
	ui.carTypeEdit->setText(toshow);

	toshow = QString::fromLocal8Bit(Cardriver.firstdri.name.c_str(), -1);
	ui.zdNameEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.firstdri.sex.c_str(), -1);
	ui.zdGenderEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.firstdri.cardtype.c_str(), -1);
	ui.zdIDEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.firstdri.number.c_str(), -1);
	ui.zdDLEdit->setText(toshow);

	toshow = QString::fromLocal8Bit(Cardriver.seconddri.name.c_str(), -1);
	ui.fdNameEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.seconddri.sex.c_str(), -1);
	ui.fdGenderEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.seconddri.cardtype.c_str(), -1);
	ui.fdIDEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.seconddri.number.c_str(), -1);
	ui.fdDLEdit->setText(toshow);

	ui.odcomboBox->clear();
	ui.odcomboBox->addItem("");
	for (int i = 0; i < Cardriver.alldriname.number; i++)
	{
		toshow = QString::fromLocal8Bit(Cardriver.alldriname.driname[i].c_str(), -1);
		ui.odcomboBox->addItem(toshow);
	}
}
void Inspection::carInformatin_slot_withPic()
{
	QString toshow = QString::fromLocal8Bit(Cardriver.plate.c_str(), -1);
	ui.LPcomboBox->setEditText(toshow);
	ui.LPEdit->setText(toshow);	//////////////////////////////////////////////////////////////////////////error
	toshow = QString::fromLocal8Bit(Cardriver.vedetail.company.c_str(), -1);
	ui.companyEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.vedetail.cartype.c_str(), -1);
	ui.carTypeEdit->setText(toshow);

	toshow = QString::fromLocal8Bit(Cardriver.firstdri.name.c_str(), -1);
	ui.zdNameEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.firstdri.sex.c_str(), -1);
	ui.zdGenderEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.firstdri.cardtype.c_str(), -1);
	ui.zdIDEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.firstdri.number.c_str(), -1);
	ui.zdDLEdit->setText(toshow);

	toshow = QString::fromLocal8Bit(Cardriver.seconddri.name.c_str(), -1);
	ui.fdNameEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.seconddri.sex.c_str(), -1);
	ui.fdGenderEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.seconddri.cardtype.c_str(), -1);
	ui.fdIDEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.seconddri.number.c_str(), -1);
	ui.fdDLEdit->setText(toshow);

	ui.LPcomboBox->setEnabled(true);

	ui.odcomboBox->clear();
	ui.odcomboBox->addItem("");
	for (int i = 0; i < Cardriver.alldriname.number; i++)
	{
		toshow = QString::fromLocal8Bit(Cardriver.alldriname.driname[i].c_str(), -1);
		ui.odcomboBox->addItem(toshow);
	}

	ui.driverPhotoButton->setEnabled(true);
	ui.driverOutPhotoButton->setEnabled(true);
	//ui.car_comboBox->setEditable(true);
	ui.car_comboBox->setEnabled(true);
	ui.car_comboBox->clear();
	ui.car_comboBox->addItem("");
	ui.car_comboBox->addItem("主司机");
	ui.car_comboBox->addItem("副司机");
	ui.car_comboBox->addItem("其他司机");

	QImage img;
	img.load(QString::fromLocal8Bit(Cardriver.vehicle.picofcar.c_str(),-1));    //change the path!!!!!
	QImage driverPicture = img.scaled(ui.cpictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	QGraphicsScene* scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(driverPicture));
	ui.cpictureView->setScene(scene);
}
void Inspection::otherDriverInf_slot()
{
	QString toshow = QString::fromLocal8Bit(Cardriver.otherdri.sex.c_str(), -1);
	ui.odGenderEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.otherdri.cardtype.c_str(), -1);
	ui.odIDEdit->setText(toshow);
	toshow = QString::fromLocal8Bit(Cardriver.otherdri.number.c_str(), -1);
	ui.odDLEdit->setText(toshow);
}
void Inspection::driverSnapShow_slot()
{
	QImage img;
	img.load(QString::fromLocal8Bit(Cardriver.vehicle.picofdri.c_str(),-1));    //change the path!!!!!
	QImage driverPicture = img.scaled(ui.dpictureView->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
	QGraphicsScene* scene = new QGraphicsScene;
	scene->addPixmap(QPixmap::fromImage(driverPicture));
	ui.dpictureView->setScene(scene);
}