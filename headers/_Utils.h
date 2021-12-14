
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsSurface.h>

double max(double a, double b);
double min(double a, double b);
double linearInterpolate(double first, double second, double blend);
double toDegrees(const double &radians);
double toRadians(const double &degrees);

MTransformationMatrix mapWorldPoseToObjectSpace(MTransformationMatrix objectSpace, MTransformationMatrix pose);
MTransformationMatrix mapObjectPoseToWorldSpace(MTransformationMatrix objectSpace, MTransformationMatrix pose);
MTransformationMatrix interpolateTransform(MTransformationMatrix xf1, MTransformationMatrix xf2, double blend);

MVector linearInterpolate(MVector v0, MVector v1, double blend);
MVector rotateVectorAlongAxis(MVector v, MVector axis, double a);

MQuaternion quaternionSlerp(const MQuaternion &a, const MQuaternion &b, double t);
MQuaternion quatFromMatrix(const MMatrix &tfm);
double quaternionDot(const MQuaternion &q1, const MQuaternion &q2);

MStatus rescaleParams(MDoubleArray &params, short &attach, double &start, double &end, double &slide,
					double &reverse, double &ratio, bool &isLoop);
double findParamFromPercentage(double &percentage, MDoubleArray &samples, unsigned int &startIndex);
MStatus sampleSurface(MFnNurbsSurface &surface, MDoubleArray &samples, double &length);

MStatus getShapeNode(MDagPath& path, bool intermediate=false);
bool isShapeNode(MDagPath& path);

bool isConnected(const MPlug& plug, bool upstream, bool downstream);
