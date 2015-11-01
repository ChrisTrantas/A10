#ifndef __MYBOUNDINGOBJECTCLASS_H_
#define __MYBOUNDINGOBJECTCLASS_H_

#include "RE\ReEng.h"


class MyBoundingObjectClass
{
	boolean m_bVisibility;	// Holds the visibility of the BO
	vector3 m_v3Color = REWHITE;	// Holds the color of the bounding object

	matrix4 m_m4ToWorld = IDENTITY_M4; // Matrix that takes us from local to world coordinates
	vector3 m_v3Center = vector3(0.0f); // Stores the center point of the Bounding Object
	vector3 m_v3Min = vector3(0.0f); // Stores the minimum vector of the bounding object Class
	vector3 m_v3Max = vector3(0.0f); // Stores the maximum vector of the bounding object Class

public:
	/*
	Method: MyBoundingObjectClass
	Usage: Constructor
	Arguments: ---
	Output: class object
	*/
	MyBoundingObjectClass(const std::vector<vector3>& vertices);

	/*
	Method: MyBoundingObjectClass
	Usage: Copy Constructor
	Arguments: class object to copy
	Output: class object instance
	*/
	MyBoundingObjectClass(MyBoundingObjectClass const& other);

	/*
	Method: operator=
	Usage: Copy Assignment Operator
	Arguments: class object to copy
	Output: ---
	*/
	MyBoundingObjectClass& operator=(MyBoundingObjectClass const& other);

	/*
	Method: ~MyBoundingObjectClass
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~MyBoundingObjectClass(void);

	void SetVisibility(boolean a_bVisibility);

	boolean GetVisibility(void);

	/*
	Method: GetCenter
	Usage: Gets the Bounding Objects's center in local coordinates
	Arguments: ---
	Output: vector3 -> Center's of the object in local coordinates
	*/
	vector3 GetCenterLocal(void) const;

	/*
	Method: GetCenterGlobal
	Usage: Gets the Bounding Object's center in global coordinates
	Arguments: ---
	Output: vector3 -> Center's of the object in global coordinates
	*/
	vector3 GetCenterGlobal(void) const;


	vector3 GetMinimum(void) const;

	vector3 GetMaximum(void) const;

	/*
	Method: SetToWorldMatrix
	Usage: Sets the Bounding Object into world coordinates
	Arguments:
	matrix4 a_m4ToWorld -> Model to World matrix
	Output: ---
	*/
	void SetModelMatrix(matrix4 a_m4ToWorld);

	/*
	Method: GetToWorldMatrix
	Usage: Gets the Bounding Object into world coordinates
	Arguments:---
	Output: matrix4 -> Model to World matrix
	*/
	matrix4 GetModelMatrix(void) const;

	// Sets the color of the Bounding Object
	void SetColor(vector3 a_v3Color);

	// Adds the Bounding Object to the Render List
	void Draw(void);

	/*
	Method: IsColliding
	Usage: Asks if there is a collision with another Bounding Box Object
	Arguments:
	MyBoundingObjectClass* const a_pOther -> Other object to check collision with
	Output: bool -> check of the collision
	*/
	bool IsColliding(MyBoundingObjectClass* const a_pOther) const;
};

#endif //__MYBOUNDINGOBJECTCLASS_H_