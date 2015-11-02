#include "MyBoundingSphereClass.h"


MyBoundingSphereClass::MyBoundingSphereClass(const std::vector<vector3>& a_lVectorList )
{
    vector3 v3Max;
    vector3 v3Min;
    if ( a_lVectorList.size() > 0 )
    {
        v3Min = v3Max = a_lVectorList[ 0 ];
    }

    // Finds the min and max coordinate values
    for (uint i = 1; i < a_lVectorList.size(); i++)
    {
        v3Min = glm::min(v3Min, a_lVectorList[i]);
        v3Max = glm::max(v3Max, a_lVectorList[i]);
    }

    // Finds the center as point between the max and min
    m_v3Center = (v3Min + v3Max) / 2.0f;

    // Finds the radius as the distance between the center and the farthest vertex of the model
    m_fRadius = 0.0f;
    if ( a_lVectorList.size() > 0 )
    {
        glm::distance( m_v3Center, a_lVectorList[ 0 ] );
    }
    for (uint i = 1; i < a_lVectorList.size(); i++)
    {
        float fDistance = glm::distance(m_v3Center, a_lVectorList[i]);
        if (m_fRadius < fDistance)
            m_fRadius = fDistance;
    }
}

MyBoundingSphereClass::MyBoundingSphereClass(MyBoundingSphereClass const& other)
{
    m_v3Center = other.m_v3Center;
    m_fRadius = other.m_fRadius;
}

MyBoundingSphereClass& MyBoundingSphereClass::operator=(MyBoundingSphereClass const& other)
{
    m_v3Center = other.m_v3Center;
    m_fRadius = other.m_fRadius;

    return *this;
}

MyBoundingSphereClass::~MyBoundingSphereClass()
{
}

bool MyBoundingSphereClass::IsColliding(MyBoundingSphereClass* a_pOther)
{
    // Checks if the sum of the radii of the spheres is greater then the distances between their centers
    return (m_fRadius + a_pOther->m_fRadius) > glm::distance(this->GetCenterGlobal(), a_pOther->GetCenterGlobal());
}

void MyBoundingSphereClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
    m_m4ToWorld = a_m4ToWorld;
}

void MyBoundingSphereClass::UpdateFromBoundingBox( const MyBoundingBoxClass* box )
{
    m_v3Center = box->GetCenterLocal();
    m_fRadius = glm::distance( box->GetCenterLocal(), box->GetMaximum() );
}

vector3 MyBoundingSphereClass::GetCenterLocal()
{
    return m_v3Center;
}

vector3 MyBoundingSphereClass::GetCenterGlobal()
{
    // Multiplies the local center with the World Matrix to get the global center
    return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f));
}

float MyBoundingSphereClass::GetRadius()
{
    return m_fRadius;
}
