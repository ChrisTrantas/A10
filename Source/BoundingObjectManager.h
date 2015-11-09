#pragma once
#include "MyBoundingObjectClass.h"
#include <memory>

class BoundingObjectManager
{
	// Instance for BO manager
	static BoundingObjectManager* m_pInstance;

	// List of BOs
	std::vector<MyBoundingObjectClass*> m_lBox; 

	// Count of BOs
	int m_nBoxCount = 0;

	// Holds the visibility of the BO
	bool m_bVisibility = false;	
	
	// World Matrix
	matrix4 m_m4ToWorld = IDENTITY_M4;

public:
	// Gets/constructs the singleton pointer
	static BoundingObjectManager* GetInstance(); 

	// Destroys the singleton
	static void ReleaseInstance(void);

	// Adds box based on model
	int AddBox(const std::vector<vector3>& vertices);

	// Sets visiblity of a specific BO
	void SetVisibility(int a_iIndex, bool a_bVisibility);

	// Gets visiblity of a specific BO
	bool GetVisibility(int a_iIndex) const;

	// Gets Center of model in Local space
	vector3 GetCenterLocal() const;

	// Gets Center of model in Global space
	vector3 GetCenterGlobal() const;

	// Get BOCount
	int GetBOCount();

	// Get Minimum value
	vector3 GetMinimum() const;

	// Get Maximum value
	vector3 GetMaximum() const;

	// Checks collisons with all other BOs
	void CheckCollisions() const;	

	// Sets the model matrix of a specific index
	void SetModelMatrix(int a_iIndex, matrix4 a_m4ToWorld);

	// Gets the model matrix of a specific index
	matrix4 GetModelMatrix() const;

	// Sets the color of the Bounding Object
	void SetColor(int a_iIndex, vector3 a_v3Color);

	// Renders all bounding objects
	void Draw();

	// Draws a specific bounding object
	void Draw(int a_iIndex);

private:
	/* Releases the objects memory */
	void Release(void);

	/* Initializes the objects fields */
	void Init(void);
};

