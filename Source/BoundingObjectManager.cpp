#include "BoundingObjectManager.h"
//  MyCameraSingleton
BoundingObjectManager* BoundingObjectManager::m_pInstance = nullptr;

void BoundingObjectManager::Init(void)
{
	m_nBoxCount = m_lBox.size();
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


void BoundingObjectManager::AddBox(const std::vector<vector3>& vertices)
{
	MyBoundingObjectClass* pBoundingBox = new MyBoundingObjectClass(vertices);
	m_lBox.push_back(pBoundingBox);
	m_nBoxCount = m_lBox.size();
}

void BoundingObjectManager::SetVisibility(bool a_bVisibility)
{
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->SetVisibility(a_bVisibility);
	}
}

bool BoundingObjectManager::GetVisibility() const
{
	return m_bVisibility;
}


//vector3 BoundingObjectManager::GetCenterLocal() const
//{
//
//}
//
//vector3 BoundingObjectManager::GetCenterGlobal() const
//{
//
//}
//
//vector3 BoundingObjectManager::GetMinimum() const
//{
//
//}
//
//vector3 BoundingObjectManager::GetMaximum() const
//{
//
//}

void BoundingObjectManager::SetModelMatrix(matrix4 a_m4ToWorld)
{
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->SetModelMatrix(a_m4ToWorld);
	}
}

//matrix4 BoundingObjectManager::GetModelMatrix() const
//{
//
//}

void BoundingObjectManager::SetColor(vector3 a_v3Color)
{
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->SetColor(a_v3Color);
	}
}

bool BoundingObjectManager::IsColliding(MyBoundingObjectClass* const a_pOther) const
{
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->IsColliding(a_pOther);
	}
}