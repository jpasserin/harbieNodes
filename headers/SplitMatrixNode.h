
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

class SplitMatrix : public MPxNode {
public:
	SplitMatrix();
	virtual ~SplitMatrix();
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }

public:
	static MTypeId id;

	static MObject aInMatrix;

	static MObject aOut00;
	static MObject aOut01;
	static MObject aOut02;
	static MObject aOut03;
	static MObject aOut10;
	static MObject aOut11;
	static MObject aOut12;
	static MObject aOut13;
	static MObject aOut20;
	static MObject aOut21;
	static MObject aOut22;
	static MObject aOut23;
	static MObject aOut30;
	static MObject aOut31;
	static MObject aOut32;
	static MObject aOut33;

};
