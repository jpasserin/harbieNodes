#pragma once

#include <maya/MPxNode.h>
#include <maya/MPointArray.h>
#include <vector>
#include <maya/MFnMesh.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnComponentListData.h>


class CorneaBulgeInit : MPxNode {
public:
	CorneaBulgeInit();
	virtual ~CorneaBulgeInit();
    static void* creator();
    static MStatus initialize();
    virtual MPxNode::SchedulingType schedulingType() const override {return MPxNode::kParallel;}
    virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

public:
	static MTypeId id;
    static MObject inMesh;
    static MObject centers;
    static MObject reference;
    static MObject distances;
    static MObject indices;
    static MObject components; // list of points that should be computed

private:
    MStatus computeDistancesAndIndices(MFnMesh &fnMesh, const MFloatPointArray &inputCenters, const MFloatPointArray &positions,
                                           MIntArray &indices, MFloatArray &distances, const MFnComponentListData &compListFn, MStatus &stat);
private:
    MMeshIsectAccelParams accelParams;
};

