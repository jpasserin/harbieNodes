#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MVector.h>
#include <maya/MTypeId.h>

 
class RotationTracker : public MPxNode {
public:
	RotationTracker();
	virtual	~RotationTracker();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; };
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute( const MPlug &plug, MDataBlock &data );

public:
	static MTypeId id;
	static MObject reference;
	static MObject tracker;
	static MObject rest;
	static MObject restX;
	static MObject restY;
	static MObject restZ;
	static MObject output;
	static MObject outputX;
	static MObject outputY;
	static MObject outputZ;
};

