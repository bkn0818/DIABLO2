#pragma once

template <typename T>
class singletonBase
{
protected:
	//싱글톤 인스턴스 선언
	static T* singleton;

	singletonBase(void) {};
	~singletonBase(void){};

public:
	//싱글톤 접근자
	static T* getSingleton(void);
	//싱글톤 해제 함수
	void releaseSingleton(void);
};

//싱글톤 초기화
template <typename T>
T* singletonBase<T>::singleton = 0;

//싱글톤 불러오는 함수
template <typename T>
T* singletonBase<T>::getSingleton(void)
{
	//만약에 싱글톤이 없다면 -> 싱글톤 생성
	if (!singleton) singleton = new T;

	//그리고 반환
	return singleton;
}

//싱글톤 메모리 해제
template <typename T>
void singletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;

		singleton = 0;
	}
}