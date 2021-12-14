#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

 
class PreviewPlane : public MPxNode {
public:
	PreviewPlane();
	virtual	~PreviewPlane();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	static void* creator();
	static MStatus initialize();
	virtual MStatus	compute( const MPlug &plug, MDataBlock &data );

public:
	static MTypeId id;

	// Inputs
	static MObject tfmA;
	static MObject tfmB;
	static MObject tfmC;
	static MObject parentInverse;
	static MObject offset;

	// Outputs
	static MObject worldMatrix;

	static MObject translateX;
	static MObject translateY;
	static MObject translateZ;
	static MObject translate;

	static MObject rotateX;
	static MObject rotateY;
	static MObject rotateZ;
	static MObject rotate;

	static MObject scaleX;
	static MObject scaleY;
	static MObject scaleZ;
	static MObject scale;
};

