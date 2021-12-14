#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h>
#include <maya/MQuaternion.h>

class SpinePointAt : public MPxNode {
public:
	SpinePointAt();
	virtual	~SpinePointAt();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	static void *creator();
	static MStatus initialize();
	virtual MStatus	compute(const MPlug& plug, MDataBlock& data);
	MQuaternion slerp2(const MQuaternion &inStart, const MQuaternion &inEnd, double blend);

public:
	static MTypeId id;
	static MObject tfmA;
	static MObject tfmB;
	static MObject parentInverse;
	static MObject axis;
	static MObject alg;
	static MObject blend;
	static MObject pointAtX;
	static MObject pointAtY;
	static MObject pointAtZ;
	static MObject pointAt;
};

