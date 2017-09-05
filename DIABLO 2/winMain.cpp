#include "stdafx.h"
#include "diablo2.h"

//전역변수 선언
HINSTANCE	_hInstance;		//윈도우 APP 고유번호
HWND		_hWnd;
bool _leftButtonDown;

LPTSTR _lpszClass = TEXT("Omega API");

diablo2 _disgaea;

//포인트 변수 -> 구조체로 이루어져있고, x, y 좌표를 담을 수 있다.
POINT _ptMouse;


//함수의 프로토 타입 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void setWindowsSize(int x, int y, int width, int height);

// Cmd == Command 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG			message;		//운영체제에서 발생하는 메시지 정보를 저장하기 위한 구조체
	WNDCLASS	wndClass;		//윈도우의 정보를 저장하기 위한 구조체(어떤 형태의 윈도우 창인가?)

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//클래스 여분 메모리 설정
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리 설정
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//윈도우 색깔을 하얗게
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//윈도우 커서 모양(현재 화살표)
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//윈도우 아이콘 모양
	wndClass.hInstance = hInstance;									//윈도우 인스턴스
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//윈도우 프로시져
	wndClass.lpszClassName = WINNAME;								//윈도우 클래스이름
	wndClass.lpszMenuName = NULL;									//윈도우 메뉴이름( 안사용)
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 스타일 지정

	//윈도우 정보를 등록해주자
	RegisterClass(&wndClass);

	_hWnd = CreateWindow(WINNAME, 
						WINNAME, 
						WS_OVERLAPPEDWINDOW, 
						WINSTARTX, 
						WINSTARTY, 
						WINSIZEX, 
						WINSIZEY, 
						NULL,
						(HMENU)NULL, 
						hInstance, 
						NULL);

	//함수를 만들었으니 써먹는다
	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);


	//이것을 해주면 윈도우 창이 눈에 보여진다
	ShowWindow(_hWnd, cmdShow);

	//메인게임이 초기화가 안되면
	if (FAILED(_disgaea.init()))
	{
		//꺼버려라
		return 0;
	}

	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			TIMEMANAGER->update(60.0);
			_disgaea.update();
			_disgaea.render();
		}

	}


	/*
	//메시지 루프 선언

	//GetMessage : 메시지가 발생해야 처리를 한다
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);		//키보드 입력 되는 순간 그 메시지 처리를 담당
		DispatchMessage(&message);		//실제로 윈도우로 메시지를 전달해주는 역할 담당
	}
	*/
	//메시지 루프를 다 돌았다면 해제해줘라.
	_disgaea.release();

	return message.wParam;
}

//함수를 씁시다
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _disgaea.MainProc(hWnd, iMessage, wParam, lParam);
}

//클라이언트 영역 재조정 함수
void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;		//가로크기
	winRect.bottom = height;	//세로크기

	AdjustWindowRect(&winRect, WINSTYLE, false);

	SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);

}
