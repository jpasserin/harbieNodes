
#include "../headers/_Utils.h"
#include "../headers/BeanRemoteNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>

#include <algorithm>


MTypeId BeanRemote::id(0x001226EC);

MObject BeanRemote::x;
MObject BeanRemote::inputX;
MObject BeanRemote::minX;
MObject BeanRemote::maxX;
MObject BeanRemote::clampMinX;
MObject BeanRemote::clampMaxX;

MObject BeanRemote::y;
MObject BeanRemote::inputY;
MObject BeanRemote::minY;
MObject BeanRemote::maxY;
MObject BeanRemote::clampMinY;
MObject BeanRemote::clampMaxY;

MObject BeanRemote::output;
MObject BeanRemote::north;
MObject BeanRemote::south;
MObject BeanRemote::east;
MObject BeanRemote::west;

BeanRemote::BeanRemote(){}

BeanRemote::~BeanRemote(){}

void* BeanRemote::creator() {
	return new BeanRemote();
}

MStatus BeanRemote::initialize() {
	MFnNumericAttribute fnNum;
	MFnCompoundAttribute fnComp;
	MStatus stat;

	// Input X
	x = fnNum.create("x", "x", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	fnNum.setKeyable(true);
	addAttribute(x);

	// Input Y
	y = fnNum.create("y", "y", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	fnNum.setKeyable(true);
	addAttribute(y);

	// Outputs
	north = fnNum.create("north", "north", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	addAttribute(north);

	south = fnNum.create("south", "south", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	addAttribute(south);

	east = fnNum.create("east", "east", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	addAttribute(east);

	west = fnNum.create("west", "west", MFnNumericData::kDouble, 0.0, &stat);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	fnNum.setStorable(true);
	fnNum.setWritable(false);
	addAttribute(west);

	// Affects
	attributeAffects(x, east);
	attributeAffects(x, west);

	attributeAffects(y, north);
	attributeAffects(y, south);

	return MS::kSuccess;
}

MStatus BeanRemote::compute(const MPlug& plug, MDataBlock& data){
	MStatus stat;

	// Check which output are connected
	MObject thisNode = this->thisMObject();
	MPlug north = MPlug(thisNode, BeanRemote::north);
	MPlug south = MPlug(thisNode, BeanRemote::south);
	MPlug east = MPlug(thisNode, BeanRemote::east);
	MPlug west = MPlug(thisNode, BeanRemote::west);

	if (plug == north || plug == south){
		const double input = data.inputValue(y).asDouble();
		computeDirection(north, south, input, data);
	}
	else if (plug == east || plug == west){
		const double input = data.inputValue(x).asDouble();
		computeDirection(east, west, input, data);
	}


	return MS::kSuccess;

}

MStatus BeanRemote::computeDirection(const MPlug& pPlug, const MPlug& nPlug, double value, MDataBlock& data){
	bool clamp = isConnected(nPlug, false, true) && isConnected(pPlug, false, true);

	double pValue, nValue;
	if (clamp){
		pValue = max(0, value);
		nValue = fabs(min(0, value));
	}
	else{
		pValue = value;
		nValue = -value;
	}

	data.outputValue(pPlug).setDouble(pValue);
	data.outputValue(nPlug).setDouble(nValue);
	data.setClean(pPlug);
	data.setClean(nPlug);

	return MS::kSuccess;
}
