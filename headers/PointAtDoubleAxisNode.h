#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

 
class PointAtDoubleAxis : public MPxNode {
public:
	PointAtDoubleAxis();
	virtual ~PointAtDoubleAxis();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute( const MPlug &plug, MDataBlock &data );
public:
	static MTypeId id;
	static MObject ref;
	static MObject trk;
	static MObject axis;
	static MObject out;
};

