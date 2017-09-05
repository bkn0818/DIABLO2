#include "stdafx.h"
#include "aStarManager.h"

aStarTile::aStarTile()
	: _totalCost(0), _costFromStart(0),
	_costToGoal(0), _parentNode(NULL)
{
}

aStarTile::~aStarTile()
{
}

HRESULT aStarManager::init()
{
	_startTile = NULL;
	_currentTile = NULL;
	_goalTile = NULL;

	return S_OK;
}

void aStarManager::release()
{
	//���ʹ� ����ϰ� ����
	this->deleteAll();

	//Ÿ�ϵ� ������
	SAFE_DELETE(_startTile);
	SAFE_DELETE(_goalTile);
	SAFE_DELETE(_currentTile);
}

void aStarManager::setCurrentMap(tagIso* currentMap, int tileNum)
{
	_vTotalList.clear();

	_startTile = NULL;
	_goalTile = NULL;
	_currentTile = NULL;

	// ���� ���� �� ����� �̴ϴ�.
	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		//temp���� �ϳ� �����ؼ�
		aStarTile* node = new aStarTile;

		//�� DA~
		//�ʱ�ȭ �����ݴϴ�!
		node->setIso(currentMap[i]);
		node->setTotalCost(0);
		node->setCostFromStart(0);
		node->setCostToGoal(0);

		//�������� �ִ��� ��������
		//Ÿ���� �Ӽ����̳� ĳ���Ͱ� ������
		//������ �����ݽô�!
//		if (node->getIso().ter == TER_LOAD || node->getIso().ter == TER_VOID) node->setIsOpen(1);
		if (node->getIso().obj == OBJ_ERASE && node->getIso().ter == TER_LOAD) node->setIsOpen(1);
		else if (node->getIso().obj == OBJ_ERASE || node->getIso().ter == TER_VOID) node->setIsOpen(0);
		else node->setIsOpen(0);

		_vTotalList.push_back(node);
	}

	setStartTile(tileNum);
}

//�� �� �ִ� �� ����
vector<aStarTile*> aStarManager::addOpenList(aStarTile* currentTile)
{
	int startX = currentTile->getIso().indexX - 1;	// �˻� ������ ��ǥ X
	int startY = currentTile->getIso().indexY - 1;	// �˻� ������ ��ǥ Y

													// �ڽ��� �ֺ�Ÿ���� �˻��ؾߵǱ� ������ ���� �ΰ� �����ϴ�.
	for (int y = 0; y < 3; ++y) for (int x = 0; x < 3; ++x)
	{
		// �迭�� ������ �ǰų� �Ѿ�� ������.
		if (startX + x < 0 || startY + y < 0) continue;
		if (startX + x >= TILEX || startY + y >= TILEY) continue;

		// �밢�� �����ֱ� ���� ����ó���Դϴ�.
		if (y == 0 && x == 0) continue;
		if (y == 0 && x == 2) continue;
		if (y == 2 && x == 0) continue;
		if (y == 2 && x == 2) continue;

		// �˻��� �������Ϳ� �����̴ϴ�.
		aStarTile* node = _vTotalList[(startY * TILEX) + startX + x + (y * TILEX)];

		// �� �����ְų� ���������̸� ������ �Ѱ��ݽô�.
		// �Ʊ� �ϴٰ� �̰Ŷ����� �ָԾ����.. (�θ��忡 ��� �Ȱ����� ��ܼ� ���ѷ��� �ȴٴ��� �׷���..)
		if (!node->getIsOpen()) continue;
		if (node == _startTile) continue;
		//if (node->getIso().ter == TER_WALL) continue;

		// ���� �θ��忡 ����Ÿ�� �־��ݽô�.
		// (�� pathfinder �Լ����� _currentTile�� NULL �̸� While�� ���������ϱ� �� ó���� NULL�� �ʱ�ȭ ������մϴ�!)
		node->setParentNode(_currentTile);

		// �߰��Ұ��� ���Ұ��� �˻��� �ӽú���
		bool addObj = true;

		// ���࿡ �Ȱ��� ���� ���� �߰����ϰ� �����ݴϴ�. (����ó��)
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == node)
			{
				addObj = false;
				break;
			}
		}

		// ����ó����
		if (!addObj) continue;

		// �� �˻縦 ������ ���Ϳ� ����ݽô�.
		_vOpenList.push_back(node);
	}

	// �˻簡 �������� ���� �Ѱ��ݽô�.
	return _vOpenList;
}

//Ÿ�� �˻�
void aStarManager::pathFinder(aStarTile* currentTile)
{
	// �˻糪 �ӽ÷� ������� �ӽ� ������ �Դϴ�.
	float tempTotalCost = 5000;
	aStarTile* tempTile;
	if (_goalTile->getIso().ter != TER_LOAD)
	{
		_goalTile = NULL;
		return;
	}

	// �� for���� ������ addOpenList() �Լ����� ������ ��ŭ�� ���ϴ�.
	// �����Ѱ� ������ �翬�� �� for���� ���� �ʰ���? (���� ������ �ӽú��� tempTile�� �翬�� �����Ⱚ)
	for (int i = 0; i < addOpenList(currentTile).size(); i++)
	{
		// ������ ��ǥ�߿� �������������� ����� ���մϴ�.
		_vOpenList[i]->setCostToGoal((abs(_goalTile->getIso().indexX - _vOpenList[i]->getIso().indexX)
			+ abs(_goalTile->getIso().indexY - _vOpenList[i]->getIso().indexY)) * 10);


		// ���� ��ǥ���� ������ ��ǥ������ ����� ���մϴ�.
		_vOpenList[i]->setCostFromStart(10);

		// ���� ���ؼ� ���� ����� ���մϴ�.
		_vOpenList[i]->setTotalCost(_vOpenList[i]->getCostToGoal() + _vOpenList[i]->getCostFromStart());

		//���� ����� ���� ���� ã���ϴ�.
		if (tempTotalCost > _vOpenList[i]->getTotalCost())
		{
			tempTotalCost = _vOpenList[i]->getTotalCost();
			tempTile = _vOpenList[i];
		}

		bool addObj = true;
		// ������ �����߿� tempTile�� ������ �߰����� �ʽ��ϴ�.
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == tempTile)
			{
				//addObj ����
				addObj = false;
				continue;
			}
		}

		// ���߿� �����Ҷ��� ���� ���°�(����ó��)
		_vOpenList[i]->setIsOpen(false);
		if (!addObj) continue;
		_vOpenList.push_back(tempTile);
	}

	if (addOpenList(currentTile).size() == NULL)
	{
		vectorClear();
		return;
	}

	//tempTile�� ������ Ÿ�Ͽ� ����������
	if (tempTile == _goalTile)
	{
		// ������������ �����̵��� ���������� ����Ʈ�� ����ְ�
		_vMoveList.insert(_vMoveList.begin(), tempTile->getIso());
		//�̶����� ������ Ÿ���� ����ݽô�~
		while (_currentTile->getParentNode() != NULL)
		{
			_vMoveList.insert(_vMoveList.begin(), _currentTile->getIso());
			_currentTile = _currentTile->getParentNode();
		}

		return;
	}

	//����� Ÿ�� ��� ���Ϳ� ����Ÿ�� Ǫ�ù�
	_vCloseList.push_back(tempTile);

	//���¸���Ʈ�߿� ����� Ÿ���� ������ ����
	for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
	{
		if (*_viOpenList == tempTile)
		{
			_viOpenList = _vOpenList.erase(_viOpenList);
			break;
		}
	}

	_currentTile = tempTile;

	pathFinder(_currentTile);
}

//������ �� �ִ� Ÿ�� �˻�
void aStarManager::movablepathFinder(aStarTile* currentTile)
{
	// �˻糪 �ӽ÷� ������� �ӽ� ������ �Դϴ�.
	float tempTotalCost = 5000;
	aStarTile* tempTile;

	// �� for���� ������ addOpenList() �Լ����� ������ ��ŭ�� ���ϴ�.
	// �����Ѱ� ������ �翬�� �� for���� ���� �ʰ���? (���� ������ �ӽú��� tempTile�� �翬�� �����Ⱚ)
	for (int i = 0; i < addOpenList(currentTile).size(); i++)
	{
		if (_vOpenList[i]->getIso().ter == TER_VOID) continue;

		// ������ ��ǥ�߿� �������������� ����� ���մϴ�.
		_vOpenList[i]->setCostToGoal((abs(_goalTile->getIso().indexX - _vOpenList[i]->getIso().indexX)
			+ abs(_goalTile->getIso().indexY - _vOpenList[i]->getIso().indexY)) * 10);


		// ���� ��ǥ���� ������ ��ǥ������ ����� ���մϴ�.
		_vOpenList[i]->setCostFromStart(10);

		// ���� ���ؼ� ���� ����� ���մϴ�.
		_vOpenList[i]->setTotalCost(_vOpenList[i]->getCostToGoal() + _vOpenList[i]->getCostFromStart());

		//���� ����� ���� ���� ã���ϴ�.
		if (tempTotalCost > _vOpenList[i]->getTotalCost())
		{
			tempTotalCost = _vOpenList[i]->getTotalCost();
			tempTile = _vOpenList[i];
		}

		bool addObj = true;
		// ������ �����߿� tempTile�� ������ �߰����� �ʽ��ϴ�.
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == tempTile)
			{
				//addObj ����
				addObj = false;
				continue;
			}
		}

		// ���߿� �����Ҷ��� ���� ���°�(����ó��)
		_vOpenList[i]->setIsOpen(false);
		if (!addObj) continue;
		_vOpenList.push_back(tempTile);
	}

	if (addOpenList(currentTile).size() == NULL)
	{
		vectorClear();
		return;
	}

	//tempTile�� ������ Ÿ�Ͽ� ����������
	if (tempTile == _goalTile)
	{
		// ������������ �����̵��� ���������� ����Ʈ�� ����ְ�
		if (!checkList(_vMovableList, tempTile))
		{
			_vMovableList.insert(_vMovableList.begin(), tempTile->getIso());
			if (_vMovableList.size() >= 5) return;
		}
		//�̶����� ������ Ÿ���� ����ݽô�~
		while (_currentTile->getParentNode() != NULL)
		{
			if (!checkList(_vMovableList, _currentTile)) _vMovableList.insert(_vMovableList.begin(), _currentTile->getIso());
			_currentTile = _currentTile->getParentNode();
		}

		return;
	}

	//����� Ÿ�� ��� ���Ϳ� ����Ÿ�� Ǫ�ù�
	_vCloseList.push_back(tempTile);

	//���¸���Ʈ�߿� ����� Ÿ���� ������ ����
	for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
	{
		if (*_viOpenList == tempTile)
		{
			_viOpenList = _vOpenList.erase(_viOpenList);
			break;
		}
	}

	_currentTile = tempTile;

	movablepathFinder(_currentTile);
}

bool aStarManager::checkList(vector<tagIso> &list, aStarTile* tile)
{
	for (int i = 0; i > list.size(); ++i)
	{
		if (list[i].indexX == tile->getIso().indexX && list[i].indexY == tile->getIso().indexY)
		{
			return true;
		}
	}

	return false;
}

//�÷��̾��� ���; ��ǥ Ÿ���� �� �� �ִ� �͵��� �����ش�
void aStarManager::renderGoalList()
{
	WCHAR str[64];

	for (int i = 0; i < _vMoveList.size(); ++i)
	{
		wsprintf(str, L"%d %d", _vMoveList[i].indexX, _vMoveList[i].indexY);
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, WINSIZEX - 200, i * 50 + 50, WINSIZEX - 100, i * 50 + 100);
	}
}

void aStarManager::drawMovableTile()
{
	WCHAR str[64];

	for (int i = 0; i < _vMovableList.size(); ++i)
	{
		wsprintf(str, L"%d %d", _vMovableList[i].indexX, _vMovableList[i].indexY);
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str,
			_vMovableList[i].centerX - CAMERAMANAGER->getX(),
			_vMovableList[i].centerY - CAMERAMANAGER->getY(),
			_vMovableList[i].centerX + 30 - CAMERAMANAGER->getX(),
			_vMovableList[i].centerY + 30 - CAMERAMANAGER->getY());
	}
}

void aStarManager::render()
{
	if (_goalTile != NULL)
	{
		WCHAR str[126];
		swprintf(str, L"%d %d", _goalTile->getIso().indexX, _goalTile->getIso().indexY);
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 500, 0, 550, 10);
	}
	if (_startTile != NULL)
	{
		WCHAR str[128];
		swprintf(str, L"%d %d", _startTile->getIso().indexX, _startTile->getIso().indexY);
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 600, 0, 650, 10);
	}
	for (int i = 0; i < _vMoveList.size(); i++)
	{
		WCHAR str[128];
		swprintf(str, L"%d %d", _vMoveList[i].indexX, _vMoveList[i].indexY);
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str,
			_vMoveList[i].centerX - CAMERAMANAGER->getX(),
			_vMoveList[i].centerY - CAMERAMANAGER->getY(),
			_vMoveList[i].centerX + 30 - CAMERAMANAGER->getX(),
			_vMoveList[i].centerY + 30 - CAMERAMANAGER->getY());
	}
}

//�̵������� Ÿ���� �����Ѵ�
void aStarManager::getMovablePath(tagIso* currentMap, int arrNum, int moveNum)
{
//	setStartTile(arrNum);
//	setCurrentMap(currentMap, arrNum);

	if (_vMovableList.size()) _vMovableList.clear();

	int startX = _startTile->getIso().indexX - moveNum;
	int startY = _startTile->getIso().indexY - moveNum;

	for (int x = 0; x < ((moveNum * 2) + 1); ++x) for (int y = 0; y < ((moveNum * 2) + 1); ++y)
	{
		if (x == moveNum && y == moveNum) continue;
		if (startX + x <= 0 || startY + y <= 0) continue;
		if (startX + x >= TILEX || startY + y >= TILEY) continue;

		setCurrentMap(currentMap, arrNum);
		setGoalTile((startY * TILEX) + startX + x + (y * TILEX));
		movablepathFinder(_startTile);
	}

	vectorClear();
	setCurrentMap(currentMap, arrNum);
}

void aStarManager::vectorClear()
{
	for (auto it = _vOpenList.begin(); it != _vOpenList.end(); ++it)
	{
		delete (*it);
	}
	_vOpenList.clear();

	for (auto it = _vCloseList.begin(); it != _vCloseList.end(); ++it)
	{
		delete (*it);
	}
	_vCloseList.clear();

	//	for (auto it = _vMoveList.begin(); it != _vMoveList.end(); ++it)
	//	{
	//		delete (&it);
	//	}
	_vMoveList.clear();
}

void aStarManager::deleteAll()
{
	for (auto it = _vTotalList.begin(); it != _vTotalList.end(); ++it)
	{
		delete (*it);
	}
	_vTotalList.clear();

	for (auto it = _vOpenList.begin(); it != _vOpenList.end(); ++it)
	{
		delete (*it);
	}
	_vOpenList.clear();

	for (auto it = _vCloseList.begin(); it != _vCloseList.end(); ++it)
	{
		delete (*it);
	}
	_vCloseList.clear();

	//	for (auto it = _vMoveList.begin(); it != _vMoveList.end(); ++it)
	//	{
	//		delete (&it);
	//	}
	_vMoveList.clear();

	//	for (auto it = _vMovableList.begin(); it != _vMovableList.end(); ++it)
	//	{
	//		delete (&it);
	//	}
	_vMovableList.clear();

}