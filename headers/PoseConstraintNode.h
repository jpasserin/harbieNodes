
#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h>
#include <vector>
#include <maya/MEulerRotation.h>


class PoseConstraint : public MPxNode {
public:
	PoseConstraint();
	virtual ~PoseConstraint();
	static void* creator();
	static MStatus initialize();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; };
	virtual MStatus compute( const MPlug& plug, MDataBlock& data );
public:
	static MTypeId id;
	static MObject input;
    static MObject blend;
    static MObject worldMatrix;
    static MObject parentInverseMatrix;

    static MObject localOffset; // DEPRECATED

    static MObject localOffsetTransform;
	static MObject localOffsetTranslate;
	static MObject localOffsetTranslateX;
	static MObject localOffsetTranslateY;
	static MObject localOffsetTranslateZ;
	static MObject localOffsetRotate;
	static MObject localOffsetRotateX;
	static MObject localOffsetRotateY;
	static MObject localOffsetRotateZ;
	static MObject localOffsetScale;
	static MObject localOffsetScaleX;
	static MObject localOffsetScaleY;
	static MObject localOffsetScaleZ;
	static MObject localOffsetShear;
	static MObject localOffsetShearX;
	static MObject localOffsetShearY;
	static MObject localOffsetShearZ;
	static MObject localOffsetRotateOrder;

    static MObject rotateOrder;
    static MTransformationMatrix::RotationOrder rotationOrders[6];
    static MObject offset;

    static MObject output;
    static MObject translate;
    static MObject translateX;
    static MObject translateY;
    static MObject translateZ;
    static MObject rotate;
    static MObject rotateX;
    static MObject rotateY;
    static MObject rotateZ;
    static MObject scale;
    static MObject scaleX;
    static MObject scaleY;
    static MObject scaleZ;
    static MObject shear;
    static MObject shearX;
    static MObject shearY;
    static MObject shearZ;
};
