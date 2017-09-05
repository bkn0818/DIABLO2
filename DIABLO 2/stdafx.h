// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
#pragma once
#include "targetver.h"


#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "WindowsCodecs.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "Comdlg32.lib")

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#define _USE_MATH_DEFINES		// ���� ����� ����
#include <math.h>
#include <assert.h>
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <tchar.h>
#include <commdlg.h>			//OPENFILENAME
#include <wincodec.h>
#include <dwrite.h>
#include <d2d1_1.h>				// direct2D headerFile
#include <d2d1_1helper.h>		// direct2D headerFileHelper




// ����� ���� ��� ���� �� ���� �����̽�
#include "commonMacroFunction.h"
#include "randomFunction.h"
#include "keyManager.h"
#include "imageManager.h"
#include "timeManager.h"
#include "effectManager.h"
#include "soundManager.h"
#include "sceneManager.h"
#include "utils.h"
#include "txtData.h"
#include "database.h"
#include "direct2DManager.h"
#include "cameraManager.h"
#include "aStarManager.h"

using namespace std;
using namespace OMEGA_UTIL;
using namespace D2D1;

//==================================
// ## �����ι� ##
//==================================

#define WINNAME (LPTSTR)(TEXT("Disgaea"))
#define WINSTARTX 0			//������ â �������� X��ǥ
#define WINSTARTY 0			//������ â �������� Y��ǥ
#define WINSIZEX  1600			//������ â ����ũ��
#define WINSIZEY  900			//������ â ����ũ��
#define WINSTYLE WS_CAPTION | WS_SYSMENU


#define RND randomFunction::getSingleton()
#define KEYMANAGER keyManager::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define TIMEMANAGER timeManager::getSingleton()
#define EFFECTMANAGER effectManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define TXTDATA txtData::getSingleton()
#define DATABASE database::getSingleton()
#define DIRECT2D direct2DManager::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define CAMERAMANAGER cameraManager::getSingleton()
#define ASTARMANAGER aStarManager::getSingleton()

#define STATMANAGER statDataBase::getSingleton()
#define INVENMANAGER invenDataBase::getSingleton()

//==================================
// ��ũ�� �Լ� ##
//==================================

#define SAFE_DELETE(p)	{if(p) {delete(p); (p) = NULL;}}
#define SAFE_RELEASE(p) {if(p) {(p)->release(); (p) = NULL;}}
#define SAFE_RELEASE_CAPITAL(p) {if(p) {(p)->Release(); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[] (p); (p) = NULL;}}

//==================================
// ## ���� ���� ##
//==================================

extern HWND _hWnd;
extern HINSTANCE _hInstance;
extern POINT _ptMouse;
extern bool _leftButtonDown;



//===============================================�޸� ���� üũ============================
#ifndef _MEMORYLEAK_H_
#define _MEMORYLEAK_H_

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
static class MemoryMng
{
public:
	MemoryMng()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(3333);

		//���� ���� ��ΰ� ��Ȯ�� ���� ���� ��� ���� �ּ��� ���� Ȱ��ȭ ��
		//���� ��� �� ���忡 ���õ� ���ڸ� ��ȣ�ȿ� �ְ� ������ϸ� ������ �ٿ� ������� �����
	}
	~MemoryMng()
	{
		_ASSERTE(_CrtCheckMemory());
	}


} MemoryLeak;
#endif      // _DEBUG
#endif      // _MEMORYLEAK_H_


