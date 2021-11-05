
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
	string jsonFilepath = "RDR2NativeTrainer\\";	//json�ļ�·��

	//���ļ�
	std::ifstream file(jsonFilepath + "RDR2NativeTrainer.json", fstream::in | fstream::out);
	//std::ifstream file("RDR2NativeTrainer.json", fstream::in );

	if (!file.is_open())
	{
		publicSetStatusText(GT("json���ݶ�ȡʧ��"), 300);
		addLogs("RDR2NativeTrainer.json��ȡʧ��");
		return;
	}

	// ����json��Json::Reader   
	Json::Reader reader;
	// Json::Value��һ�ֺ���Ҫ�����ͣ����Դ����������͡���int, string, object, array...   
	json json;


	if (reader.parse(file, json))
	{
		try
		{
			//��ȡ�����б�
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
				addLogs("��������д����ɣ�");
			}

			//��ȡ�����б�
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
				addLogs("��������д����ɣ�");
			}

			//��ȡ�ؾ��б�
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
				addLogs("�ؾ�����д����ɣ�");
			}


			//��ȡ��ƥ�б�
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
				addLogs("��ƥ����д����ɣ�");

			}

			//��ȡ�����б�
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
				addLogs("��������д����ɣ�");

			}

			//��ȡ�����б�
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
				addLogs("��������д����ɣ�");

			}

			//��ȡ��������
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
				addLogs("��������д����ɣ�");

			}


			publicSetStatusText(GT("��ʼ�����"), 300);
		}
		catch (const std::exception& e)
		{
			publicSetStatusText(GT("��ʼ��ʧ��"), 300);
			addLogs("�����ļ���ȡ����");
			addLogs(e.what());
		}
	}
	file.close();
}

