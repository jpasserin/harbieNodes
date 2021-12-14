
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class BeanRemote : public MPxNode {
public:
	BeanRemote();
	virtual ~BeanRemote();
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;
	MStatus computeDirection(const MPlug& pPlug, const MPlug& nPlug, double value, MDataBlock& data);
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }

public:
	static MTypeId id;

	static MObject x;
	static MObject inputX;
	static MObject minX;
	static MObject maxX;
	static MObject clampMinX;
	static MObject clampMaxX;

	static MObject y;
	static MObject inputY;
	static MObject minY;
	static MObject maxY;
	static MObject clampMinY;
	static MObject clampMaxY;

	static MObject output;
	static MObject north;
	static MObject south;
	static MObject east;
	static MObject west;
};
