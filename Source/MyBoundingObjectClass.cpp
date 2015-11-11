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
     *  // From Real Time Collision Detection
     *  struct OBB {
     *      Point c;     // OBB center point
     *      Vector u[3]; // Local x-, y-, and z-axes
     *      Vector e;    // Positive halfwidth extents of OBB along each axis
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

    float thisProjRadius; // Length of the projection of this OBB
    float thatProjRadius; // Length of the projection of that OBB

    // Test local axes of this OBB
    for (int i = 0; i < 3; i++)	// Iterates through each dimension of this OBB
    {
        // Sets the projection length of this OBB to the half the length of the box in this OBBs ith dimension
        thisProjRadius = thisObb->GetHalfWidth()[i];
        
        // Sets the projection length of that OBB to be its half projection onto this OBBs ith dimension
        thatProjRadius =
            thatObb->GetHalfWidth()[0] * absRotation[i][0]
            + thatObb->GetHalfWidth()[1] * absRotation[i][1]
            + thatObb->GetHalfWidth()[2] * absRotation[i][2];

        // Sees if the distance between the the OBB exceed the the combine lengths of the projected Radii
        if (abs(translation[i]) > thisProjRadius + thatProjRadius)
            return false;
    }

    // Test local axes of that OBB
    for (int i = 0; i < 3; i++)	// Iterates through each dimension of this OBB
    {
        // Sets the projection length of this OBB to the half the length of the box in that OBBs ith dimension
        thisProjRadius =
            thisObb->GetHalfWidth()[0] * absRotation[i][0]
            + thisObb->GetHalfWidth()[1] * absRotation[i][1]
            + thisObb->GetHalfWidth()[2] * absRotation[i][2];

        // Sets the projection length of that OBB to be its half projection onto that OBBs ith dimension
        thatProjRadius = thatObb->GetHalfWidth()[i];

        // Sees if the distance between the the OBB exceed the the combine lengths of the projected Radii
        if (abs(translation[i]) > thisProjRadius + thatProjRadius)
            return false;
    }

	// Test axis L = A0 x B0
	thisProjRadius = thisObb->GetHalfWidth()[1] * absRotation[2][0] + thisObb->GetHalfWidth()[2] * absRotation[1][0];
	thatProjRadius = thatObb->GetHalfWidth()[1] * absRotation[0][2] + thatObb->GetHalfWidth()[2] * absRotation[0][1];
	if (abs(translation[2] * rotation[1][0] - translation[1] * rotation[2][0]) > thisProjRadius + thatProjRadius) return 0;

	// Test axis L = A0 x B1
	thisProjRadius = thisObb->GetHalfWidth()[1] * absRotation[2][1] + thisObb->GetHalfWidth()[2] * absRotation[1][1];
	thatProjRadius = thatObb->GetHalfWidth()[0] * absRotation[0][2] + thatObb->GetHalfWidth()[2] * absRotation[0][0];
	if (abs(translation[2] * rotation[1][1] - translation[1] * rotation[2][1]) > thisProjRadius + thatProjRadius) return 0;

	// Test axis L = A0 x B2
	thisProjRadius = thisObb->GetHalfWidth()[1] * absRotation[2][2] + thisObb->GetHalfWidth()[2] * absRotation[1][2];
	thatProjRadius = thatObb->GetHalfWidth()[0] * absRotation[0][1] + thatObb->GetHalfWidth()[1] * absRotation[0][0];
	if (abs(translation[2] * rotation[1][2] - translation[1] * rotation[2][2]) > thisProjRadius + thatProjRadius) return 0;

	// Test axis L = A1 x B0
	thisProjRadius = thisObb->GetHalfWidth()[0] * absRotation[2][0] + thisObb->GetHalfWidth()[2] * absRotation[0][0];
	thatProjRadius = thatObb->GetHalfWidth()[1] * absRotation[1][2] + thatObb->GetHalfWidth()[2] * absRotation[1][1];
	if (abs(translation[0] * rotation[2][0] - translation[2] * rotation[0][0]) > thisProjRadius + thatProjRadius) return 0;

	// Test axis L = A1 x B1
	thisProjRadius = thisObb->GetHalfWidth()[0] * absRotation[2][1] + thisObb->GetHalfWidth()[2] * absRotation[0][1];
	thatProjRadius = thatObb->GetHalfWidth()[0] * absRotation[1][2] + thatObb->GetHalfWidth()[2] * absRotation[1][0];
	if (abs(translation[0] * rotation[2][1] - translation[2] * rotation[0][1]) > thisProjRadius + thatProjRadius) return 0;

	// Test axis L = A1 x B2
	thisProjRadius = thisObb->GetHalfWidth()[0] * absRotation[2][2] + thisObb->GetHalfWidth()[2] * absRotation[0][2];
	thatProjRadius = thatObb->GetHalfWidth()[0] * absRotation[1][1] + thatObb->GetHalfWidth()[1] * absRotation[1][0];
	if (abs(translation[0] * rotation[2][2] - translation[2] * rotation[0][2]) > thisProjRadius + thatProjRadius) return 0;
	
	// Test axis L = A2 x B0
	thisProjRadius = thisObb->GetHalfWidth()[0] * absRotation[1][0] + thisObb->GetHalfWidth()[1] * absRotation[0][0];
	thatProjRadius = thatObb->GetHalfWidth()[1] * absRotation[2][2] + thatObb->GetHalfWidth()[2] * absRotation[2][1];
	if (abs(translation[1] * rotation[0][0] - translation[0] * rotation[1][0]) > thisProjRadius + thatProjRadius) return 0;
	
	// Test axis L = A2 x B1
	thisProjRadius = thisObb->GetHalfWidth()[0] * absRotation[1][1] + thisObb->GetHalfWidth()[1] * absRotation[0][1];
	thatProjRadius = thatObb->GetHalfWidth()[0] * absRotation[2][2] + thatObb->GetHalfWidth()[2] * absRotation[2][0];
	if (abs(translation[1] * rotation[0][1] - translation[0] * rotation[1][1]) > thisProjRadius + thatProjRadius) return 0;
	
	// Test axis L = A2 x B2
	thisProjRadius = thisObb->GetHalfWidth()[0] * absRotation[1][2] + thisObb->GetHalfWidth()[1] * absRotation[0][2];
	thatProjRadius = thatObb->GetHalfWidth()[0] * absRotation[2][1] + thatObb->GetHalfWidth()[1] * absRotation[2][0];
	if (abs(translation[1] * rotation[0][2] - translation[0] * rotation[1][2]) > thisProjRadius + thatProjRadius) return 0;


    // Returns true since there is no case where the 
    return true;
}

// Get the local coordinate system
CoordinateSystem MyBoundingObjectClass::GetLocalCoordinateSystem() const
{
    // Remove the translation from the world matrix
    matrix4 rotation = m_m4ToWorld;
    rotation[ 3 ] = vector4( 0, 0, 0, 1 );

    // Get the local coordinate axes (we need to normalize in case the matrix scales)
    CoordinateSystem local;
    local.XAxis = glm::normalize( TransformVector( rotation, vector3( 1, 0,  0 ) ) );
    local.YAxis = glm::normalize( TransformVector( rotation, vector3( 0, 1,  0 ) ) );
    local.ZAxis = glm::normalize( TransformVector( rotation, vector3( 0, 0, -1 ) ) ); // OpenGL is a RHS
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
