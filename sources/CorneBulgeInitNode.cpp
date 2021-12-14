
#include "../headers/CorneaBulgeInitNode.h"

#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MMatrix.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnFloatArrayData.h>
#include <maya/MFloatMatrix.h>
#include <maya/MFnSingleIndexedComponent.h>


MTypeId CorneaBulgeInit::id(0x001226EB);
MObject CorneaBulgeInit::inMesh;
MObject CorneaBulgeInit::centers;
MObject CorneaBulgeInit::reference;
MObject CorneaBulgeInit::distances;
MObject CorneaBulgeInit::indices;
MObject CorneaBulgeInit::components;

CorneaBulgeInit::CorneaBulgeInit() {}

CorneaBulgeInit::~CorneaBulgeInit() {}

void* CorneaBulgeInit::creator(){
    return new CorneaBulgeInit();
}


MStatus CorneaBulgeInit::initialize(){
    MStatus stat;
    MFnMatrixAttribute fnMat;
    MFnCompoundAttribute fnComp;
    MFnTypedAttribute fnTyped;

    inMesh = fnTyped.create("inMesh", "mesh", MFnData::kMesh, MObject::kNullObj, &stat);
    CHECK_MSTATUS(stat);
    stat = addAttribute(inMesh);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    centers = fnMat.create("centers", "cen", MFnMatrixAttribute::kFloat, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    fnMat.setKeyable(true);
    fnMat.setArray(true);
    fnMat.setUsesArrayDataBuilder(true);
    stat = addAttribute(centers);

    reference = fnTyped.create("reference", "ref", MFnData::kMesh, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    stat = addAttribute(reference);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    distances = fnTyped.create("distances", "dist", MFnNumericData::kFloatArray, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    fnTyped.setWritable(false);
    fnTyped.setStorable(false);
    stat = addAttribute(distances);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    indices = fnTyped.create("indices", "ids", MFnNumericData::kIntArray, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    fnTyped.setWritable(false);
    fnTyped.setStorable(false);
    stat = addAttribute(indices);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    components = fnTyped.create("components", "ics", MFnComponentListData::kComponentList);
	fnTyped.setStorable(true);   // To be stored during file-save
    addAttribute(components);

    attributeAffects(inMesh, distances);
    attributeAffects(centers, distances);
    attributeAffects(reference, distances);
    attributeAffects(components, distances);

    attributeAffects(inMesh, indices);
    attributeAffects(centers, indices);
    attributeAffects(reference, indices);
    attributeAffects(components, indices);

	return MS::kSuccess;
}


MStatus CorneaBulgeInit::compute(const MPlug& plug, MDataBlock& data){
    // Inputs
    MStatus stat;
    MObject inMesh = data.inputValue(CorneaBulgeInit::inMesh).asMesh();
    MArrayDataHandle centers = data.inputArrayValue(CorneaBulgeInit::centers);
    // MPlug cpListPlug = MPlug(node, CorneaBulgeInit::components);
    MObject compList = data.inputValue(CorneaBulgeInit::components).data();
    // MObject compList = cpListPlug.asMObject();
    MFnComponentListData compListFn(compList);

    MFnMesh fnMesh (inMesh, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    unsigned int numVertices = (unsigned)fnMesh.numVertices();
    MFloatArray distances(numVertices, 9999.9f);

    // create an array of the centers
    MFloatPointArray inputCenters(centers.elementCount());
    for (unsigned int j=0; j < centers.elementCount(); ++j) {
        inputCenters[j] = centers.inputValue().asFloatMatrix()[3];
        centers.next();
    }

    // get the vtx positions
    MFloatPointArray positions;
    stat = fnMesh.getPoints(positions, MSpace::kWorld);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    MObject reference = data.inputValue(CorneaBulgeInit::reference, &stat).asMesh();
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    stat = fnMesh.setObject(reference);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    MIntArray indices(numVertices, 999);

    this->computeDistancesAndIndices(fnMesh, inputCenters, positions, indices, distances, compListFn, stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    // Output
    MFnFloatArrayData fnFloatArray;
    MObject outD = fnFloatArray.create(distances, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    data.outputValue(CorneaBulgeInit::distances).setMObject(outD);
    data.setClean(CorneaBulgeInit::distances);

    MFnIntArrayData fnIntArray;
    MObject outI = fnIntArray.create(indices, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    data.outputValue(CorneaBulgeInit::indices).setMObject(outI);
    data.setClean(CorneaBulgeInit::indices);

	return MS::kSuccess;
}


MStatus CorneaBulgeInit::computeDistancesAndIndices(MFnMesh &fnMesh, const MFloatPointArray &inputCenters, const MFloatPointArray &positions,
                                             MIntArray &indices, MFloatArray &distances, const MFnComponentListData &compListFn, MStatus &stat) {
    MVector rayDir;
    MFloatPoint hitPoint;
    float rayDistance;
    this->accelParams = fnMesh.autoUniformGridParams();
    MFn::Type componentType = MFn::kMeshVertComponent;

    for (unsigned int i=0; i < inputCenters.length(); ++i) {
        for (unsigned int j=0; j < compListFn.length(); ++j){
            MObject comp = compListFn[j];
            if (comp.apiType() != componentType)
                continue;

            MFnSingleIndexedComponent siComp(comp);
            for (int k = 0; k < siComp.elementCount(); k++) {
                int pointIndex = siComp.element(k);

                rayDir = positions[pointIndex] - inputCenters[i];
                fnMesh.closestIntersection(inputCenters[i], rayDir.normal(), nullptr, nullptr, false, MSpace::kWorld, 99999.9f, false,
                                           &this->accelParams, hitPoint, &rayDistance, nullptr, nullptr, nullptr, nullptr, 0.001, &stat);
                CHECK_MSTATUS_AND_RETURN_IT(stat);

                if (distances[pointIndex] < (positions[pointIndex] - hitPoint).length())
                    continue;

                distances[pointIndex] = (positions[pointIndex] - hitPoint).length();
                indices[pointIndex] = i;

                if (rayDir.length() < rayDistance)
                    distances[pointIndex] *= -1.0f;
            }
        }
    }

    return MS::kSuccess;
}



























