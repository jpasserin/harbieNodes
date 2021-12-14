#include "../headers/_Utils.h"
#include "../headers/SplitMatrixNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MMatrix.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MGlobal.h>

MTypeId SplitMatrix::id(0x00122700);

MObject SplitMatrix::aInMatrix;
MObject SplitMatrix::aOut00;
MObject SplitMatrix::aOut01;
MObject SplitMatrix::aOut02;
MObject SplitMatrix::aOut03;
MObject SplitMatrix::aOut10;
MObject SplitMatrix::aOut11;
MObject SplitMatrix::aOut12;
MObject SplitMatrix::aOut13;
MObject SplitMatrix::aOut20;
MObject SplitMatrix::aOut21;
MObject SplitMatrix::aOut22;
MObject SplitMatrix::aOut23;
MObject SplitMatrix::aOut30;
MObject SplitMatrix::aOut31;
MObject SplitMatrix::aOut32;
MObject SplitMatrix::aOut33;

SplitMatrix::SplitMatrix(){}

SplitMatrix::~SplitMatrix(){}

void* SplitMatrix::creator() {
	return new SplitMatrix();
}

MStatus SplitMatrix::initialize() {
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute mAttr;
	MStatus stat;

	aInMatrix = mAttr.create("inMatrix", "inMatrix", MFnMatrixAttribute::kDouble, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	addAttribute(aInMatrix);


	aOut00 = nAttr.create("out00", "out00", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut00);

	aOut01 = nAttr.create("out01", "out01", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut01);

	aOut02 = nAttr.create("out02", "out02", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut02);

	aOut03 = nAttr.create("out03", "out03", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut03);

	aOut10 = nAttr.create("out10", "out10", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut10);

	aOut11 = nAttr.create("out11", "out11", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut11);

	aOut12 = nAttr.create("out12", "out12", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut12);

	aOut13 = nAttr.create("out13", "out13", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut13);

	aOut20 = nAttr.create("out20", "out20", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut20);

	aOut21 = nAttr.create("out21", "out21", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut21);

	aOut22 = nAttr.create("out22", "out22", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut22);

	aOut23 = nAttr.create("out23", "out23", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut23);

	aOut30 = nAttr.create("out30", "out30", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut30);

	aOut31 = nAttr.create("out31", "out31", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut31);

	aOut32 = nAttr.create("out32", "out32", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut32);

	aOut33 = nAttr.create("out33", "out33", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	nAttr.setStorable(true); nAttr.setWritable(false);
	addAttribute(aOut33);

	attributeAffects(aInMatrix, aOut00);
	attributeAffects(aInMatrix, aOut01);
	attributeAffects(aInMatrix, aOut02);
	attributeAffects(aInMatrix, aOut03);
	attributeAffects(aInMatrix, aOut10);
	attributeAffects(aInMatrix, aOut11);
	attributeAffects(aInMatrix, aOut12);
	attributeAffects(aInMatrix, aOut13);
	attributeAffects(aInMatrix, aOut20);
	attributeAffects(aInMatrix, aOut21);
	attributeAffects(aInMatrix, aOut22);
	attributeAffects(aInMatrix, aOut23);
	attributeAffects(aInMatrix, aOut30);
	attributeAffects(aInMatrix, aOut31);
	attributeAffects(aInMatrix, aOut32);
	attributeAffects(aInMatrix, aOut33);
	return MS::kSuccess;
}

MStatus SplitMatrix::compute(const MPlug& plug, MDataBlock& data){
	MStatus stat;

    MMatrix inMat = data.inputValue(aInMatrix, &stat).asMatrix();
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	MDataHandle hOut00 = data.outputValue(aOut00, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut00.setDouble(inMat[0][0]);
	hOut00.setClean();

	MDataHandle hOut01 = data.outputValue(aOut01, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut01.setDouble(inMat[0][1]);
	hOut01.setClean();

	MDataHandle hOut02 = data.outputValue(aOut02, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut02.setDouble(inMat[0][2]);
	hOut02.setClean();

	MDataHandle hOut03 = data.outputValue(aOut03, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut03.setDouble(inMat[0][3]);
	hOut03.setClean();

	MDataHandle hOut10 = data.outputValue(aOut10, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut10.setDouble(inMat[1][0]);
	hOut10.setClean();

	MDataHandle hOut11 = data.outputValue(aOut11, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut11.setDouble(inMat[1][1]);
	hOut11.setClean();

	MDataHandle hOut12 = data.outputValue(aOut12, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut12.setDouble(inMat[1][2]);
	hOut12.setClean();

	MDataHandle hOut13 = data.outputValue(aOut13, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut13.setDouble(inMat[1][3]);
	hOut13.setClean();

	MDataHandle hOut20 = data.outputValue(aOut20, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut20.setDouble(inMat[2][0]);
	hOut20.setClean();

	MDataHandle hOut21 = data.outputValue(aOut21, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut21.setDouble(inMat[2][1]);
	hOut21.setClean();

	MDataHandle hOut22 = data.outputValue(aOut22, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut22.setDouble(inMat[2][2]);
	hOut22.setClean();

	MDataHandle hOut23 = data.outputValue(aOut23, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut23.setDouble(inMat[2][3]);
	hOut23.setClean();

	MDataHandle hOut30 = data.outputValue(aOut30, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut30.setDouble(inMat[3][0]);
	hOut30.setClean();

	MDataHandle hOut31 = data.outputValue(aOut31, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut31.setDouble(inMat[3][1]);
	hOut31.setClean();

	MDataHandle hOut32 = data.outputValue(aOut32, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut32.setDouble(inMat[3][2]);
	hOut32.setClean();

	MDataHandle hOut33 = data.outputValue(aOut33, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	hOut33.setDouble(inMat[3][3]);
	hOut33.setClean();

	return MS::kSuccess;

}

