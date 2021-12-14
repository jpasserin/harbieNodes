
#include "../headers/_Utils.h"
#include "../headers/FkIk2BonesNode.h"

#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MMatrix.h>
#include <cmath>

MTypeId FkIk2Bones::id(0x001226D0);

MObject FkIk2Bones::fkA;
MObject FkIk2Bones::fkB;
MObject FkIk2Bones::fkC;
MObject FkIk2Bones::root;
MObject FkIk2Bones::effector;
MObject FkIk2Bones::upv;

MObject FkIk2Bones::boneATfm;
MObject FkIk2Bones::boneBTfm;
MObject FkIk2Bones::boneCTfm;

MObject FkIk2Bones::blend;
MObject FkIk2Bones::lengthA;
MObject FkIk2Bones::lengthB;
MObject FkIk2Bones::roll;
MObject FkIk2Bones::scaleA;
MObject FkIk2Bones::scaleB;
MObject FkIk2Bones::maxStretch;
MObject FkIk2Bones::softness;
MObject FkIk2Bones::slide;
MObject FkIk2Bones::reverse;
MObject FkIk2Bones::negate;

// outputs
MObject FkIk2Bones::boneAPos;
MObject FkIk2Bones::boneAPosX;
MObject FkIk2Bones::boneAPosY;
MObject FkIk2Bones::boneAPosZ;

MObject FkIk2Bones::boneARot;
MObject FkIk2Bones::boneARotX;
MObject FkIk2Bones::boneARotY;
MObject FkIk2Bones::boneARotZ;

MObject FkIk2Bones::boneAScl;
MObject FkIk2Bones::boneASclX;
MObject FkIk2Bones::boneASclY;
MObject FkIk2Bones::boneASclZ;

MObject FkIk2Bones::boneBPos;
MObject FkIk2Bones::boneBPosX;
MObject FkIk2Bones::boneBPosY;
MObject FkIk2Bones::boneBPosZ;

MObject FkIk2Bones::boneBRot;
MObject FkIk2Bones::boneBRotX;
MObject FkIk2Bones::boneBRotY;
MObject FkIk2Bones::boneBRotZ;

MObject FkIk2Bones::boneBScl;
MObject FkIk2Bones::boneBSclX;
MObject FkIk2Bones::boneBSclY;
MObject FkIk2Bones::boneBSclZ;

MObject FkIk2Bones::boneCPos;
MObject FkIk2Bones::boneCPosX;
MObject FkIk2Bones::boneCPosY;
MObject FkIk2Bones::boneCPosZ;

MObject FkIk2Bones::boneCRot;
MObject FkIk2Bones::boneCRotX;
MObject FkIk2Bones::boneCRotY;
MObject FkIk2Bones::boneCRotZ;

MObject FkIk2Bones::boneCScl;
MObject FkIk2Bones::boneCSclX;
MObject FkIk2Bones::boneCSclY;
MObject FkIk2Bones::boneCSclZ;

MObject FkIk2Bones::outBoneA;
MObject FkIk2Bones::outBoneB;
MObject FkIk2Bones::outBoneC;


FkIk2Bones::FkIk2Bones() {}

FkIk2Bones::~FkIk2Bones() {}

void* FkIk2Bones::creator() {
	return new FkIk2Bones();
}


MStatus FkIk2Bones::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnMatrixAttribute fnMat;
	MFnUnitAttribute fnUnit;
	MFnEnumAttribute fnEnum;
    MFnCompoundAttribute fnComp;

	fkA = fnMat.create("FkA", "fka", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
	fnMat.setUsesArrayDataBuilder(true);
    fnMat.setArray(true);
	stat = FkIk2Bones::addAttribute(fkA);

	fkB = fnMat.create("FkB", "fkb", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
    fnMat.setUsesArrayDataBuilder(true);
    fnMat.setArray(true);
	stat = FkIk2Bones::addAttribute(fkB);

	fkC = fnMat.create("FkC", "fkc", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
    fnMat.setUsesArrayDataBuilder(true);
    fnMat.setArray(true);
	stat = FkIk2Bones::addAttribute(fkC);

	root = fnMat.create("Root", "root", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
    fnMat.setUsesArrayDataBuilder(true);
    fnMat.setArray(true);
	stat = FkIk2Bones::addAttribute(root);

	effector = fnMat.create("effector", "eff", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
    fnMat.setUsesArrayDataBuilder(true);
    fnMat.setArray(true);
	stat = FkIk2Bones::addAttribute(effector);

	upv = fnMat.create("upVector", "upv", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setKeyable(true);
    fnMat.setUsesArrayDataBuilder(true);
    fnMat.setArray(true);
	stat = FkIk2Bones::addAttribute(upv);

	blend = fnNum.create("blend", "blnd", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	fnNum.setMin(0.0);
	fnNum.setMax(1.0);
	stat = FkIk2Bones::addAttribute(blend);
	CHECK_MSTATUS(stat);

	lengthA = fnNum.create("lengthA", "lena", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = FkIk2Bones::addAttribute(lengthA);
	CHECK_MSTATUS(stat);

	lengthB = fnNum.create("lengthB", "lenb", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = FkIk2Bones::addAttribute(lengthB);
	CHECK_MSTATUS(stat);

	roll = fnNum.create("roll", "roll", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
    fnNum.setMin(-360);
    fnNum.setMax(360);
	stat = FkIk2Bones::addAttribute(roll);
	CHECK_MSTATUS(stat);

	scaleA = fnNum.create("scaleA", "Scla", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = FkIk2Bones::addAttribute(scaleA);
	CHECK_MSTATUS(stat);

	scaleB = fnNum.create("scaleB", "Sclb", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = FkIk2Bones::addAttribute(scaleB);
	CHECK_MSTATUS(stat);

	maxStretch = fnNum.create("maxStretch", "mstr", MFnNumericData::kDouble, 2.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = FkIk2Bones::addAttribute(maxStretch);
	CHECK_MSTATUS(stat);

	softness = fnNum.create("softness", "soft", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
    fnNum.setMin(0.0);
    fnNum.setMax(1.0);
	stat = FkIk2Bones::addAttribute(softness);
	CHECK_MSTATUS(stat);

	slide = fnNum.create("slide", "sld", MFnNumericData::kDouble, 0.5, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
    fnNum.setMin(0.0);
    fnNum.setMax(1.0);
	stat = FkIk2Bones::addAttribute(slide);
	CHECK_MSTATUS(stat);

	reverse = fnNum.create("reverse", "rev", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
    fnNum.setMin(0.0);
    fnNum.setMax(1.0);
	stat = FkIk2Bones::addAttribute(reverse);
	CHECK_MSTATUS(stat);

	negate = fnNum.create("negate", "neg", MFnNumericData::kBoolean, false, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setKeyable(true);
	stat = FkIk2Bones::addAttribute(negate);
	CHECK_MSTATUS(stat);


	//OUTPUTS
	boneATfm = fnMat.create("boneATfm", "bam", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setStorable(true);
	fnMat.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneATfm);

	boneBTfm = fnMat.create("boneBTfm", "bbm", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setStorable(true);
	fnMat.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneBTfm);

	boneCTfm = fnMat.create("boneCTfm", "bcm", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS(stat);
	fnMat.setStorable(true);
	fnMat.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneCTfm);


	boneAPosX = fnUnit.create("boneAPosX", "bapx", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneAPosY = fnUnit.create("boneAPosY", "bapy", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneAPosZ = fnUnit.create("boneAPosZ", "bapz", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneAPos = fnNum.create("boneAPos", "bap", boneAPosX, boneAPosY, boneAPosZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneAPos);
	CHECK_MSTATUS(stat);

	boneBPosX = fnUnit.create("boneBPosX", "bbpx", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBPosY = fnUnit.create("boneBPosY", "bbpy", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBPosZ = fnUnit.create("boneBPosZ", "bbpz", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBPos = fnNum.create("boneBPos", "bbp", boneBPosX, boneBPosY, boneBPosZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneBPos);
	CHECK_MSTATUS(stat);

	boneCPosX = fnUnit.create("boneCPosX", "bcpx", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCPosY = fnUnit.create("boneCPosY", "bcpy", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCPosZ = fnUnit.create("boneCPosZ", "bcpz", MFnUnitAttribute::kDistance, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCPos = fnNum.create("boneCPos", "bcp", boneCPosX, boneCPosY, boneCPosZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneCPos);
	CHECK_MSTATUS(stat);

	boneARotX = fnUnit.create("boneARotX", "barx", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneARotY = fnUnit.create("boneARotY", "bary", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneARotZ = fnUnit.create("boneARotZ", "barz", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneARot = fnNum.create("boneARot", "bar", boneARotX, boneARotY, boneARotZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneARot);
	CHECK_MSTATUS(stat);

	boneBRotX = fnUnit.create("boneBRotX", "bbrx", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBRotY = fnUnit.create("boneBRotY", "bbry", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBRotZ = fnUnit.create("boneBRotZ", "bbrz", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBRot = fnNum.create("boneBRot", "bbr", boneBRotX, boneBRotY, boneBRotZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneBRot);
	CHECK_MSTATUS(stat);

	boneCRotX = fnUnit.create("boneCRotX", "bcrx", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCRotY = fnUnit.create("boneCRotY", "bcry", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCRotZ = fnUnit.create("boneCRotZ", "bcrz", MFnUnitAttribute::kAngle, 0.0, &stat);
	CHECK_MSTATUS(stat);
	fnUnit.setStorable(true);
	fnUnit.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCRot = fnNum.create("boneCRot", "bcr", boneCRotX, boneCRotY, boneCRotZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneCRot);
	CHECK_MSTATUS(stat);

	boneASclX = fnNum.create("boneASclX", "basx", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneASclY = fnNum.create("boneASclY", "basy", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneASclZ = fnNum.create("boneASclZ", "basz", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneAScl = fnNum.create("boneAScl", "bas", boneASclX, boneASclY, boneASclZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneAScl);
	CHECK_MSTATUS(stat);

	boneBSclX = fnNum.create("boneBSclX", "bbsx", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBSclY = fnNum.create("boneBSclY", "bbsy", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBSclZ = fnNum.create("boneBSclZ", "bbsz", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneBScl = fnNum.create("boneBScl", "bbs", boneBSclX, boneBSclY, boneBSclZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneBScl);
	CHECK_MSTATUS(stat);

	boneCSclX = fnNum.create("boneCSclX", "bcsx", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCSclY = fnNum.create("boneCSclY", "bcsy", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCSclZ = fnNum.create("boneCSclZ", "bcsz", MFnNumericData::kDouble, 1.0, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	CHECK_MSTATUS(stat);

	boneCScl = fnNum.create("boneCScl", "bcs", boneCSclX, boneCSclY, boneCSclZ, &stat);
	CHECK_MSTATUS(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	stat = FkIk2Bones::addAttribute(boneCScl);
	CHECK_MSTATUS(stat);


    outBoneA = fnComp.create("outBoneA", "outA", &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
//    fnComp.setArray(true);
    fnComp.addChild(boneAPos);
    fnComp.addChild(boneARot);
    fnComp.addChild(boneAScl);
    fnComp.addChild(boneATfm);
    stat = FkIk2Bones::addAttribute(outBoneA);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    outBoneB = fnComp.create("outBoneB", "outB", &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
//    fnComp.setArray(true);
    fnComp.addChild(boneBPos);
    fnComp.addChild(boneBRot);
    fnComp.addChild(boneBScl);
    fnComp.addChild(boneBTfm);
    stat = FkIk2Bones::addAttribute(outBoneB);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    outBoneC = fnComp.create("outBoneC", "outC", &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
//    fnComp.setArray(true);
    fnComp.addChild(boneCPos);
    fnComp.addChild(boneCRot);
    fnComp.addChild(boneCScl);
    fnComp.addChild(boneCTfm);
    stat = FkIk2Bones::addAttribute(outBoneC);
    CHECK_MSTATUS_AND_RETURN_IT(stat);


	// TODO: this needs to be cleanedup by overriding 'setDependantsDirty()' so that when the system is in FK, only FK inputs attrs
	// TODO: affect the ouputs, and when in IK, only IK input attrs affecs the outputs, and both when theyre is a blend.
	// TODO: this will prevent computing the system when in full IK mode, and an FK controller is rotated.

    MObject inAttrs[] = {fkA, fkB, fkC, blend, lengthA, lengthB, roll, scaleA, scaleB, maxStretch,
						softness, slide, reverse, negate, root, effector, upv};

	MObject outAttrs[] = {boneAPos, boneARot, boneAScl, boneBPos, boneBRot, boneBScl,
						  boneCPos, boneCRot, boneCScl, boneATfm, boneBTfm, boneCTfm};

	for (const auto inAttr : inAttrs)
		for (const auto outAttr : outAttrs)
			FkIk2Bones::attributeAffects(inAttr, outAttr);

	return MS::kSuccess;
}


MStatus FkIk2Bones::compute(const MPlug &plug, MDataBlock &data) {
	MStatus stat;
	 // we don't care what plug we're setting, cause we're gonna compute and set ALL the plugs at once
	 // then set them all to be clean, avoiding redundant redundant computation.

	double blend = data.inputValue(FkIk2Bones::blend).asDouble();
	bool negate = data.inputValue(FkIk2Bones::negate).asBool();
	double lengthA = data.inputValue(FkIk2Bones::lengthA).asDouble();
	double lengthB = data.inputValue(FkIk2Bones::lengthB).asDouble();
	double roll = toRadians(data.inputValue(FkIk2Bones::roll).asDouble());
	double scaleA = data.inputValue(FkIk2Bones::scaleA).asDouble();
	double scaleB = data.inputValue(FkIk2Bones::scaleB).asDouble();
	double maxStretch = data.inputValue(FkIk2Bones::maxStretch).asDouble();
	double softness = data.inputValue(FkIk2Bones::softness).asDouble();
	double slide = data.inputValue(FkIk2Bones::slide).asDouble();
	double reverse = data.inputValue(FkIk2Bones::reverse).asDouble();

    int count;

    // IK
    MArrayDataHandle root_tfm_Handle (data.inputArrayValue(FkIk2Bones::root));
	MArrayDataHandle ik_tfm_Handle (data.inputArrayValue(FkIk2Bones::effector));
	MArrayDataHandle upv_tfm_Handle (data.inputArrayValue(FkIk2Bones::upv));

    MMatrix root_tfm;
	MMatrix ik_tfm;
	MMatrix upv_tfm;

    count = root_tfm_Handle.elementCount();
    for (unsigned int i = 0; i < count ; ++i){
        root_tfm_Handle.jumpToElement(i);
        root_tfm *= root_tfm_Handle.inputValue().asMatrix();
    }

    count = ik_tfm_Handle.elementCount();
    for (unsigned int i = 0; i < count ; ++i){
        ik_tfm_Handle.jumpToElement(i);
        ik_tfm *= ik_tfm_Handle.inputValue().asMatrix();
    }
    count = upv_tfm_Handle.elementCount();
    for (unsigned int i = 0; i < count ; ++i){
        upv_tfm_Handle.jumpToElement(i);
        upv_tfm *= upv_tfm_Handle.inputValue().asMatrix();
    }


//    MTransformationMatrix fkA_tfm, fkB_tfm, fkC_tfm;

    // FK
    MArrayDataHandle fkA_Handle (data.inputArrayValue(FkIk2Bones::fkA));
    MArrayDataHandle fkB_Handle (data.inputArrayValue(FkIk2Bones::fkB));
    MArrayDataHandle fkC_Handle (data.inputArrayValue(FkIk2Bones::fkC));

    MMatrix fkA_tfm;
    MMatrix fkB_tfm;
    MMatrix fkC_tfm;

    count = fkA_Handle.elementCount();
    for (unsigned int i = 0; i < count ; ++i){
        fkA_Handle.jumpToElement(i);
        fkA_tfm *= fkA_Handle.inputValue().asMatrix();
    }
    count = fkB_Handle.elementCount();
    for (unsigned int i = 0; i < count ; ++i){
        fkB_Handle.jumpToElement(i);
        fkB_tfm *= fkB_Handle.inputValue().asMatrix();
    }
    count = fkC_Handle.elementCount();
    for (unsigned int i = 0; i < count ; ++i){
        fkC_Handle.jumpToElement(i);
        fkC_tfm *= fkC_Handle.inputValue().asMatrix();
    }


	// Outputs
	MTransformationMatrix boneA_tfm;
	MTransformationMatrix boneB_tfm;
	MTransformationMatrix boneC_tfm;


	// Process
	if (blend == 0.0)
		this->computeFK(fkA_tfm, fkB_tfm, fkC_tfm, lengthA, lengthB, negate, 
						boneA_tfm, boneB_tfm, boneC_tfm);

	else if (blend == 1.0)
        this->computeIK(root_tfm, ik_tfm, upv_tfm, lengthA, lengthB, roll,
                        scaleA, scaleB, maxStretch, slide, reverse, negate, softness, 
                        boneA_tfm, boneB_tfm, boneC_tfm);

    else{
		this->computeFK(fkA_tfm, fkB_tfm, fkC_tfm, lengthA, lengthB, negate, boneA_tfm, boneB_tfm, boneC_tfm);

		MTransformationMatrix ik_boneA;
		MTransformationMatrix ik_boneB;
		MTransformationMatrix ik_boneC;

        this->computeIK(root_tfm, ik_tfm, upv_tfm, lengthA, lengthB, roll,
                        scaleA, scaleB, maxStretch, slide, reverse, negate, softness, 
                        ik_boneA, ik_boneB, ik_boneC);

        // remove scale to avoid shearing issue
        // This is not necessary in Softimage because the scaling hierarchy is not computed the same way. 
		double noScale[3] = {1,1,1};
		ik_boneA.setScale(noScale, MSpace::kWorld);
		ik_boneB.setScale(noScale, MSpace::kWorld);
		boneA_tfm.setScale(noScale, MSpace::kWorld);
		boneB_tfm.setScale(noScale, MSpace::kWorld);
            
        // map the secondary transforms from global to local
        ik_boneC = mapWorldPoseToObjectSpace(ik_boneB, ik_boneC);
        boneC_tfm = mapWorldPoseToObjectSpace(boneB_tfm, boneC_tfm);
        ik_boneB = mapWorldPoseToObjectSpace(ik_boneA, ik_boneB);
        boneB_tfm = mapWorldPoseToObjectSpace(boneA_tfm, boneB_tfm);


		MTransformationMatrix blend_boneA;
		MTransformationMatrix blend_boneB;
		MTransformationMatrix blend_boneC;

        // now blend them!
		blend_boneA = interpolateTransform(boneA_tfm, ik_boneA, blend);
		blend_boneB = interpolateTransform(boneB_tfm, ik_boneB, blend);
		blend_boneC = interpolateTransform(boneC_tfm, ik_boneC, blend);
            
        // now map the local transform back to global!
		blend_boneB = mapObjectPoseToWorldSpace(blend_boneA, blend_boneB);
        blend_boneC = mapObjectPoseToWorldSpace(blend_boneB, blend_boneC);

		this->computeFK(blend_boneA, blend_boneB, blend_boneC, lengthA, lengthB, negate, boneA_tfm, boneB_tfm, boneC_tfm);
	}

	// set Translation
	data.outputValue(FkIk2Bones::boneAPos).setMVector( boneA_tfm.getTranslation(MSpace::kTransform) );
	data.outputValue(FkIk2Bones::boneBPos).setMVector( boneB_tfm.getTranslation(MSpace::kTransform) );
	data.outputValue(FkIk2Bones::boneCPos).setMVector( boneC_tfm.getTranslation(MSpace::kTransform) );

	// set Rotation
	MEulerRotation boneA_rot = boneA_tfm.eulerRotation();
	data.outputValue(FkIk2Bones::boneARot).set3Double( boneA_rot.x, boneA_rot.y, boneA_rot.z );

	MEulerRotation boneB_rot = boneB_tfm.eulerRotation();
	data.outputValue(FkIk2Bones::boneBRot).set3Double( boneB_rot.x, boneB_rot.y, boneB_rot.z );

	MEulerRotation boneC_rot = boneC_tfm.eulerRotation();
	data.outputValue(FkIk2Bones::boneCRot).set3Double( boneC_rot.x, boneC_rot.y, boneC_rot.z );

	// set Scaling
	double3 boneA_scl {1.0, 1.0, 1.0};
	boneA_tfm.getScale(boneA_scl, MSpace::kTransform);
	data.outputValue(FkIk2Bones::boneAScl).set3Double(boneA_scl[0], boneA_scl[1], boneA_scl[2]);

	double3 boneB_scl {1.0, 1.0, 1.0};
	boneB_tfm.getScale(boneB_scl, MSpace::kTransform);
	data.outputValue(FkIk2Bones::boneBScl).set3Double(boneB_scl[0], boneB_scl[1], boneB_scl[2]);

	double3 boneC_scl {1.0, 1.0, 1.0};
	boneC_tfm.getScale(boneC_scl, MSpace::kTransform);
	data.outputValue(FkIk2Bones::boneCScl).set3Double(boneC_scl[0], boneC_scl[1], boneC_scl[2]);


	// set the output matrices
	data.outputValue(FkIk2Bones::boneATfm).setMMatrix(boneA_tfm.asMatrix());
	data.outputValue(FkIk2Bones::boneBTfm).setMMatrix(boneB_tfm.asMatrix());
	data.outputValue(FkIk2Bones::boneCTfm).setMMatrix(boneC_tfm.asMatrix());


	// set plugs to clean
	data.setClean(FkIk2Bones::boneAPos);
	data.setClean(FkIk2Bones::boneARot);
	data.setClean(FkIk2Bones::boneAScl);

	data.setClean(FkIk2Bones::boneBPos);
	data.setClean(FkIk2Bones::boneBRot);
	data.setClean(FkIk2Bones::boneBScl);

	data.setClean(FkIk2Bones::boneCPos);
	data.setClean(FkIk2Bones::boneCRot);
	data.setClean(FkIk2Bones::boneCScl);

	data.setClean(FkIk2Bones::boneATfm);
	data.setClean(FkIk2Bones::boneBTfm);
	data.setClean(FkIk2Bones::boneCTfm);

	return MS::kSuccess;
}


void FkIk2Bones::computeFK(const MTransformationMatrix &fkA_tfm, const MTransformationMatrix &fkB_tfm, const MTransformationMatrix &fkC_tfm,
                           double &lengthA, double &lengthB, const bool &negate,
                           MTransformationMatrix &boneA_tfm, MTransformationMatrix &boneB_tfm,
                           MTransformationMatrix &boneC_tfm) {


	MVector posA = fkA_tfm.getTranslation(MSpace::kTransform);
	MVector posB = fkB_tfm.getTranslation(MSpace::kTransform);
	MVector posC = fkC_tfm.getTranslation(MSpace::kTransform);
	MVector xAxis, yAxis, zAxis;

    double3 globalScale {0.0, 0.0, 0.0};
    fkA_tfm.getScale(globalScale, MSpace::kTransform);

	// Bone A ----------------------------------------
	xAxis = posB - posA;
	zAxis = {0.0, 0.0, 1.0};
	zAxis = zAxis.rotateBy(fkA_tfm.rotation());
	yAxis = zAxis ^ xAxis;
	yAxis.normalize();

	if (negate) {
		xAxis *= -1;
		yAxis *= -1;
		zAxis *= -1;
	}

	yAxis *= globalScale[0];
	zAxis *= globalScale[0];

	const double mdataA[4][4] {xAxis.x, xAxis.y, xAxis.z, 0.0,
                               yAxis.x, yAxis.y, yAxis.z, 0.0,
                               zAxis.x, zAxis.y, zAxis.z, 0.0,
                               posA.x, posA.y, posA.z, 1.0};

	MMatrix boneA(mdataA);
	boneA_tfm = boneA;

	// Bone B ----------------------------------------
	xAxis = posC - posB;
	zAxis = {0.0, 0.0, 1.0};
	zAxis = zAxis.rotateBy(fkB_tfm.rotation());
	yAxis = zAxis ^ xAxis;
	yAxis.normalize();

	if (negate){
		xAxis *= -1;
		yAxis *= -1;
		zAxis *= -1;
	}

	yAxis *= globalScale[0];
	zAxis *= globalScale[0];

	const double mdataB[4][4] {xAxis.x, xAxis.y, xAxis.z, 0.0,
                               yAxis.x, yAxis.y, yAxis.z, 0.0,
                               zAxis.x, zAxis.y, zAxis.z, 0.0,
                               posB.x, posB.y, posB.z, 1.0};

	MMatrix boneB(mdataB);
	boneB_tfm = boneB;

	// Bone C ----------------------------------------
	boneC_tfm = fkC_tfm;

}


void FkIk2Bones::computeIK(const MTransformationMatrix &rootTfm, const MTransformationMatrix &effector,
                           const MTransformationMatrix &upVector, const double &lengthA, const double &lengthB,
                           const double &roll, const double &scaleA, const double &scaleB, const double &maxStretch,
                           const double &slide, const double &reverse, const bool &negate, double &softness,
                           MTransformationMatrix &boneA_tfm, MTransformationMatrix &boneB_tfm,
                           MTransformationMatrix &boneC_tfm) {

	MVector rootPos = rootTfm.translation(MSpace::kTransform);
	MVector effPos = effector.translation(MSpace::kTransform);
	MVector upvPos = upVector.translation(MSpace::kTransform);
	MVector rollAxis = effPos - rootPos;
    double distance = rollAxis.length();
    rollAxis.normalize();

    MVector upVAxis = upvPos - rootPos;
    upVAxis.normalize();


	// Original Axis -----------------------------------
	// Compute the 3 axis as if the first bone was pointing to the effector
	MVector xAxis, yAxis, zAxis;
	xAxis = effPos - rootPos;
	xAxis.normalize();
	yAxis = linearInterpolate(rootPos, effPos, .5);
	yAxis = upvPos - yAxis;
	yAxis.normalize();
	yAxis = rotateVectorAlongAxis(yAxis, rollAxis, roll);
	zAxis = xAxis ^ yAxis;
	zAxis.normalize();


	// Stretch -----------------------------------------
	// Get Global Scaling
    double3 globalScale {0.0, 0.0, 0.0};
    rootTfm.getScale(globalScale, MSpace::kTransform);

    // Stretch
    double boneA_length;
    double boneB_length;
    this->computeIkStretch(lengthA, lengthB, scaleA, scaleB, maxStretch, softness, distance, 
    	slide, reverse, globalScale[0], boneA_length, boneB_length);

    
    
	// Angle ----------------------------------------
    double angle = this->computeIkAngle(distance, boneA_length, boneB_length, (reverse>0.5));
	if (angle != 0.0)
		xAxis = rotateVectorAlongAxis(xAxis, zAxis, -angle);


 
	// Bone A ----------------------------------------
	xAxis *= boneA_length;

	yAxis = zAxis ^ xAxis;
	yAxis.normalize();

	if (negate){
		xAxis *= -1;
		yAxis *= -1;
		zAxis *= -1;
	}

	yAxis *= globalScale[0];
	zAxis *= globalScale[0];


    const double mdataA[4][4] = {xAxis.x, xAxis.y, xAxis.z, 0.0,
                                 yAxis.x, yAxis.y, yAxis.z, 0.0,
                                 zAxis.x, zAxis.y, zAxis.z, 0.0,
                                 rootPos.x, rootPos.y, rootPos.z, 1.0};

	MMatrix boneA(mdataA);
	boneA_tfm = boneA;


	// Bone B ----------------------------------------
	MVector posB;
	if (negate)
		posB = MVector(-1,0,0) * boneA + rootPos;
	else
		posB = MVector(1,0,0) * boneA + rootPos;

	xAxis = effector.getTranslation(MSpace::kWorld) - posB;
	xAxis.normalize();
	xAxis *= boneB_length;

	yAxis = zAxis ^ xAxis;
	yAxis.normalize();

	if (negate)
		xAxis *= -1;

	yAxis *= globalScale[0];
	zAxis *= globalScale[0];

    const double mdataB[4][4] = {xAxis.x, xAxis.y, xAxis.z, 0.0,
                                 yAxis.x, yAxis.y, yAxis.z, 0.0,
                                 zAxis.x, zAxis.y, zAxis.z, 0.0,
                                 posB.x, posB.y, posB.z, 1.0};


	MMatrix boneB(mdataB);
	boneB_tfm = boneB;


	// Bone C ----------------------------------------
	MVector posC;
	if (negate)
		posC = MVector(-1,0,0) * boneB + posB;
	else
		posC = MVector(1,0,0) * boneB + posB;

    boneC_tfm = effector;
    boneC_tfm.setTranslation(posC, MSpace::kWorld);

}

double FkIk2Bones::computeIkAngle(const double distance, const double lengthA, const double lengthB, const bool invert) {
	double angle = 0.0;
	if((distance < lengthA + lengthB) && (distance > fabs(lengthA - lengthB) + 1E-6)){
	
		// use the law of cosine for lengthA
		double a = lengthA;
		double b = distance;
		double c = lengthB;

		angle = acos(min(1, (a * a + b * b - c * c) / (2 * a * b)));

		// invert the angles if need be
		if (invert)
			angle *= -1;
	}

	return angle;
}


void FkIk2Bones::computeIkStretch(const double &lengthA, const double &lengthB, const double &scaleA, const double &scaleB,
                                  const double &maxStretch, double &softness, double &distance,
                                  const double &slide, const double &reverse, const double globalScale,
                                  double &outA, double &outB) const {


	// Distance with MaxStretch ---------------------
	double restLength = (lengthA * scaleA + lengthB * scaleB) * globalScale;
	double distanceWithStretch = distance;
	if (distance > (restLength * (maxStretch)))
		distanceWithStretch = restLength * maxStretch;

	// Adapt Softness value to chain length --------
	double scaleSoftness = softness * restLength * .1;

	// Stretch and softness ------------------------
	// We use the real distance from root to controler to calculate the softness
	// This way we have softness working even when there is no stretch
	double stretch = max(1, distanceWithStretch / restLength);
	double da = restLength - scaleSoftness;
	if ((scaleSoftness > 0) && (distance > da)){
		double newlen = scaleSoftness*(1.0 - exp(-(distance -da)/scaleSoftness)) + da;
		stretch = distanceWithStretch / newlen;
	}

	outA = lengthA * stretch * scaleA * globalScale;
	outB = lengthB * stretch * scaleB * globalScale;

	// Reverse -------------------------------------
	double d = distanceWithStretch / (outA + outB);

	double reverseScale;
	if(reverse < 0.5)
		reverseScale = 1-(reverse*2 * (1-d));
	else
		reverseScale = 1-((1-reverse)*2 * (1-d));

	outA *= reverseScale;
	outB *= reverseScale;

	// Slide ---------------------------------------
	double slideAdd;
	if (slide < .5)
		slideAdd = (outA * (slide * 2)) - (outA);
	else
		slideAdd = (outB * (slide * 2)) - (outB);

	outA += slideAdd;
	outB -= slideAdd;
}






















