#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Brandon M. Rodriguez - bmr1334@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Make MyMesh array
	m_pMesh = new MyMesh[46];
	for (int i = 0; i < 46; i++) {
		m_pMesh[i].GenerateCube(1.0f, C_BLACK);
	}
	m_pMesh->GenerateCube(1.0f, C_BLACK);

	//Make MyMesh object
	m_pMesh1 = new MyMesh();
	m_pMesh1->GenerateCube(1.0f, C_WHITE);
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
	// Clear the screen
	ClearScreen();

	//set up matrices
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	vector3 v3Position(3,-1,0);
	static matrix4 m4Model;

	//move shape
	m4Model = glm::translate(m4Model, vector3(0.01f, 0.0f, 0.0f));

	//put cubes into shape
	m_pMesh[0].Render(m4Projection, m4View, m4Model);
	m_pMesh[1].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(0, 1, 0)));
	m_pMesh[2].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(0, 2, 0)));
	m_pMesh[3].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(0, 3, 0)));
	m_pMesh[4].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1, 0, 0)));
	m_pMesh[5].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2, 0, 0)));
	m_pMesh[6].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, 0, 0)));
	m_pMesh[7].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(5, 0, 0)));
	m_pMesh[8].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1, 1, 0)));
	m_pMesh[9].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1, 2, 0)));
	m_pMesh[10].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1, 3, 0)));
	m_pMesh[11].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2, 3, 0)));
	m_pMesh[12].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2, 1, 0)));
	m_pMesh[13].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, 1, 0)));
	m_pMesh[14].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(4, 1, 0)));
	m_pMesh[15].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(5, 1, 0)));
	m_pMesh[16].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, 2, 0)));
	m_pMesh[17].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(4, 2, 0)));
	m_pMesh[18].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, 3, 0)));
	m_pMesh[19].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2, 4, 0)));
	m_pMesh[20].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, 5, 0)));
	m_pMesh[21].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(3, -1, 0)));
	m_pMesh[22].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(5, -1, 0)));
	m_pMesh[23].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(2, -2, 0)));
	m_pMesh[24].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(1, -2, 0)));
	m_pMesh[25].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1, -2, 0)));
	m_pMesh[26].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2, -2, 0)));
	m_pMesh[27].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3, -1, 0)));
	m_pMesh[28].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-5, -1, 0)));
	m_pMesh[29].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1, 0, 0)));
	m_pMesh[30].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2, 0, 0)));
	m_pMesh[31].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3, 0, 0)));
	m_pMesh[32].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-5, 0, 0)));
	m_pMesh[33].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1, 1, 0)));
	m_pMesh[34].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2, 1, 0)));
	m_pMesh[35].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3, 1, 0)));
	m_pMesh[36].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-4, 1, 0)));
	m_pMesh[37].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-5, 1, 0)));
	m_pMesh[38].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1, 2, 0)));
	m_pMesh[39].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3, 2, 0)));
	m_pMesh[40].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-4, 2, 0)));
	m_pMesh[41].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-1, 3, 0)));
	m_pMesh[41].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2, 3, 0)));
	m_pMesh[43].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3, 3, 0)));
	m_pMesh[44].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-2, 4, 0)));
	m_pMesh[45].Render(m4Projection, m4View, m4Model * glm::translate(IDENTITY_M4, vector3(-3, 5, 0)));
	
	//unused, just keeping here for later reference
	//m4Model = glm::translate(IDENTITY_M4, v3Position);
	//m4Model = glm::rotate(m4Model, 45.0f, vector3(0,0,1));

	m_pMesh->Render(m4Projection, m4View, m4Model);

	//unused, just keeping here for later reference
	//m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	//m_pMesh1->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(vector3( 3.0f, 0.0f, 0.0f)));
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
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
	if (m_pMesh != nullptr)
	{
		delete [] m_pMesh; //deletes all objects in array
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);

	//release GUI
	ShutdownGUI();
}