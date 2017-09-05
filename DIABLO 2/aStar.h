#pragma once
#include "tile.h"
#include <vector>

//aStar 알고리즘 계산을 위한 타일들
class aStarTile
{
private:
	tagIso _iso;

	//f = g + h
	float _totalCost;			//f
	float _costFromStart;		//g == 시작점부터 현재 노드(타일)까지 비용
	float _costToGoal;			//h == 현재 노드(타일)부터 끝점까지 비용

	bool _isOpen;				//갈 수 있는 타일인가 

	aStarTile* _parentNode;		//이 타일의 상위노드(계속 갱신 되는)

public:
	aStarTile();
	~aStarTile();

	// === getter, setter === //

	//iso 타일
	void setIso(tagIso iso) { _iso = iso; }
	tagIso &getIso() { return _iso; }

	//최종 코스트(f)
	void setTotalCost(float totalCost) { _totalCost = totalCost; }
	float getTotalCost() { return _totalCost; }

	//시작지점부터의 코스트(g)
	void setCostFromStart(float costFromStart) { _costFromStart = costFromStart; }
	float getCostFromStart() { return _costFromStart; }

	//도착지점부터의 코스트(h)
	void setCostToGoal(float costToGoal) { _costToGoal = costToGoal; }
	float getCostToGoal() { return _costToGoal; }

	//부모 노드
	void setParentNode(aStarTile* t) { _parentNode = t; }
	aStarTile* getParentNode() { return _parentNode; }

	//갈 수 있는 길?
	void setIsOpen(bool isOpen) { _isOpen = isOpen; }
	bool getIsOpen(void) { return _isOpen; }
};

// 이게 ㄹㅇ 에이스타 클래스
class aStar
{
private:
	// 맵 전체
	vector<aStarTile*> _vTotalList;
	vector<aStarTile*>::iterator _viTotalList;

	//지금 타일 기준으로 탐색한 갈 수 있는 길
	vector<aStarTile*> _vOpenList;
	vector<aStarTile*>::iterator _viOpenList;

	//탐색한 갈 수 있는 길(OpenList) 중 비용이 제일 적은 길
	vector<aStarTile *> _vCloseList;
	vector<aStarTile *>::iterator _viCloseList;

	//캐릭터에게 넘길 가까운 길
	vector<tagIso> _vMoveList;
	vector<tagIso>::iterator _viMoveList;

	//갈 수 있는 타일
	vector<tagIso> _vMovableList;
	vector<tagIso>::iterator _viMovableList;

	//현재 맵 <-- 안쓸꺼임!!! 저어어얼대로!!!!
	//	tagIso* _currentMap;

	aStarTile* _startTile;		//시작 타일
	aStarTile* _goalTile;		//목표 타일
	aStarTile* _currentTile;	//지금 타일

public:
	aStar();
	~aStar();

	HRESULT init();
	void release();
	void render();

	//여기서 맵을 가져옵니다!!
	void setCurrentMap(tagIso* currentMap, int tileNum);

	//길을 찾을때 필요한 함수입니다!
	//pathFinder안에서 쓰입니다!
	vector<aStarTile*> addOpenList(aStarTile* currentTile);

	//길을 찾을때 쓰는 함수입니다!
	//찾은 값들은 마지막에 _vMoveList안에 저장됩니다!
	void pathFinder(aStarTile* currentTile);

	void renderGoalList();

	//벡터들을 전부 지워줍니다!
	//길을 다 찾고 난 뒤에 캐릭터가 도착하면 그때 써줍시다!
	void vectorClear();

	// === inline, gettter, setter === // 

	//시작 타일
	inline void setStartTile(int arrNum)
	{
		_startTile = _vTotalList[arrNum];
		_currentTile = _startTile;
	}
	aStarTile* getStartTile() { return _startTile; }

	//목표 타일
	inline void setGoalTile(int arrNum) { _goalTile = _vTotalList[arrNum]; }
	aStarTile* getGoalTile() { return _goalTile; }

	//시작지점을 기준으로 길찾기를 시작합니다!
	inline void startPathFinder() { pathFinder(_startTile); }

	//플레이어가 이동 가능한 타일(아직 안만듬)
	//	void setMoveTile(tagIso tile); <- 이 함수를 아래와 같이 변경함.(민수)
	void getMovablePath(tagIso tile);

	//캐릭터에게 이동해야될 길을 넘겨준다
	//이 함수는 캐릭터한테 갈 수 있는 길을 넘겨주는게 아니라,
	//캐릭터가 도착지점까지 어떻게 도착해야하는지 넘겨주는 것이다.
	inline vector<tagIso> &getMoveTile() { return _vMoveList; }
};