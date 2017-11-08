/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2017/05
----------------------------------------------*/
#ifndef __MYRIGIDBODY_H_
#define __MYRIGIDBODY_H_

#include "Definitions.h"
#include "MyMesh.h"
#include "MyCamera.h"

namespace Simplex {
	Mesh* m_pBS;

	class MyRigidBody {

	public:
		MyRigidBody();
		~MyRigidBody();
		void Render(MyCamera* a_pCamera, matrix4 a_m4Model);
	};
};
#endif //__MYRIGIDBODY_H_