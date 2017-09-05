#pragma once

template <typename T>
class singletonBase
{
protected:
	//�̱��� �ν��Ͻ� ����
	static T* singleton;

	singletonBase(void) {};
	~singletonBase(void){};

public:
	//�̱��� ������
	static T* getSingleton(void);
	//�̱��� ���� �Լ�
	void releaseSingleton(void);
};

//�̱��� �ʱ�ȭ
template <typename T>
T* singletonBase<T>::singleton = 0;

//�̱��� �ҷ����� �Լ�
template <typename T>
T* singletonBase<T>::getSingleton(void)
{
	//���࿡ �̱����� ���ٸ� -> �̱��� ����
	if (!singleton) singleton = new T;

	//�׸��� ��ȯ
	return singleton;
}

//�̱��� �޸� ����
template <typename T>
void singletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;

		singleton = 0;
	}
}