#pragma once
#include "MyBoundingObjectClass.h"

class BoundingObjectManager
{
	static BoundingObjectManager* m_pInstance;
	std::shared_ptr<MyBoundingObjectClass> m_pBoundingObject;
	std::shared_ptr<MyBoundingBoxClass> m_pReorientedBoundingBox;
	std::shared_ptr<MyBoundingSphereClass> m_pBoundingSphere;

	bool m_bVisibility;	// Holds the visibility of the BO
	vector3 m_v3Color = REBLACK;	// Holds the color of the bounding object

	matrix4 m_m4ToWorld = IDENTITY_M4; // Matrix that takes us from local to world coordinates
	vector3 m_v3Center = vector3(0.0f); // Stores the center point of the Bounding Object
	vector3 m_v3Min = vector3(0.0f); // Stores the minimum vector of the bounding object Class
	vector3 m_v3Max = vector3(0.0f); // Stores the maximum vector of the bounding object Class

public:
	// Gets/constructs the singleton pointer
	static BoundingObjectManager* GetInstance(); // problem here
	
	// Destroys the singleton
	static void ReleaseInstance(void);

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

private:
	//constructor
	BoundingObjectManager();
	//copy constuctor
	BoundingObjectManager(BoundingObjectManager const& other);
	// copy assignment operator
	BoundingObjectManager& operator=(BoundingObjectManager const& other);
	// Destructor
	~BoundingObjectManager(void);

	/* Releases the objects memory */
	void Release(void);
	/* Initializes the objects fields */
	void Init(void);
};

