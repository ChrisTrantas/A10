#ifndef __MYBOUNDINGSPHERECLASS_H_
#define __MYBOUNDINGSPHERECLASS_H_

#include "RE\ReEng.h"

class MyBoundingSphereClass
{
	matrix4 m_m4ToWorld = IDENTITY_M4;	// The World Matrix of the sphere

	vector3 m_v3Center = vector3();		// The center of the sphere in local space
	float m_fRadius = 0.0f;				// The radius of the sphere

public:
	// Creates bounding sphere from list of vertices
	MyBoundingSphereClass(const std::vector<vector3>& vertices);	

	// Creates bounding sphere from other bounding sphere
	MyBoundingSphereClass(MyBoundingSphereClass const& other);

	// Sets field values to equal that of another bounding sphere
	MyBoundingSphereClass& operator=(MyBoundingSphereClass const& other);

	// Deallocates bounding sphere
	~MyBoundingSphereClass();

	// Finds if this bounding sphere is colliding with another bounding sphere
	bool IsColliding(MyBoundingSphereClass* other);

	// Sets the World Matrix of the sphere
	void SetModelMatrix(matrix4 m4ToWorld);


	vector3 GetCenterLocal(void);	// Gets the center of the bounding sphere in local space
	vector3 GetCenterGlobal(void);	// Gets the center of the bounding sphere in global space

	float GetRadius(void);	// Gets the radius of the bounding sphere
};

#endif //__MYBOUNDINGSPHERECLASS_H_
