#include "MyBoundingObjectClass.h"


MyBoundingObjectClass::MyBoundingObjectClass(const std::vector<vector3>& a_lVectorList)
{
	m_pBoundingBox = new MyBoundingBoxClass(a_lVectorList);

	m_pReorientedBoundingBox = new MyBoundingBoxClass(a_lVectorList);

	m_pBoundingSphere = new MyBoundingSphereClass(a_lVectorList);
}

MyBoundingObjectClass::MyBoundingObjectClass(MyBoundingObjectClass const& other)
{
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_bVisibility = other.m_bVisibility;
	m_v3Color = other.m_v3Color;

	m_pBoundingBox = new MyBoundingBoxClass(*other.m_pBoundingBox);
	m_pReorientedBoundingBox = new MyBoundingBoxClass(*other.m_pReorientedBoundingBox);
	m_pBoundingSphere = new MyBoundingSphereClass(*other.m_pBoundingSphere);
}

MyBoundingObjectClass& MyBoundingObjectClass::operator=(MyBoundingObjectClass const& other)
{
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_bVisibility = other.m_bVisibility;
	m_v3Color = other.m_v3Color;

	*m_pBoundingBox = *other.m_pBoundingBox;
	*m_pReorientedBoundingBox = *other.m_pReorientedBoundingBox;
	*m_pBoundingSphere = *other.m_pBoundingSphere;

	return *this;
}

MyBoundingObjectClass::~MyBoundingObjectClass()
{
	delete m_pBoundingBox;
	delete m_pReorientedBoundingBox;
	delete m_pBoundingSphere;
}

bool MyBoundingObjectClass::IsColliding(MyBoundingObjectClass* const a_pOther) const
{
	// Checks if the bounding sphere is colliding
	if (!m_pBoundingSphere->IsColliding(a_pOther->m_pBoundingSphere))
	{
		return false;
	}

	// Checks if oriented bounding box is colliding
	return (m_pReorientedBoundingBox->IsColliding(a_pOther->m_pReorientedBoundingBox));

	// In the future, we would add Oriented Bounding Box collision
}

void MyBoundingObjectClass::Draw(MeshManagerSingleton* a_pMeshMngr)
{
	a_pMeshMngr->AddCubeToQueue(glm::translate(m_pBoundingBox->GetCenterGlobal()) * glm::extractMatrixRotation(m_m4ToWorld) * glm::scale(m_pBoundingBox->GetHalfWidth() * 2.0f), m_v3Color, WIRE);
	a_pMeshMngr->AddCubeToQueue(glm::translate(m_pReorientedBoundingBox->GetCenterGlobal()) * glm::scale(m_pReorientedBoundingBox->GetHalfWidth() * 2.0f), m_v3Color, WIRE);
	a_pMeshMngr->AddSphereToQueue(glm::translate(m_pBoundingSphere->GetCenterGlobal()) *   glm::scale(vector3(m_pBoundingSphere->GetRadius()* 2.0f)), m_v3Color, WIRE);
}

#pragma region Accessors
void MyBoundingObjectClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	m_m4ToWorld = a_m4ToWorld;
	
	m_pBoundingBox->SetModelMatrix(a_m4ToWorld);
	*m_pReorientedBoundingBox = m_pBoundingBox->GetReorientedBoundingBox();
	m_pBoundingSphere->SetModelMatrix(a_m4ToWorld);
}

matrix4 MyBoundingObjectClass::GetModelMatrix() const
{
	return m_m4ToWorld;
}

vector3 MyBoundingObjectClass::GetCenterLocal() const
{
	return m_v3Center;
}

vector3 MyBoundingObjectClass::GetCenterGlobal() const
{
	return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f));
}

vector3 MyBoundingObjectClass::GetMinimum() const
{
	return m_pReorientedBoundingBox->GetMinimum();
}

vector3 MyBoundingObjectClass::GetMaximum() const
{
	return m_pReorientedBoundingBox->GetMaximum();
}

void MyBoundingObjectClass::SetVisibility(boolean a_bVisibility)
{
	m_bVisibility = a_bVisibility;
}

boolean MyBoundingObjectClass::GetVisibility(void)
{
	return m_bVisibility;
}
#pragma endregion