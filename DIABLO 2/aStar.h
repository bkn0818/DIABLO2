#pragma once
#include "tile.h"
#include <vector>

//aStar �˰��� ����� ���� Ÿ�ϵ�
class aStarTile
{
private:
	tagIso _iso;

	//f = g + h
	float _totalCost;			//f
	float _costFromStart;		//g == ���������� ���� ���(Ÿ��)���� ���
	float _costToGoal;			//h == ���� ���(Ÿ��)���� �������� ���

	bool _isOpen;				//�� �� �ִ� Ÿ���ΰ� 

	aStarTile* _parentNode;		//�� Ÿ���� �������(��� ���� �Ǵ�)

public:
	aStarTile();
	~aStarTile();

	// === getter, setter === //

	//iso Ÿ��
	void setIso(tagIso iso) { _iso = iso; }
	tagIso &getIso() { return _iso; }

	//���� �ڽ�Ʈ(f)
	void setTotalCost(float totalCost) { _totalCost = totalCost; }
	float getTotalCost() { return _totalCost; }

	//�������������� �ڽ�Ʈ(g)
	void setCostFromStart(float costFromStart) { _costFromStart = costFromStart; }
	float getCostFromStart() { return _costFromStart; }

	//�������������� �ڽ�Ʈ(h)
	void setCostToGoal(float costToGoal) { _costToGoal = costToGoal; }
	float getCostToGoal() { return _costToGoal; }

	//�θ� ���
	void setParentNode(aStarTile* t) { _parentNode = t; }
	aStarTile* getParentNode() { return _parentNode; }

	//�� �� �ִ� ��?
	void setIsOpen(bool isOpen) { _isOpen = isOpen; }
	bool getIsOpen(void) { return _isOpen; }
};

// �̰� ���� ���̽�Ÿ Ŭ����
class aStar
{
private:
	// �� ��ü
	vector<aStarTile*> _vTotalList;
	vector<aStarTile*>::iterator _viTotalList;

	//���� Ÿ�� �������� Ž���� �� �� �ִ� ��
	vector<aStarTile*> _vOpenList;
	vector<aStarTile*>::iterator _viOpenList;

	//Ž���� �� �� �ִ� ��(OpenList) �� ����� ���� ���� ��
	vector<aStarTile *> _vCloseList;
	vector<aStarTile *>::iterator _viCloseList;

	//ĳ���Ϳ��� �ѱ� ����� ��
	vector<tagIso> _vMoveList;
	vector<tagIso>::iterator _viMoveList;

	//�� �� �ִ� Ÿ��
	vector<tagIso> _vMovableList;
	vector<tagIso>::iterator _viMovableList;

	//���� �� <-- �Ⱦ�����!!! ��������!!!!
	//	tagIso* _currentMap;

	aStarTile* _startTile;		//���� Ÿ��
	aStarTile* _goalTile;		//��ǥ Ÿ��
	aStarTile* _currentTile;	//���� Ÿ��

public:
	aStar();
	~aStar();

	HRESULT init();
	void release();
	void render();

	//���⼭ ���� �����ɴϴ�!!
	void setCurrentMap(tagIso* currentMap, int tileNum);

	//���� ã���� �ʿ��� �Լ��Դϴ�!
	//pathFinder�ȿ��� ���Դϴ�!
	vector<aStarTile*> addOpenList(aStarTile* currentTile);

	//���� ã���� ���� �Լ��Դϴ�!
	//ã�� ������ �������� _vMoveList�ȿ� ����˴ϴ�!
	void pathFinder(aStarTile* currentTile);

	void renderGoalList();

	//���͵��� ���� �����ݴϴ�!
	//���� �� ã�� �� �ڿ� ĳ���Ͱ� �����ϸ� �׶� ���ݽô�!
	void vectorClear();

	// === inline, gettter, setter === // 

	//���� Ÿ��
	inline void setStartTile(int arrNum)
	{
		_startTile = _vTotalList[arrNum];
		_currentTile = _startTile;
	}
	aStarTile* getStartTile() { return _startTile; }

	//��ǥ Ÿ��
	inline void setGoalTile(int arrNum) { _goalTile = _vTotalList[arrNum]; }
	aStarTile* getGoalTile() { return _goalTile; }

	//���������� �������� ��ã�⸦ �����մϴ�!
	inline void startPathFinder() { pathFinder(_startTile); }

	//�÷��̾ �̵� ������ Ÿ��(���� �ȸ���)
	//	void setMoveTile(tagIso tile); <- �� �Լ��� �Ʒ��� ���� ������.(�μ�)
	void getMovablePath(tagIso tile);

	//ĳ���Ϳ��� �̵��ؾߵ� ���� �Ѱ��ش�
	//�� �Լ��� ĳ�������� �� �� �ִ� ���� �Ѱ��ִ°� �ƴ϶�,
	//ĳ���Ͱ� ������������ ��� �����ؾ��ϴ��� �Ѱ��ִ� ���̴�.
	inline vector<tagIso> &getMoveTile() { return _vMoveList; }
};