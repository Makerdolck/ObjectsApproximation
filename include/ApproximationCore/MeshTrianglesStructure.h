#pragma once

#ifndef __MESH_TRIANGLES_STRUCTURE_H__
#define __MESH_TRIANGLES_STRUCTURE_H__

#include "VectorGeometric.h"


struct MeshTriangles
{
	std::vector<PointGeometric>		points;
	std::vector<VectorGeometric>	vectorsNormal;
};



#endif /* __MESH_TRIANGLES_STRUCTURE_H__ */ 