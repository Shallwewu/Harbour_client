#include <string>
using namespace std;

struct Pinformation     //������Ա��¼��Ϣ
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

struct Carinfo     //����������¼��Ϣ
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

struct cardetail     //����������Ϣ
{
	string lp;
	string company;
	string cartype;
};

struct driverinfo    //˾��������Ϣ
{
	string name;
	string sex;
	string birth;
	string cardtype;
	string number;
};

struct otherdriname
{
	int number;    //����˾������
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
	bool iccompare();     //IC���Աȷ���
	bool precordsave();    //�洢��¼
	int peopletype();     //�ж����࿨���в�ͬ����
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
	void setplate(string &lp);   //���ó��ƺ�
	bool Getcarinfo();        //��ȡ����������˾����Ϣ �Լ�����˾������ 
	bool Getotherdri();       //��ȡ����ĳ��˾����Ϣ
	bool crecordsave();       //��¼�洢
	void carclear();          //��Ϣ���
	void driverclear();
};
