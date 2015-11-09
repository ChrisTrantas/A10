#include "MyBoundingObjectClass.h"

#define EPSILON 0.0001f

static inline vector3 TransformVector( const matrix4& mat, const vector3& vec )
{
    return vector3( mat * vector4( vec, 1.0f ) );
}

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

// Check if two OBBs are colliding via the separation axis test
bool MyBoundingObjectClass::AreOBBsColliding( MyBoundingObjectClass* const other ) const
{
    /**
     *  struct OBB {
     *      Point c; // OBB center point
     *      Vector u[3]; // Local x-, y-, and z-axes
     *      Vector e; // Positive halfwidth extents of OBB along each axis
     *  };
     */

    // Get the two OBBs
    MyBoundingBoxClass* const thisObb = this->m_pBoundingBox;
    MyBoundingBoxClass* const thatObb = other->m_pBoundingBox;

    // Get the two local coordinate systems
    CoordinateSystem thisAxes = GetLocalCoordinateSystem();
    CoordinateSystem thatAxes = other->GetLocalCoordinateSystem();

    // Create the rotation and absolute rotation matrices
    matrix4 rotation( 1.0f );
    matrix4 absRotation( 1.0f );

    // Compute the rotation and absolute rotation matrices
    for ( int i = 0; i < 3; ++i )
    {
        for ( int j = 0; j < 3; ++j )
        {
            rotation[ i ][ j ] = glm::dot( thisAxes[ i ], thatAxes[ j ] );
            absRotation[ i ][ j ] = abs( rotation[ i ][ j ] ) + EPSILON;
        }
    }

     // Compute the translation vector
    vector3 translation = thatObb->GetCenterGlobal() - thisObb->GetCenterGlobal();
    translation.x = glm::dot( translation, thisAxes[ 0 ] );
    translation.y = glm::dot( translation, thisAxes[ 1 ] );
    translation.z = glm::dot( translation, thisAxes[ 2 ] );
}

// Get the local coordinate system
CoordinateSystem MyBoundingObjectClass::GetLocalCoordinateSystem() const
{
    CoordinateSystem local;
    local.XAxis = TransformVector( m_m4ToWorld, vector3( 1, 0,  0 ) );
    local.YAxis = TransformVector( m_m4ToWorld, vector3( 0, 1,  0 ) );
    local.ZAxis = TransformVector( m_m4ToWorld, vector3( 0, 0, -1 ) ); // OpenGL is a RHS
    return local;
}

// Check if we're colliding with the given bounding object
bool MyBoundingObjectClass::IsColliding( MyBoundingObjectClass* const a_pOther ) const
{
    // Checks if the bounding sphere is colliding
    if ( !m_pBoundingSphere->IsColliding( a_pOther->m_pBoundingSphere ) )
    {
        return false;
    }

    // Checks if oriented bounding box is colliding
    if ( !m_pReorientedBoundingBox->IsColliding( a_pOther->m_pReorientedBoundingBox ) )
    {
        return false;
    }

    return AreOBBsColliding( a_pOther );
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
