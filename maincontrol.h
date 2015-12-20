#include <string>
using namespace std;

struct Pinformation     //进出人员纪录信息
{
	string name;
	string type;
	string stype;
	string number;
	string ship;
	string time;
	string area;
	string station;
	string inout;
	string alarm;
	string pic;
	string spic;
	string validdate;
};

struct UIpinfo
{
	string name;
	string cardtype;
	string number;
	string ship;
	string validdate;
};

struct Carinfo     //进出车辆纪录信息
{
	string lp;
	string company;
	string driver;
	string cardtype;
	string number;
	string time;
	string area;
	string inout;
	string picofdri;
	string picofcar;
	string spicofdri;
	string spicofcar;
};

struct cardetail     //车辆具体信息
{
	string lp;
	string company;
	string cartype;
};

struct driverinfo    //司机具体信息
{
	string name;
	string sex;
	string birth;
	string cardtype;
	string number;
};

struct otherdriname
{
	int number;    //其它司机个数
	string driname[100];
};

struct carplate
{
	int number;
	string lpnumber[100];
};

class People
{
public:
	int icexist;
	int flag_snap;
	//int flag_door;
	Pinformation peopleinfo;
	//	UIpinfo uipeoinfo;
	People()
	{
	};
	~People(){};
	void setpinfo(Pinformation &peoinfo);
	bool iccompare();     //IC卡对比方法
	bool precordsave();    //存储纪录
	int peopletype();     //判定哪类卡进行不同操作
	void peopleclear();
	bool WTvalid();
};

class Car
{
public:
	string plate;
	Carinfo vehicle;
	cardetail vedetail;
	driverinfo firstdri;
	driverinfo seconddri;
	driverinfo otherdri;
	otherdriname alldriname;
	carplate allplate;
	string choosedrina;
	int flagdri_snap;
	int flag_traffic;
	Car()
	{
	};
	~Car(){};
	void setplate(string &lp);   //设置车牌号
	bool Getcarinfo();        //获取车辆、主副司机信息 以及其他司机名字 
	bool Getotherdri();       //获取其他某个司机信息
	bool crecordsave();       //纪录存储
	void carclear();          //信息清除
	void driverclear();
};
