#include "MyRigidBody.h"

using namespace Simplex;
MyRigidBody::MyRigidBody()
{
	m_pBS = new Mesh();
	m_pBS->GenerateIcoSphere(1.0f, 2, C_GREEN_LIME);
}

void MyRigidBody::Render(MyCamera* a_pCamera, matrix4 a_m4Model)
{
	m_pBS->RenderWire(a_pCamera->GetProjectionMatrix(), a_pCamera->GetViewMatrix(),
		glm::value_ptr(a_m4Model), 1);
}

MyRigidBody::~MyRigidBody() {
	if (m_pBS) {
		delete m_pBS;
		m_pBS = nullptr;
	}
}
