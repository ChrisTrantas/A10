#ifndef __MYBOUNDINGOBJECTCLASS_H_
#define __MYBOUNDINGOBJECTCLASS_H_

#include "RE\ReEng.h"

#include "MyBoundingBoxClass.h"
#include "MyBoundingSphereClass.h"

#include <memory> // for std::shared_ptr

class MyBoundingObjectClass
{
    std::shared_ptr<MyBoundingBoxClass> m_pBoundingBox;
    std::shared_ptr<MyBoundingBoxClass> m_pReorientedBoundingBox;
    std::shared_ptr<MyBoundingSphereClass> m_pBoundingSphere;

    bool m_bVisibility;	// Holds the visibility of the BO
    vector3 m_v3Color = REBLACK;	// Holds the color of the bounding object

    matrix4 m_m4ToWorld = IDENTITY_M4; // Matrix that takes us from local to world coordinates
    vector3 m_v3Center = vector3(0.0f); // Stores the center point of the Bounding Object
    vector3 m_v3Min = vector3(0.0f); // Stores the minimum vector of the bounding object Class
    vector3 m_v3Max = vector3(0.0f); // Stores the maximum vector of the bounding object Class

public:
    // Creates Bounding Object from list of vertices
    MyBoundingObjectClass(const std::vector<vector3>& vertices);

    MyBoundingObjectClass(MyBoundingObjectClass const& other);

    MyBoundingObjectClass& operator=(MyBoundingObjectClass const& other);

    ~MyBoundingObjectClass();


    void SetVisibility(bool a_bVisibility);

    bool GetVisibility() const;


    vector3 GetCenterLocal() const;

    vector3 GetCenterGlobal() const;


    vector3 GetMinimum() const;

    vector3 GetMaximum() const;


    void SetModelMatrix(matrix4 a_m4ToWorld);
    matrix4 GetModelMatrix() const;

    // Sets the color of the Bounding Object
    void SetColor(vector3 a_v3Color);

    /// <summary>
    /// Draws this bounding object.
    /// </summary>
    void Draw();

    bool IsColliding(MyBoundingObjectClass* const a_pOther) const;
};

#endif //__MYBOUNDINGOBJECTCLASS_H_