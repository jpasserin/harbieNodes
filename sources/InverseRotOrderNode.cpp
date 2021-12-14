
#include "../headers/InverseRotOrderNode.h"

#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>

MTypeId InverseRotOrder::id(0x001226CE);
MObject InverseRotOrder::rotOrder;
MObject InverseRotOrder::output;

InverseRotOrder::InverseRotOrder() {}

InverseRotOrder::~InverseRotOrder() {}

void* InverseRotOrder::creator() {
	return new InverseRotOrder();
}

MStatus InverseRotOrder::initialize() {
	// attributes are writable by default
	// attributes are storable by default
	// attributes are readable by default
	// attributes not keyable by default
	MStatus stat;
	MFnNumericAttribute fnNum;
	MFnMatrixAttribute fnMat;
	MFnEnumAttribute fnEnum;

	rotOrder = fnEnum.create("rotationOrder", "ro", 0);
	fnEnum.addField("xyz", 0);
	fnEnum.addField("yzx", 1);
	fnEnum.addField("zxy", 2);
	fnEnum.addField("xzy", 3);
	fnEnum.addField("yxz", 4);
	fnEnum.addField("zyx", 5);
	fnEnum.setKeyable(true);
	stat = InverseRotOrder::addAttribute(rotOrder);
	CHECK_MSTATUS(stat);

	output = fnNum.create("output", "out", MFnNumericData::kShort, 0);
	fnNum.setWritable(false);
	fnNum.setStorable(true);
	stat = InverseRotOrder::addAttribute(output);
	CHECK_MSTATUS(stat);

	InverseRotOrder::attributeAffects(rotOrder, output);

	return MS::kSuccess;
}

MStatus InverseRotOrder::compute( const MPlug& plug, MDataBlock& data ) {

	if (plug != output)
		return MS::kUnknownParameter;

	short rotOrder = data.inputValue(InverseRotOrder::rotOrder).asShort();
	short inv_rot[] {5, 3, 4, 1, 2, 0};

	data.outputValue(InverseRotOrder::output).setShort( inv_rot[rotOrder] );
	data.setClean(plug);

	return MS::kSuccess;
}



