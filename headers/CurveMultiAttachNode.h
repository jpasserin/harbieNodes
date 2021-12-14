#pragma once

#include <vector>

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDoubleArray.h>
#include <maya/MMatrixArray.h>
#include <maya/MTypeId.h>



class CurveMultiAttach : public MPxNode {
public:
	CurveMultiAttach();
	virtual	~CurveMultiAttach();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	static void *creator();
	static MStatus initialize();
	virtual MStatus	compute(const MPlug& plug, MDataBlock& data);

	MMatrixArray getTransform(MFnNurbsCurve &curve, MDoubleArray &params, MVector &upVector, 
							bool isParametric, MMatrix &matrix);

public:
	static MTypeId id;

	// Input
	static MObject curve;
	static MObject u;
	static MObject curveMatrix;
	static MObject parentInverse;
	static MObject upVector;
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

