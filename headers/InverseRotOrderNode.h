#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h> 

 
class InverseRotOrder : public MPxNode {
public:
	InverseRotOrder();
	virtual ~InverseRotOrder();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; };
	static void* creator();
	static MStatus initialize();
	virtual MStatus	compute( const MPlug& plug, MDataBlock& data );
public:
	static MTypeId id;
	static MObject rotOrder;
	static MObject output;
};
