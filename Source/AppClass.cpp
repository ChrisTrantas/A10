#include "AppClass.h"

AppClass::AppClass( HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow, bool a_bUsingConsole )
    : super( hInstance, lpCmdLine, nCmdShow, a_bUsingConsole )
    , _reorientedBB1( std::vector<vector3>() )
    , _reorientedBB2( std::vector<vector3>() )
{
}

void AppClass::InitWindow(String a_sWindowName)
{
    super::InitWindow("MyBoundingSphereClass example"); // Window Name

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

    m_pBB1 = new MyBoundingBoxClass( m_pMeshMngr->GetVertexList( "Steve" ) );
    m_pBB2 = new MyBoundingBoxClass( m_pMeshMngr->GetVertexList( "Creeper" ) );

    m_pBS1 = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Steve"));
    m_pBS2 = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Creeper"));

    m_pBO1 = new MyBoundingObjectClass(m_pMeshMngr->GetVertexList("Steve"));
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
    m_pBB1->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
    m_pBB2->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Creeper"));

    // Get the color for the un-oriented bounding boxes
    vector3 v3Color = REWHITE;
    if (m_pBB1->IsColliding(m_pBB2))
        v3Color = RERED;

    // Add wire frames for the un-oriented bounding boxes
    //m_pMeshMngr->AddCubeToQueue(glm::translate(m_pBB1->GetCenterGlobal()) * ToMatrix4(m_qArcBall) * glm::scale(m_pBB1->GetHalfWidth() * 2.0f), v3Color, WIRE);
    m_pMeshMngr->AddCubeToQueue(glm::translate(m_pBB2->GetCenterGlobal()) * glm::scale(m_pBB2->GetHalfWidth() * 2.0f), v3Color, WIRE);

    // Get our re-oriented bounding boxes
    _reorientedBB1 = m_pBB1->GetReorientedBoundingBox();
    _reorientedBB2 = m_pBB2->GetReorientedBoundingBox();

    // Get the color for the re-oriented bounding boxes
    v3Color = REBLACK;
    if ( _reorientedBB1.IsColliding( &_reorientedBB2 ) )
    {
        // Pink
        v3Color.r = 1.000000000f;
        v3Color.g = 0.752941251f;
        v3Color.b = 0.796078503f;
    }

    // Add wire frames for the re-oriented bounding boxes
   // m_pMeshMngr->AddCubeToQueue( glm::translate( _reorientedBB1.GetCenterGlobal() ) * glm::scale( _reorientedBB1.GetHalfWidth() * 2.0f ), v3Color, WIRE );
    m_pMeshMngr->AddCubeToQueue( glm::translate( _reorientedBB2.GetCenterGlobal() ) * glm::scale( _reorientedBB2.GetHalfWidth() * 2.0f ), v3Color, WIRE );
    
    
    
    // Set the bounding sphere's position
    m_pBS1->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
    m_pBS2->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Creeper"));

    // Get the color for the bounding spheres
    v3Color = REBLACK;
    if (m_pBS1->IsColliding(m_pBS2))
    {
        v3Color.r = 1.000000000f;
        v3Color.g = 0;
        v3Color.b = 0;
    }

    // Add wire frames for the bounding spheres
    //m_pMeshMngr->AddSphereToQueue(glm::translate(m_pBS1->GetCenterGlobal()) *   glm::scale(vector3(m_pBS1->GetRadius()* 2.0f)), v3Color, WIRE);
    m_pMeshMngr->AddSphereToQueue(glm::translate(m_pBS2->GetCenterGlobal()) * glm::scale(vector3(m_pBS2->GetRadius()* 2.0f)), v3Color, WIRE);

    m_pBO1->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
    m_pBO1->Draw();

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
    SafeDelete(m_pBB1);
    SafeDelete(m_pBB2);
}