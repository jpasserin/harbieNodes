
#include "../headers/_Utils.h"
#include "../headers/PointAtBlendedAxisNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MFnEnumAttribute.h>

#include <cmath>




MTypeId PointAtBlendedAxis::id(0x001226D8);
MObject PointAtBlendedAxis::mA;
MObject PointAtBlendedAxis::mB;
MObject PointAtBlendedAxis::blend;
MObject PointAtBlendedAxis::axis;
MObject PointAtBlendedAxis::out;


PointAtBlendedAxis::PointAtBlendedAxis() {}

PointAtBlendedAxis::~PointAtBlendedAxis() {}

void* PointAtBlendedAxis::creator() {
	return new PointAtBlendedAxis();
}

MStatus PointAtBlendedAxis::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnMatrixAttribute fnMat;
	MFnUnitAttribute fnUnit;
	MFnEnumAttribute fnEnum;

	mA = fnMat.create("matrixA", "mA", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PointAtBlendedAxis::addAttribute(mA);
	CHECK_MSTATUS(stat);

	mB = fnMat.create("matrixB", "mB", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PointAtBlendedAxis::addAttribute(mB);
	CHECK_MSTATUS(stat);

	blend = fnNum.create("blend", "b", MFnNumericData::kDouble, 0.5, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
    fnNum.setMin(0);
    fnNum.setMax(1);
	stat = PointAtBlendedAxis::addAttribute(blend);
	CHECK_MSTATUS(stat);

	axis = fnEnum.create("axis", "a", 2);
	fnEnum.addField("X", 0);
	fnEnum.addField("Y", 1);
	fnEnum.addField("Z", 2);
	fnEnum.addField("-X", 3);
	fnEnum.addField("-Y", 4);
	fnEnum.addField("-Z", 5);
	stat = PointAtBlendedAxis::addAttribute(axis);
	CHECK_MSTATUS(stat);

	//outputs
	out = fnNum.createPoint("out", "out", &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);
	PointAtBlendedAxis::addAttribute(out);

	//affects
	MObject inAttrs[] {mA, mB, axis, blend};
	MObject outAtrs[] {out};

	for (const auto inAttr : inAttrs)
		for (const auto outAttr : outAtrs)
			PointAtBlendedAxis::attributeAffects(inAttr, outAttr);

	return MS::kSuccess;
}

MStatus PointAtBlendedAxis::compute( const MPlug &plug, MDataBlock &data ) {

	if (plug != out)
		return MS::kUnknownParameter;

	//inputs
	MTransformationMatrix mA = MTransformationMatrix(data.inputValue(PointAtBlendedAxis::mA).asMatrix());
	MTransformationMatrix mB = MTransformationMatrix(data.inputValue(PointAtBlendedAxis::mB).asMatrix());
	
	double blend = data.inputValue(PointAtBlendedAxis::blend).asDouble();
	short axis = data.inputValue(PointAtBlendedAxis::axis).asShort();

	MVector direction;
	if (axis == 0)
		direction = MVector(1, 0, 0);
	else if (axis == 1)
		direction = MVector(0, 1, 0);
	else if (axis == 2)
		direction = MVector(0, 0, 1);
	else if (axis == 3)
		direction = MVector(-1, 0, 0);
	else if (axis == 4)
		direction = MVector(0, -1, 0);
	else if (axis == 5)
		direction = MVector(0, 0, -1);

	// MQuaternion q = quaternionSlerp(mA.rotation(), mB.rotation(), blend);
	MQuaternion q = slerp(mA.rotation(), mB.rotation(), blend);
	direction = direction.rotateBy(q);

	// This doesn't work
    // h.setMVector( vOut );
    // h.set3Double( vOut );
	data.outputValue(PointAtBlendedAxis::out).set3Float(direction.x, direction.y, direction.z);
	data.setClean(PointAtBlendedAxis::out);

	return MS::kSuccess;
}






















