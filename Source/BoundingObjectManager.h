#pragma once
#include "MyBoundingObjectClass.h"
#include <memory>

class BoundingObjectManager
{
	static BoundingObjectManager* m_pInstance;
	std::vector<MyBoundingObjectClass*> m_lBox; // 

	int m_nBoxCount = 0;
	bool m_bVisibility = false;	// Holds the visibility of the BO
	//vector3 m_v3Color = REBLACK;	// Holds the color of the bounding object
	
	matrix4 m_m4ToWorld = IDENTITY_M4;

public:
	//// Gets/constructs the singleton pointer
	static BoundingObjectManager* GetInstance(); 
	//
	//// Destroys the singleton
	static void ReleaseInstance(void);

	void AddBox(const std::vector<vector3>& vertices);

	void SetVisibility(bool a_bVisibility);

	bool GetVisibility() const;


	vector3 GetCenterLocal() const;

	vector3 GetCenterGlobal() const;


	vector3 GetMinimum() const;

	vector3 GetMaximum() const;

	bool IsColliding(MyBoundingObjectClass* const a_pOther) const;	// Checks if this is colliding with another bounding object.

	void SetModelMatrix(matrix4 a_m4ToWorld);
	matrix4 GetModelMatrix() const;

	// Sets the color of the Bounding Object
	void SetColor(vector3 a_v3Color);

	/// <summary>
	/// Draws this bounding object.
	/// </summary>
	void Draw();

private:
	////constructor
	//BoundingObjectManager();
	////copy constuctor
	//BoundingObjectManager(BoundingObjectManager const& other);
	//// copy assignment operator
	//BoundingObjectManager& operator=(BoundingObjectManager const& other);
	// Destructor
//	~BoundingObjectManager(void);

	/* Releases the objects memory */
	void Release(void);
	/* Initializes the objects fields */
	void Init(void);
};

