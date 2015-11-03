#include "BoundingObjectManager.h"
//  MyCameraSingleton
BoundingObjectManager* BoundingObjectManager::m_pInstance = nullptr;

BoundingObjectManager* BoundingObjectManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new BoundingObjectManager();
	}
	return m_pInstance;
}
void BoundingObjectManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

