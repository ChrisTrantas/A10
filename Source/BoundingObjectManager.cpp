#include "BoundingObjectManager.h"

//  BoundingObjectManager Singleton
BoundingObjectManager* BoundingObjectManager::m_pInstance = nullptr;

// Initilze variable to count size of list
void BoundingObjectManager::Init(void)
{
	m_nBoxCount = m_lBox.size();
}

// Gets instance of BoundingObjectManager
BoundingObjectManager* BoundingObjectManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new BoundingObjectManager();
	}
	return m_pInstance;
}

// Releases Instance of BoundingObjectManager
void BoundingObjectManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

// Make a new bounding object based on model then adds it to the list
int BoundingObjectManager::AddBox(const std::vector<vector3>& vertices)
{
	// A.) Add a box based on model
	m_lBox.push_back(new MyBoundingObjectClass(vertices));

	// C.) Get the number of BO in the manager
	m_nBoxCount = m_lBox.size();

	return m_lBox.size() - 1;
}

// C.) Get the number of BO in the manager
int BoundingObjectManager::GetBOCount()
{
	return m_lBox.size();
}

// Sets the visibility
void BoundingObjectManager::SetVisibility(int a_iIndex, bool a_bVisibility)
{
	// E). Set the visibilty of a specific BO
	m_lBox[a_iIndex]->SetVisibility(a_bVisibility);

}

// Gets the visibility of a specific BO
bool BoundingObjectManager::GetVisibility(int a_iIndex) const
{
	return m_lBox[a_iIndex]->GetVisibility();
}

// Sets the model matrix of a specific BO
void BoundingObjectManager::SetModelMatrix(int a_iIndex, matrix4 a_m4ToWorld)
{
	m_lBox[a_iIndex]->SetModelMatrix(a_m4ToWorld);
}

// Gets the model matrix of a specific BO
matrix4 BoundingObjectManager::GetModelMatrix() const
{
	return m_m4ToWorld;
}

// Sets the color of a specific BO
void BoundingObjectManager::SetColor(int a_iIndex, vector3 a_v3Color)
{
	// D.) Set the color of a specific BO
	m_lBox[a_iIndex]->SetColor(a_v3Color);
}


// Checks collisions of all BOs
void BoundingObjectManager::CheckCollisions() const
{
	// Set color to black if its not colliding
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->SetColor(REBLACK);
	}

	// Set color to red if colliding
	for (int i = 0; i < m_nBoxCount; i++)
	{
		for (int j = i + 1; j < m_nBoxCount; j++)
		{
			if (m_lBox[i]->IsColliding(m_lBox[j]))
			{
				m_lBox[i]->SetColor(RERED);
				m_lBox[j]->SetColor(RERED);
			}				
		}
	}
}

// Draw all BOs
void BoundingObjectManager::Draw()
{
	for (int i = 0; i < m_nBoxCount; i++)
	{
		m_lBox[i]->Draw();
	}
}

// Render specific BO
void BoundingObjectManager::Draw(int a_iIndex)
{
	m_lBox[a_iIndex]->Draw();
}