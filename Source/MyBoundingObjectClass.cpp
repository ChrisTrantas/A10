#include "MyBoundingObjectClass.h"


MyBoundingObjectClass::MyBoundingObjectClass(const std::vector<vector3>& a_lVectorList)
{
    m_pBoundingBox = new MyBoundingBoxClass(a_lVectorList);
    m_pReorientedBoundingBox = new MyBoundingBoxClass( a_lVectorList );
    m_pBoundingSphere = new MyBoundingSphereClass( a_lVectorList );
}

MyBoundingObjectClass::MyBoundingObjectClass(MyBoundingObjectClass const& other)
{
    m_m4ToWorld = other.m_m4ToWorld;

    m_bVisibility = other.m_bVisibility;
    m_v3Color = other.m_v3Color;

    *m_pBoundingBox = *other.m_pBoundingBox;
    *m_pReorientedBoundingBox = *other.m_pReorientedBoundingBox;
    *m_pBoundingSphere = *other.m_pBoundingSphere;
}

MyBoundingObjectClass& MyBoundingObjectClass::operator=(MyBoundingObjectClass const& other)
{
    m_m4ToWorld = other.m_m4ToWorld;

    m_bVisibility = other.m_bVisibility;
    m_v3Color = other.m_v3Color;

    *m_pBoundingBox = *other.m_pBoundingBox;
    *m_pReorientedBoundingBox = *other.m_pReorientedBoundingBox;
    *m_pBoundingSphere = *other.m_pBoundingSphere;

    return *this;
}

MyBoundingObjectClass::~MyBoundingObjectClass()
{
}

bool MyBoundingObjectClass::IsColliding(MyBoundingObjectClass* const a_pOther) const
{
    // Checks if the bounding sphere is colliding
    if (!m_pBoundingSphere->IsColliding(a_pOther->m_pBoundingSphere))
    {
        return false;
    }

    // Checks if oriented bounding box is colliding
    return m_pReorientedBoundingBox->IsColliding(a_pOther->m_pReorientedBoundingBox);
}

void MyBoundingObjectClass::Draw()
{
	if (m_bVisibility)
	{
		MeshManagerSingleton* meshManager = MeshManagerSingleton::GetInstance();

		// Add the oriented bounding box to the render queue
		meshManager->AddCubeToQueue(glm::translate(m_pBoundingBox->GetCenterGlobal())
			* glm::extractMatrixRotation(m_m4ToWorld)
			* glm::scale(m_pBoundingBox->GetHalfWidth() * 2.0f),
			m_v3Color, WIRE);

		// Add the re-oriented bounding box to the render queue
		meshManager->AddCubeToQueue(glm::translate(m_pReorientedBoundingBox->GetCenterGlobal())
			* glm::scale(m_pReorientedBoundingBox->GetHalfWidth() * 2.0f),
			m_v3Color, WIRE);

		// Add the bounding sphere to the render queue
		meshManager->AddSphereToQueue(glm::translate(m_pBoundingSphere->GetCenterGlobal())
			* glm::scale(vector3(m_pBoundingSphere->GetRadius()* 2.0f)),
			m_v3Color, WIRE);
	}
}

#pragma region Accessors

void MyBoundingObjectClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
    m_m4ToWorld = a_m4ToWorld;
    
    m_pBoundingBox->SetModelMatrix(a_m4ToWorld);
    *m_pReorientedBoundingBox = m_pBoundingBox->GetReorientedBoundingBox();

    m_pBoundingSphere->UpdateFromBoundingBox( m_pReorientedBoundingBox );

    matrix4 sphereWorld = glm::translate( vector3( a_m4ToWorld[ 3 ] ) );
    m_pBoundingSphere->SetModelMatrix( sphereWorld );
}

matrix4 MyBoundingObjectClass::GetModelMatrix() const
{
    return m_m4ToWorld;
}

void MyBoundingObjectClass::SetColor(vector3 a_v3Color)
{
    m_v3Color = a_v3Color;
}

vector3 MyBoundingObjectClass::GetColor()
{
    return m_v3Color;
}

vector3 MyBoundingObjectClass::GetCenterLocal() const
{
    return m_pBoundingSphere->GetCenterLocal();
}

vector3 MyBoundingObjectClass::GetCenterGlobal() const
{
    return m_pBoundingSphere->GetCenterGlobal();
}

vector3 MyBoundingObjectClass::GetMinimum() const
{
    return m_pReorientedBoundingBox->GetMinimum();
}

vector3 MyBoundingObjectClass::GetMaximum() const
{
    return m_pReorientedBoundingBox->GetMaximum();
}

void MyBoundingObjectClass::SetVisibility(bool a_bVisibility)
{
    m_bVisibility = a_bVisibility;
}

bool MyBoundingObjectClass::GetVisibility() const
{
    return m_bVisibility;
}

#pragma endregion
