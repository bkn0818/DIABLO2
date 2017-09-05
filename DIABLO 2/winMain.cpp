#include "stdafx.h"
#include "diablo2.h"

//�������� ����
HINSTANCE	_hInstance;		//������ APP ������ȣ
HWND		_hWnd;
bool _leftButtonDown;

LPTSTR _lpszClass = TEXT("Omega API");

diablo2 _disgaea;

//����Ʈ ���� -> ����ü�� �̷�����ְ�, x, y ��ǥ�� ���� �� �ִ�.
POINT _ptMouse;


//�Լ��� ������ Ÿ�� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void setWindowsSize(int x, int y, int width, int height);

// Cmd == Command 
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int cmdShow)
{
	MSG			message;		//�ü������ �߻��ϴ� �޽��� ������ �����ϱ� ���� ����ü
	WNDCLASS	wndClass;		//�������� ������ �����ϱ� ���� ����ü(� ������ ������ â�ΰ�?)

	_hInstance = hInstance;

	wndClass.cbClsExtra = 0;										//Ŭ���� ���� �޸� ����
	wndClass.cbWndExtra = 0;										//������ ���� �޸� ����
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//������ ������ �Ͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//������ Ŀ�� ���(���� ȭ��ǥ)
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//������ ������ ���
	wndClass.hInstance = hInstance;									//������ �ν��Ͻ�
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//������ ���ν���
	wndClass.lpszClassName = WINNAME;								//������ Ŭ�����̸�
	wndClass.lpszMenuName = NULL;									//������ �޴��̸�( �Ȼ��)
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//������ ��Ÿ�� ����

	//������ ������ ���������
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

	//�Լ��� ��������� ��Դ´�
	setWindowsSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);


	//�̰��� ���ָ� ������ â�� ���� ��������
	ShowWindow(_hWnd, cmdShow);

	//���ΰ����� �ʱ�ȭ�� �ȵǸ�
	if (FAILED(_disgaea.init()))
	{
		//��������
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
	//�޽��� ���� ����

	//GetMessage : �޽����� �߻��ؾ� ó���� �Ѵ�
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);		//Ű���� �Է� �Ǵ� ���� �� �޽��� ó���� ���
		DispatchMessage(&message);		//������ ������� �޽����� �������ִ� ���� ���
	}
	*/
	//�޽��� ������ �� ���Ҵٸ� ���������.
	_disgaea.release();

	return message.wParam;
}

//�Լ��� ���ô�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	return _disgaea.MainProc(hWnd, iMessage, wParam, lParam);
}

//Ŭ���̾�Ʈ ���� ������ �Լ�
void setWindowsSize(int x, int y, int width, int height)
{
	RECT winRect;

	winRect.left = 0;
	winRect.top = 0;
	winRect.right = width;		//����ũ��
	winRect.bottom = height;	//����ũ��

	AdjustWindowRect(&winRect, WINSTYLE, false);

	SetWindowPos(_hWnd, NULL, x, y, (winRect.right - winRect.left),
		(winRect.bottom - winRect.top), SWP_NOZORDER | SWP_NOMOVE);

}
