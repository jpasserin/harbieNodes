#pragma  once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 

 
class AbsRotationTracker : public MPxNode
{
public:
	AbsRotationTracker();
	virtual ~AbsRotationTracker();
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute( const MPlug& plug, MDataBlock& data );

public:
	static MTypeId id;
	static MObject axis;
	static MObject reference;
	static MObject tracker;
	static MObject outputA;
	static MObject outputB;
};
