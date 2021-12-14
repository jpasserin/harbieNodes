
#include "../headers/_Utils.h"
#include "../headers/SurfaceMultiAttachNode.h"

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


MTypeId SurfaceMultiAttach::id(0x001226DA);

// Input Attribute Handles
MObject SurfaceMultiAttach::surface;
MObject SurfaceMultiAttach::sampling;
MObject SurfaceMultiAttach::u;
MObject SurfaceMultiAttach::v;
MObject SurfaceMultiAttach::tangent;
MObject SurfaceMultiAttach::surfaceMatrix;
MObject SurfaceMultiAttach::parentInverse;
MObject SurfaceMultiAttach::attach;
MObject SurfaceMultiAttach::isLoop;
MObject SurfaceMultiAttach::length;
MObject SurfaceMultiAttach::slide;
MObject SurfaceMultiAttach::start;
MObject SurfaceMultiAttach::end;
MObject SurfaceMultiAttach::reverse;

// output Attribute Handles
MObject SurfaceMultiAttach::translate;
MObject SurfaceMultiAttach::translateX;
MObject SurfaceMultiAttach::translateY;
MObject SurfaceMultiAttach::translateZ;
MObject SurfaceMultiAttach::rotate;
MObject SurfaceMultiAttach::rotateX;
MObject SurfaceMultiAttach::rotateY;
MObject SurfaceMultiAttach::rotateZ;
MObject SurfaceMultiAttach::output;


SurfaceMultiAttach::SurfaceMultiAttach() {}
SurfaceMultiAttach::~SurfaceMultiAttach() {}

void* SurfaceMultiAttach::creator() {
	return new SurfaceMultiAttach();
}


MStatus SurfaceMultiAttach::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute mAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;
	MFnCompoundAttribute cAttr;

	// Inputs
	surface = tAttr.create("surface", "surface", MFnData::kNurbsSurface);
	stat = addAttribute(surface);

	sampling = nAttr.create("sampling", "sampling", MFnNumericData::kInt, 1000);
	nAttr.setMin(10);
	nAttr.setKeyable(true);
	stat = addAttribute(sampling);

	tangent = eAttr.create("tangent", "tangent", 0);
	eAttr.addField("U", 0);
	eAttr.addField("V", 1);  
	eAttr.setKeyable(true); 
	stat = addAttribute(tangent);

	u = nAttr.create("u", "u", MFnNumericData::kDouble, -0.5);
	nAttr.setKeyable(true);
 	nAttr.setArray(true);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	nAttr.setUsesArrayDataBuilder(true);
	stat = addAttribute(u);

	v = nAttr.create("v", "v", MFnNumericData::kDouble, -0.5);
	nAttr.setKeyable(true);
 	nAttr.setArray(true);
	nAttr.setMin(0.0);
	nAttr.setMax(1.0);
	nAttr.setUsesArrayDataBuilder(true);
	stat = addAttribute(v);

	surfaceMatrix = mAttr.create("surfaceMatrix", "surfaceMatrix");
	mAttr.setKeyable(true);
	stat = addAttribute(surfaceMatrix);

	parentInverse = mAttr.create("parentInverse", "parentInverse");
	mAttr.setKeyable(true);
	stat = addAttribute(parentInverse);

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
	stat = attributeAffects(surface, translate);
	stat = attributeAffects(sampling, translate);
	stat = attributeAffects(u, translate);
	stat = attributeAffects(v, translate);
	stat = attributeAffects(tangent, translate);
	stat = attributeAffects(surfaceMatrix, translate);
	stat = attributeAffects(parentInverse, translate);
	stat = attributeAffects(attach, translate);
	stat = attributeAffects(isLoop, translate);
	stat = attributeAffects(length, translate);
	stat = attributeAffects(slide, translate);
	stat = attributeAffects(start, translate);
	stat = attributeAffects(end, translate);
	stat = attributeAffects(reverse, translate);

	stat = attributeAffects(surface, rotate);
	stat = attributeAffects(sampling, rotate);
	stat = attributeAffects(u, rotate);
	stat = attributeAffects(v, rotate);
	stat = attributeAffects(tangent, rotate);
	stat = attributeAffects(surfaceMatrix, rotate);
	stat = attributeAffects(parentInverse, rotate);
	stat = attributeAffects(attach, rotate);
	stat = attributeAffects(isLoop, rotate);
	stat = attributeAffects(length, rotate);
	stat = attributeAffects(slide, rotate);
	stat = attributeAffects(start, rotate);
	stat = attributeAffects(end, rotate);
	stat = attributeAffects(reverse, rotate);

	return MS::kSuccess;
}


MStatus SurfaceMultiAttach::compute(const MPlug& plug, MDataBlock& dataBlock) {
	MStatus stat;

	MDataHandle surfaceHandle = dataBlock.inputValue(surface);
	MDataHandle samplingHandle = dataBlock.inputValue(sampling);
	MDataHandle tangentHandle = dataBlock.inputValue(tangent);
	MDataHandle surfaceMatrixHandle = dataBlock.inputValue(surfaceMatrix);
	MDataHandle parentInverseHandle = dataBlock.inputValue(parentInverse);
	MDataHandle attachHandle = dataBlock.inputValue(attach);
	MDataHandle isLoopHandle = dataBlock.inputValue(isLoop);
	MDataHandle lengthHandle = dataBlock.inputValue(length);
	MDataHandle slideHandle = dataBlock.inputValue(slide);
	MDataHandle startHandle = dataBlock.inputValue(start);
	MDataHandle endHandle = dataBlock.inputValue(end);
	MDataHandle reverseHandle = dataBlock.inputValue(reverse);
	MArrayDataHandle uArrayHandle = dataBlock.inputArrayValue(u);
	MArrayDataHandle vArrayHandle = dataBlock.inputArrayValue(v);
	MArrayDataHandle outputArrayHandle = dataBlock.outputArrayValue(output);

	// Inputs
	MFnNurbsSurface surface = surfaceHandle.asNurbsSurface();
	unsigned int sampling = samplingHandle.asInt();
	int tangent = tangentHandle.asInt();
	MMatrix surfaceMatrix = surfaceMatrixHandle.asMatrix();
	MMatrix parentInverse = parentInverseHandle.asMatrix();
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

	// v value
	unsigned int vCount = vArrayHandle.elementCount();
	MDoubleArray vParams;
	MDataHandle vHandle;
	for (unsigned int i = 0; i<vCount; i++) {
		vHandle = vArrayHandle.inputValue(&stat);
		vParams.append(vHandle.asDouble());
		vArrayHandle.next();
	}

	double ratio;
	MDoubleArray samples(sampling);
	if (attach != 0){
		double currentLength = 1.0;
		sampleSurface(surface, samples, currentLength);
		ratio = length / currentLength;
	}
		
	// Rescale the U Params depending on start/end/slide parameters
	rescaleParams(uParams, attach, start, end, slide, reverse, ratio, isLoop);

	// Process
	MMatrixArray outMatrix = getTransform(surface, samples, uParams, vParams, tangent, (attach==0), surfaceMatrix);

	// Output
	unsigned int count = outMatrix.length();
	MArrayDataBuilder transBuilder(output, count, &stat);
	MDataHandle trHandle, tHandle, rHandle;
	for (unsigned int i=0; i<count; i++) {
		trHandle = transBuilder.addElement(i);
		tHandle = trHandle.child(translate);
		rHandle = trHandle.child(rotate);

		outMatrix[i] *= parentInverse; 

		MTransformationMatrix tfm(outMatrix[i]);
		MVector t = tfm.getTranslation(MSpace::kWorld);
		MEulerRotation r = tfm.eulerRotation();

		tHandle.set(t.x, t.y, t.z);
		rHandle.set(r.x, r.y, r.z);
		trHandle.setClean();		
	}
	stat = outputArrayHandle.set(transBuilder);
	stat = outputArrayHandle.setAllClean();

	return MS::kSuccess;
}

MMatrixArray SurfaceMultiAttach::getTransform(MFnNurbsSurface &surface, MDoubleArray &samples, MDoubleArray &uParams, MDoubleArray &vParams,
											int &tangent, bool isParametric, MMatrix &matrix) {

	double uStartDomain, uEndDomain, uRangeDomain, vStartDomain, vEndDomain, vRangeDomain;
	surface.getKnotDomain(uStartDomain, uEndDomain,vStartDomain, vEndDomain);
	uRangeDomain = uEndDomain - uStartDomain;
	vRangeDomain = vEndDomain - vStartDomain;

	unsigned int uCount = uParams.length();
	unsigned int vCount = vParams.length();
	unsigned int count = uCount * vCount;

	MMatrixArray output(count);
	MPoint point;
	MVector outTangent, uTangent, vTangent, normal, cross;
	double uParam, vParam;
	unsigned int startIndex;
	for (unsigned int j=0; j<vCount; j++){
		startIndex = 0; // This is used for optimization to avoid parsing the whole uSamples array
		// We're not sampling on V, that's not necessary
		vParam = vParams[j] * vRangeDomain + vStartDomain;
		for (unsigned int i=0; i<uCount; i++){
			if (isParametric) // Parametric
				uParam = uParams[i];
			else
				uParam = findParamFromPercentage(uParams[i], samples, startIndex);

			uParam = uParam * uRangeDomain + uStartDomain;

			surface.getPointAtParam(uParam, vParam, point, MSpace::kWorld);
			normal = surface.normal(uParam, vParam, MSpace::kWorld);
			surface.getTangents(uParam, vParam, uTangent, vTangent, MSpace::kWorld);

			// ROTATION
			if (tangent == 0) 
				outTangent = uTangent;
			else
				outTangent = vTangent;

			point *= matrix;
			outTangent *= matrix;
			normal *= matrix;

			cross = normal ^ outTangent;
			outTangent = cross ^ normal;


			double mData[4][4] = { outTangent.x, outTangent.y, outTangent.z, 0.0,
								  normal.x, normal.y, normal.z, 0.0,
								  cross.x, cross.y, cross.z, 0.0,
								  point.x, point.y, point.z, 1.0 };
			
			MMatrix mat {mData};
			output[j*uCount+i] = mat;
		}
	}

	return output;
}
