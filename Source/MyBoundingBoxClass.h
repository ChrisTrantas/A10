/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/10
----------------------------------------------*/
#ifndef __MYBOUNDINGBOXCLASS_H_
#define __MYBOUNDINGBOXCLASS_H_

#include "RE\ReEng.h"

//System Class
class MyBoundingBoxClass
{
    matrix4 m_m4ToWorld = IDENTITY_M4; //Matrix that will take us from local to world coordinate
    vector3 m_v3Center = vector3(0.0f); //Will store the center point of the box Class
    vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the box Class
    vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the box Class
    vector3 m_v3HalfWidth = vector3(0.0f);//Will store half the size of all sides

public:
    /*
    Method: MyBoundingBoxClass
    Usage: Constructor
    Arguments: ---
    Output: class object
    */
    MyBoundingBoxClass( const std::vector<vector3>& vertices );

    /*
    Method: MyBoundingBoxClass
    Usage: Copy Constructor
    Arguments: class object to copy
    Output: class object instance
    */
    MyBoundingBoxClass(MyBoundingBoxClass const& other);

    /*
    Method: operator=
    Usage: Copy Assignment Operator
    Arguments: class object to copy
    Output: ---
    */
    MyBoundingBoxClass& operator=(MyBoundingBoxClass const& other);

    /*
    Method: ~MyBoundingBoxClass
    Usage: Destructor
    Arguments: ---
    Output: ---
    */
    ~MyBoundingBoxClass(void);

    /*
    Method: Swap
    Usage: Changes object contents for other object's
    Arguments:
    other -> object to swap content from
    Output: ---
    */
    void Swap(MyBoundingBoxClass& other);

    /*
    Method: SetToWorldMatrix
    Usage: Sets the Bounding Box into world coordinates
    Arguments:
    matrix4 a_m4ToWorld -> Model to World matrix
    Output: ---
    */
    void SetModelMatrix(matrix4 a_m4ToWorld);

    /// <summary>
    /// Gets the re-oriented version of this bounding box.
    /// </summary>
    MyBoundingBoxClass GetReorientedBoundingBox() const;

    /*
    Method: GetToWorldMatrix
    Usage: Gets the Bounding Box into world coordinates
    Arguments:---
    Output: matrix4 -> Model to World matrix
    */
    matrix4 GetModelMatrix(void) const;

    /*
    Method: GetCenter
    Usage: Gets the Bounding Box's center in local coordinates
    Arguments: ---
    Output: vector3 -> Center's of the box in local coordinates
    */
    vector3 GetCenterLocal(void) const;

    /*
    Method: GetCenterGlobal
    Usage: Gets the Bounding Box's center in global coordinates
    Arguments: ---
    Output: vector3 -> Center's of the box in global coordinates
    */
    vector3 GetCenterGlobal(void) const;

    /*
    Method: GetHalfWidth
    Usage: Gets the Bounding Box sizes for all sides (divided in half)
    Arguments: ---
    Output: vector3 -> HalfWidth Vector
    */
    vector3 GetHalfWidth(void) const;

    /*
    Method: IsColliding
    Usage: Asks if there is a collision with another Bounding Box Object
    Arguments:
    MyBoundingBoxClass* const a_pOther -> Other object to check collision with
    Output: bool -> check of the collision
    */
    bool IsColliding(MyBoundingBoxClass* const a_pOther) const;

	vector3 GetMaximum(void) const;

	vector3 GetMinimum(void) const;

private:
    /*
    Method: Release
    Usage: Deallocates member fields
    Arguments: ---
    Output: ---
    */
    void Release(void);

    /*
    Method: Init
    Usage: Allocates member fields
    Arguments: ---
    Output: ---
    */
    void Init(void);
};

#endif //__MYBOUNDINGBOXCLASS_H__