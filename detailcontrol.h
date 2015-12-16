#ifndef __DETAILCONTROL_H
#define __DETAILCONTROL_H
#include "maincontrol.h"

string Getnowtime();
string Obtaintime();
string Entiretime();
bool Remotesave(string &infomation);
bool Clientsockini();
void ICdeal(string data,int inorout);
void ICsnapsave(unsigned int inorout);
void WTdeal(void);
void confiopendoor(unsigned int inorout);
void pcaptureauto();
void opendoor();
void notopendoor(void);
int Cardeal(string &newlp);
bool Chosedri(string name);
void driregister(string &name,string &type,string &number);
void controldoor(unsigned int doorNO);
int doorConditionInit();
int getDoorCondition(unsigned int doorNO);
void confiopengate();
void closegate();
void snapandsave(unsigned int inorout);
char* U2G(const char* utf8);
char* G2U(const char* gb2312);
void WTpassstate(unsigned int state);
string Getnowyear();
#endif