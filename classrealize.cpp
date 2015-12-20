//#include <stdio.h>
//#include <windows.h>
//#include <stdlib.h>
//#include <ctype.h>
//#define _AFXDLL
//#include <afx.h>
#include "detailcontrol.h"
#include "sqlite3.h"
//#include "atlbase.h"
//#include "atlstr.h"
#include <iostream>
#include "atlbase.h"
#include "atlstr.h"


int pupdate=0;
int cupdate=0;
People ICpp;
People WTpp;
Car Cardriver;
extern sqlite3* pdb;
extern int typenum;

bool People::iccompare()
{
	//int ic;
	//string strsql,nowtime,sqltimeb,sqltimee;
	//char* cErrMsg; 
	//char **dbResult;
	//int nRow,nColumn;;
	////	string table=BOARDTABLE;
	////	string item=CARDNUMBER;
	////	strsql="select count(*) as ic from ";
	//strsql="select XM,ZWCBM,YXQQ,YXQZ from shp_lcc_info_table where ZJHM='";
	//strsql+=peopleinfo.number;
	//strsql+="'";
	////	int nRes=sqlite3_exec(pDB , strSql.c_str() ,0 ,0, &cErrMsg);
	//int nRes=sqlite3_get_table(pdb,strsql.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg); 
	//if (nRes != SQLITE_OK)  
	//{  
	//	cout<<"select fail: "<<cErrMsg<<endl;  
	//	return false;  
	//}  
	//if(nRow==0)
	//{
	//	cout<<"IC did not exist"<<endl;
	//	return false; 
	//}
	//if (dbResult[nColumn]!=NULL)
	//{
	//	peopleinfo.name=U2G(dbResult[nColumn]);
	//}
	//if (dbResult[nColumn+1]!=NULL)
	//{
	//	peopleinfo.ship=U2G(dbResult[nColumn+1]);
	//}
	//if ((dbResult[nColumn+2]!=NULL)&&(dbResult[nColumn+3]!=NULL))
	//{
	//	nowtime = Obtaintime();
	//	sqltimeb = U2G(dbResult[nColumn+2]);
	//	sqltimee = U2G(dbResult[nColumn+3]);
	//	if (nowtime>=sqltimeb&&nowtime<=sqltimee)
	//	{
	//		/*cout<<"有效期内"<<endl;*/
	//	}
	//	else
	//	{
	//		/*cout<<"已过期"<<endl;*/
	//		return false;
	//	}
	//}
	//return true;
	string nowyear="";
	nowyear=Getnowyear();
	if (ICpp.peopleinfo.validdate<nowyear)
	{
		return false;
	}
	return true;
}

bool People::precordsave()
{
	string strsql,strsql1,psqlremote;
	char* cErrMsg; 
	strsql="insert into driver_rec_table(name,cat,id,vechicle,date,harbour,station,direction,alarm,pic) values('"; 
	strsql+=G2U((peopleinfo.name).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.stype).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.number).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.ship).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.time).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.area).c_str());
	strsql+= "','";
	strsql+=G2U((peopleinfo.station).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.inout).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.alarm).c_str());
	strsql+= "','"; 
	strsql+=G2U((peopleinfo.spic).c_str());
	strsql+= "')"; 
	int nRes = sqlite3_exec(pdb , strsql.c_str() ,0 ,0, &cErrMsg); 
	if (nRes != SQLITE_OK)    
	{  
		cout<<"add record fail: "<<cErrMsg<<endl;  
		return false;  
	}  
	else  
	{  
		cout<<"add record success" <<endl;  
	}  
	strsql1="insert into driver_rec_table(name,cat,id,vechicle,date,harbour,station,direction,alarm,pic) values('"; 
	strsql1+=peopleinfo.name;
	strsql1+= "','"; 
	strsql1+=peopleinfo.stype;
	strsql1+= "','"; 
	strsql1+=peopleinfo.number;
	strsql1+= "','"; 
	strsql1+=peopleinfo.ship;
	strsql1+= "','"; 
	strsql1+=peopleinfo.time;
	strsql1+= "','"; 
	strsql1+=peopleinfo.area;
	strsql1+= "','"; 
	strsql1+=peopleinfo.station;
	strsql1+= "','"; 
	strsql1+=peopleinfo.inout;
	strsql1+= "','"; 
	strsql1+=peopleinfo.alarm;
	strsql1+= "','"; 
	strsql1+=peopleinfo.spic;
	strsql1+= "')"; 
	psqlremote="sql:"+strsql1+"\0";
	Remotesave(psqlremote);

	return true;  
}

bool People::WTvalid()          //二维码有效性判断
{
	string strsql;
	char* cErrMsg; 
	char **dbResult;
	int nRow,nColumn;

	if (typenum==6 || typenum==7 || typenum==8)
	{
		string shiparri="";
		strsql="select STATUS from crs_shp_table where ZWCBM='";     //船舶在港状态判断
		strsql+=G2U((WTpp.peopleinfo.ship).c_str());
		strsql+="' or YWCBM='";
		strsql+=G2U((WTpp.peopleinfo.ship).c_str());
		strsql+="';";
		int nRes = sqlite3_get_table(pdb,strsql.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg); 
		if (nRes != SQLITE_OK)
		{  
			cout<<"select fail: "<<cErrMsg<<endl;  
			return false;
		}  
		if (nColumn!=0)
		{
			if(dbResult[nColumn]!=NULL)
			{
				shiparri=U2G(dbResult[nColumn]);
				if (shiparri=="离港")
				{
					return false;
				}
			}
		}
		else
			return false;
	}
	return true;
}

void People::peopleclear()
{
	icexist=0;
	flag_snap=0;
	peopleinfo.name="";
	peopleinfo.type="";
	peopleinfo.stype="";
	peopleinfo.number="";
	peopleinfo.ship="";
	peopleinfo.time="";
	peopleinfo.area="";
	peopleinfo.inout="";
	//by damon
	peopleinfo.alarm="";
	peopleinfo.station="";
	peopleinfo.pic = "";
	peopleinfo.spic = "";
	peopleinfo.validdate="";
}

bool Car::Getcarinfo()
{
	string strsql1,strsql2,strsql3,strsql4;
	char* cErrMsg; 
	char **dbResult;
	int i,j,nRow,nColumn;
	strsql1="select * from vehicleinfo where WYCPH='";  //判断车牌号是否存在
	strsql1+=G2U(plate.c_str());                
	strsql1+="'";
	//strsql2="select 车.公司,车.类型,司机.名字,司机.性别,司机.出生年月,司机.证件类型,司机.证件号码 from 车,司机 where 车.车牌='"； //获取主司机信息
	//   strsql2+=plate;
	//strsql2+="'";
	//strsql2+=" and 车.主司机=司机.名字 and 车.公司=司机.公司";

	strsql2="select vehicleinfo.GSQC,vehicleinfo.CLLXDM,driverinfo_use.XM,driverinfo_use.XBDM,driverinfo_use.CSRQ,driverinfo_use.ZJLBDM,driverinfo_use.ZJHM from vehicleinfo,driverinfo_use where vehicleinfo.WYCPH='"; //获取主司机信息
	strsql2+=G2U(plate.c_str()); 
	strsql2+="'";
	strsql2+=" and vehicleinfo.PD=driverinfo_use.XM and vehicleinfo.GSQC=driverinfo_use.GSQC";

	//   strsql3="select 司机.名字,司机.性别,司机.出生年月,司机.证件类型,司机.证件号码 from 司机 where 司机.名字=(select 车.副司机 from 车 where 车.车牌='"   //获取副司机信息
	//strsql3+=plate;
	//strsql3+="')";

	strsql3="select driverinfo_use.XM,driverinfo_use.XBDM,driverinfo_use.CSRQ,driverinfo_use.ZJLBDM,driverinfo_use.ZJHM from driverinfo_use,vehicleinfo where vehicleinfo.WYCPH='";  //获取副司机信息
	strsql3+=G2U(plate.c_str()); 
	strsql3+="' and vehicleinfo.SD=driverinfo_use.XM and vehicleinfo.GSQC=driverinfo_use.GSQC";

	//strsql4="select 司机名字 from 司机 where 司机.公司=(select 公司 from 车 where 车牌='";   //获取公司里所有司机名字
	//strsql4+=plate;
	//strsql4+="')";

	strsql4="select driverinfo_use.XM from driverinfo_use where driverinfo_use.GSQC=(select vehicleinfo.GSQC from vehicleinfo where vehicleinfo.WYCPH='";   //获取公司里所有司机名字
	strsql4+=G2U(plate.c_str()); 
	strsql4+="')";

	int res = sqlite3_get_table(pdb,strsql1.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg);   

	if (res != SQLITE_OK)
	{  
		cout<<"select fail: "<<cErrMsg<<endl;  
		return false;  
	}  

	if(nRow==0)
	{
		cout<<"LP did not exist"<<endl;
		return false; 
	}
	else
	{
		res = sqlite3_get_table(pdb,strsql2.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg);  //获取主司机信息
		if (res != SQLITE_OK)
		{  
			cout<<"select fail: "<<cErrMsg<<endl;  
			return false;  
		}
		vedetail.lp=plate;
		vehicle.lp=plate;
		if(nRow>0)
		{
			if (dbResult[nColumn]!=NULL)
			{
				vedetail.company=U2G(dbResult[nColumn]);
				vehicle.company=U2G(dbResult[nColumn]);
			}
			if (dbResult[nColumn+1]!=NULL)
			{
				vedetail.cartype=U2G(dbResult[nColumn+1]);
				if (vedetail.cartype=="41")
				{
					vedetail.cartype="货车";
				}
				else if (vedetail.cartype=="42")
				{
					vedetail.cartype="客车";
				}
				else if (vedetail.cartype=="43")
				{
					vedetail.cartype="小车";
				}
				else if (vedetail.cartype=="49")
				{
					vedetail.cartype="其他";
				}
			}
			if (dbResult[nColumn+2]!=NULL)
			{
				firstdri.name=U2G(dbResult[nColumn+2]);
			}
			if (dbResult[nColumn+3]!=NULL)
			{
				firstdri.sex=U2G(dbResult[nColumn+3]);
			}
			if (dbResult[nColumn+4]!=NULL)
			{
				firstdri.birth=U2G(dbResult[nColumn+4]);
			}
			if (dbResult[nColumn+5]!=NULL)
			{
				firstdri.cardtype=U2G(dbResult[nColumn+5]);
				if (firstdri.cardtype=="10")
				{
					firstdri.cardtype="身份证";
				}
				else if (firstdri.cardtype=="14")
				{
					firstdri.cardtype="护照";
				}
			}
			if (dbResult[nColumn+6]!=NULL)
			{
				firstdri.number=U2G(dbResult[nColumn+6]);
			}
		}
		res = sqlite3_get_table(pdb,strsql3.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg);  //获取副司机信息
		if (res != SQLITE_OK)
		{  
			cout<<"select fail: "<<cErrMsg<<endl;  
			return false;  
		} 
		if(nRow>0)
		{
			if (dbResult[nColumn]!=NULL)
			{
				seconddri.name=U2G(dbResult[nColumn]);
			}
			if (dbResult[nColumn+1]!=NULL)
			{
				seconddri.sex=U2G(dbResult[nColumn+1]);
			}
			if (dbResult[nColumn+2]!=NULL)
			{
				seconddri.birth=U2G(dbResult[nColumn+2]);
			}
			if (dbResult[nColumn+3]!=NULL)
			{
				seconddri.cardtype=U2G(dbResult[nColumn+3]);
				if (seconddri.cardtype=="10")
				{
					seconddri.cardtype="身份证";
				}
				else if (seconddri.cardtype=="14")
				{
					seconddri.cardtype="护照";
				}
			}
			if (dbResult[nColumn+4]!=NULL)
			{
				seconddri.number=U2G(dbResult[nColumn+4]);
			}
		}
		res = sqlite3_get_table(pdb,strsql4.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg);  //获取公司所有司机名字
		if (res != SQLITE_OK)
		{  
			cout<<"select fail: "<<cErrMsg<<endl;  
			return false;  
		} 
		alldriname.number=min(nRow,100);   //司机人数
		for(int q=0;q<alldriname.number;q++)    
		{
			if (dbResult[nColumn+q]!=NULL)
			{
				alldriname.driname[q]=U2G(dbResult[nColumn+q]);
			}
		}
	}
	return true;  
}

void Car::setplate(string &lp)
{
	plate=lp;
}

bool Car::Getotherdri()
{
	char* cErrMsg; 
	char **dbResult;
	int i,j,nRow,nColumn;
	string strsql;
	strsql="select driverinfo_use.XM,driverinfo_use.XBDM,driverinfo_use.CSRQ,driverinfo_use.ZJLBDM,driverinfo_use.ZJHM from driverinfo_use where driverinfo_use.XM='";  //获取其他选中的司机信息
	strsql+=G2U(choosedrina.c_str());
	strsql+="' and driverinfo_use.GSQC='";
	strsql+=G2U(vedetail.company.c_str());
	strsql+="'";
	int res=sqlite3_get_table(pdb,strsql.c_str(), &dbResult, &nRow, &nColumn,&cErrMsg);
	if (res != SQLITE_OK)
	{  
		cout<<"select fail: "<<cErrMsg<<endl;  
		return false;  
	}
	if(nRow>0)
	{
		if (dbResult[nColumn]!=NULL)
		{
			otherdri.name=U2G(dbResult[nColumn]);
		}
		if (dbResult[nColumn+1]!=NULL)
		{
			otherdri.sex=U2G(dbResult[nColumn+1]);
		}
		if (dbResult[nColumn+2]!=NULL)
		{
			otherdri.birth=U2G(dbResult[nColumn+2]);
		}
		if (dbResult[nColumn+3]!=NULL)
		{
			otherdri.cardtype=U2G(dbResult[nColumn+3]);
			if (otherdri.cardtype=="10")
			{
				otherdri.cardtype="身份证";
			}
			else if (otherdri.cardtype=="14")
			{
				otherdri.cardtype="护照";
			}
		}
		if (dbResult[nColumn+4]!=NULL)
		{
			otherdri.number=U2G(dbResult[nColumn+4]);
		}
	}
	return true; 
}

bool Car::crecordsave()
{
	string strsql,strsql1,csqlremote;
	char* cErrMsg; 
	strsql="insert into vehicle_rec_table(plate,company,driver,idtype,idnum,date,harbour,direction,drvpic,vhlpic) values('";
	strsql+=G2U((vehicle.lp).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.company).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.driver).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.cardtype).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.number).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.time).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.area).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.inout).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.spicofdri).c_str());
	strsql+= "','"; 
	strsql+=G2U((vehicle.spicofcar).c_str());
	strsql+= "')"; 

	int nRes = sqlite3_exec(pdb , strsql.c_str() ,0 ,0, &cErrMsg); 
	if (nRes != SQLITE_OK)    
	{  
		cout<<"add car record fail: "<<cErrMsg<<endl;  
		return false;  
	}  
	else  
	{  
		cout<<"add car record success" <<endl;  
	}  
	strsql1="insert into vehicle_rec_table(plate,company,driver,idtype,idnum,date,harbour,direction,drvpic,vhlpic) values('";
	strsql1+=vehicle.lp;
	strsql1+= "','"; 
	strsql1+=vehicle.company;
	strsql1+= "','"; 
	strsql1+=vehicle.driver;
	strsql1+= "','"; 
	strsql1+=vehicle.cardtype;
	strsql1+= "','"; 
	strsql1+=vehicle.number;
	strsql1+= "','"; 
	strsql1+=vehicle.time;
	strsql1+= "','"; 
	strsql1+=vehicle.area;
	strsql1+= "','"; 
	strsql1+=vehicle.inout;
	strsql1+= "','"; 
	strsql1+=vehicle.spicofdri;
	strsql1+= "','"; 
	strsql1+=vehicle.spicofcar;
	strsql1+= "')"; 

	csqlremote="sql:"+strsql1+"\0";
	Remotesave(csqlremote);

	return true;
}

void Car::carclear()
{
	//int icexist;
	//plate="";
	//vehicle.lp="";
	vehicle.company="";
	vehicle.driver="";
	vehicle.cardtype="";
	vehicle.number="";
	vehicle.time="";
	vehicle.area="";
	vehicle.inout="";

	//by damon
	vehicle.picofdri = "";
	vehicle.picofcar = "";
	vehicle.spicofdri = "";
	vehicle.spicofcar = "";

	choosedrina="";
	vedetail.lp="";
	vedetail.company="";
	vedetail.cartype="";
	firstdri.name="";
	firstdri.sex="";
	firstdri.birth="";
	firstdri.cardtype="";
	firstdri.number="";
	seconddri.name="";
	seconddri.sex="";
	seconddri.birth="";
	seconddri.cardtype="";
	seconddri.number="";
	otherdri.name="";
	otherdri.sex="";
	otherdri.birth="";
	otherdri.cardtype="";
	otherdri.number="";
	alldriname.number=0;
	flagdri_snap=0;
	flag_traffic=0;
	//memset(&vedetail,0,sizeof(vedetail));
	//memset(&firstdri,0,sizeof(firstdri));
	//memset(&seconddri,0,sizeof(seconddri));
	//memset(&otherdri,0,sizeof(otherdri));
	//memset(&alldriname,0,sizeof(alldriname));
}

void Car::driverclear()
{
	firstdri.name="";
	firstdri.sex="";
	firstdri.birth="";
	firstdri.cardtype="";
	firstdri.number="";
	seconddri.name="";
	seconddri.sex="";
	seconddri.birth="";
	seconddri.cardtype="";
	seconddri.number="";	
}