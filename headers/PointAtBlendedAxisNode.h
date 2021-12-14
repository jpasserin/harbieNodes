
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

 
class PointAtBlendedAxis : public MPxNode {
public:
	PointAtBlendedAxis();
	virtual ~PointAtBlendedAxis();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute( const MPlug &plug, MDataBlock &data );

public:
	static MTypeId id;
	static MObject mA;
	static MObject mB;
	static MObject blend;
	static MObject axis;
	static MObject out;
};

