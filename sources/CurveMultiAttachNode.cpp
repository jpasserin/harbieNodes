
#include "../headers/_Utils.h"
#include "../headers/CurveMultiAttachNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>

#include <cfloat>
#include <cmath>


MTypeId CurveMultiAttach::id(0x001226F1);

// Input Attribute Handles
MObject CurveMultiAttach::curve;
MObject CurveMultiAttach::u;
MObject CurveMultiAttach::curveMatrix;
MObject CurveMultiAttach::parentInverse;
MObject CurveMultiAttach::upVector;
MObject CurveMultiAttach::attach;
MObject CurveMultiAttach::isLoop;
MObject CurveMultiAttach::length;
MObject CurveMultiAttach::slide;
MObject CurveMultiAttach::start;
MObject CurveMultiAttach::end;
MObject CurveMultiAttach::reverse;

// output Attribute Handles
MObject CurveMultiAttach::translate;
MObject CurveMultiAttach::translateX;
MObject CurveMultiAttach::translateY;
MObject CurveMultiAttach::translateZ;
MObject CurveMultiAttach::rotate;
MObject CurveMultiAttach::rotateX;
MObject CurveMultiAttach::rotateY;
MObject CurveMultiAttach::rotateZ;
MObject CurveMultiAttach::output;


CurveMultiAttach::CurveMultiAttach() {}

CurveMultiAttach::~CurveMultiAttach() {}

void* CurveMultiAttach::creator() {
	return new CurveMultiAttach();
}


MStatus CurveMultiAttach::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute mAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;
	MFnCompoundAttribute cAttr;

	// Inputs
	curve = tAttr.create("curve", "curve", MFnData::kNurbsCurve);
	stat = addAttribute(curve);

	u = nAttr.create("u", "u", MFnNumericData::kDouble, -0.5);
	nAttr.setKeyable(true);
 	nAttr.setArray(true);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	nAttr.setUsesArrayDataBuilder(true);
	stat = addAttribute(u);

	curveMatrix = mAttr.create("curveMatrix", "curveMatrix");
	mAttr.setKeyable(true);
	stat = addAttribute(curveMatrix);

	parentInverse = mAttr.create("parentInverse", "parentInverse");
	mAttr.setKeyable(true);
	stat = addAttribute(parentInverse);

	upVector = nAttr.createPoint("upVector", "upVector");
	nAttr.setStorable(false);
	nAttr.setWritable(true);
	stat = addAttribute(upVector);

	attach = eAttr.create("attach", "attach", 0);
	eAttr.addField("Parametric", 0);
	eAttr.addField("Percentage", 1);
	eAttr.addField("FixedLength", 2);
	eAttr.setKeyable(true);
	stat = addAttribute(attach);

	isLoop = nAttr.create("isLoop", "isLoop", MFnNumericData::kBoolean, false);
	nAttr.setKeyable(true);
	stat = addAttribute(isLoop);

	length = nAttr.create("length", "length", MFnNumericData::kDouble, 1.0);
	nAttr.setKeyable(true);
	nAttr.setMin(0.0001);
	stat = addAttribute(length);

	slide = nAttr.create("slide", "slide", MFnNumericData::kDouble, 0.0);
	nAttr.setKeyable(true);
	stat = addAttribute(slide);

	start = nAttr.create("start", "start", MFnNumericData::kDouble, 0.0);
	nAttr.setKeyable(true);
	nAttr.setMin(0);
	nAttr.setMax(1);
	stat = addAttribute(start);

	end = nAttr.create("end", "end", MFnNumericData::kDouble, 1.0);
	nAttr.setKeyable(true);
	nAttr.setMin(0);
	nAttr.setMax(1);
	stat = addAttribute(end);

	reverse = nAttr.create("reverse", "reverse", MFnNumericData::kDouble, 0.0);
	nAttr.setKeyable(true);
	nAttr.setMin(0);
	nAttr.setMax(1);
	stat = addAttribute(reverse);

	// Outputs
	translateX = uAttr.create("translateX", "translateX", MFnUnitAttribute::kDistance);
	uAttr.setWritable(false);
	uAttr.setStorable(true);
	translateY = uAttr.create("translateY", "translateY", MFnUnitAttribute::kDistance);
	uAttr.setWritable(false);
	uAttr.setStorable(true);
	translateZ = uAttr.create("translateZ", "translateZ", MFnUnitAttribute::kDistance);
	uAttr.setWritable(false);
	uAttr.setStorable(true);
	translate = nAttr.create("translate", "translate", translateX, translateY, translateZ);
	nAttr.setWritable(false);

	rotateX = uAttr.create("rotateX", "rotateX", MFnUnitAttribute::kAngle);
	uAttr.setWritable(false);
	uAttr.setStorable(true);
	rotateY = uAttr.create("rotateY", "rotateY", MFnUnitAttribute::kAngle);
	uAttr.setWritable(false);
	uAttr.setStorable(true);
	rotateZ = uAttr.create("rotateZ", "rotateZ", MFnUnitAttribute::kAngle);
	uAttr.setWritable(false);
	uAttr.setStorable(true);
	rotate = nAttr.create("rotate", "rotate", rotateX, rotateY, rotateZ);
	nAttr.setWritable(false);

	output = cAttr.create("output", "output");
	cAttr.setWritable(false);
	cAttr.setArray(true);
	cAttr.addChild(translate);
	cAttr.addChild(rotate);
	cAttr.setUsesArrayDataBuilder(true);
	stat = addAttribute(output);


	// Connect
	stat = attributeAffects(curve, translate);
	stat = attributeAffects(u, translate);
	stat = attributeAffects(curveMatrix, translate);
	stat = attributeAffects(parentInverse, translate);
	stat = attributeAffects(upVector, translate);
	stat = attributeAffects(attach, translate);
	stat = attributeAffects(isLoop, translate);
	stat = attributeAffects(length, translate);
	stat = attributeAffects(slide, translate);
	stat = attributeAffects(start, translate);
	stat = attributeAffects(end, translate);
	stat = attributeAffects(reverse, translate);

	stat = attributeAffects(curve, rotate);
	stat = attributeAffects(u, rotate);
	stat = attributeAffects(curveMatrix, rotate);
	stat = attributeAffects(parentInverse, rotate);
	stat = attributeAffects(upVector, rotate);
	stat = attributeAffects(attach, rotate);
	stat = attributeAffects(isLoop, rotate);
	stat = attributeAffects(length, rotate);
	stat = attributeAffects(slide, rotate);
	stat = attributeAffects(start, rotate);
	stat = attributeAffects(end, rotate);
	stat = attributeAffects(reverse, rotate);

	return MS::kSuccess;
}


MStatus CurveMultiAttach::compute(const MPlug& plug, MDataBlock& dataBlock) {
	MStatus stat;

	MDataHandle curveHandle = dataBlock.inputValue(curve);
	MDataHandle curveMatrixHandle = dataBlock.inputValue(curveMatrix);
	MDataHandle parentInverseHandle = dataBlock.inputValue(parentInverse);
	MDataHandle upVectorHandle = dataBlock.inputValue(upVector);
	MDataHandle attachHandle = dataBlock.inputValue(attach);
	MDataHandle isLoopHandle = dataBlock.inputValue(isLoop);
	MDataHandle lengthHandle = dataBlock.inputValue(length);
	MDataHandle slideHandle = dataBlock.inputValue(slide);
	MDataHandle startHandle = dataBlock.inputValue(start);
	MDataHandle endHandle = dataBlock.inputValue(end);
	MDataHandle reverseHandle = dataBlock.inputValue(reverse);
	MArrayDataHandle uArrayHandle = dataBlock.inputArrayValue(u);
	MArrayDataHandle outputArrayHandle = dataBlock.outputArrayValue(output);

	// Inputs
	MFnNurbsCurve curve = curveHandle.asNurbsCurve();
	MMatrix curveMatrix = curveMatrixHandle.asMatrix();
	MMatrix parentInverse = parentInverseHandle.asMatrix();
	MVector upVector = upVectorHandle.asFloatVector();
	short attach = attachHandle.asShort();
	bool isLoop = isLoopHandle.asBool();
	double length = lengthHandle.asDouble();
	double slide = slideHandle.asDouble();
	double start = startHandle.asDouble();
	double end = endHandle.asDouble();
	double reverse = reverseHandle.asDouble();

	// U value
	unsigned int uCount = uArrayHandle.elementCount();
	MDoubleArray uParams;	
	MDataHandle uHandle;
	for(unsigned int i = 0; i<uCount; i++) {
		uHandle = uArrayHandle.inputValue(&stat);
		uParams.append(uHandle.asDouble());
		uArrayHandle.next();
	}

	double ratio;
	if (attach == 2) ratio = length / curve.length();

	// Rescale the U Params depending on start/end/slide parameters
	rescaleParams(uParams, attach, start, end, slide, reverse, ratio, isLoop);

	// Process
	MMatrixArray outMatrix = getTransform(curve, uParams, upVector, (attach==0), curveMatrix);

	// Output
	MArrayDataBuilder transBuilder(output, uCount, &stat);
	MDataHandle trHandle, tHandle, rHandle;
	MVector t;
	MEulerRotation r;
	MMatrix m;
	for (unsigned int i = 0; i<uCount; i++) {
		trHandle = transBuilder.addElement(i);
		tHandle = trHandle.child(translate);
		rHandle = trHandle.child(rotate);

		outMatrix[i] *= parentInverse; 

		MTransformationMatrix tfm(outMatrix[i]);
		t = tfm.getTranslation(MSpace::kWorld);
		r = tfm.eulerRotation();

		tHandle.set(t.x, t.y, t.z);
		rHandle.set(r.x, r.y, r.z);
		trHandle.setClean();		
	}
	stat = outputArrayHandle.set(transBuilder);
	stat = outputArrayHandle.setAllClean();

	return MS::kSuccess;
}

MMatrixArray CurveMultiAttach::getTransform(MFnNurbsCurve &curve, MDoubleArray &params, MVector &upVector, 
											bool isParametric, MMatrix &matrix) {

	double currentLength, paramLength;
	if (!isParametric) // Percentage
		currentLength = curve.length();

	double startDomain, endDomain, rangeDomain;
	curve.getKnotDomain(startDomain, endDomain);
	rangeDomain = endDomain - startDomain;

	MMatrixArray output(params.length());
	MPoint point;
	MVector tangent, normal, cross;
	double param;
	for (unsigned int i=0; i < params.length(); i++){
		if (isParametric) // Parametric
			param = params[i] * rangeDomain + startDomain;
		else {
			paramLength = params[i] * currentLength;
			param = curve.findParamFromLength(paramLength);
		}

		curve.getPointAtParam(param, point, MSpace::kWorld);
		tangent = curve.tangent(param, MSpace::kWorld);

		point *= matrix;
		tangent *= matrix;

		normal = upVector - MVector(point);
		normal.normalize();

		cross = tangent ^ normal;

		double mData[4][4] = { tangent.x, tangent.y, tangent.z, 0.0,
							  normal.x, normal.y, normal.z, 0.0,
							  cross.x, cross.y, cross.z, 0.0,
							  point.x, point.y, point.z, 1.0 };
		
		MMatrix mat {mData};
		output[i] = mat;
	}

	return output;
}

	



