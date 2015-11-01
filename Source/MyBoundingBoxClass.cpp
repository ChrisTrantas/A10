#include "MyBoundingBoxClass.h"


static vector3 TransformVector( const matrix4& matrix, const vector3& vec )
{
    return vector3( matrix * vector4( vec, 1.0f ) );
}


//  MyBoundingBoxClass
void MyBoundingBoxClass::Init( void )
{
    m_m4ToWorld = IDENTITY_M4;

    m_v3Center = vector3( 0.0f );
    m_v3Min = vector3( 0.0f );
    m_v3Max = vector3( 0.0f );

    m_v3HalfWidth = vector3( 0.0f );
}

void MyBoundingBoxClass::Swap( MyBoundingBoxClass& other )
{
    std::swap( m_m4ToWorld, other.m_m4ToWorld );

    std::swap( m_v3Center, other.m_v3Center );
    std::swap( m_v3Min, other.m_v3Min );
    std::swap( m_v3Max, other.m_v3Max );

    std::swap( m_v3HalfWidth, other.m_v3HalfWidth );
}

void MyBoundingBoxClass::Release( void )
{

}

//The big 3
MyBoundingBoxClass::MyBoundingBoxClass( const std::vector<vector3>& a_lVectorList )
{
    //Init the default values
    Init();

    //Count the points of the incoming list
    uint nVertexCount = a_lVectorList.size();

    //If there are none just return, we have no information to create the BS from
    if ( nVertexCount == 0 )
        return;

    //Max and min as the first vector of the list
    m_v3Max = m_v3Min = a_lVectorList[ 0 ];

    //Get the max and min out of the list
    for ( uint nVertex = 1; nVertex < nVertexCount; nVertex++ )
    {
        if ( m_v3Min.x > a_lVectorList[ nVertex ].x ) //If min is larger than current
            m_v3Min.x = a_lVectorList[ nVertex ].x;
        else if ( m_v3Max.x < a_lVectorList[ nVertex ].x )//if max is smaller than current
            m_v3Max.x = a_lVectorList[ nVertex ].x;

        if ( m_v3Min.y > a_lVectorList[ nVertex ].y ) //If min is larger than current
            m_v3Min.y = a_lVectorList[ nVertex ].y;
        else if ( m_v3Max.y < a_lVectorList[ nVertex ].y )//if max is smaller than current
            m_v3Max.y = a_lVectorList[ nVertex ].y;

        if ( m_v3Min.z > a_lVectorList[ nVertex ].z ) //If min is larger than current
            m_v3Min.z = a_lVectorList[ nVertex ].z;
        else if ( m_v3Max.z < a_lVectorList[ nVertex ].z )//if max is smaller than current
            m_v3Max.z = a_lVectorList[ nVertex ].z;
    }

    //with the max and the min we calculate the center
    m_v3Center = ( m_v3Min + m_v3Max ) / 2.0f;

    //we calculate the distance between all the values of min and max vectors
    m_v3HalfWidth.x = glm::distance( vector3( m_v3Min.x, 0.0f, 0.0f ), vector3( m_v3Max.x, 0.0f, 0.0f ) ) / 2.0f;
    m_v3HalfWidth.y = glm::distance( vector3( 0.0f, m_v3Min.y, 0.0f ), vector3( 0.0f, m_v3Max.y, 0.0f ) ) / 2.0f;
    m_v3HalfWidth.z = glm::distance( vector3( 0.0f, 0.0f, m_v3Min.z ), vector3( 0.0f, 0.0f, m_v3Max.z ) ) / 2.0f;

}

MyBoundingBoxClass::MyBoundingBoxClass( MyBoundingBoxClass const& other )
{
    m_m4ToWorld = other.m_m4ToWorld;

    m_v3Center = other.m_v3Center;
    m_v3Min = other.m_v3Min;
    m_v3Max = other.m_v3Max;

    m_v3HalfWidth = other.m_v3HalfWidth;
}

MyBoundingBoxClass& MyBoundingBoxClass::operator=( MyBoundingBoxClass const& other )
{
    if ( this != &other )
    {
        Release();
        Init();
        MyBoundingBoxClass temp( other );
        Swap( temp );
    }
    return *this;
}

MyBoundingBoxClass::~MyBoundingBoxClass() { Release(); };


// Gets the re-oriented bounding box
MyBoundingBoxClass MyBoundingBoxClass::GetReorientedBoundingBox() const
{
    MyBoundingBoxClass other( *this );



    // Get the non-translated world matrix
    matrix4 nonTranslation = m_m4ToWorld;
    nonTranslation[ 3 ] = vector4( 0, 0, 0, 1 );

    // Get all of the points for the transformed bounding box
    std::vector<vector3> points( 8 );
    points[ 0 ] = TransformVector( nonTranslation, vector3( m_v3Min.x, m_v3Min.y, m_v3Min.z ) );
    points[ 1 ] = TransformVector( nonTranslation, vector3( m_v3Min.x, m_v3Min.y, m_v3Max.z ) );
    points[ 2 ] = TransformVector( nonTranslation, vector3( m_v3Min.x, m_v3Max.y, m_v3Min.z ) );
    points[ 3 ] = TransformVector( nonTranslation, vector3( m_v3Min.x, m_v3Max.y, m_v3Max.z ) );
    points[ 4 ] = TransformVector( nonTranslation, vector3( m_v3Max.x, m_v3Min.y, m_v3Min.z ) );
    points[ 5 ] = TransformVector( nonTranslation, vector3( m_v3Max.x, m_v3Min.y, m_v3Max.z ) );
    points[ 6 ] = TransformVector( nonTranslation, vector3( m_v3Max.x, m_v3Max.y, m_v3Min.z ) );
    points[ 7 ] = TransformVector( nonTranslation, vector3( m_v3Max.x, m_v3Max.y, m_v3Max.z ) );

    // Calculate the new min and max from those transformed points
    vector3 newMin = points[0];
    vector3 newMax = points[0];
    for ( auto iter = points.begin(); iter != points.end(); ++iter )
    {
        newMin = glm::min( newMin, *iter );
        newMax = glm::max( newMax, *iter );
    }




    // Set the min and max
    other.m_v3Min = newMin;
    other.m_v3Max = newMax;

    // Recalculate the center and half width
    other.m_v3Center = ( other.m_v3Min + other.m_v3Max ) * 0.5f;
    other.m_v3HalfWidth = other.m_v3Max - other.m_v3Center;

    // Reset the matrix
    other.m_m4ToWorld = glm::translate( vector3( m_m4ToWorld[ 3 ] ) );

    return other;
}


//Accessors
void MyBoundingBoxClass::SetModelMatrix( matrix4 a_m4ToWorld ) { m_m4ToWorld = a_m4ToWorld; }

matrix4 MyBoundingBoxClass::GetModelMatrix( void ) const { return m_m4ToWorld; }

vector3 MyBoundingBoxClass::GetCenterLocal( void ) const { return m_v3Center; }

vector3 MyBoundingBoxClass::GetCenterGlobal( void ) const { return vector3( m_m4ToWorld * vector4( m_v3Center, 1.0f ) ); }

vector3 MyBoundingBoxClass::GetHalfWidth( void ) const { return m_v3HalfWidth; }

vector3 MyBoundingBoxClass::GetMaximum(void) const { return m_v3Max; }

vector3 MyBoundingBoxClass::GetMinimum(void) const { return m_v3Min; }

//--- Non Standard Singleton Methods
bool MyBoundingBoxClass::IsColliding( MyBoundingBoxClass* const a_pOther ) const
{
    //Get all vectors in global space
    vector3 v3Min = TransformVector( m_m4ToWorld, m_v3Min );
    vector3 v3Max = TransformVector( m_m4ToWorld, m_v3Max );

    vector3 v3MinO = TransformVector( a_pOther->m_m4ToWorld, a_pOther->m_v3Min );
    vector3 v3MaxO = TransformVector( a_pOther->m_m4ToWorld, a_pOther->m_v3Max );

    /*
    Are they colliding?
    For boxes we will assume they are colliding, unless at least one of the following conditions is not met
    */
    bool bColliding = true;

    //Check for X
    if ( v3Max.x < v3MinO.x )
        bColliding = false;
    if ( v3Min.x > v3MaxO.x )
        bColliding = false;

    //Check for Y
    if ( v3Max.y < v3MinO.y )
        bColliding = false;
    if ( v3Min.y > v3MaxO.y )
        bColliding = false;

    //Check for Z
    if ( v3Max.z < v3MinO.z )
        bColliding = false;
    if ( v3Min.z > v3MaxO.z )
        bColliding = false;

    return bColliding;
}
