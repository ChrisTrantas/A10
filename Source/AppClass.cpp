#include "AppClass.h"

AppClass::AppClass( HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow, bool a_bUsingConsole )
    : super( hInstance, lpCmdLine, nCmdShow, a_bUsingConsole )
{
}

void AppClass::InitWindow(String a_sWindowName)
{
    super::InitWindow("MyBoundingSphereClas"); // Window Name

    // Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
    //if this line is in Init Application it will depend on the .cfg file, if it
    //is on the InitVariables it will always force it regardless of the .cfg
    m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
    //Initialize positions
    m_v3O1 = vector3(-2.5f, 0.0f, 0.0f);
    m_v3O2 = vector3(2.5f, 0.0f, 0.0f);	

    //Load Models
    m_pMeshMngr->LoadModel("Minecraft\\MC_Steve.obj", "Steve");
    m_pMeshMngr->LoadModel("Minecraft\\MC_Creeper.obj", "Creeper");

	// Initilize Manager
	m_pObjectManager = BoundingObjectManager::GetInstance(); // I did stuff

	// Sets the Index of Steve and Creeper
	iSteveBOIndex = m_pObjectManager->AddBox(m_pMeshMngr->GetVertexList("Steve"));
	iCreeperBOIndex = m_pObjectManager->AddBox(m_pMeshMngr->GetVertexList("Creeper"));

	// Prints number of BO to the console
	std::printf("Number of Bounding Objects: %d", m_pObjectManager->GetBOCount());
}

void AppClass::Update(void)
{
    //Update the system's time
    m_pSystem->UpdateTime();

    //Update the mesh manager's time without updating for collision detection
    m_pMeshMngr->Update(false);

    //First person camera movement
    if (m_bFPC == true)
        CameraRotation();

    ArcBall();

    //Set the model matrices for both objects and Bounding Spheres
    m_pMeshMngr->SetModelMatrix( glm::translate( m_v3O1 ) * ToMatrix4( m_qArcBall ), "Steve" );
    m_pMeshMngr->SetModelMatrix( glm::translate( m_v3O2 ), "Creeper" );

    // Set the bounding boxes' world matrices
	m_pObjectManager->SetModelMatrix(iSteveBOIndex, m_pMeshMngr->GetModelMatrix("Steve"));
	m_pObjectManager->SetModelMatrix(iCreeperBOIndex, m_pMeshMngr->GetModelMatrix("Creeper"));

	// Check collisons then draws
	m_pObjectManager->CheckCollisions();
	m_pObjectManager->Draw();

	

    //Adds all loaded instance to the render list
    m_pMeshMngr->AddInstanceToRenderList("ALL");	

    //Indicate the FPS
    int nFPS = m_pSystem->GetFPS();

    //print info into the console
    printf("FPS: %d            \r", nFPS);//print the Frames per Second

    //Print info on the screen
    m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
    m_pMeshMngr->Print("FPS:");
    m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

    //Render the grid based on the camera's mode:
    switch (m_pCameraMngr->GetCameraMode())
    {
    default: //Perspective
        m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
        break;
    case CAMERAMODE::CAMROTHOX:
        m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
        break;
    case CAMERAMODE::CAMROTHOY:
        m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
        break;
    case CAMERAMODE::CAMROTHOZ:
        m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
        break;
    }
    
    m_pMeshMngr->Render(); //renders the render list

    m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
    super::Release(); //release the memory of the inherited fields
	m_pObjectManager->ReleaseInstance();
}