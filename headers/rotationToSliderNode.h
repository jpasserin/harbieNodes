#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MTypeId.h>

 
class RotationToSlider : public MPxNode {
public:
	RotationToSlider();
	virtual ~RotationToSlider();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; };
	static void* creator();
	static MStatus initialize();
	virtual MStatus	compute(const MPlug& plug, MDataBlock& data) override;

public:
    static MTypeId id;
	static MObject angle;
	static MObject rotMin;
	static MObject rotMax;
	static MObject sliderMin;
	static MObject sliderMax;
	static MObject output;
};
