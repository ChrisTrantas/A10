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

// Makex a new bounding object based on model then adds it to the list
void BoundingObjectManager::AddBox(const std::vector<vector3>& vertices)
{
	// A
	MyBoundingObjectClass* pBoundingBox = new MyBoundingObjectClass(vertices);
	m_lBox.push_back(pBoundingBox);
	// C
	m_nBoxCount = m_lBox.size();
}

// Sets the visibility
void BoundingObjectManager::SetVisibility(bool a_bVisibility)
{
	// E
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->SetVisibility(a_bVisibility);
	}
}

// Gets the visibility of the bounding box
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

matrix4 BoundingObjectManager::GetModelMatrix() const
{

	return m_m4ToWorld;
	
}

// Sets the color of the bounding box
void BoundingObjectManager::SetColor(vector3 a_v3Color)
{
	// D
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->SetColor(a_v3Color);
	}
}


// Checks collisions of all BOs
bool BoundingObjectManager::IsColliding(MyBoundingObjectClass* const a_pOther) const
{
	// G
	for (int i = 0; i < m_nBoxCount; i++)
	{
		// H
		if (m_lBox[i]->IsColliding(a_pOther))
		{
			m_lBox[i]->SetColor(RERED);
			//m_lBox[a_pOther]->SetColor(RERED);
			return true;
		}
		return false;
	}

	
}