#pragma once
#include "inc/natives.h"
#include <time.h>
#include <string>
#include <functional>
using namespace std;


void publicSetStatusText(string text, DWORD time);//显示提示文字
string GT(const string& strGBK);//GBK编码转换到UTF8编码
string CheckUpdates();	//检测更新

void addLogs(std::string log);	// 写入日志
void addLogs(const char* log);	// 写入日志
void PrintSubtitle(std::string text);	// 绘制字幕

void Clearlog();		// 清空日志


void runOnAllNearbyPedsToPlayer(std::function<void(int)> callback); // 扫描所有NPC

void runOnAllNearbyVehiclesToPlayer(std::function<void(int)> callback); // 扫描所有载具和马匹

// 绘制UI 边框
void drawBoxOutline(float x, float y, float radius, float thickness, int r, int g, int b, int a);
void drawBoxOutline(float x, float y, float x_radius, float y_radius, float thickness, int r, int g, int b, int a);

int GetBaseRank(int attributeIndex, bool isHorse = false);	// 获取属性值

// 上帝视角相关
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
float getHeightAboveGround(Vector3 position);	// 获取地面坐标