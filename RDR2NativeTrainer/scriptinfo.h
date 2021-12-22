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

// ��ҩ
vector<dataInfo> medicine{
	{ "COMPOSITE_LOOTABLE_AGARITA_DEF",		GT(erMsg) }
};

vector<dataInfo> newPed{
	{ "A_C_BOARLEGENDARY_01",		GT(erMsg) }
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


struct overlay
{
    string name;
    int visibility;
    int tx_id;
    int tx_normal;
    int tx_color_type;
    float tx_opacity;
    int tx_unk;
    int palette;
    int palette_color_primary;
    int palette_color_secondary;
    int palette_color_tertiary;
    int var;
    float opacity;

};


//vector<overlay> overlay_all_layers {
//    {"eyebrows", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"scars", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"eyeliners", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"lipsticks", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"acne", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"shadows", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"beardstabble", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"paintedmasks", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"ageing", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"blush", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"complex", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"disc", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"foundation", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"freckles", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"grime", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"hair", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"moles", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//    {"spots", 0, 1, 0, 0, 0, 1.0, 0, 0, 0, 0, 0, 0, 1.0 },
//};