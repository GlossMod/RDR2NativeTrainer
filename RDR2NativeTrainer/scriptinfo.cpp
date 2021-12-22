
#include "scriptinfo.h"
//#include "json/json.hpp"
#include "json/json/json.h"

#include "Function.h"

#include <iostream>  
#include <fstream> 
#include <cstring>
#include <iomanip>
//using namespace nlohmann;
using json = Json::Value;

void UpDataJsonData()
{
	string jsonFilepath = "RDR2NativeTrainer\\";	//json文件路径

	//打开文件
	std::ifstream file(jsonFilepath + "RDR2NativeTrainer.json", fstream::in | fstream::out);
	//std::ifstream file("RDR2NativeTrainer.json", fstream::in );

	if (!file.is_open())
	{
		publicSetStatusText(GT("json数据读取失败"), 300);
		addLogs("RDR2NativeTrainer.json读取失败");
		return;
	}

	// 解析json用Json::Reader   
	Json::Reader reader;
	// Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array...   
	json json;


	if (reader.parse(file, json))
	{
		try
		{
			//获取武器列表
			if (!json["weapon"].isNull())
			{
				weapon.clear();
				for (int i = 0; i < json["weapon"].size(); i++)
				{
					dataInfo newWeaponInfo;
					newWeaponInfo.code = json["weapon"][i]["code"].asString();
					newWeaponInfo.name = json["weapon"][i]["name"].asString();
					weapon.push_back(newWeaponInfo);
				}
				addLogs("武器数据写入完成！");
			}

			//获取天气列表
			if (!json["weather"].isNull())
			{
				weather.clear();
				for (int i = 0; i < json["weather"].size(); i++)
				{
					dataInfo newWT;
					newWT.code = json["weather"][i]["code"].asString();
					newWT.name = json["weather"][i]["name"].asString();
					weather.push_back(newWT);
				}
				addLogs("天气数据写入完成！");
			}

			//获取载具列表
			if (!json["vehicle"].isNull())
			{
				vehicle.clear();
				for (int i = 0; i < json["vehicle"].size(); i++)
				{
					dataInfo newVI;
					newVI.code = json["vehicle"][i]["code"].asString();
					newVI.name = json["vehicle"][i]["name"].asString();
					vehicle.push_back(newVI);
				}
				addLogs("载具数据写入完成！");
			}


			//获取马匹列表
			if (!json["horse"].isNull())
			{
				horse.clear();
				for (int i = 0; i < json["horse"].size(); i++)
				{
					dataInfo newHS;
					newHS.code = json["horse"][i]["code"].asString();
					newHS.name = json["horse"][i]["name"].asString();
					horse.push_back(newHS);
				}
				addLogs("马匹数据写入完成！");

			}

			//获取动物列表
			if (!json["animal"].isNull())
			{
				animal.clear();
				for (int i = 0; i < json["animal"].size(); i++)
				{
					dataInfo newAL;
					newAL.code = json["animal"][i]["code"].asString();
					newAL.name = json["animal"][i]["name"].asString();
					animal.push_back(newAL);

				}
				addLogs("动物数据写入完成！");

			}

			//获取人物列表
			if (!json["ped"].isNull())
			{
				ped.clear();
				for (int i = 0; i < json["ped"].size(); i++)
				{
					dataInfo newPed;
					newPed.code = json["ped"][i]["code"].asString();
					newPed.name = json["ped"][i]["name"].asString();
					ped.push_back(newPed);

				}
				addLogs("人物数据写入完成！");

			}

			//获取传送数据
			if (!json["teleport"].isNull())
			{

				teleport.clear();
				for (int i = 0; i < json["teleport"].size(); i++)
				{
					TeleportInfo newTp;

					newTp.pos.x = std::stof(to_string(json["teleport"][i]["x"].asFloat()));
					newTp.pos.y = std::stof(to_string(json["teleport"][i]["y"].asFloat()));
					newTp.pos.z = std::stof(to_string(json["teleport"][i]["z"].asFloat()));
					newTp.name = json["teleport"][i]["name"].asString();
					teleport.push_back(newTp);
				}
				addLogs("传送坐标写入完成！");

			}
			// 获取草药数据
			if (!json["medicine"].isNull())
			{
				medicine.clear();
				for (int i = 0; i < json["medicine"].size(); i++)
				{
					dataInfo newPed;
					newPed.code = json["medicine"][i]["code"].asString();
					newPed.name = json["medicine"][i]["name"].asString();
					medicine.push_back(newPed);

				}
				addLogs("草药数据写入完成！");
			}

			if (!json["newPed"].isNull())
			{
				newPed.clear();
				for (int i = 0; i < json["newPed"].size(); i++)
				{
					dataInfo Ped;
					Ped.code = json["newPed"][i]["code"].asString();
					Ped.name = json["newPed"][i]["name"].asString();
					newPed.push_back(Ped);

				}
				addLogs("草药数据写入完成！");
			}


			publicSetStatusText(GT("初始化完成"), 300);
		}
		catch (const std::exception& e)
		{
			publicSetStatusText(GT("初始化失败"), 300);
			addLogs("配置文件读取错误：");
			addLogs(e.what());
		}
	}
	file.close();
}

