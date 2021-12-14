
#include "../headers/_Utils.h"
#include "../headers/SpinePointAtNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MGlobal.h>

#include <cfloat>
#include <cmath>


MTypeId SpinePointAt::id(0x001226CF);
MObject SpinePointAt::tfmA;
MObject SpinePointAt::tfmB;
MObject SpinePointAt::parentInverse;
MObject SpinePointAt::axis;
MObject SpinePointAt::alg;
MObject SpinePointAt::blend;
MObject SpinePointAt::pointAtX;
MObject SpinePointAt::pointAtY;
MObject SpinePointAt::pointAtZ;
MObject SpinePointAt::pointAt;

SpinePointAt::SpinePointAt() {}

SpinePointAt::~SpinePointAt() {}

void* SpinePointAt::creator() {
	return new SpinePointAt();
}

MStatus SpinePointAt::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnMatrixAttribute fnMat;
	MFnUnitAttribute fnUnit;
	MFnEnumAttribute fnEnum;

	tfmA = fnMat.create("tfmA", "tfma", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = SpinePointAt::addAttribute(tfmA);
	CHECK_MSTATUS(stat);

	tfmB = fnMat.create("tfmB", "tfmb", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = SpinePointAt::addAttribute(tfmB);
	CHECK_MSTATUS(stat);

	axis = fnEnum.create("axis", "a", 2);
	fnEnum.addField("X", 0);
	fnEnum.addField("Y", 1);
	fnEnum.addField("Z", 2);
	fnEnum.addField("-X", 3);
	fnEnum.addField("-Y", 4);
	fnEnum.addField("-Z", 5);
	fnEnum.setKeyable(true);
	stat = SpinePointAt::addAttribute(axis);
	CHECK_MSTATUS(stat);
	
	alg = fnEnum.create("alg", "alg", 0);
	fnEnum.addField("spinePointAt", 0);
	fnEnum.addField("mayaSlerp", 1);
	fnEnum.setKeyable(true);
	stat = SpinePointAt::addAttribute(alg);
	CHECK_MSTATUS(stat);
	
	blend = fnNum.create("blend", "b", MFnNumericData::kDouble, 0.5, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = SpinePointAt::addAttribute(blend);
	CHECK_MSTATUS(stat);

	pointAtX = fnNum.create("pointAtX", "pax", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	pointAtY = fnNum.create("pointAtY", "pay", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	pointAtZ = fnNum.create("pointAtZ", "paz", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	pointAt = fnNum.create("pointAt", "pa", pointAtX, pointAtY, pointAtZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);
	SpinePointAt::addAttribute(pointAt);

	SpinePointAt::attributeAffects(tfmA, pointAt);
	SpinePointAt::attributeAffects(tfmB, pointAt);
	SpinePointAt::attributeAffects(parentInverse, pointAt);
	SpinePointAt::attributeAffects(axis, pointAt);
	SpinePointAt::attributeAffects(blend, pointAt);
	SpinePointAt::attributeAffects(alg, pointAt);
	return MS::kSuccess;
}


MStatus SpinePointAt::compute( const MPlug& plug, MDataBlock& data ) {

	if (plug != pointAt)
		return MS::kSuccess;

	const short axis = data.inputValue(SpinePointAt::axis).asShort();
	const short myAlg = data.inputValue(SpinePointAt::alg).asShort();
	const double blend = data.inputValue(SpinePointAt::blend).asDouble();

	MTransformationMatrix tfmA = MTransformationMatrix(data.inputValue(SpinePointAt::tfmA).asMatrix());
	MTransformationMatrix tfmB = MTransformationMatrix(data.inputValue(SpinePointAt::tfmB).asMatrix());
	
	double noScale[3] = {1,1,1};
	tfmA.setScale(noScale, MSpace::kWorld);
	tfmB.setScale(noScale, MSpace::kWorld);

	MQuaternion quatA = quatFromMatrix(tfmA.asMatrix());
	MQuaternion quatB = quatFromMatrix(tfmB.asMatrix());
	MQuaternion quatC;

	if (myAlg == 0) {
		// Tyler's update
		quatC = slerp2(quatA, quatB, blend);
	}
	else if (myAlg == 1) {
		// the slerp included in Maya
		quatC = slerp(quatA, quatB, blend);
	}

	MVector vOut;
	if (axis == 0)
		vOut = MVector(1, 0, 0);
	else if (axis == 1)
		vOut = MVector(0, 1, 0);
	else if (axis == 2)
		vOut = MVector(0, 0, 1);
	else if (axis == 3)
		vOut = MVector(-1, 0, 0);
	else if (axis == 4)
		vOut = MVector(0, -1, 0);
	else if (axis == 5)
		vOut = MVector(0, 0, -1);

	vOut = vOut.rotateBy(quatC);

	data.outputValue(SpinePointAt::pointAt).setMVector(vOut);
	data.setClean(plug);

	return MS::kSuccess;
}

MQuaternion SpinePointAt::slerp2(const MQuaternion &inStart, const MQuaternion &inEnd, double blend) {
	double qdot = quaternionDot(inStart, inEnd);
	double scl1, scl2;
	double adot = abs(qdot);
	// because of quaternion double-cover, we can just check if the
	// dot is either +1 or -1
	if (adot > 1 - 1e-5) {
		if (adot > 1 - 1e-12) {
			// If we're *REALLY* close, just bail
			MQuaternion out(inStart);
			return out;
		}
		// If we're close, just NLerp
		MQuaternion out = ((1.0 - blend) * inStart + (blend * inEnd));
		out.normalizeIt();
		return out;
	}

	double angle = acos(qdot);
	double factor = 1.0 / sin(angle);

	scl1 = sin((1.0 - blend) * angle) * factor;
	scl2 = sin(blend * angle) * factor;

	MQuaternion st = MQuaternion(inStart);
	MQuaternion en = MQuaternion(inEnd);
	MQuaternion out = st.scaleIt(scl1) + en.scaleIt(scl2);
	return out;
}

