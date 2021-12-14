
#include "../headers/AbsRotationTrackerNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <cmath>

MTypeId AbsRotationTracker::id(0x001226D2);
MObject AbsRotationTracker::axis;
MObject AbsRotationTracker::reference;
MObject AbsRotationTracker::tracker;
MObject AbsRotationTracker::outputA;
MObject AbsRotationTracker::outputB;

AbsRotationTracker::AbsRotationTracker() {}

AbsRotationTracker::~AbsRotationTracker() {}

void* AbsRotationTracker::creator() {
	return new AbsRotationTracker();
}

MStatus AbsRotationTracker::initialize() {
	// attributes are writable by default
	// attributes are storable by default
	// attributes are readable by default
	// attributes not keyable by default
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnMatrixAttribute fnMat;
	MFnUnitAttribute fnUnit;
	MFnEnumAttribute fnEnum;

	// Inputs
	axis = fnEnum.create("axis", "axis", 0, &stat);
	CHECK_MSTATUS(stat);
	fnEnum.addField("X", 0);
	fnEnum.addField("Y", 1);
	fnEnum.addField("Z", 2);
	fnEnum.setKeyable(true);
	stat = AbsRotationTracker::addAttribute(axis);
	CHECK_MSTATUS(stat);

	reference = fnMat.create("reference", "ref", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = AbsRotationTracker::addAttribute(reference);
	CHECK_MSTATUS(stat);

	tracker = fnMat.create("tracker", "trk", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = AbsRotationTracker::addAttribute(tracker);
	CHECK_MSTATUS(stat);

	// Outputs
	outputA = fnNum.create("outputA", "outa", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = AbsRotationTracker::addAttribute(outputA);
	CHECK_MSTATUS(stat);

	outputB = fnNum.create("outputB", "outb", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = AbsRotationTracker::addAttribute(outputB);
	CHECK_MSTATUS(stat);

	// Affects
	AbsRotationTracker::attributeAffects(axis, outputA);
	AbsRotationTracker::attributeAffects(reference, outputA);
	AbsRotationTracker::attributeAffects(tracker, outputA);

	AbsRotationTracker::attributeAffects(axis, outputB);
	AbsRotationTracker::attributeAffects(reference, outputB);
	AbsRotationTracker::attributeAffects(tracker, outputB);

	return MS::kSuccess;
}

MStatus AbsRotationTracker::compute( const MPlug& plug, MDataBlock& data ) {

	// Inputs
	short axis = data.inputValue(AbsRotationTracker::axis).asShort();
	MTransformationMatrix ref_tfm = data.inputValue(AbsRotationTracker::reference).asMatrix();
	MTransformationMatrix trk_tfm = data.inputValue(AbsRotationTracker::tracker).asMatrix();

	// Compute
	MVector dir;
	double a=0.0, b=0.0;
	if (axis == 0){
		dir = {1.0, 0.0 ,0.0};
		dir *= trk_tfm.asMatrix();
		dir *= ref_tfm.asMatrixInverse();
		a = std::asin(dir.y);
		b = std::asin(dir.z);
	}
	else if (axis == 1){
		dir = {0.0, 1.0, 0.0};
		dir *= trk_tfm.asMatrix();
		dir *= ref_tfm.asMatrixInverse();
		a = std::asin(dir.z);
		b = std::asin(dir.x);
	}
	else if (axis == 2){
		dir = {0.0, 0.0, 1.0};
		dir *= trk_tfm.asMatrix();
		dir *= ref_tfm.asMatrixInverse();
		a = std::asin(dir.x);
		b = std::asin(dir.y);
	}

	// Outputs
	data.outputValue(AbsRotationTracker::outputA).setDouble(a);
	data.outputValue(AbsRotationTracker::outputB).setDouble(b);

	// Set plugs to clean
	data.setClean(AbsRotationTracker::outputA);
	data.setClean(AbsRotationTracker::outputB);

	return MS::kSuccess;
}