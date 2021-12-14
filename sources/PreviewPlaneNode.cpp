
#include "../headers/_Utils.h"
#include "../headers/PreviewPlaneNode.h"

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

#include <cmath>
#include <cfloat>


MTypeId PreviewPlane::id(0x001226CD);
MObject PreviewPlane::tfmA;
MObject PreviewPlane::tfmB;
MObject PreviewPlane::tfmC;
MObject PreviewPlane::parentInverse;
MObject PreviewPlane::offset;
MObject PreviewPlane::worldMatrix;
MObject PreviewPlane::translateX;
MObject PreviewPlane::translateY;
MObject PreviewPlane::translateZ;
MObject PreviewPlane::translate;
MObject PreviewPlane::rotateX;
MObject PreviewPlane::rotateY;
MObject PreviewPlane::rotateZ;
MObject PreviewPlane::rotate;
MObject PreviewPlane::scaleX;
MObject PreviewPlane::scaleY;
MObject PreviewPlane::scaleZ;
MObject PreviewPlane::scale;


PreviewPlane::PreviewPlane() {}
PreviewPlane::~PreviewPlane() {}


void* PreviewPlane::creator() {
	return new PreviewPlane();
}


MStatus PreviewPlane::initialize() {
	// attributes are writable by default
	// attributes are storable by default
	// attributes are readable by default
	// attributes not keyable by default
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnMatrixAttribute fnMat;
	MFnUnitAttribute fnUnit;

	tfmA = fnMat.create("tfmA", "tfma", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PreviewPlane::addAttribute(tfmA);
	CHECK_MSTATUS(stat);

	tfmB = fnMat.create("tfmB", "tfmb", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PreviewPlane::addAttribute(tfmB);
	CHECK_MSTATUS(stat);

	tfmC = fnMat.create("tfmC", "tfmc", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PreviewPlane::addAttribute(tfmC);
	CHECK_MSTATUS(stat);

	parentInverse = fnMat.create("parentInverse", "pari", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	stat = PreviewPlane::addAttribute(parentInverse);
	CHECK_MSTATUS(stat);

	offset = fnNum.create("offset", "off", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = PreviewPlane::addAttribute(offset);
	CHECK_MSTATUS(stat);

	translateX = fnUnit.create("translateX", "tx", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	translateY = fnUnit.create("translateY", "ty", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	translateZ = fnUnit.create("translateZ", "tz", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	translate = fnNum.create("translate", "tran", translateX, translateY, translateZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);
	stat = PreviewPlane::addAttribute(translate);
	CHECK_MSTATUS(stat);

	rotateX = fnUnit.create("rotateX", "rx", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	rotateY = fnUnit.create("rotateY", "ry", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	rotateZ = fnUnit.create("rotateZ", "rz", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	rotate = fnNum.create("rotate", "rot", rotateX, rotateY, rotateZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);
	stat = PreviewPlane::addAttribute(rotate);
	CHECK_MSTATUS(stat);

	scaleX = fnNum.create("scaleX", "sx", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	scaleY = fnNum.create("scaleY", "sy", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	scaleZ = fnNum.create("scaleZ", "sz", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	scale = fnNum.create("scale", "scl", scaleX, scaleY, scaleZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setWritable(false);
	fnNum.setStorable(true);
	stat = PreviewPlane::addAttribute(scale);
	CHECK_MSTATUS(stat);

	worldMatrix = fnMat.create("worldMatrix", "wmat", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setWritable(false);
	fnMat.setStorable(true);
	stat = PreviewPlane::addAttribute(worldMatrix);
	CHECK_MSTATUS(stat);

	// Only make the attributes affect worldMatrix, as we'll set all the output plugs in one go, and avoid redundant calculations
	PreviewPlane::attributeAffects(tfmA, worldMatrix);
	PreviewPlane::attributeAffects(tfmB, worldMatrix);
	PreviewPlane::attributeAffects(tfmC, worldMatrix);
	PreviewPlane::attributeAffects(parentInverse, worldMatrix);
	PreviewPlane::attributeAffects(offset, worldMatrix);

	PreviewPlane::attributeAffects(tfmA, translate);
	PreviewPlane::attributeAffects(tfmB, translate);
	PreviewPlane::attributeAffects(tfmC, translate);
	PreviewPlane::attributeAffects(parentInverse, translate);
	PreviewPlane::attributeAffects(offset, translate);

	PreviewPlane::attributeAffects(tfmA, rotate);
	PreviewPlane::attributeAffects(tfmB, rotate);
	PreviewPlane::attributeAffects(tfmC, rotate);
	PreviewPlane::attributeAffects(parentInverse, rotate);
	PreviewPlane::attributeAffects(offset, rotate);

	PreviewPlane::attributeAffects(tfmA, scale);
	PreviewPlane::attributeAffects(tfmB, scale);
	PreviewPlane::attributeAffects(tfmC, scale);
	PreviewPlane::attributeAffects(parentInverse, scale);
	PreviewPlane::attributeAffects(offset, scale);

	return MS::kSuccess;
}


MStatus PreviewPlane::compute(const MPlug& plug, MDataBlock& data) {

	// Dont care what plug were processing, cause we're setting all the plug values at the end,
	// and setting them all to 'clean' afterwards

	MStatus stat;
	MTransformationMatrix mA = data.inputValue(PreviewPlane::tfmA).asMatrix();
	MTransformationMatrix mB = data.inputValue(PreviewPlane::tfmB).asMatrix();
	MTransformationMatrix mC = data.inputValue(PreviewPlane::tfmC).asMatrix();
	MMatrix pInverse = data.inputValue(PreviewPlane::parentInverse).asMatrix();
	double  offset = data.inputValue(PreviewPlane::offset).asDouble();

	MVector posA, posB, posC, xAxis, yAxis, zAxis;

	posA = mA.translation(MSpace::kWorld);
	posB = mB.translation(MSpace::kWorld);
	posC = mC.translation(MSpace::kWorld);

	yAxis = posA - posB;
	yAxis.normalize();
	zAxis = posC - posA;
	zAxis.normalize();
	xAxis = zAxis ^ yAxis;

	MQuaternion rot = quatFromMatrix(mA.asMatrix());
	if (xAxis.length() < (DBL_EPSILON - 1.0) ){
		xAxis = MVector(0, 0, -1);
		xAxis *= rot.asMatrix();
	}

	xAxis.normalize();
	zAxis = posC - posB;
	double angle = yAxis.angle(zAxis);
	rot.setAxisAngle(xAxis, angle * (-0.5));
	zAxis = yAxis.rotateBy(rot);
	zAxis *= -1;
	yAxis = zAxis ^ xAxis;
	yAxis.normalize();

	double mdata[4][4] {xAxis.x, xAxis.y, xAxis.z, 0.0,
				   		yAxis.x, yAxis.y, yAxis.z, 0.0,
				   		zAxis.x, zAxis.y, zAxis.z, 0.0,
						0.0, 0.0, 0.0, 1.0};

	MTransformationMatrix outMatrix { MMatrix(mdata) };
	MVector offsetZ { 0.0, 0.0, offset };
	offsetZ *= outMatrix.asMatrix();

	outMatrix.setTranslation((posB + offsetZ), MSpace::kWorld);
	
	outMatrix = outMatrix.asMatrix() * pInverse;

	MVector translate = outMatrix.getTranslation(MSpace::kWorld);
	MEulerRotation rotate = outMatrix.eulerRotation();
	double scale[3];
	outMatrix.getScale(scale, MSpace::kWorld );

	data.outputValue(PreviewPlane::worldMatrix).setMMatrix( outMatrix.asMatrix() );
	data.outputValue(PreviewPlane::translate).setMVector(translate);
	data.outputValue(PreviewPlane::rotate).set3Double(rotate[0], rotate[1], rotate[2]);
	data.outputValue(PreviewPlane::scale).set3Double(scale[0], scale[1], scale[2]);

	data.setClean(PreviewPlane::worldMatrix);
	data.setClean(PreviewPlane::translate);
	data.setClean(PreviewPlane::rotate);
	data.setClean(PreviewPlane::scale);

	return MS::kSuccess;
}

















