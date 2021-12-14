# pragma once

#include <maya/MPxNode.h>
#include <maya/MPxDeformerNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnMesh.h>


class CorneaBulge : public MPxDeformerNode {
public:
	CorneaBulge();
	virtual ~CorneaBulge();
    static void* creator();
    static MStatus initialize();
    virtual MPxNode::SchedulingType schedulingType() const override {return MPxNode::kParallel;}
    virtual MStatus deform(MDataBlock &block, MItGeometry &iterator, const MMatrix &matrix, unsigned int multiIndex) override;
public:
	static MTypeId id;
    static MObject centers;
    static MObject collider;
    static MObject distances;
    static MObject indices;
private:
    MMeshIsectAccelParams accelParams;
};
