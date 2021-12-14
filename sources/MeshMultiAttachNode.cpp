
#include "../headers/_Utils.h"
#include "../headers/MeshMultiAttachNode.h"

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MDagPath.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshVertex.h>
#include <maya/MQuaternion.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>

#include <cfloat>
#include <cmath>


MTypeId MeshMultiAttach::id(0x001226DB);

// Input Attribute Handles
MObject MeshMultiAttach::mesh;
MObject MeshMultiAttach::meshMatrix;
MObject MeshMultiAttach::attach;
MObject MeshMultiAttach::component;
MObject MeshMultiAttach::index;
MObject MeshMultiAttach::orient;
MObject MeshMultiAttach::parentInverse;

// output Attribute Handles
MObject MeshMultiAttach::translate;
MObject MeshMultiAttach::translateX;
MObject MeshMultiAttach::translateY;
MObject MeshMultiAttach::translateZ;
MObject MeshMultiAttach::rotate;
MObject MeshMultiAttach::rotateX;
MObject MeshMultiAttach::rotateY;
MObject MeshMultiAttach::rotateZ;
MObject MeshMultiAttach::output;


MeshMultiAttach::MeshMultiAttach() {}

MeshMultiAttach::~MeshMultiAttach() {}

void* MeshMultiAttach::creator() {
	return new MeshMultiAttach();
}


MStatus MeshMultiAttach::initialize() {
	// attributes are writable, storable, readable, not keyable by default
	MStatus stat;
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute mAttr;
	MFnUnitAttribute uAttr;
	MFnEnumAttribute eAttr;
	MFnCompoundAttribute cAttr;

	// Inputs
	mesh = tAttr.create("mesh", "mesh", MFnData::kMesh);
	stat = addAttribute(mesh);

	meshMatrix = mAttr.create("meshMatrix", "meshMatrix");
	mAttr.setKeyable(true);
	stat = addAttribute(meshMatrix);

	attach = eAttr.create("attach", "attach", 0);
	eAttr.addField("Vertex", 0);
	eAttr.addField("Edge", 1); 
	eAttr.addField("Polygon", 2); 
	eAttr.setKeyable(true); 
	stat = addAttribute(attach);

	index = nAttr.create("index", "index", MFnNumericData::kInt, -1);
	nAttr.setKeyable(true);
	nAttr.setMin(0);
	stat = addAttribute(index);

	orient = nAttr.create("orient", "orient", MFnNumericData::kBoolean, false);
	nAttr.setKeyable(true);
	stat = addAttribute(orient);

	parentInverse = mAttr.create("parentInverse", "parentInverse");
	mAttr.setKeyable(true);
	stat = addAttribute(parentInverse);

	component = cAttr.create("component", "component");
	cAttr.setWritable(true);
	cAttr.setArray(true);
	cAttr.addChild(index);
	cAttr.addChild(orient);
	cAttr.addChild(parentInverse);
	cAttr.setUsesArrayDataBuilder(true);
	stat = addAttribute(component);

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
	stat = attributeAffects(mesh, translate);
	stat = attributeAffects(meshMatrix, translate);
	stat = attributeAffects(attach, translate);
	stat = attributeAffects(index, translate);
	stat = attributeAffects(orient, translate);
	stat = attributeAffects(parentInverse, translate);

	stat = attributeAffects(mesh, rotate);
	stat = attributeAffects(meshMatrix, rotate);
	stat = attributeAffects(attach, rotate);
	stat = attributeAffects(index, rotate);
	stat = attributeAffects(orient, rotate);
	stat = attributeAffects(parentInverse, rotate);

	return MS::kSuccess;
}


MStatus MeshMultiAttach::compute(const MPlug& plug, MDataBlock& dataBlock) {
	MStatus stat;

	MDataHandle meshHandle = dataBlock.inputValue(mesh);
	MDataHandle meshMatrixHandle = dataBlock.inputValue(meshMatrix);
	MDataHandle attachHandle = dataBlock.inputValue(attach);
	MArrayDataHandle componentArrayHandle = dataBlock.inputArrayValue(component);
	MArrayDataHandle outputArrayHandle = dataBlock.outputArrayValue(output);

	// Inputs
	MMatrix meshMatrix = meshMatrixHandle.asMatrix();
	short attach = attachHandle.asShort();
	MObject mesh = meshHandle.asMesh();
	MDagPath dummyDagPath;
	MItMeshVertex vertexIter(dummyDagPath);
	MItMeshEdge edgeIter(dummyDagPath);
	MItMeshPolygon polygonIter(dummyDagPath);

	if (mesh != MObject::kNullObj) {
		if (attach == 0)
			vertexIter.reset(mesh);
		else if (attach == 1){
			edgeIter.reset(mesh);
			vertexIter.reset(mesh);
		}
		else if (attach == 2)
			polygonIter.reset(mesh);
	}

	// Components
	unsigned int count = componentArrayHandle.elementCount();

	MMatrix mat;
	MArrayDataBuilder transBuilder(output, count, &stat);
	for(unsigned int i = 0; i<count; i++) {
		MDataHandle componentHandle = componentArrayHandle.inputValue(&stat);
		MDataHandle indexHandle = componentHandle.child(index);
		MDataHandle orientHandle = componentHandle.child(orient);
		MDataHandle parentInverseHandle = componentHandle.child(parentInverse);

		int componentIndex = indexHandle.asInt();
		bool componentOrient = orientHandle.asBool();
		MMatrix parentInverse = parentInverseHandle.asMatrix();

		// Process
		if (attach == 0)
			mat = computeVertex(vertexIter, componentIndex, componentOrient, meshMatrix);
		else if (attach == 1)
			mat = computeEdge(edgeIter, vertexIter, componentIndex, componentOrient, meshMatrix);
		else if (attach == 2)
			mat = computePolygon(polygonIter, componentIndex, componentOrient, meshMatrix);

		// Parent
		MTransformationMatrix tfm(mat * parentInverse);
		MVector t = tfm.getTranslation(MSpace::kWorld);
		MEulerRotation r = tfm.eulerRotation();

		//Output
		MDataHandle trHandle = transBuilder.addElement(i);
		MDataHandle tHandle = trHandle.child(translate);
		MDataHandle rHandle = trHandle.child(rotate);

		tHandle.set(t.x, t.y, t.z);
		rHandle.set(r.x, r.y, r.z);
		trHandle.setClean();

		componentArrayHandle.next();
	}

	stat = outputArrayHandle.set(transBuilder);
	stat = outputArrayHandle.setAllClean();

	return MS::kSuccess;
}

MMatrix MeshMultiAttach::computeVertex(MItMeshVertex &iter, int &index, bool &orient, MMatrix &matrix){
	int dummyIndex;
	iter.setIndex(index, dummyIndex);
	MPoint point = iter.position(MSpace::kWorld);

	MVector tangent, normal, z;
	MIntArray neighbors;
	if (orient){
		iter.getNormal(normal, MSpace::kWorld);

		iter.getConnectedVertices(neighbors);
		iter.setIndex(neighbors[0], dummyIndex);

		tangent = MVector(iter.position(MSpace::kWorld) - point);
		tangent.normalize();

		z = normal ^ tangent;
		tangent = z ^ normal;
	}

	double mArray[4][4] = { normal.x, normal.y, normal.z, 0.0,
							tangent.x, tangent.y, tangent.z, 0.0,
							z.x, z.y, z.z, 0.0,
							point.x, point.y, point.z, 1.0 };

	MMatrix mat{mArray};

	return mat * matrix;
}

MMatrix MeshMultiAttach::computeEdge(MItMeshEdge &iter, MItMeshVertex &vertexIter, int &index, bool &orient, MMatrix &matrix){
	int dummyIndex;
	iter.setIndex(index, dummyIndex);
	MPoint point = iter.center(MSpace::kWorld);

	MVector normal0, normal1;
	MVector tangent, normal, z;
	if (orient){
		normal = MVector(iter.point(0, MSpace::kWorld) - point);
		normal.normalize();

		int vertIndex0 = iter.index(0);
		vertexIter.setIndex(vertIndex0, dummyIndex);
		vertexIter.getNormal(normal0, MSpace::kWorld);

		int vertIndex1 = iter.index(1);
		vertexIter.setIndex(vertIndex1, dummyIndex);
		vertexIter.getNormal(normal1, MSpace::kWorld);

		tangent = (normal0 + normal1) / 2.0;

		z = normal ^ tangent;
		tangent = z ^ normal;
	}

	double mArray[4][4] = { normal.x, normal.y, normal.z, 0.0,
							tangent.x, tangent.y, tangent.z, 0.0,
							z.x, z.y, z.z, 0.0,
							point.x, point.y, point.z, 1.0 };

	MMatrix mat{mArray};

	return mat * matrix;
}

MMatrix MeshMultiAttach::computePolygon(MItMeshPolygon &iter, int &index, bool &orient, MMatrix &matrix){
	int dummyIndex;
	MPointArray points;
	iter.setIndex(index, dummyIndex);
	iter.getPoints(points, MSpace::kWorld);
	MPoint point = averageVector(points);

	MVectorArray normals;
	MVector tangent, normal, z;
	if (orient){
		iter.getNormals(normals, MSpace::kWorld);
		normal = averageVector(normals);
		normal.normalize();

		tangent = MVector(points[0] - point);
		tangent.normalize();

		z = normal ^ tangent;
		tangent = z ^ normal;
	}

	double mArray[4][4] = { normal.x, normal.y, normal.z, 0.0,
							tangent.x, tangent.y, tangent.z, 0.0,
							z.x, z.y, z.z, 0.0,
							point.x, point.y, point.z, 1.0 };

	MMatrix mat{mArray};

	return mat * matrix;
}

MPoint MeshMultiAttach::averageVector(MPointArray &points) {
	unsigned int vertexCount = points.length();
	MPoint point;
	for(unsigned int j = 0; j<vertexCount; j++) {
		point += points[j];
	}
	point = point / vertexCount;

	return point;
}

MVector MeshMultiAttach::averageVector(MVectorArray &vectors) {
	unsigned int vertexCount = vectors.length();
	MVector vector;
	for(unsigned int j = 0; j<vertexCount; j++) {
		vector += vectors[j];
	}
	vector = vector / vertexCount;

	return vector;
}
