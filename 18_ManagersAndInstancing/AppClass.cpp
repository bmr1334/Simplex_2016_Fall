#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera
	m_pCamera = new MyCamera();
	m_pCamera->SetPositionTargetAndUp(
		vector3(0.0f, 3.0f, 20.0f), //Where my eyes are
		vector3(0.0f, 3.0f, 19.0f), //where what I'm looking at is
		AXIS_Y);					//what is up

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);
	m_pMesh = new MyMesh();
	m_pMesh->GenerateTube(1.0f, 0.7f, 2.0f, 10, C_GREEN_LIME);

	for (uint i = 0; i < 5000; ++i) {
		matrix4* m4Temp = new matrix4();
		*m4Temp = glm::translate(IDENTITY_M4, vector3(i * 2.0f, 0.0f, 0.0f)) * ToMatrix4(m_qArcBall);
		m4List.push_back(m4Temp);
	}
	m_RB = new MyRigidBody();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();	
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();


	*m4List[0] = glm::translate(IDENTITY_M4, vector3(0*3.0f, 0.0f, 0.0f)) * ToMatrix4(m_qArcBall);

	m_pMesh->Render(m_pCamera, m4List);

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
		
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	for (uint i = 0; i < m4List.size(); ++i) {
		SafeDelete(m4List[i]);
	}

	m4List.clear();

	//release the camera
	SafeDelete(m_pMesh);
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
