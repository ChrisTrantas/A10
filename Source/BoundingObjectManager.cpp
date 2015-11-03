#include "BoundingObjectManager.h"
//  MyCameraSingleton
BoundingObjectManager* BoundingObjectManager::m_pInstance = nullptr;

void BoundingObjectManager::Init(void)
{

}

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

void BoundingObjectManager::SetVisibility(bool a_bVisibility)
{

}

bool BoundingObjectManager::GetVisibility() const
{

}


vector3 BoundingObjectManager::GetCenterLocal() const
{

}

vector3 BoundingObjectManager::GetCenterGlobal() const
{

}

vector3 BoundingObjectManager::GetMinimum() const
{

}

vector3 BoundingObjectManager::GetMaximum() const
{

}

void BoundingObjectManager::SetModelMatrix(matrix4 a_m4ToWorld)
{

}

matrix4 BoundingObjectManager::GetModelMatrix() const
{

}
