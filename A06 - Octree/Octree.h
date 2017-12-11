#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{
	//System Class
	class Octree
	{
		//variables
		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;
		std::vector<uint> entityList; //list of entities under this octant

		static uint leavesAmount; //amount of leaves
		static uint octantAmount; //amount of octants
		static uint subMax; //maximum amount of subdivisions
		static uint entityAmount; //amount of entities

		Octree* octantParent = nullptr; //stores parent of current octant
		Octree* octantChildren[8]; //stores children of current octant
		Octree* octantRoot = nullptr; //root octant
		vector3 octantSize = ZERO_V3; //size of octant
		vector3 octantCenter = ZERO_V3; //stores center of octant
		vector3 octantMinimum = ZERO_V3; //stores minimum vector of octant
		vector3 octantMaximum = ZERO_V3; //stores max vector of octant
		uint octantID = 0; //stores current ID of octant
		uint currentLevel = 0; //stores current level of octant
		uint childrenNumber = 0; //number of children of octant
		uint currentEntityAmount = 0; //numer of entities currently in existence
		
		std::vector<Octree*> rootChildren; //list of nodes that contain objects

	public:
		bool wireFrameSwitch = true; //used to switch between wireframe and no wireframe

		//constructors
		Octree(uint p_MaxLevel = 2, uint p_EntityCount = 5);
		Octree(vector3 p_Center, vector3 p_Size);
		Octree(Octree const& other);
		
		//returns true if two objects are colliding
		bool IsColliding(uint a_uRBIndex);

		//delete all children
		void DeleteChildren(void);

		//gets a parent in the octree
		Octree* GetParent(void);

		//set octree dimensions
		void SetDimensions(void);

		//gets number of octants in the octree
		uint GetOctantCount(void);

		//is this an outside octant?
		bool IsOutside(void);

		//gets center of octant
		vector3 GetGlobalCenter(void);

		//gets octant minimum
		vector3 GetGlobalMin(void);

		//gets octant maximum
		vector3 GetGlobalMax(void);

		//clears all entities from octree
		void ClearEntityList(void);

		//displays a specific octant
		void Display(uint a_uIndex, vector3 a_v3Color = C_RED);

		//gets number of leaves in the octree
		uint GetLeafCount(void);

		//gets size of octant
		vector3 GetSize(void);

		//does the octant contain more than this many entities?
		bool HasMore(uint a_nEntities);

		//displays current octant
		void CurrDisplay(vector3 a_v3Color = C_RED);

		//displays all octants
		void TotalDisplay(vector3 a_v3Color = C_RED);

		//subdivides the octree
		void Subdivide(void);

		//gets a child in the octree
		Octree* GetChild(uint a_nChild);

		//destructor
		~Octree(void);

	private:
		void Release(void);
		void Init(void);
	};
}
#endif