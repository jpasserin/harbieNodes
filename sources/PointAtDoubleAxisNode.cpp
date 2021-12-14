
#include "../headers/PointAtDoubleAxisNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MGlobal.h>
#include <cmath>



MTypeId PointAtDoubleAxis::id(0x001226D6);
MObject PointAtDoubleAxis::ref;
MObject PointAtDoubleAxis::trk;
MObject PointAtDoubleAxis::axis;
MObject PointAtDoubleAxis::out;

PointAtDoubleAxis::PointAtDoubleAxis() {}

PointAtDoubleAxis::~PointAtDoubleAxis() {}

void* PointAtDoubleAxis::creator() {
	return new PointAtDoubleAxis();
}

MStatus PointAtDoubleAxis::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnMatrixAttribute fnMat;
	MFnEnumAttribute fnEnum;

	ref = fnMat.create("reference", "ref", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PointAtDoubleAxis::addAttribute(ref);
	CHECK_MSTATUS(stat);

	trk = fnMat.create("tracker", "trk", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PointAtDoubleAxis::addAttribute(trk);
	CHECK_MSTATUS(stat);

	axis = fnEnum.create("axis", "a", 2);
	fnEnum.addField("Z", 0);
	fnEnum.addField("Y", 1);
	stat = PointAtDoubleAxis::addAttribute(axis);
	CHECK_MSTATUS(stat);

	// Outputs
	out = fnNum.createPoint("out", "out", &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);
	PointAtDoubleAxis::addAttribute(out);

	// Affects
	MObject inAttrs[] = {ref, trk, axis};
	MObject outAttrs[] = {out};

	for (const auto inAttr : inAttrs)
		for (const auto outAttr : outAttrs)
			PointAtDoubleAxis::attributeAffects(inAttr, outAttr);


	return MS::kSuccess;
}

MStatus PointAtDoubleAxis::compute( const MPlug &plug, MDataBlock &data ) {

	if (plug != out)
		return MS::kUnknownParameter;

	// inputs
	MMatrix ref = data.inputValue(PointAtDoubleAxis::ref).asMatrix();
	MMatrix trk = data.inputValue(PointAtDoubleAxis::trk).asMatrix();
	short axis = data.inputValue(PointAtDoubleAxis::axis).asShort();

	// extra data
    MVector ref_x = {ref(0,0), ref(0,1), ref(0,2)};
    MVector ref_y = {ref(1,0), ref(1,1), ref(1,2)};
    MVector ref_z = {ref(2,0), ref(2,1), ref(2,2)};

    MVector trk_x = {trk(0,0), trk(0,1), trk(0,2)};
    MVector trk_y = {trk(1,0), trk(1,1), trk(1,2)};
    MVector trk_z = {trk(2,0), trk(2,1), trk(2,2)};

    MVector trk_pri, ref_pri;

	if (axis == 0){ // Z
		trk_pri = trk_z;
		ref_pri = ref_z;
	}
	else if (axis == 1){// Y
		trk_pri = ref_y;
		ref_pri = ref_y;
	}

	double angle = ref_x.angle(trk_pri);
	double blend = std::abs(std::sin(angle - M_PI*.5));

    MVector alt;
	if (axis == 0)
		alt = ref_x ^ trk_y;
	else if (axis == 1)
	    alt = trk_z ^ ref_x;

	if ((ref_pri.angle(trk_pri) < 0) && (ref_x.angle(trk_x) < 0))
	    trk_pri *= -1;

	MVector direction = (trk_pri * (1.0 - blend)) + (alt * blend);
	direction.normalize();

	// This doesn't work
    // h.setMVector( vOut );
    // h.set3Double( vOut );
	data.outputValue(PointAtDoubleAxis::out).set3Float(direction.x, direction.y, direction.z);
	data.setClean(PointAtDoubleAxis::out);

	return MS::kSuccess;
}
