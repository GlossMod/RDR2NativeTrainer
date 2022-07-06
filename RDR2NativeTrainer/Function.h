#pragma once
#include "inc/natives.h"
#include <time.h>
#include <string>
#include <functional>
using namespace std;


void publicSetStatusText(string text, DWORD time);//��ʾ��ʾ����
string CheckUpdates();	//������

void addLogs(std::string log);	// д����־
void addLogs(const char* log);	// д����־
void PrintSubtitle(std::string text);	// ������Ļ

void Clearlog();		// �����־


void runOnAllNearbyPedsToPlayer(std::function<void(int)> callback); // ɨ������NPC

void runOnAllNearbyVehiclesToPlayer(std::function<void(int)> callback); // ɨ�������ؾߺ���ƥ

// ����UI �߿�
void drawBoxOutline(float x, float y, float radius, float thickness, int r, int g, int b, int a);
void drawBoxOutline(float x, float y, float x_radius, float y_radius, float thickness, int r, int g, int b, int a);

int GetBaseRank(int attributeIndex, bool isHorse = false);	// ��ȡ����ֵ

// �ϵ��ӽ����
float clamp360(float value);
namespace camShit {
	int cam = 0;
	Vector3 location = { 0,0,0 };
	Vector3 rotation = { 0,0,0 };//GET_GAMEPLAY_CAM_ROT(2);
	Vector3 velocityForFocus = { 0, 0, 0 };
	int entity = 0;
	bool doCamRot = true;
	bool doCamPos = true;
	bool doFocus = true;
	bool doLookat = true;
	void loop();
	void attachToLocation(float x, float y, float z, bool setFocusOnArea = true);
	void resetCam(bool clearFocus = true);
}
float getHeightAboveGround(Vector3 position);	// ��ȡ��������




//
//
//// �û�������¼����
//class UserConfig {	
//	
//	
//public:
//	void Save(string key);
//	void Load(string key);
//	void Clear();
//};