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



//数据类型
struct dataInfo
{
	string code;
	string name;
};

string erMsg = "未找到RDR2NativeTrainer.json文件";

//人物
vector<dataInfo> ped{
	{"A_M_Y_AsbMiner_01",						GT(erMsg)},
};

//动物
vector<dataInfo> animal{
	{"A_C_DOGAMERICANFOXHOUND_01",				GT(erMsg)},
};

//马匹
vector<dataInfo> horse{
	{"A_C_HORSE_AMERICANPAINT_GREYOVERO",		GT(erMsg)},
};

//载具
vector<dataInfo> vehicle {
	{"breach_cannon",			GT(erMsg)},
};

//天气
vector<dataInfo> weather{
	{"HIGHPRESSURE",				GT(erMsg)}
};

//武器
vector<dataInfo> weapon{
	{"LASSO",		GT(erMsg)}
};




//传送数据
struct TeleportInfo
{	
	Vector3 pos;
	string name;
};
vector<TeleportInfo> teleport{
	{{1232.205,	-1251.088,	73.67763},					GT(erMsg)}
};

void UpDataJsonData();