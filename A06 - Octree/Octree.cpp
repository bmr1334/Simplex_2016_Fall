#include "Octree.h"
using namespace Simplex;

//variables
uint Octree::leavesAmount;
uint Octree::octantAmount;
uint Octree::subMax;
uint Octree::entityAmount;

//root constructor
Octree::Octree(uint p_MaxLevel, uint p_EntityCount)
{
	//make root octant
	Init();
	octantRoot = this;
	subMax = p_MaxLevel;
	entityAmount = p_EntityCount;

	//octant minimum and maximum should be global center
	octantMaximum = octantMinimum = m_pEntityMngr->GetRigidBody()->GetCenterGlobal();
	currentEntityAmount = m_pEntityMngr->GetEntityCount();

	//loop to set values for all entities
	for (uint i = 0; i < currentEntityAmount; i++)
	{
		entityList.push_back(i);

		//getters for min and max of rigidbodies
		vector3 rb_min = m_pEntityMngr->GetRigidBody(i)->GetMinGlobal();
		vector3 rb_max = m_pEntityMngr->GetRigidBody(i)->GetMaxGlobal();

		//set x minimum and maximum
		if (rb_min.x < octantMinimum.x)	octantMinimum.x = rb_min.x;
		if (rb_max.x > octantMaximum.x)	octantMaximum.x = rb_max.x;

		//set y minimum and maximum
		if (rb_min.y < octantMinimum.y)	octantMinimum.y = rb_min.y;
		if (rb_max.y > octantMaximum.y)	octantMaximum.y = rb_max.y;

		//set z minimum and maximum
		if (rb_min.z < octantMinimum.z) octantMinimum.z = rb_min.z;
		if (rb_max.z > octantMaximum.z)	octantMaximum.z = rb_max.z;
	}

	//octant center and size calculations
	octantCenter = (octantMinimum + octantMaximum) / 2.f;
	octantSize = octantMaximum - octantMinimum;

	//subdivide octree to make children
	Subdivide();

	//give dimensions to leaves
	SetDimensions();
}

//initializes values
void Octree::Init(void)
{
	m_pEntityMngr = MyEntityManager::GetInstance();
	m_pMeshMngr = MeshManager::GetInstance();
	octantID = octantAmount;
	octantAmount++;
}

//constructor
Octree::Octree(vector3 p_Center, vector3 p_Size)
{
	Init();
	octantCenter = p_Center;
	octantSize = p_Size;
	octantMinimum = p_Center - octantSize / 2.f;
	octantMaximum = p_Center + octantSize / 2.f;
}

//copy constructor
Octree::Octree(Octree const & other)
{
	Init();

	//copy values
	currentLevel = other.currentLevel;
	octantSize = other.octantSize;
	octantCenter = other.octantCenter;
	octantMinimum = other.octantMinimum;
	octantMaximum = other.octantMaximum;
	octantParent = other.octantParent;
	Release();

	//loop to create more children
	childrenNumber = other.childrenNumber;
	for (uint i = 0; i < childrenNumber; ++i)
	{
		octantChildren[i] = new Octree(*other.octantChildren[i]);
	}

	//loop to create new entity list
	currentEntityAmount = other.currentEntityAmount;
	for (uint i = 0; i < currentEntityAmount; ++i)
	{
		entityList.push_back(other.entityList[i]);
	}

	//copy children over if at root
	octantRoot = other.octantRoot;
	if (this == octantRoot)
	{
		float childCount = other.rootChildren.size();
		for (uint i = 0; i < childCount; ++i)
		{
			rootChildren.push_back(other.rootChildren[i]);
		}
	}
}

//is this an outside octant?
bool Octree::IsOutside(void)
{
	return childrenNumber == 0;
}

//displays a specific octant
void Simplex::Octree::Display(uint a_uIndex, vector3 a_v3Color)
{
	//should we display all the octants?
	if (a_uIndex >= octantAmount && wireFrameSwitch == true)
	{
		TotalDisplay();
		return;
	}

	//display the current octant
	rootChildren[a_uIndex]->CurrDisplay(a_v3Color);
}

//clears all entities from octree
void Octree::ClearEntityList(void)
{
	//loop through all children and remove from entity list
	for (uint i = 0; i < childrenNumber; ++i)
	{
		octantChildren[i]->ClearEntityList();
	}
	entityList.clear();
}

//displays current octant
void Octree::CurrDisplay(vector3 a_v3Color)
{
	//should we display the wireframe?
	if (wireFrameSwitch == true)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, octantCenter) * glm::scale(IDENTITY_M4, octantSize), a_v3Color);
	}
}

//displays all octants
void Simplex::Octree::TotalDisplay(vector3 a_v3Color)
{
	//only display when wireframe is active
	if (wireFrameSwitch == true)
	{
		//display current octant if on leaf
		if (IsOutside()) CurrDisplay(a_v3Color);
		else //not a leaf
		{
			//display all children
			for (uint i = 0; i < childrenNumber; ++i)
			{
				octantChildren[i]->TotalDisplay(a_v3Color);
			}
		}
	}
}

//does the octant contain more than this many entities?
bool Octree::HasMore(uint a_nEntities)
{
	return currentEntityAmount > a_nEntities;
}

//are they colliding?
bool Octree::IsColliding(uint a_uRBIndex)
{
	MyRigidBody* rigidBody = m_pEntityMngr->GetRigidBody(a_uRBIndex);
	vector3 rbMax = rigidBody->GetMaxGlobal();
	vector3 rbMin = rigidBody->GetMinGlobal();

	//position checks
	if (rbMax.x > octantMinimum.x && rbMax.y > octantMinimum.y && rbMax.z > octantMinimum.z &&
		rbMin.x < octantMaximum.x && rbMin.y < octantMaximum.y && rbMin.z < octantMaximum.z)
	{
		return true;
	}
	else //no collision, so return false
		return false;
}

//subdivides the octree
void Octree::Subdivide(void)
{
	//stop subdividing at subMax
	if (!HasMore(entityAmount) || currentLevel >= subMax)
	{
		octantRoot->rootChildren.push_back(this);
		leavesAmount++;
		return;
	}

	//are we trying to exceed max possible subdivisions?
	if (childrenNumber == 8)
	{
		std::cout << "Cannot exceed maximum subdivisions!" << std::endl;
		return;
	}

	//creating each octant at the right position
	octantChildren[0] = new Octree(octantCenter + vector3(-octantSize.x / 4, octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octantChildren[1] = new Octree(octantCenter + vector3(-octantSize.x / 4, octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	octantChildren[2] = new Octree(octantCenter + vector3(-octantSize.x / 4, -octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octantChildren[3] = new Octree(octantCenter + vector3(-octantSize.x / 4, -octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	octantChildren[4] = new Octree(octantCenter + vector3(octantSize.x / 4, -octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octantChildren[5] = new Octree(octantCenter + vector3(octantSize.x / 4, -octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	octantChildren[6] = new Octree(octantCenter + vector3(octantSize.x / 4, octantSize.y / 4, -octantSize.z / 4), octantSize / 2.f);
	octantChildren[7] = new Octree(octantCenter + vector3(octantSize.x / 4, octantSize.y / 4, octantSize.z / 4), octantSize / 2.f);
	childrenNumber = 8;

	//initialize children
	for (uint i = 0; i < childrenNumber; ++i)
	{
		octantChildren[i]->octantParent = this;
		octantChildren[i]->currentLevel = currentLevel + 1;
		octantChildren[i]->octantRoot = octantRoot;

		//add rigidbodies to children
		for (uint j = 0; j < currentEntityAmount; ++j)
		{
			//if colliding, push back that information
			if (octantChildren[i]->IsColliding(entityList[j]))
			{
				octantChildren[i]->entityList.push_back(entityList[j]);
			}
		}

		octantChildren[i]->currentEntityAmount = octantChildren[i]->entityList.size();
		octantChildren[i]->Subdivide();
	}
}

//gets size of octant
vector3 Octree::GetSize(void)
{
	return octantSize;
}

//gets number of leaves in the octree
uint Simplex::Octree::GetLeafCount(void)
{
	return leavesAmount;
}

//gets a parent in the octree
Octree * Octree::GetParent(void)
{
	return octantParent;
}

//gets a child in the octree
Octree * Octree::GetChild(uint a_nChild)
{
	//no children, so return nullptr
	if (childrenNumber == 0) return nullptr;
	else //there's children, so return them
		return octantChildren[a_nChild];
}

//gets center of octant
vector3 Octree::GetGlobalCenter(void)
{
	return octantCenter;
}

//gets octant minimum
vector3 Octree::GetGlobalMin(void)
{
	return octantMinimum;
}

//gets octant maximum
vector3 Octree::GetGlobalMax(void)
{
	return octantMaximum;
}

//gets number of octants in the octree
uint Octree::GetOctantCount(void)
{
	return octantAmount;
}

//set octree dimensions
void Simplex::Octree::SetDimensions()
{
	//is this a leaf?
	if (IsOutside())
	{
		//give it proper dimensions
		for (uint i = 0; i < currentEntityAmount; ++i)
		{
			m_pEntityMngr->AddDimension(entityList[i], octantID);
		}
	}
	else
	{
		//give non-leaves proper dimensions
		for (uint i = 0; i < childrenNumber; ++i)
		{
			octantChildren[i]->SetDimensions();
		}
	}
}

//kill everything but root
void Octree::DeleteChildren(void)
{
	//skip all the leaves
	if (IsOutside()) return;
	else
	{
		//delete children
		for (uint i = 0; i < childrenNumber; ++i)
		{
			octantChildren[i]->DeleteChildren();
			SafeDelete(octantChildren[i]);
		}
	}
}

void Octree::Release(void)
{
	if (this == octantRoot) DeleteChildren();
}

//destructor
Octree::~Octree(void)
{
	Release();
}