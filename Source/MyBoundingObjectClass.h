#ifndef __MYBOUNDINGOBJECTCLASS_H__
#define __MYBOUNDINGOBJECTCLASS_H__

#include "RE\ReEng.h"

#include "MyBoundingBoxClass.h"
#include "MyBoundingSphereClass.h"

struct CoordinateSystem
{
    vector3 XAxis;
    vector3 YAxis;
    vector3 ZAxis;

    inline vector3 operator[]( size_t index ) const
    {
        vector3 result;
        switch ( index )
        {
            case 0: result = XAxis; break;
            case 1: result = YAxis; break;
            case 2: result = ZAxis; break;
        }
        return result;
    }
};

class MyBoundingObjectClass
{
    MyBoundingBoxClass* m_pBoundingBox;
    MyBoundingBoxClass* m_pReorientedBoundingBox;
    MyBoundingSphereClass* m_pBoundingSphere;

    bool m_bVisibility;	// Holds the visibility of the BO
    vector3 m_v3Color = REBLACK;	// Holds the color of the bounding object

    matrix4 m_m4ToWorld = IDENTITY_M4;	// Matrix that takes us from local to world coordinates

    // Checks to see if our OBB is colliding with another bounding object's OBB
    bool AreOBBsColliding( MyBoundingObjectClass* const other ) const;

public:
    // Creates Bounding Object from list of vertices
    MyBoundingObjectClass(const std::vector<vector3>& vertices);

    MyBoundingObjectClass(MyBoundingObjectClass const& other);	// Copy Constructor
    MyBoundingObjectClass& operator=(MyBoundingObjectClass const& other);	// Copy Assignment Operator
    ~MyBoundingObjectClass();	// Destructor


    void Draw();	// Renderes reference shapes to show thhe bounding object
    bool IsColliding(MyBoundingObjectClass* const a_pOther) const;	// Checks if this is colliding with another bounding object.

    vector3 GetCenterLocal() const;		// Gets the center of the bounding object in local space
    vector3 GetCenterGlobal() const;	// Gets the center of the bounding object in global space

    vector3 GetMinimum() const;	// Gets the minimum vector 3 of the bounding object
    vector3 GetMaximum() const;	// Gets the maximum vector 3 of the bounding object

    // Gets this bounding object's local coordinate system
    CoordinateSystem GetLocalCoordinateSystem() const;

#pragma region Accessors
    void SetModelMatrix(matrix4 a_m4ToWorld);	// Sets the to world matrix of the bounding object
    matrix4 GetModelMatrix() const;	// Sets the to world matrix of the bounding object

    void SetVisibility(bool a_bVisibility);	// Sets the visibility of the bounding object
    bool GetVisibility() const;	//Gets the visibility of the bounding object

    void SetColor(vector3 a_v3Color); // Sets the color of the Bounding Object when drawn
    vector3 GetColor(); // Gets the current color 
#pragma endregion
};

#endif //__MYBOUNDINGOBJECTCLASS_H__