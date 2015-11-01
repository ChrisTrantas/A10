#ifndef __MYBOUNDINGOBJECTCLASS_H_
#define __MYBOUNDINGOBJECTCLASS_H_

#include "RE\ReEng.h"

#include "MyBoundingBoxClass.h"
#include "MyBoundingSphereClass.h"

class MyBoundingObjectClass
{
	MyBoundingBoxClass* m_pBoundingBox;
	MyBoundingBoxClass* m_pReorientedBoundingBox;
	MyBoundingSphereClass* m_pBoundingSphere;

	boolean m_bVisibility;	// Holds the visibility of the BO
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

	~MyBoundingObjectClass(void);


	void SetVisibility(boolean a_bVisibility);

	boolean GetVisibility(void);


	vector3 GetCenterLocal(void) const;

	vector3 GetCenterGlobal(void) const;


	vector3 GetMinimum(void) const;

	vector3 GetMaximum(void) const;


	void SetModelMatrix(matrix4 a_m4ToWorld);
	matrix4 GetModelMatrix(void) const;

	// Sets the color of the Bounding Object
	void SetColor(vector3 a_v3Color);

	// Adds the Bounding Object to the Render List
	void Draw(MeshManagerSingleton*);

	bool IsColliding(MyBoundingObjectClass* const a_pOther) const;
};

#endif //__MYBOUNDINGOBJECTCLASS_H_