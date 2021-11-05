/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#pragma once
#include "GbkToUtf8.h"
#include "inc\types.h"
#include <string>
#include <vector>
using namespace std;

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))



//��������
struct dataInfo
{
	string code;
	string name;
};

string erMsg = "δ�ҵ�RDR2NativeTrainer.json�ļ�";

//����
vector<dataInfo> ped{
	{"A_M_Y_AsbMiner_01",						GT(erMsg)},
};

//����
vector<dataInfo> animal{
	{"A_C_DOGAMERICANFOXHOUND_01",				GT(erMsg)},
};

//��ƥ
vector<dataInfo> horse{
	{"A_C_HORSE_AMERICANPAINT_GREYOVERO",		GT(erMsg)},
};

//�ؾ�
vector<dataInfo> vehicle {
	{"breach_cannon",			GT(erMsg)},
};

//����
vector<dataInfo> weather{
	{"HIGHPRESSURE",				GT(erMsg)}
};

//����
vector<dataInfo> weapon{
	{"LASSO",		GT(erMsg)}
};




//��������
struct TeleportInfo
{	
	Vector3 pos;
	string name;
};
vector<TeleportInfo> teleport{
	{{1232.205,	-1251.088,	73.67763},					GT(erMsg)}
};

void UpDataJsonData();