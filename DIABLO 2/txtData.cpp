#include "stdafx.h"
#include "txtData.h"


txtData::txtData()
{
}


txtData::~txtData()
{
}

HRESULT txtData::init()
{

	return S_OK;
}

void txtData::release()
{

}

//세이브
void txtData::txtSave(const WCHAR* saveFileName, vector<wstring> vStr)
{
	HANDLE file;

	WCHAR str[128];
	DWORD write;

	wcsncpy_s(str, 128, vectorArrayCombine(vStr), 126);

	file = CreateFile(saveFileName, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(file, str, 128, &write, NULL);

	CloseHandle(file);
}

WCHAR* txtData::vectorArrayCombine(vector<wstring> vArray)
{
	WCHAR str[128];

	ZeroMemory(str, sizeof(str));

	for (int i = 0; i < vArray.size(); i++)
	{
		wcsncat_s(str, 128, vArray[i].c_str(), 126);
		if (i + 1 < vArray.size()) wcscat(str, L",");
	}

	return str;
}


//로드
vector<wstring> txtData::txtLoad(const WCHAR* loadFileName)
{
	HANDLE file;

	WCHAR str[128];
	DWORD read;

	file = CreateFile(loadFileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(file, str, 128, &read, NULL);

	CloseHandle(file);

	return charArraySeparation(str);
}

vector<wstring> txtData::charArraySeparation(WCHAR charArray[])
{
	vector<wstring> vArray;

	WCHAR* temp;
	WCHAR* separator = L",";
	WCHAR* token;

	token = wcstok(charArray, separator);

	if (token == NULL) return vArray;

	vArray.push_back(token);

	while (NULL != (token = wcstok(NULL, separator)))
	{
		vArray.push_back(token);
	}

	return vArray;
}

