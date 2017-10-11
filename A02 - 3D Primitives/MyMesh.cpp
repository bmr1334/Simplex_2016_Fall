#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3) //less than 3 would be a plane!
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// -------------------------------

	//variables
	vector3 topVert(0.0f, a_fHeight, 0.0f);
	vector3 bottomCenter(0.0f, 0.0f, 0.0f);
	std::vector<vector3> bottomCircle;
	float x, z;

	//loops around bottom ring to find positions of vertices
	for (int i = 0; a_nSubdivisions > i; i++) {
		x = a_fRadius * cos(((PI * 2) * i) / a_nSubdivisions);
		z = a_fRadius * sin(((PI * 2) * i) / a_nSubdivisions);

		//fill circle with vertex values
		bottomCircle.push_back(vector3(x, 0.0f, z));
	}
	
	for (int i = 0; a_nSubdivisions > i; i++) {
		//at last spot, must loop around since at ring size cap
		if (bottomCircle.size() - 1 == i) { //connect sides then bottom
			AddTri(bottomCircle[0], bottomCircle[i], topVert);
			AddTri(bottomCenter, bottomCircle[i], bottomCircle[0]);
		}
		else { //connect sides then bottom
			AddTri(bottomCircle[i + 1], bottomCircle[i], topVert);
			AddTri(bottomCenter, bottomCircle[i], bottomCircle[i + 1]);
		}
	}
	
	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3) //less than 3 would be a plane!
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// -------------------------------

	//variables
	vector3 topCenter(0.0f, a_fHeight, 0.0f);
	vector3 bottomCenter(0.0f, 0.0f, 0.0f);
	std::vector<vector3> bottomCircle;
	std::vector<vector3> topCircle;
	float x, z;

	//loops around both circles to find positions of vertices
	for (int i = 0; a_nSubdivisions > i; i++) {
		x = a_fRadius * cos(((PI * 2) * i) / a_nSubdivisions);
		z = a_fRadius * sin(((PI * 2) * i) / a_nSubdivisions);
		
		//populate circles with vertex values
		topCircle.push_back(vector3(x, a_fHeight, z));
		bottomCircle.push_back(vector3(x, 0.0f, z));
	}

	for (int i = 0; a_nSubdivisions > i; i++) {
		//at last spot, must loop around since at circle size cap
		if (bottomCircle.size() - 1 == i) { //connect from top to bottom
			AddTri(topCircle[0], topCircle[i], topCenter);
			AddQuad(bottomCircle[0], bottomCircle[i], topCircle[0], topCircle[i]);
			AddTri(bottomCenter, bottomCircle[i], bottomCircle[0]);
		}
		else { //connect from top to bottom
			AddTri(topCircle[i + 1], topCircle[i], topCenter);
			AddQuad(bottomCircle[i + 1], bottomCircle[i], topCircle[i + 1], topCircle[i]);
			AddTri(bottomCenter, bottomCircle[i], bottomCircle[i + 1]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3) //less than 3 would be a plane!
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// -------------------------------

	//variables
	std::vector<vector3> outerBottomCircle;
	std::vector<vector3> innerBottomCircle;
	std::vector<vector3> outerTopCircle;
	std::vector<vector3> innerTopCircle;
	float x, z, innerX, innerZ;
	float y = 0.0f;

	//loops around all four circles to find positions of vertices
	for (int i = 0; a_nSubdivisions > i; i++) {
		x = a_fOuterRadius * cos(((PI * 2) * i) / a_nSubdivisions);
		z = a_fOuterRadius * sin(((PI * 2) * i) / a_nSubdivisions);
		innerX = a_fInnerRadius * cos(((PI * 2) * i) / a_nSubdivisions);
		innerZ = a_fInnerRadius * sin(((PI * 2) * i) / a_nSubdivisions);

		//fill appropriate circles with vertex values
		outerBottomCircle.push_back(vector3(x, y, z));
		outerTopCircle.push_back(vector3(x, a_fHeight, z));
		innerBottomCircle.push_back(vector3(innerX, y, innerZ));
		innerTopCircle.push_back(vector3(innerX, a_fHeight, innerZ));
	}

	//draw circles, looping through same way as before
	for (int i = 0; a_nSubdivisions > i; i++) {
		if (outerBottomCircle.size() - 1 == i) { //at last spot, must loop around since at circle size cap
			//top
			AddQuad(outerTopCircle[0], outerTopCircle[i], innerTopCircle[0], innerTopCircle[i]);

			//sides
			AddQuad(outerBottomCircle[0], outerBottomCircle[i], outerTopCircle[0], outerTopCircle[i]);
			AddQuad(innerTopCircle[0], innerTopCircle[i], innerBottomCircle[0], innerBottomCircle[i]);

			//and bottom
			AddQuad(innerBottomCircle[0], innerBottomCircle[i], outerBottomCircle[0], outerBottomCircle[i]);
		}
		else { //not at last spot
			//top
			AddQuad(outerTopCircle[i + 1], outerTopCircle[i], innerTopCircle[i + 1], innerTopCircle[i]);

			//sides
			AddQuad(outerBottomCircle[i + 1], outerBottomCircle[i], outerTopCircle[i + 1], outerTopCircle[i]);
			AddQuad(innerTopCircle[i + 1], innerTopCircle[i], innerBottomCircle[i + 1], innerBottomCircle[i]);

			//and bottom
			AddQuad(innerBottomCircle[i + 1], innerBottomCircle[i], outerBottomCircle[i + 1], outerBottomCircle[i]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 3) //less than 3 would be a plane!
	{
		a_nSubdivisions = 3;
	}
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	// -------------------------------

	//variables
	vector3 topCenter(0.0f, a_fRadius, 0.0f);
	vector3 bottomCenter(0.0f, 0.0f, 0.0f);
	std::vector<vector3> bottomCircle;
	std::vector<vector3> topCircle;
	std::vector<vector3> circleOne;
	std::vector<vector3> circleTwo;
	std::vector<vector3> circleThree;
	float x, z;

	//loops around all circles to find positions of vertices
	for (int i = 0; a_nSubdivisions > i; i++) {
		x = a_fRadius * cos((i * (PI * 2)) / a_nSubdivisions);
		z = a_fRadius * sin((i * (PI * 2)) / a_nSubdivisions);
		
		//fill circles with appropriate vertex values
		topCircle.push_back(vector3(x * a_fRadius, a_fRadius, z * a_fRadius));
		circleOne.push_back(vector3(x * a_fRadius * 1.5, (3 * a_fRadius) / 4, z * a_fRadius * 1.5)); //3/4 of the way to the top
		circleTwo.push_back(vector3(x * a_fRadius * 1.6, a_fRadius / 2, z * a_fRadius * 1.6)); //halfway
		circleThree.push_back(vector3(x * a_fRadius * 1.5, a_fRadius / 4, z * a_fRadius * 1.5)); //quarter of the way
		bottomCircle.push_back(vector3(x * a_fRadius, 0.0f, z * a_fRadius));
	}

	//draw circles, looping through same way as before
	for (int i = 0; a_nSubdivisions > i; i++) {
		if (bottomCircle.size() - 1 == i) { //at last spot, must loop around since at circle size cap
			//top faces
			AddTri(topCircle[0], topCircle[i], topCenter);

			//side faces, from top to bottom
			AddQuad(circleOne[0], circleOne[i], topCircle[0], topCircle[i]);
			AddQuad(circleTwo[0], circleTwo[i], circleOne[0], circleOne[i]);
			AddQuad(circleThree[0], circleThree[i], circleTwo[0], circleTwo[i]);
			AddQuad(bottomCircle[0], bottomCircle[i], circleThree[0], circleThree[i]);

			//botom faces
			AddTri(bottomCenter, bottomCircle[i], bottomCircle[0]);
		}
		else { //not at last spot
			//top faces
			AddTri(topCircle[i + 1], topCircle[i], topCenter);

			//side faces, from top to bottom
			AddQuad(circleOne[i + 1], circleOne[i], topCircle[i + 1], topCircle[i]);
			AddQuad(circleTwo[i + 1], circleTwo[i], circleOne[i + 1], circleOne[i]);
			AddQuad(circleThree[i + 1], circleThree[i], circleTwo[i + 1], circleTwo[i]);
			AddQuad(bottomCircle[i + 1], bottomCircle[i], circleThree[i + 1], circleThree[i]);

			//and bottom faces
			AddTri(bottomCenter, bottomCircle[i], bottomCircle[i + 1]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}