#include "AppClass.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Brandon M. Rodriguez - bmr1334@rit.edu";

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
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Add objects to the Manager
	for (int j = -50; j < 50; j += 2)
	{
		for (int i = -50; i < 50; i += 2)
		{
			m_pMyMeshMngr->AddConeToRenderList(glm::translate(vector3(i, 0.0f, j)));
		}
	}
}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//calculate camera movement
	vector3 v3View = m_v3Pos;
	v3View.z -= 1.0f; //look one unit in front of camera
	
	//set camera values
	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
	m_pCamera->SetPosition(m_v3Pos);
	//CameraRotation(1.0f);
	
	//matrix4 m4RotX = glm::rotate(IDENTITY_M4, m_v3Orientation.x, AXIS_X);
	//matrix4 m4RotY = glm::rotate(IDENTITY_M4, m_v3Orientation.y, AXIS_Y);
	//matrix4 m4RotZ = glm::rotate(IDENTITY_M4, m_v3Orientation.z, AXIS_Z);
	//matrix4 m4Orientation = m4RotX * m4RotY * m4RotZ;

	//matrix4 m4Model = m4Orientation;

	

	//m_pCamera = glm::lookAt(m_v3Pos, )
	//m_pCamera->SetTarget(vector3(v3View.x, v3View.y, v3View.z));

	//matrix4 m4Model = ToMatrix4(m_qQuaternion);

	vector4 forward;
	vector4 up;
	forward = ToMatrix4(m_qQuaternion) * vector4(0.0f, 0.0f, 1.0f, 1.0f);
	up = ToMatrix4(m_qQuaternion) * vector4(0.0f, 1.0f, 0.0f, 1.0f);
	vector3 f = vector3(forward.x, forward.y, forward.z);
	vector3 u = vector3(up.x, up.y, up.z);

	vector4 lookAtMat = vector4(0.0f, 0.0f, 1.0f, 1.0f) * glm::lookAt(m_v3Pos, f, u);

	//m_pCamera->SetTarget(vector3(lookAtMat.x, lookAtMat.y, lookAtMat.z));
	
	m_pCamera->SetTarget(vector3(v3View.x, v3View.y, v3View.z));

	//m_pCamera->SetPositionTargetAndUp(m_v3Pos, vector3(0, 0, 1 - m_v3Pos.z), vector3(0, 1, 0));
	//m_pCamera->SetPositionTargetAndUp(m_v3Pos, vector3(0, 0, 1 - m_v3Pos.z), vector3(0, 1, 0));



	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
