
#include "../headers/_Utils.h"
#include "../headers/rotationToSliderNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>


// Static Members
MTypeId RotationToSlider::id(0x001226CB);
MObject RotationToSlider::angle;
MObject RotationToSlider::rotMin;
MObject RotationToSlider::rotMax;
MObject RotationToSlider::sliderMin;
MObject RotationToSlider::sliderMax;
MObject RotationToSlider::output;

//---------------------------------------------------

RotationToSlider::RotationToSlider(){}

RotationToSlider::~RotationToSlider(){}

void* RotationToSlider::creator() {
	return new RotationToSlider();
}

MStatus RotationToSlider::initialize() {
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnUnitAttribute fnUnit;

	angle = fnNum.create("angle", "a", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = RotationToSlider::addAttribute(angle);
	CHECK_MSTATUS(stat);

	rotMin = fnNum.create("rotMin", "rmin", MFnNumericData::kDouble, -90.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	fnNum.setMin(-360);
	fnNum.setMax(-0.0001);
	stat = RotationToSlider::addAttribute(rotMin);
	CHECK_MSTATUS(stat);

	rotMax = fnNum.create("rotMax", "rmax", MFnNumericData::kDouble, 90.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	fnNum.setMin(0.0001);
	fnNum.setMax(360);
	stat = RotationToSlider::addAttribute(rotMax);
	CHECK_MSTATUS(stat);

	sliderMin = fnUnit.create("sliderMin", "smin", MFnUnitAttribute::kDistance, -1.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setKeyable(true);
	stat = RotationToSlider::addAttribute(sliderMin);
	CHECK_MSTATUS(stat);

	sliderMax = fnUnit.create("sliderMax", "smax", MFnUnitAttribute::kDistance, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setKeyable(true);
	stat = RotationToSlider::addAttribute(sliderMax);
	CHECK_MSTATUS(stat);

	output = fnUnit.create("output", "out", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);
	stat = RotationToSlider::addAttribute(output);
	CHECK_MSTATUS(stat);

	RotationToSlider::attributeAffects(angle, output);
	RotationToSlider::attributeAffects(rotMin, output);
	RotationToSlider::attributeAffects(rotMax, output);
	RotationToSlider::attributeAffects(sliderMin, output);
	RotationToSlider::attributeAffects(sliderMax, output);

	return MS::kSuccess;
}





MStatus RotationToSlider::compute(const MPlug &plug, MDataBlock &data) {
	if (plug != RotationToSlider::output)
		return MS::kUnknownParameter;

	double angle = data.inputValue(RotationToSlider::angle).asDouble();

	//Process
	double outValue = 0.0;
	if (angle > 0.0){
		double rotMax = data.inputValue(RotationToSlider::rotMax).asDouble();
		double sliderMax = data.inputValue(RotationToSlider::sliderMax).asDouble();
		outValue = sliderMax * min(1.0, angle / rotMax);
	}
	else{
		double rotMin = data.inputValue(RotationToSlider::rotMin).asDouble();
		double sliderMin = data.inputValue(RotationToSlider::sliderMin).asDouble();
		outValue = sliderMin * min(1.0, angle / rotMin);
	}

	// division by zero is bad.
	if (std::isnan(outValue))
		outValue = 0.0;

	// set the output handle
	data.outputValue(RotationToSlider::output).setDouble(outValue);
	
	// set the plug clean
	data.setClean(plug);
	return MS::kSuccess;
}











