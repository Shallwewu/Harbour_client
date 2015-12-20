#include <iostream>
#include <atlbase.h>
#include <atlstr.h>
#include <IPHlpApi.h>
# pragma comment(lib,"Iphlpapi.lib")
using namespace std;

#include <time.h>
#include <fstream>
#include "RSA.h"

#define MAX_KEY_LENGTH 1024
#define MAX_VALUE_NAME 2048

extern int registrateT();
extern int registrateM();