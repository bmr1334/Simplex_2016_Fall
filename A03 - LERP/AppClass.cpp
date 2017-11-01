#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Brandon M. Rodriguez - bmr1334@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUp(AXIS_Z * 20.0f, ZERO_V3, AXIS_Y);

	//if the light position is zero move it
	if (m_pLightMngr->GetPosition(1) == ZERO_V3)
		m_pLightMngr->SetPosition(vector3(0.0f, 0.0f, 3.0f));

	//if the background is cornflowerblue change it to black (its easier to see)
	if (vector3(m_v4ClearColor) == C_BLUE_CORNFLOWER)
	{
		m_v4ClearColor = vector4(ZERO_V3, 1.0f);
	}

	//if there are no segments create 7
	if (m_uOrbits < 1)
		m_uOrbits = 7;

	float fSize = 1.0f; //initial size of orbits
	for (int i = 0; i < m_uOrbits; i++) {

		orbitsVector.resize(m_uOrbits);
	}

	//creating a color using the spectrum 
	uint uColor = 650; //650 is Red
					   //prevent division by 0
	float decrements = 250.0f / (m_uOrbits > 1 ? static_cast<float>(m_uOrbits - 1) : 1.0f); //decrement until you get to 400 (which is violet)
																							/*
																							This part will create the orbits, it start at 3 because that is the minimum subdivisions a torus can have
																							*/

	uint uSides = 3; //start with the minimal 3 sides
	float x, y;

	for (uint i = 0; i < m_uOrbits; i++) {
		for (uint u = 0; u < m_uOrbits + uSides; u++) {

			//calculate x and y positions of points
			x = fSize * cos(((PI * 2) * u) / (uSides + i));
			y = fSize * sin(((PI * 2) * u) / (uSides + i));

			orbitsVector[i].push_back(vector3(x, y, 0));

		}
		fSize += 0.5f;
	}

	fSize = 1.0f;

	for (uint i = uSides; i < m_uOrbits + uSides; i++)
	{
		vector3 v3Color = WaveLengthToRGB(uColor); //calculate color based on wavelength
		m_shapeList.push_back(m_pMeshMngr->GenerateTorus(fSize, fSize - 0.1f, 3, i, v3Color)); //generate a custom torus and add it to the meshmanager
		fSize += 0.5f; //increment the size for the next orbit
		uColor -= static_cast<uint>(decrements); //decrease the wavelength
	}
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix(); //view Matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix(); //Projection Matrix
																 //matrix4 m4Offset = IDENTITY_M4; //offset of the orbits, starts as the global coordinate system
																 /*
																 The following offset will orient the orbits as in the demo, start without it to make your life easier.
																 */
	matrix4 m4Offset = glm::rotate(IDENTITY_M4, 90.0f, AXIS_Z);

	//get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

											   //calculate current model position
	float fPercentage = MapValue(fTimer, 0.0f, 2.0f, 0.0f, 1.0f); //calculates percentage from 0-1 based on time

																  // draw a shapes
	static std::vector<uint> stops;

	for (uint j = 0; j < m_uOrbits; j++) {
		stops.push_back(0);
	}
	//stops;

	for (uint i = 0; i < m_uOrbits; i++)
	{
		m_pMeshMngr->AddMeshToRenderList(m_shapeList[i], glm::rotate(m4Offset, 90.0f, AXIS_X));
		vector3 v3CurrentPos = glm::lerp(orbitsVector[i][stops[i]], orbitsVector[i][(stops[i] + 1) % orbitsVector[i].size()], fPercentage); //move from last segment to next segment 
		matrix4 m4Model = glm::translate(m4Offset, v3CurrentPos);

		//have we finished the current segment?
		if (fPercentage >= 1.0f) {
			stops[i]++;
			fTimer = m_pSystem->GetDeltaTime(uClock);
			stops[i] %= orbitsVector[i].size(); //make sure we stay in bounds
		}

		//draw spheres
		m_pMeshMngr->AddSphereToRenderList(m4Model * glm::scale(vector3(0.1)), C_WHITE);
	}

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
	//release GUI
	ShutdownGUI();
}