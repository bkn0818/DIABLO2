#include "stdafx.h"
#include "aStarManager.h"
#include "aStar.h"

aStarTile::aStarTile()
	: _totalCost(0), _costFromStart(0),
	_costToGoal(0), _parentNode(NULL)
{
}

aStarTile::~aStarTile()
{
}

aStar::aStar()
{
}

aStar::~aStar()
{
}


HRESULT aStar::init()
{
	_startTile = NULL;
	_currentTile = NULL;
	_goalTile = NULL;

	return S_OK;
}

void aStar::release()
{
	//벡터는 깔끔하게 정리
	_vTotalList.clear();
	_vOpenList.clear();
	_vCloseList.clear();
	_vMoveList.clear();
	_vMovableList.clear();

	//타일들 날리기
	SAFE_DELETE(_startTile);
	SAFE_DELETE(_goalTile);
	SAFE_DELETE(_currentTile);
}

void aStar::setCurrentMap(tagIso* currentMap, int tileNum)
{
	_vTotalList.clear();
	_startTile = NULL;
	_goalTile = NULL;
	_currentTile = NULL;

	// 현재 맵을 다 담아줄 겁니다.
	for (int i = 0; i < TILEX * TILEY; ++i)
	{
		//temp변수 하나 선언해서
		aStarTile* node = new aStarTile;

		//싹 DA~
		//초기화 시켜줍니다!
		node->setIso(currentMap[i]);
		node->setTotalCost(0);
		node->setCostFromStart(0);
		node->setCostToGoal(0);

		//지나갈수 있는지 없는지는
		//타일의 속성값이나 캐릭터가 있을때
		//못가게 막아줍시다!
		if (node->getIso().ter == TER_LOAD) node->setIsOpen(1);
		else node->setIsOpen(0);

		_vTotalList.push_back(node);
	}

	setStartTile(tileNum);
}

//갈 수 있는 길 색출
vector<aStarTile*> aStar::addOpenList(aStarTile* currentTile)
{
	int startX = currentTile->getIso().indexX - 1;	// 검사 시작할 좌표 X
	int startY = currentTile->getIso().indexY - 1;	// 검사 시작할 좌표 Y

													// 자신의 주변타일을 검사해야되기 때문에 포문 두개 돌립니다.
	for (int y = 0; y < 3; ++y) for (int x = 0; x < 3; ++x)
	{
		// 배열이 음수가 되거나 넘어가면 터져요.
		if (startX + x < 0 || startY + y < 0) continue;
		if (startX + x >= TILEX || startY + y >= TILEY) continue;

		// 대각선 막아주기 위한 예외처리입니다.
		if (y == 0 && x == 0) continue;
		if (y == 0 && x == 2) continue;
		if (y == 2 && x == 0) continue;
		if (y == 2 && x == 2) continue;

		// 검사할 영역벡터에 넣을겁니다.
		aStarTile* node = _vTotalList[(startY * TILEX) + startX + x + (y * TILEX)];

		// 길 막혀있거나 시작지점이면 무조건 넘겨줍시다.
		// 아까 하다가 이거때문에 애먹었어요.. (부모노드에 계속 똑같은거 담겨서 무한루프 된다던가 그런거..)
		if (!node->getIsOpen()) continue;
		if (node == _startTile) continue;

		// 이제 부모노드에 현재타일 넣어줍시다.
		// (밑 pathfinder 함수에서 _currentTile가 NULL 이면 While문 빠져나가니까 꼭 처음은 NULL로 초기화 해줘야합니다!)
		node->setParentNode(_currentTile);

		// 추가할건지 안할건지 검사할 임시변수
		bool addObj = true;

		// 만약에 똑같은 값이 들어가면 추가못하게 막아줍니다. (예외처리)
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == node)
			{
				addObj = false;
				break;
			}
		}

		// 예외처리용
		if (!addObj) continue;

		// 다 검사를 했으니 벡터에 담아줍시다.
		_vOpenList.push_back(node);
	}

	// 검사가 끝났으니 벡터 넘겨줍시다.
	return _vOpenList;
}

//타일 검사
void aStar::pathFinder(aStarTile* currentTile)
{
	// 검사나 임시로 담기위한 임시 변수들 입니다.
	float tempTotalCost = 5000;
	aStarTile* tempTile;

	// 이 for문은 위에서 addOpenList() 함수에서 검출한 만큼만 돕니다.
	// 검출한게 없으면 당연히 이 for문은 돌지 않겠죠? (돌지 않으면 임시변수 tempTile은 당연히 쓰레기값)
	for (int i = 0; i < addOpenList(currentTile).size(); i++)
	{
		// 검출한 좌표중에 도착지점까지의 비용을 구합니다.
		_vOpenList[i]->setCostToGoal((abs(_goalTile->getIso().indexX - _vOpenList[i]->getIso().indexX)
			+ abs(_goalTile->getIso().indexY - _vOpenList[i]->getIso().indexY)) * 10);


		// 현재 좌표부터 검출한 좌표까지의 비용을 구합니다.
		_vOpenList[i]->setCostFromStart(10);

		// 둘을 더해서 최종 비용을 구합니다.
		_vOpenList[i]->setTotalCost(_vOpenList[i]->getCostToGoal() + _vOpenList[i]->getCostFromStart());

		//가장 비용이 작은 값을 찾습니다.
		if (tempTotalCost > _vOpenList[i]->getTotalCost())
		{
			tempTotalCost = _vOpenList[i]->getTotalCost();
			tempTile = _vOpenList[i];
		}

		bool addObj = true;
		// 검출한 벡터중에 tempTile과 같으면 추가하지 않습니다.
		for (_viOpenList = _vOpenList.begin(); _viOpenList != _vOpenList.end(); ++_viOpenList)
		{
			if (*_viOpenList == tempTile)
			{
				//addObj 폴스
				addObj = false;
				continue;
			}
		}

		// 나중에 검출할때를 위해 막는것(예외처리)
		_vOpenList[i]->setIsOpen(false);
		if (!addObj) continue;
		_vOpenList.push_back(tempTile);
	}

	if (addOpenList(currentTile).size() == NULL)
	{
		vectorClear();
		return;
	}

	//tempTile이 도착할 타일에 도착했으면
	if (tempTile == _goalTile)
	{
		// 도착지점까지 움직이도록 움직여야할 리스트에 담아주고
		_vMoveList.insert(_vMoveList.begin(), tempTile->getIso());
		//이때까지 지나온 타일을 담아줍시다~
		while (_currentTile->getParentNode() != NULL)
		{
			_vMoveList.insert(_vMoveList.begin(), _currentTile->getIso());
			_currentTile = _currentTile->getParentNode();
		}

		return;
	}

	//가까운 타일 담는 벡터에 템프타일 푸시백
	_vCloseList.push_back(tempTile);

	//오픈리스트중에 가까운 타일이 있으면 삭제
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

//플레이어의 경우; 목표 타일이 될 수 있는 것들을 보여준다
void aStar::renderGoalList()
{
	WCHAR str[64];

	for (int i = 0; i < _vMoveList.size(); ++i)
	{
		wsprintf(str, L"%d %d", _vMoveList[i].indexX, _vMoveList[i].indexY);
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, WINSIZEX - 200, i * 50 + 50, WINSIZEX - 100, i * 50 + 100);
	}
}

void aStar::render()
{
	if (_goalTile)
	{
		WCHAR str[126];
		swprintf(str, L"%d %d", _goalTile->getIso().indexX, _goalTile->getIso().indexY);
		DIRECT2D->drawTextD2D(DIRECT2D->_defaultBrush, str, 500, 0, 550, 10);
	}
	if (_startTile)
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

/*
//이동가능한 타일을 설정한다
void aStar::setMoveTile(tagIso tile)  <- 이 함수를 아래와 같이 변경함.(민수)
{
int startX = tile.indexX - 5;//-5칸 -5칸부터 주르륵 검색하는 것...
int startY = tile.indexY - 5;

for (int i = 0; i < 11; i++)
{
for (int j = 0; j < 11; j++)
{
//일단 범위밖은 뜨지 않음
if (startX + j < 0 || startY + i < 0) continue;
if (startX + j > TILEX - 1 || startY + i > TILEY - 1) continue;

tagIso node = _vTotalList[(startY * TILEX) + startX + j + (i * TILEX)]->getIso();

if (node.ter == 1) continue;//벽이라면 뜨지 않음

if (abs(node.indexX - _startTile->getIso().indexX) + abs(node.indexY - _startTile->getIso().indexY) <= 5)
{
//타일 이미지를 띄운다
_vMoveList.push_back(node);//니가 움직일 수 있는 존나 요만큼의 공간
}
}
}
}
*/

//이동가능한 타일을 설정한다
void aStar::getMovablePath(tagIso tile)
{

}

void aStar::vectorClear()
{
	_vOpenList.clear();
	_vCloseList.clear();
	_vMoveList.clear();
}