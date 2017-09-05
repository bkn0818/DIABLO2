#pragma once
#include "singletonBase.h"
#include <vector>
#include <string>

class txtData : public singletonBase<txtData>
{
public:
	HRESULT init();
	void release();

	//���̺�
	void txtSave(const WCHAR* saveFileName, vector<wstring> vStr);
	WCHAR* vectorArrayCombine(vector<wstring> vArray);

	//�ε�
	vector<wstring> txtLoad(const WCHAR* loadFileName);
	vector<wstring> charArraySeparation(WCHAR charArray[]);

	txtData();
	~txtData();
};

