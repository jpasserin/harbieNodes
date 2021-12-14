#pragma once

#include <vector>

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MDoubleArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MTypeId.h>



class SurfaceMultiAttach : public MPxNode {
public:
	SurfaceMultiAttach();
	virtual	~SurfaceMultiAttach();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	static void *creator();
	static MStatus initialize();
	virtual MStatus	compute(const MPlug& plug, MDataBlock& data);

	MMatrixArray SurfaceMultiAttach::getTransform(MFnNurbsSurface &surface, MDoubleArray &samples, MDoubleArray &uParams, MDoubleArray &vParams,
											int &tangent, bool isParametric, MMatrix &matrix);


public:
	static MTypeId id;

	// Input
	static MObject surface;
	static MObject sampling;
	static MObject u;
	static MObject v;
	static MObject tangent;
	static MObject surfaceMatrix;
	static MObject parentInverse;
	static MObject attach;
	static MObject isLoop;
	static MObject length;
	static MObject slide;
	static MObject start;
	static MObject end;
	static MObject reverse;

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
};

