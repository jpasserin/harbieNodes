

#include "../headers/PoseConstraintNode.h"
#include <maya/MGlobal.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MDistance.h>
#include <maya/MFnDependencyNode.h>

#include <algorithm>
#include <vector>


MTypeId PoseConstraint::id(0x001226E2);
MObject PoseConstraint::input;
MObject PoseConstraint::blend;
MObject PoseConstraint::worldMatrix;
MObject PoseConstraint::parentInverseMatrix;
MObject PoseConstraint::localOffset;
MObject PoseConstraint::localOffsetTransform;
MObject PoseConstraint::localOffsetTranslate;
MObject PoseConstraint::localOffsetTranslateX;
MObject PoseConstraint::localOffsetTranslateY;
MObject PoseConstraint::localOffsetTranslateZ;
MObject PoseConstraint::localOffsetRotate;
MObject PoseConstraint::localOffsetRotateX;
MObject PoseConstraint::localOffsetRotateY;
MObject PoseConstraint::localOffsetRotateZ;
MObject PoseConstraint::localOffsetScale;
MObject PoseConstraint::localOffsetScaleX;
MObject PoseConstraint::localOffsetScaleY;
MObject PoseConstraint::localOffsetScaleZ;
MObject PoseConstraint::localOffsetShear;
MObject PoseConstraint::localOffsetShearX;
MObject PoseConstraint::localOffsetShearY;
MObject PoseConstraint::localOffsetShearZ;
MObject PoseConstraint::localOffsetRotateOrder;

MObject PoseConstraint::rotateOrder;
MObject PoseConstraint::offset;
MTransformationMatrix::RotationOrder PoseConstraint::rotationOrders[6] = {MTransformationMatrix::kXYZ,
                                                                          MTransformationMatrix::kYZX,
                                                                          MTransformationMatrix::kZXY,
                                                                          MTransformationMatrix::kXZY,
                                                                          MTransformationMatrix::kYXZ,
                                                                          MTransformationMatrix::kZYX};
MObject PoseConstraint::output;
MObject PoseConstraint::translate;
MObject PoseConstraint::translateX;
MObject PoseConstraint::translateY;
MObject PoseConstraint::translateZ;
MObject PoseConstraint::rotate;
MObject PoseConstraint::rotateX;
MObject PoseConstraint::rotateY;
MObject PoseConstraint::rotateZ;
MObject PoseConstraint::scale;
MObject PoseConstraint::scaleX;
MObject PoseConstraint::scaleY;
MObject PoseConstraint::scaleZ;
MObject PoseConstraint::shear;
MObject PoseConstraint::shearX;
MObject PoseConstraint::shearY;
MObject PoseConstraint::shearZ;


PoseConstraint::PoseConstraint() {}

PoseConstraint::~PoseConstraint() {}

void* PoseConstraint::creator() {
	return new PoseConstraint();
}

MStatus PoseConstraint::initialize() {
	// attributes are writable by default
	// attributes are storable by default
	// attributes are readable by default
	// attributes not keyable by default

	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnUnitAttribute fnUnit;
	MFnMatrixAttribute fnMat;
	MFnCompoundAttribute fnComp;
    MFnEnumAttribute fnEnum;

	// Input Attributes
	offset = fnMat.create("offset", "off", MFnMatrixAttribute::kDouble);
	fnMat.setKeyable(true);
	addAttribute(offset);

	worldMatrix = fnMat.create("worldMatrix", "wmat", MFnMatrixAttribute::kDouble, &stat);
    CHECK_MSTATUS(stat);
    fnMat.setKeyable(true);

    blend = fnNum.create("blend", "blnd", MFnNumericData::kDouble, 1.0, &stat);
    CHECK_MSTATUS(stat);
    fnNum.setMin(0.0);
    fnNum.setMax(1.0);
    fnNum.setKeyable(true);

	localOffsetTranslateX = fnUnit.create("localOffsetTranslateX", "lotrax", MFnUnitAttribute::kDistance, 0.0);
	fnUnit.setStorable(true);
	fnUnit.setKeyable(true);
	localOffsetTranslateY = fnUnit.create("localOffsetTranslateY", "lotray", MFnUnitAttribute::kDistance, 0.0);
	fnUnit.setStorable(true);
	fnUnit.setKeyable(true);
	localOffsetTranslateZ = fnUnit.create("localOffsetTranslateZ", "lotraz", MFnUnitAttribute::kDistance, 0.0);
	fnUnit.setStorable(true);
	fnUnit.setKeyable(true);
	localOffsetTranslate = fnNum.create("localOffsetTranslate", "lotra", localOffsetTranslateX, localOffsetTranslateY, localOffsetTranslateZ);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetRotateX = fnUnit.create("localOffsetRotateX", "lorotx", MFnUnitAttribute::kAngle, 0.0);
	fnUnit.setStorable(true);
	fnUnit.setKeyable(true);
	localOffsetRotateY = fnUnit.create("localOffsetRotateY", "loroty", MFnUnitAttribute::kAngle, 0.0);
	fnUnit.setStorable(true);
	fnUnit.setKeyable(true);
	localOffsetRotateZ = fnUnit.create("localOffsetRotateZ", "lorotz", MFnUnitAttribute::kAngle, 0.0);
	fnUnit.setStorable(true);
	fnUnit.setKeyable(true);
	localOffsetRotate = fnNum.create("localOffsetRotate", "lorot", localOffsetRotateX, localOffsetRotateY, localOffsetRotateZ);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetScaleX = fnNum.create("localOffsetScaleX", "losclx", MFnNumericData::kDouble, 1.0);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetScaleY = fnNum.create("localOffsetScaleY", "loscly", MFnNumericData::kDouble, 1.0);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetScaleZ = fnNum.create("localOffsetScaleZ", "losclz", MFnNumericData::kDouble, 1.0);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetScale = fnNum.create("localOffsetScale", "loscl", localOffsetScaleX, localOffsetScaleY, localOffsetScaleZ);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetShearX = fnNum.create("localOffsetShearX", "loshrx", MFnNumericData::kDouble, 0.0);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetShearY = fnNum.create("localOffsetShearY", "loshry", MFnNumericData::kDouble, 0.0);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetShearZ = fnNum.create("localOffsetShearZ", "loshrz", MFnNumericData::kDouble, 0.0);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);
	localOffsetShear = fnNum.create("localOffsetShear", "loshr", localOffsetShearX, localOffsetShearY, localOffsetShearZ);
	fnNum.setStorable(true);
	fnNum.setKeyable(true);

	localOffsetRotateOrder = fnEnum.create("localOffsetRotateOrder", "lorord", 0, &stat);
    fnEnum.setKeyable(true);
    fnEnum.addField("xyz", 0);
    fnEnum.addField("yzx", 1);
    fnEnum.addField("zxy", 2);
    fnEnum.addField("xzy", 3);
    fnEnum.addField("yxz", 4);
    fnEnum.addField("zyx", 5);

	localOffsetTransform = fnComp.create("localOffsetTransform", "lofft");
	fnComp.addChild(localOffsetTranslate);
	fnComp.addChild(localOffsetRotate);
	fnComp.addChild(localOffsetScale);
	fnComp.addChild(localOffsetShear);
    fnComp.addChild(localOffsetRotateOrder);
	CHECK_MSTATUS(stat);

	localOffset = fnMat.create("localOffset", "loff", MFnMatrixAttribute::kDouble);
	fnMat.setKeyable(true);

    input = fnComp.create("input", "in", &stat);
    CHECK_MSTATUS(stat);
    fnComp.addChild(worldMatrix);
    fnComp.addChild(blend);
    fnComp.addChild(localOffset);
    fnComp.addChild(localOffsetTransform);
    fnComp.setArray(true);
    fnComp.setUsesArrayDataBuilder(true);
    addAttribute(input);

    rotateOrder = fnEnum.create("rotateOrder", "rord", 1, &stat);
    fnEnum.setKeyable(true);
    fnEnum.addField("xyz", 0);
    fnEnum.addField("yzx", 1);
    fnEnum.addField("zxy", 2);
    fnEnum.addField("xzy", 3);
    fnEnum.addField("yxz", 4);
    fnEnum.addField("zyx", 5);
    addAttribute(rotateOrder);

    parentInverseMatrix = fnMat.create("parentInverseMatrix", "pinv", MFnMatrixAttribute::kDouble, &stat);
    CHECK_MSTATUS(stat);
    fnMat.setKeyable(true);
	addAttribute(parentInverseMatrix);

	// Output Attributes
	translateX = fnUnit.create("translateX", "trax", MFnUnitAttribute::kDistance, 0.0);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	translateY = fnUnit.create("translateY", "tray", MFnUnitAttribute::kDistance, 0.0);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	translateZ = fnUnit.create("translateZ", "traz", MFnUnitAttribute::kDistance, 0.0);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	translate = fnNum.create("translate", "tra", translateX, translateY, translateZ);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	rotateX = fnUnit.create("rotateX", "rotx", MFnUnitAttribute::kAngle, 0.0);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	rotateY = fnUnit.create("rotateY", "roty", MFnUnitAttribute::kAngle, 0.0);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	rotateZ = fnUnit.create("rotateZ", "rotz", MFnUnitAttribute::kAngle, 0.0);
	fnUnit.setWritable(false);
	fnUnit.setStorable(true);

	rotate = fnNum.create("rotate", "rot", rotateX, rotateY, rotateZ);
	fnNum.setWritable(false);

	scaleX = fnNum.create("scaleX", "sclx", MFnNumericData::kDouble, 1.0);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	scaleY = fnNum.create("scaleY", "scly", MFnNumericData::kDouble, 1.0);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	scaleZ = fnNum.create("scaleZ", "sclz", MFnNumericData::kDouble, 1.0);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	scale = fnNum.create("scale", "scl", scaleX, scaleY, scaleZ);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

    shearX = fnNum.create("shearX", "shrx", MFnNumericData::kDouble, 1.0);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	shearY = fnNum.create("shearY", "shry", MFnNumericData::kDouble, 1.0);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	shearZ = fnNum.create("shearZ", "shrz", MFnNumericData::kDouble, 1.0);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	shear = fnNum.create("shear", "shr", shearX, shearY, shearZ);
	fnNum.setWritable(false);
	fnNum.setStorable(true);

	output = fnComp.create("output", "out");
	fnComp.setWritable(false);
	fnComp.addChild(translate);
	fnComp.addChild(rotate);
	fnComp.addChild(scale);
	fnComp.addChild(shear);
	CHECK_MSTATUS(stat);
	addAttribute(output);


	std::vector<MObject *> iobjs = {
		&worldMatrix, &blend, &parentInverseMatrix, &rotateOrder, &localOffset, &localOffsetTransform,
		&localOffsetTranslate, &localOffsetTranslateX, &localOffsetTranslateY, &localOffsetTranslateZ,
		&localOffsetRotate, &localOffsetRotateX, &localOffsetRotateY, &localOffsetRotateZ,
		&localOffsetScale, &localOffsetScaleX, &localOffsetScaleY, &localOffsetScaleZ,
		&localOffsetShear, &localOffsetShearX, &localOffsetShearY, &localOffsetShearZ,
		&localOffsetRotateOrder
	};
	std::vector<MObject *> oobjs = {
		 &translate, &translateX, &translateY, &translateZ,
		 &rotate, &rotateX, &rotateY, &rotateZ,
		 &scale, &scaleX, &scaleY, &scaleZ,
		 &shear, &shearX, &shearY, &shearZ
	};

	for (auto &ii : iobjs) {
		for (auto &oo : oobjs) {
			attributeAffects(*ii, *oo);
		}
	}

	return MS::kSuccess;
}

MStatus PoseConstraint::compute( const MPlug& plug, MDataBlock& data ) {
	/*
	if (plug != translate && plug != rotate && plug != scale && plug != shear)
		return MS::kUnknownParameter;
	*/
    // inputs
    MArrayDataHandle inputArrayHandle = data.inputArrayValue(input);
    MMatrix invParentMat = data.inputValue(parentInverseMatrix).asMatrix();
    MMatrix offsetTfm = data.inputValue(offset).asMatrix();
    short rotOrder = data.inputValue(rotateOrder).asShort();

	// process
	MTransformationMatrix tfm = invParentMat.inverse();
	MDataHandle component;
	double wtA, wtB;

	// a
	MVector posA = tfm.getTranslation(MSpace::kTransform);
    MQuaternion rotA = tfm.rotation();
    double sclA[3];
    tfm.getScale(sclA, MSpace::kTransform);
    double shrA[3];
    tfm.getShear(shrA, MSpace::kTransform);

	// interpolate ab
    for (unsigned int i = 0; i < inputArrayHandle.elementCount(); ++i) {
        component = inputArrayHandle.inputValue();
        wtB = component.child(blend).asDouble();
		// Get the input transformation data as a matrix
		MDataHandle loff = component.child(localOffsetTransform);
		MVector posB = loff.child(localOffsetTranslate).asVector();
		double3& sclBr = loff.child(localOffsetScale).asDouble3();
		double3& shrBr = loff.child(localOffsetShear).asDouble3();

		short lrordV = loff.child(localOffsetRotateOrder).asShort();

		double3& rotBEuler = loff.child(localOffsetRotate).asDouble3();
		MTransformationMatrix mOff = MTransformationMatrix::identity;
		mOff.setScale(sclBr, MSpace::kTransform);
		mOff.setRotation(rotBEuler, PoseConstraint::rotationOrders[lrordV]);
		mOff.setTranslation(posB, MSpace::kTransform);
		mOff.setShear(shrBr, MSpace::kTransform);

		// Get the matrices
		MMatrix wmat = component.child(worldMatrix).asMatrix();
		MMatrix deprecatedOffsetMatrix = component.child(localOffset).asMatrix();

		// multiply the two
		tfm = mOff.asMatrix() * deprecatedOffsetMatrix * wmat;

		// Keep the double3 read/write values separate (sclBr/sclBw)
		// Otherwise, if they're reused, we somehow
		// write data back to the MDataHandle, and these
		// values shoot off to infinity
		double3 sclBw, shrBw;

		// decompose the multiplication
        MQuaternion rotB = tfm.rotation();
		posB = tfm.getTranslation(MSpace::kTransform);
		tfm.getScale(sclBw, MSpace::kTransform);
		tfm.getShear(shrBw, MSpace::kTransform);

		// interpolate the data
        wtA = 1.0 - wtB;
        posA = (posA * wtA) + (posB * wtB);
        rotA = slerp(rotA, rotB, wtB);
        sclA[0] = (sclA[0] * wtA) + (sclBw[0] * wtB);
        sclA[1] = (sclA[1] * wtA) + (sclBw[1] * wtB);
        sclA[2] = (sclA[2] * wtA) + (sclBw[2] * wtB);

        shrA[0] = (shrA[0] * wtA) + (shrBw[0] * wtB);
        shrA[1] = (shrA[1] * wtA) + (shrBw[1] * wtB);
        shrA[2] = (shrA[2] * wtA) + (shrBw[2] * wtB);

        inputArrayHandle.next();
    }

	MTransformationMatrix outTfm;
    outTfm.setScale(sclA, MSpace::kTransform);
    outTfm.setRotationQuaternion(rotA[0], rotA[1], rotA[2], rotA[3], MSpace::kTransform);
    outTfm.setTranslation(posA, MSpace::kTransform);
    outTfm.setShear(shrA, MSpace::kTransform);
	outTfm = offsetTfm * outTfm.asMatrix() * invParentMat;
    outTfm.reorderRotation(PoseConstraint::rotationOrders[rotOrder]);

	// ouputs
    posA = outTfm.translation(MSpace::kTransform);
	MEulerRotation rot = outTfm.eulerRotation();
	outTfm.getScale(sclA, MSpace::kTransform);
	outTfm.getShear(shrA, MSpace::kTransform);

	MDataHandle outTra = data.outputValue(translate);
	MDataHandle outRot = data.outputValue(rotate);
	MDataHandle outScl = data.outputValue(scale);
	MDataHandle outShr = data.outputValue(shear);

	// set
    outTra.setMVector(posA);
    outRot.set(rot.x, rot.y, rot.z);
    outScl.set(sclA[0], sclA[1], sclA[2]);
    outShr.set(shrA[0], shrA[1], shrA[2]);

    // clean
	data.setClean(translate);
	data.setClean(translateX);
	data.setClean(translateY);
	data.setClean(translateZ);
    data.setClean(rotate);
    data.setClean(rotateX);
    data.setClean(rotateY);
    data.setClean(rotateZ);
    data.setClean(scale);
    data.setClean(scaleX);
    data.setClean(scaleY);
    data.setClean(scaleZ);
    data.setClean(shear);
    data.setClean(shearX);
    data.setClean(shearY);
    data.setClean(shearZ);
	return MS::kSuccess;
}

