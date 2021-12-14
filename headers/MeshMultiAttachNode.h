#pragma once

#include <vector>

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MIntArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MVectorArray.h>
#include <maya/MTypeId.h>



class MeshMultiAttach : public MPxNode {
public:
	MeshMultiAttach();
	virtual	~MeshMultiAttach();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	static void *creator();
	static MStatus initialize();
	virtual MStatus	compute(const MPlug& plug, MDataBlock& data);

	MMatrix MeshMultiAttach::computeVertex(MItMeshVertex &iter, int &index, bool &orient, MMatrix &matrix);
	MMatrix MeshMultiAttach::computeEdge(MItMeshEdge &iter, MItMeshVertex &vertexIter, int &index, bool &orient, MMatrix &matrix);
	MMatrix MeshMultiAttach::computePolygon(MItMeshPolygon &iter, int &index, bool &orient, MMatrix &matrix);
	MPoint MeshMultiAttach::averageVector(MPointArray &points);
	MVector MeshMultiAttach::averageVector(MVectorArray &vectors);

public:
	static MTypeId id;

	// Input
	static MObject mesh;
	static MObject meshMatrix;
	static MObject attach;
	static MObject component;
	static MObject index;
	static MObject orient;
	static MObject parentInverse;

	// Output
	static MObject translate;
	static MObject translateX;
	static MObject translateY;
	static MObject translateZ;
	static MObject rotateX;
	static MObject rotateY;
	static MObject rotateZ;
	static MObject rotate;
	static MObject output;

	MIntArray indexes;
	MIntArray orients;
	MMatrixArray matrices;
	MMatrixArray transform;
};

