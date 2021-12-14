
#include "../headers/CorneaBulgeNode.h"

#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MFnFloatArrayData.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatArray.h>
#include <maya/MFloatMatrix.h>
#include <maya/MItGeometry.h>
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MMatrix.h>

MTypeId CorneaBulge::id(0x001226EA);
MObject CorneaBulge::centers;
MObject CorneaBulge::collider;
MObject CorneaBulge::distances;
MObject CorneaBulge::indices;

CorneaBulge::CorneaBulge() {}

CorneaBulge::~CorneaBulge() {}

void* CorneaBulge::creator(){
    return new CorneaBulge();
}


MStatus CorneaBulge::initialize(){
    MStatus stat;
    MFnMatrixAttribute fnMat;
    MFnTypedAttribute fnTyped;

    centers = fnMat.create("centers", "cen", MFnMatrixAttribute::kFloat, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    fnMat.setKeyable(true);
    fnMat.setArray(true);
    fnMat.setUsesArrayDataBuilder(true);
    stat = addAttribute(CorneaBulge::centers);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    collider = fnTyped.create("collider", "col", MFnData::kMesh, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    stat = addAttribute(CorneaBulge::collider);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    distances = fnTyped.create("distances", "dist", MFnNumericData::kFloatArray, MObject::kNullObj, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    stat = addAttribute(distances);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    indices = fnTyped.create("indices", "id", MFnNumericData::kIntArray, MObject::kNullObj, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    stat = addAttribute(indices);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    attributeAffects(centers, outputGeom);
    attributeAffects(collider, outputGeom);
    attributeAffects(distances, outputGeom);
    attributeAffects(indices, outputGeom);

    MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer CorneaBulge weights;");

	return MS::kSuccess;
}


MStatus CorneaBulge::deform(MDataBlock &block, MItGeometry &iterator, const MMatrix &matrix, unsigned int multiIndex){
    MStatus stat;

    MObject collider = block.inputValue(CorneaBulge::collider).asMesh();
    MFnMesh fnMesh (collider, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);

    float envelope = block.inputValue(CorneaBulge::envelope).asFloat();
    if (!envelope)
        return MS::kSuccess;

    MArrayDataHandle centers = block.inputArrayValue(CorneaBulge::centers);
    unsigned int numElements = centers.elementCount();

    MFloatPointArray centerPositions(numElements);
    for (unsigned int j=0; j < numElements; ++j) {
        centerPositions[j] = centers.inputValue().asFloatMatrix()[3];
        centers.next();
    }

    MObject distArrayObj = block.inputValue(CorneaBulge::distances).data();
    MFnFloatArrayData fnFloatArray (distArrayObj, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    MFloatArray distances = fnFloatArray.array();
    if (distances.length() < iterator.count())
        return MS::kInvalidParameter;

    MObject idArrayObj = block.inputValue(CorneaBulge::indices).data();
    MFnIntArrayData fnIntArray (idArrayObj, &stat);
    CHECK_MSTATUS_AND_RETURN_IT(stat);
    MIntArray centerIndices = fnIntArray.array();
    if (centerIndices.length() < iterator.count())
        return MS::kInvalidParameter;

    this->accelParams = fnMesh.autoUniformGridParams();
    float rayDistance = 0.0f;
    float weight;
    MVector rayDir;
    MFloatPoint pt;
    MFloatPoint hitPoint;
    for (unsigned int id; !iterator.isDone(); iterator.next()){
        id = (unsigned)iterator.index();

        weight = this->weightValue(block, multiIndex, id);
        if (weight < 0.0001)
            continue;

        if (centerIndices[id] > (numElements - 1))
            continue;

        pt = iterator.position();
        rayDir = (pt - centerPositions[centerIndices[id]]).normal();
        fnMesh.closestIntersection(centerPositions[centerIndices[id]], rayDir, nullptr, nullptr, false, MSpace::kWorld, 9999.9, false,
                                   &this->accelParams, hitPoint, &rayDistance, nullptr, nullptr, nullptr, nullptr, 0.001, &stat);
        if (MFAIL(stat))
            continue;

        // TODO: Deal with local/world space conversions

        hitPoint += rayDir * distances[id];
        iterator.setPosition(pt + ((hitPoint - pt) * weight * envelope));
    }

    return MS::kSuccess;
}
