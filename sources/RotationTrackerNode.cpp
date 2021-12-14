#include "../headers/_Utils.h"
#include "../headers/RotationTrackerNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>

#include <maya/MGlobal.h>
#include <cmath>
#include <functional>


// Static Members
MTypeId RotationTracker::id(0x001226CC);
MObject RotationTracker::reference;
MObject RotationTracker::tracker;
MObject RotationTracker::rest;
MObject RotationTracker::restX;
MObject RotationTracker::restY;
MObject RotationTracker::restZ;
MObject RotationTracker::output;
MObject RotationTracker::outputX;
MObject RotationTracker::outputY;
MObject RotationTracker::outputZ;

//------------------------------------------

RotationTracker::RotationTracker() {}

RotationTracker::~RotationTracker() {}

void* RotationTracker::creator() {
	return new RotationTracker();
}


MStatus RotationTracker::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute mAttr;
	MFnUnitAttribute uAttr;

	reference = mAttr.create("reference", "ref");
	mAttr.setKeyable(true);
	stat = addAttribute(reference);

	tracker = mAttr.create("tracker", "trk");
	mAttr.setKeyable(true);
	stat = addAttribute(tracker);

	restX = uAttr.create( "restX", "rx", MFnUnitAttribute::kAngle, 0.0 );    
	restY = uAttr.create( "restY", "ry", MFnUnitAttribute::kAngle, 0.0 );    
	restZ = uAttr.create( "restZ", "rz", MFnUnitAttribute::kAngle, 0.0 );    
	rest = nAttr.create( "rest", "r", restX, restY, restZ);
	uAttr.setStorable(true);
	nAttr.setDefault(0.,0.,0.);
	nAttr.setKeyable(true);
	nAttr.setWritable(true);
	stat = addAttribute(rest);    

 	// Output
	outputX = uAttr.create( "outputX", "ox", MFnUnitAttribute::kAngle, 0.0 );    
	outputY = uAttr.create( "outputY", "oy", MFnUnitAttribute::kAngle, 0.0 );    
	outputZ = uAttr.create( "outputZ", "oz", MFnUnitAttribute::kAngle, 0.0 );    
	output = nAttr.create( "output", "o", outputX, outputY, outputZ);
	uAttr.setStorable(true);
	nAttr.setDefault(0.,0.,0.);
	nAttr.setKeyable(false);
	nAttr.setWritable(false);
	nAttr.setReadable(true);
	stat = addAttribute(output);  


 	// Affects
	attributeAffects(reference, output);
	attributeAffects(tracker, output);
	attributeAffects(rest, output);
	attributeAffects(restX, output);
	attributeAffects(restY, output);
	attributeAffects(restZ, output);

	attributeAffects(reference, outputX);
	attributeAffects(tracker, outputX);
	attributeAffects(rest, outputX);
	attributeAffects(restX, outputX);
	attributeAffects(restY, outputX);
	attributeAffects(restZ, outputX);

	attributeAffects(reference, outputY);
	attributeAffects(tracker, outputY);
	attributeAffects(rest, outputY);
	attributeAffects(restX, outputY);
	attributeAffects(restY, outputY);
	attributeAffects(restZ, outputY);

	attributeAffects(reference, outputZ);
	attributeAffects(tracker, outputZ);
	attributeAffects(rest, outputZ);
	attributeAffects(restX, outputZ);
	attributeAffects(restY, outputZ);
	attributeAffects(restZ, outputZ);

	return MS::kSuccess;
}


MStatus RotationTracker::compute(const MPlug &plug, MDataBlock &data) {
	// inputs
	MTransformationMatrix ref = MTransformationMatrix(data.inputValue(reference).asMatrix());
	MTransformationMatrix trk = MTransformationMatrix(data.inputValue(tracker).asMatrix());

	MDataHandle restHandle = data.inputValue(rest);	
	double3 radVal3;
	radVal3[0] = restHandle.child(restX).asDouble(); 
	radVal3[1] = restHandle.child(restY).asDouble();
	radVal3[2] = restHandle.child(restZ).asDouble();
	MEulerRotation er (radVal3);

	MTransformationMatrix loc;
	loc.rotateTo(er);

	MMatrix m = loc.asMatrix() * ref.asMatrix();
	m = trk.asMatrix() * m.inverse();
	er = MTransformationMatrix(m).eulerRotation();

	MDataHandle outHandle = data.outputValue(output);
	outHandle.set(er.x, er.y, er.z);

	data.setClean(plug);
	return MS::kSuccess;
}