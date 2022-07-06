#include "Function.h"

// 显示中间文字
void publicSetStatusText(string text, DWORD time)
{
	char* cText = const_cast<char*>(text.c_str());

	int Fuck_Speed = 1;
	clock_t Time_FuckStart = clock();

	time = GetTickCount64() + time;

	while (GetTickCount64() < time)
	{
		//UI::SET_TEXT_SCALE(0.55, 0.55);
		UIDEBUG::_BG_SET_TEXT_SCALE(0.55, 0.55);
		UI::SET_TEXT_COLOR_RGBA(255, 255, 255, 255);
		//UI::SET_TEXT_CENTRE(1);	// 文本居中 失效

		//UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);	// 设置阴影 失效
		//UI::DRAW_TEXT(cText, 0.5, 0.5);	// DRAW_TEXT 显示文本失效
		UIDEBUG::_BG_DISPLAY_TEXT(cText, 0.5, 0.5);
		WAIT(0);
	}
}
// 显示字幕
void PrintSubtitle(std::string text)
{
	const char* varString = GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", (char*)text.c_str());
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(varString);
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}	

//GBK编码转换到UTF8编码
string GT(const string& strGBK)
{
	string strOutUTF8 = "";
	WCHAR* str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n); n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char* str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

#include <atlstr.h>
#include <wininet.h>
#pragma  comment(lib, "wininet.lib")

#include "json/json/json.h"

#include <sstream>
#include <iomanip>
#include <stdlib.h>

#pragma comment(lib,"WinInet.lib")
//using namespace nlohmann;
using json = Json::Value;

//对网页进行转码  
wchar_t* Convert(CString str, int targetCodePage)
{

	int iunicodeLen = MultiByteToWideChar(targetCodePage, 0, (LPCTSTR)str.GetBuffer(), -1, NULL, 0);
	wchar_t* pUnicode = NULL;
	pUnicode = new wchar_t[iunicodeLen + 1];
	memset(pUnicode, 0, (iunicodeLen + 1) * sizeof(wchar_t));
	MultiByteToWideChar(targetCodePage, 0, (LPCTSTR)str.GetBuffer(), -1, (LPWSTR)pUnicode, iunicodeLen);//映射一个字符串到一个款字节中  
	return pUnicode;
}

//获取网页的源码，参数1为网页链接，2为缓冲区指针
bool GetHtml(LPCTSTR szURL, CString& getbuf)
{
	HINTERNET   hInternet, hUrl;
	char        buffer[1124];
	WCHAR       wBuffer[1124];

	DWORD       dwBytesRead = 0;
	DWORD       dwBytesWritten = 0;
	BOOL        bIsFirstPacket = true;
	BOOL        bRet = true;
	int         nNowcopyDate = 0;

	hInternet = InternetOpen(_T("Mozilla/4.0 (compatible)"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, INTERNET_INVALID_PORT_NUMBER, 0);//初始化应用程序，使用WinNet  
	if (hInternet == NULL)
		return FALSE;

	hUrl = InternetOpenUrl(hInternet, szURL, NULL, 0, INTERNET_FLAG_RELOAD, 0);//打开一个资源 ftp，gopher，http开头  

	if (hUrl == NULL)
	{

		DWORD m = GetLastError();
		return FALSE;
	}

	do
	{
		memset(buffer, 0, sizeof(char) * 1124);
		InternetReadFile(hUrl, buffer, sizeof(char) * 1024, &dwBytesRead);
		bIsFirstPacket = false;
		nNowcopyDate = +dwBytesRead;
		wchar_t* punicode;
		punicode = Convert(buffer, CP_ACP);// 对源码进行转码   第二个参数为网页的编码格式  


		//CString strTmp = CString(buffer);   

		getbuf += _T("\r\n");
		getbuf += punicode;
		delete punicode;
	}     while (dwBytesRead > 0);

	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);
	return TRUE;

}

//检测更新
string CheckUpdates()
{
	double nowV = 0.951;	
	std::stringstream str;
	str << std::setprecision(3) << nowV;
	string v = "当前版本 " + str.str() + " ";
	try
	{
		Json::Reader reader;
		Json::Value root;
		//json js;
		CString  retbuf;
		if (GetHtml(TEXT("https://mod.3dmgame.com/mod/API/147160"), retbuf))	//是否成功读取，将值写入retbuf
		{
			//retbuf值：
			//{"id":"147160","mods_version":"0.81","mods_author":"◕小莫◕","mods_click_cnt":295451,"mods_download_cnt":90961,"mods_mark_cnt":37389,"mods_collection_cnt":19098,"mods_updateTime":"2020-03-29 15:39:57"}
			// 解析返回的内容
			if (reader.parse(retbuf.GetBuffer(), root))
			{
				//root["mods_version"];
				//v += "最新版本 " + root["mods_version"].asString() + " ";
				if (root["mods_version"].asString() != str.str())
				{
					v += "[有更新可用]";
				}
				else
				{
					v += "[已是最新版本]";
				}
			}
			else
			{
				v += "[无法检测更新,数据解析失败！]";
			}
			
			return v;
			
		}
		else
		{
			return (v + "[无法检测更新,数据读取失败]");
		}
	}
	catch (const std::exception&)
	{
		return (v + "[错误：无法检测更新]");
	}
}



#include<fstream>

// 写入日志
void addLogs(std::string log)
{
	ofstream write;
	ifstream read;

	// 时间
	std::string stime;
	std::stringstream strtime;
	std::time_t currenttime = std::time(0);
	char tAll[255];
	std::strftime(tAll, sizeof(tAll), "%Y-%m-%d %H:%M:%S", std::localtime(&currenttime));
	strtime << tAll;
	stime = strtime.str();
	log = stime + "：" + log;

	write.open("RDR2NativeTrainer\\logs.txt", ios::app);	//用ios::app不会覆盖文件内容
	write << log << endl;
	write.close();

	read.close();

}
void addLogs(const char* log)
{
	std::string s;
	s = log;	
	addLogs(s);
}
void Clearlog()
{
	fstream file("RDR2NativeTrainer\\logs.txt", ios::out);
	file.close();

}



#define nearbyPedArraySize 10000
struct nearbyEnts {
	int size;
	int64_t entities[nearbyPedArraySize];
};
// 扫描玩家周围所有NPC
void runOnAllNearbyPedsToPlayer(std::function<void(int)> callback) {
	nearbyEnts arr;
	arr.size = nearbyPedArraySize;
	int ped = PLAYER::PLAYER_PED_ID();
	int size = PED::GET_PED_NEARBY_PEDS(ped, (int*)&arr, ped, 0/*unk*/);
	for (int i = 0; i < size; i++) {
		callback(arr.entities[i]);
	}
}
// 扫描玩家周围所有载具和马匹
void runOnAllNearbyVehiclesToPlayer(std::function<void(int)> callback) {
	nearbyEnts arr;
	arr.size = nearbyPedArraySize;
	int ped = PLAYER::PLAYER_PED_ID();
	int size = PED::GET_PED_NEARBY_VEHICLES(ped, (int*)&arr);
	for (int i = 0; i < size; i++) {
		callback(arr.entities[i]);
	}
}

//绘制UI 背景颜色
void DrawGameRect(float x, float y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	GRAPHICS::DRAW_RECT(x, y, width, height, r, g, b, a, true, true/*confused screaming*/);
}
// 绘制UI 边框
void drawBoxOutline(float x, float y, float radius, float thickness, int r, int g, int b, int a)
{
	int rx, ry;//originally float but int does make more sense...
	GRAPHICS::GET_SCREEN_RESOLUTION(&rx, &ry);
	float aspectRatio = (float)ry / (float)rx;//want y to be the standard

	float xLeft = x - radius * aspectRatio;
	float xRight = x + radius * aspectRatio;
	float yTop = y - radius;
	float yBottom = y + radius;
	float length = radius * 2;
	DrawGameRect(xLeft + radius * aspectRatio, yTop + thickness / 2.0f, length * aspectRatio, thickness, r, g, b, a);//top bar
	DrawGameRect(xLeft + radius * aspectRatio, yBottom - thickness / 2.0f, length * aspectRatio, thickness, r, g, b, a);//bottom bar
	DrawGameRect(xLeft + thickness * aspectRatio / 2.0f, yTop + radius, thickness * aspectRatio, length, r, g, b, a);//left bar
	DrawGameRect(xRight - thickness * aspectRatio / 2.0f, yTop + radius, thickness * aspectRatio, length, r, g, b, a);//right bar
}

void drawBoxOutline(float x, float y, float x_radius, float y_radius, float thickness, int r, int g, int b, int a)
{
	int rx, ry;//originally float but int does make more sense...
	GRAPHICS::GET_SCREEN_RESOLUTION(&rx, &ry);
	float aspectRatio = (float)ry / (float)rx;//want y to be the standard

	float xLeft = x - x_radius * aspectRatio;
	float xRight = x + x_radius * aspectRatio;
	float yTop = y - y_radius;
	float yBottom = y + y_radius;

	float length = x_radius * 2;
	DrawGameRect(xLeft + x_radius * aspectRatio, yTop + thickness / 2.0f, length * aspectRatio, thickness, r, g, b, a);//top bar
	DrawGameRect(xLeft + x_radius * aspectRatio, yBottom - thickness / 2.0f, length * aspectRatio, thickness, r, g, b, a);//bottom bar
	DrawGameRect(xLeft + thickness * aspectRatio / 2.0f, yTop + y_radius, thickness * aspectRatio, length, r, g, b, a);//left bar
	DrawGameRect(xRight - thickness * aspectRatio / 2.0f, yTop + y_radius, thickness * aspectRatio, length, r, g, b, a);//right bar
}


//int GetBASERANK
int GetBaseRank(int attributeIndex, bool isHorse)
{
	Ped ped = PLAYER::PLAYER_PED_ID();

	if (isHorse)
	{
		ped = PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(PLAYER::PLAYER_ID());
		if (PED::IS_PED_ON_MOUNT(PLAYER::PLAYER_PED_ID()))
		{
			ped = PED::GET_MOUNT(PLAYER::PLAYER_PED_ID());	// 获取玩家骑乘的马匹
		}
	}

	int val =  ATTRIBUTE::GET_ATTRIBUTE_BASE_RANK(ped, attributeIndex);


	return val;
}


// 上帝镜头
float clamp360(float value) {
	while (value < 0)
		value += 360;
	return value;
}
namespace camShit {
	void loop() {
		if (CAM::DOES_CAM_EXIST(cam)) {
			if (doCamRot)
				CAM::SET_CAM_ROT(cam, rotation.x, rotation.y, rotation.z, 2);	//
			if (doCamPos)
				CAM::SET_CAM_COORD(cam, location.x, location.y, location.z);	//设置镜头坐标
			if (doFocus)
				STREAMING::SET_FOCUS_POS_AND_VEL(location.x, location.y, location.z, velocityForFocus.x, velocityForFocus.y, velocityForFocus.z);	// 设置新焦点
			if (doLookat) {
				CAM::POINT_CAM_AT_COORD(cam, rotation.x, rotation.y, rotation.z);	//将镜头指向坐标
				Vector3 r = CAM::GET_CAM_ROT(cam, 2);	//获取镜头焦点
				r.x = clamp360(r.x);
				r.y = clamp360(r.y);
				r.z = clamp360(r.z);
				CAM::SET_CAM_ROT(cam, r.x, 0/*r.y*/, r.z, 2);
			}
		}
	}

	void attachToLocation(float x, float y, float z, bool setFocusOnArea) {
		doCamRot = true;//no need to set rot... we are doing lookat
		doCamPos = true;
		doFocus = true;
		doLookat = false;
		if (setFocusOnArea) {
			STREAMING::CLEAR_FOCUS();	//清除聚焦
			STREAMING::SET_FOCUS_POS_AND_VEL(x, y, z, 0, 0, 0);//should be right I think	设置新焦点
		}
		entity = 0;
		if (!CAM::DOES_CAM_EXIST(cam))	//摄像机是否存在
			cam = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 1);	//创建摄像机
		CAM::SET_CAM_COORD(cam, x, y, z);	//设置摄像机坐标
		CAM::SET_CAM_ACTIVE(cam, 1);	//激活摄像机
		CAM::RENDER_SCRIPT_CAMS(1, 0, 3000, 1, 0, false/*idk*/);	//渲染摄像机
	}

	void resetCam(bool clearFocus) {
		if (CAM::DOES_CAM_EXIST(cam)) {
			if (clearFocus)
				STREAMING::CLEAR_FOCUS();
			CAM::SET_CAM_ACTIVE(cam, 0);
			CAM::RENDER_SCRIPT_CAMS(0, 0, 3000, 1, 0, false/*idk*/);
			CAM::DESTROY_CAM(cam, 0);
			cam = 0;
		}
		cam = 0;
	}
	bool enabled() {

		return CAM::DOES_CAM_EXIST(cam);
	}
}
// 获取地面Z轴
float getHeightAboveGround(Vector3 position)
{
	float groundZ = 0;
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(position.x, position.y, 1000.0f, &groundZ, true);
	return groundZ;
}


//
//// 用户操作记录保存
//class UserConfig {
//	
//	std::string configFilePath = "\\RDR2NativeTrainer\\UserConfig.json";
//	// 配置储存对象 json数据
//	json config; 
//
//public:
//	UserConfig() {
//		
//	}
//	void Save(string key) 
//	{
//		
//	}
//};