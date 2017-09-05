#pragma once
#include "singletonBase.h"
#include <vector>
#include <map>

class elements
{
public:
	const WCHAR* name;
	int currentHP;
	int maxHP;
	float angle;
	float accelaration;
	float maxSpeed;

	elements(){};
	~elements(){};
};

class database : public singletonBase<database>
{
private:
	typedef vector<wstring> arrElements;
	typedef vector<wstring>::iterator iterElements;
	typedef map<wstring, elements*> arrElement;
	typedef map<wstring, elements*>::iterator iterElement;

private:
	arrElement _mTotalElment;

	float _angle;

public:
	HRESULT init();
	void release();

	void loadDatabase(wstring name);

	//접근자
	elements* getElementData(wstring str) { return _mTotalElment.find(str)->second; }

	//설정자
	void setElementDataCurrentHP(wstring str, float ch);
	void setElementDataMaxHP(wstring str, float mh);
	void setElementDataAngle(wstring str, float a);
	void setElementDataAccelaration(wstring str, float accel);
	void setElementDataMaxSpeed(wstring str, float ms);

	void setAngle(float angle) { _angle = angle; }
	float getAngle() { return _angle; }


	database();
	~database();
};

