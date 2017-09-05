#include "stdafx.h"
#include "database.h"


database::database()
{
}


database::~database()
{

}

HRESULT database::init()
{
	_angle = 0.0f;

	loadDatabase(L"database.txt");


	return S_OK;
}

void database::release()
{

}


void database::loadDatabase(wstring name)
{
	//데이터를 읽어오자
	arrElements vTemp;
	vTemp = TXTDATA->txtLoad(name.c_str());

	wstring str;
	int count = 0;

	//칸막이 | 를 기점으로 속성들을 읽어오자!

	for (int i = 0; i < vTemp.size(); i++)
	{
		//|,배종류,현체력,최체력,앵글,맥스스피드,가속도
		if (vTemp[i] == L"|")
		{
			elements* em = new elements;
			str = vTemp[i + 1];

			_mTotalElment.insert(pair<wstring, elements*>(vTemp[i + 1], em));
			
			if (i != 0) count += 7;
			continue;
		}

		iterElement mIter = _mTotalElment.find(str);

		if (i == count + 1) mIter->second->name = vTemp[i].c_str();
		else if (i == count + 2) mIter->second->currentHP = (int)_wtoi(vTemp[i].c_str());
		else if (i == count + 3) mIter->second->maxHP = (int)_wtoi(vTemp[i].c_str());
		else if (i == count + 4) mIter->second->angle = (float)_wtoi(vTemp[i].c_str());
		else if (i == count + 5) mIter->second->maxSpeed = (float)_wtoi(vTemp[i].c_str());
		else if (i == count + 6) mIter->second->accelaration = (float)_wtoi(vTemp[i].c_str());
	}

	vTemp.clear();
}


//설정자
void database::setElementDataCurrentHP(wstring str, float ch)
{
	iterElement mIter = _mTotalElment.find(str);
	mIter->second->currentHP = ch;
}

void database::setElementDataMaxHP(wstring str, float mh)
{
	iterElement mIter = _mTotalElment.find(str);
	mIter->second->maxHP = mh;
}

void database::setElementDataAngle(wstring str, float a)
{
	iterElement mIter = _mTotalElment.find(str);
	mIter->second->angle = a;
}

void database::setElementDataAccelaration(wstring str, float accel)
{
	iterElement mIter = _mTotalElment.find(str);
	mIter->second->accelaration = accel;
}

void database::setElementDataMaxSpeed(wstring str, float ms)
{
	iterElement mIter = _mTotalElment.find(str);
	mIter->second->maxSpeed = ms;
}
