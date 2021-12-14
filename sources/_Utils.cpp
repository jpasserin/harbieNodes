
#include "../headers/_Utils.h"
#include <maya/MTransformationMatrix.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

double max(double a, double b) {
	return (a < b) ? b : a;
}
double min(double a, double b) {
	return (a < b) ? a : b;
}

double toDegrees(const double &radians) {
	return (radians * 180.0) / M_PI;
}

double toRadians(const double &degrees) {
	return (degrees * M_PI) / 180.0;
}


MTransformationMatrix mapWorldPoseToObjectSpace(MTransformationMatrix objectSpace, MTransformationMatrix pose) {
	return MTransformationMatrix(pose.asMatrix() * objectSpace.asMatrixInverse());
}

MTransformationMatrix  mapObjectPoseToWorldSpace(MTransformationMatrix objectSpace, MTransformationMatrix pose) {
	return MTransformationMatrix(pose.asMatrix() * objectSpace.asMatrix());
}

MTransformationMatrix interpolateTransform(MTransformationMatrix xf1, MTransformationMatrix xf2, double blend) {
	if (blend == 1.0)
		return xf2;
	else if (blend == 0.0)
		return xf1;

	// translate
	MVector t = linearInterpolate(xf1.getTranslation(MSpace::kWorld), xf2.getTranslation(MSpace::kWorld), blend);

	// scale
	double threeDoubles[3];
	xf1.getScale(threeDoubles, MSpace::kWorld);
	MVector xf1_scl(threeDoubles[0], threeDoubles[1], threeDoubles[2]);

	xf2.getScale(threeDoubles, MSpace::kWorld);
	MVector xf2_scl(threeDoubles[0], threeDoubles[1], threeDoubles[2]);

	MVector vs = linearInterpolate(xf1_scl, xf2_scl, blend);
	double scale[3] = { vs.x, vs.y, vs.z };

	// rotate
	MQuaternion q = slerp(xf1.rotation(), xf2.rotation(), blend);
	// MQuaternion q = quaternionSlerp(xf1.rotation(), xf2.rotation(), blend);

	// out
	MTransformationMatrix result;

	result.setTranslation(t, MSpace::kWorld);
	result.setRotationQuaternion(q.x, q.y, q.z, q.w);
	result.setScale(scale, MSpace::kWorld);

	return result;
}

MVector linearInterpolate(MVector v0, MVector v1, double blend) {
	MVector v;
	v.x = linearInterpolate(v0.x, v1.x, blend);
	v.y = linearInterpolate(v0.y, v1.y, blend);
	v.z = linearInterpolate(v0.z, v1.z, blend);

	return v;
}

double linearInterpolate(double first, double second, double blend) {
	return first * (1 - blend) + second * blend;
}

MVector rotateVectorAlongAxis(MVector v, MVector axis, double a) {
	// Angle as to be in radians

	double sa = sin(a / 2.0);
	double ca = cos(a / 2.0);

	MQuaternion q1 = MQuaternion(v.x, v.y, v.z, 0);
	MQuaternion q2 = MQuaternion(axis.x * sa, axis.y * sa, axis.z * sa, ca);
	MQuaternion q2n = MQuaternion(-axis.x * sa, -axis.y * sa, -axis.z * sa, ca);
	MQuaternion q = q2 * q1;
	q *= q2n;

	return MVector(q.x, q.y, q.z);
}

// -------------------------------------------------------------------
// QUATERNIONS
// -------------------------------------------------------------------
MQuaternion quaternionSlerp(const MQuaternion &a, const MQuaternion &b, double t) {
	double cosq = quaternionDot(a, b);
	if (cosq < 0.0)
		cosq = quaternionDot(a, -b);
	double theta = acos(cosq);
	double sinq = sin(theta);

	double w1, w2;
	if (sinq > 0.001) {
		w1 = sin((1.0 - t) * theta) / sinq;
		w2 = sin(t * theta) / sinq;
	}
	else {
		w1 = 1.0 - t;
		w2 = t;
	}
	return (w1 * a) + (w2 * b);
}

double quaternionDot(const MQuaternion &q1, const MQuaternion &q2) {
	return (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
}

MQuaternion quatFromMatrix(const MMatrix &tfm) {
	double x, y, z, w;
	w = std::sqrt(max(0.0, 1.0 + tfm[0][0] + tfm[1][1] + tfm[2][2])) / 2.0;
	x = std::sqrt(max(0.0, 1.0 + tfm[0][0] - tfm[1][1] - tfm[2][2])) / 2.0;
	y = std::sqrt(max(0.0, 1.0 - tfm[0][0] + tfm[1][1] - tfm[2][2])) / 2.0;
	z = std::sqrt(max(0.0, 1.0 - tfm[0][0] - tfm[1][1] + tfm[2][2])) / 2.0;
	x = std::copysign(x, tfm[1][2] - tfm[2][1]);
	y = std::copysign(y, tfm[2][0] - tfm[0][2]);
	z = std::copysign(z, tfm[0][1] - tfm[1][0]);

	return MQuaternion(x, y, z, w);
}


// -------------------------------------------------------------------
// NURBS
// -------------------------------------------------------------------
MStatus rescaleParams(MDoubleArray &params, short &attach, double &start, double &end, double &slide,
					double &reverse, double &ratio, bool &isLoop){
	double range = end - start;
	for (unsigned int i=0; i < params.length(); i++){
		if (attach == 2) { // FixedLength
			params[i] *= ratio;

			// Start End Range, needs to take into account ratio
			params[i] = (params[i] * range + start * ratio) + slide;

			// Reverse : Locking to the end of the surface
			double reverseParam = params[i] + (1 - ratio);
			params[i] = params[i] * (1-reverse) + reverseParam * reverse;
		}
		else // Start End Range
			params[i] = (params[i] * range + start) + slide;

		// Loop
		if (isLoop) {
			if (params[i] > 1.0) params[i] = std::fmod(params[i], 1.0);
		}
		else
			params[i] = min(1.0, max(0.0, params[i]));
	}
	
	return MS::kSuccess;
}

double findParamFromPercentage(double &percentage, MDoubleArray &samples, unsigned int &startIndex) {
	// param is return as normalized param (0-1)
	unsigned int sampling = samples.length();
	for (unsigned int i=startIndex; i < sampling; i++){
		if (percentage >= samples[i] && percentage <= samples[i+1]){
			double linear = (percentage - samples[i]) / (samples[i+1] - samples[i]);
			return (i + linear) / (sampling - 1.0 );
			startIndex = i;
		}
	}
	return 0.0;
}

MStatus sampleSurface(MFnNurbsSurface &surface, MDoubleArray &samples, double &length) {
	unsigned int sampling = samples.length();

	double uStartDomain, uEndDomain, vStartDomain, vEndDomain;
	surface.getKnotDomain(uStartDomain, uEndDomain, vStartDomain, vEndDomain);
	double uRangeDomain = uEndDomain - uStartDomain;
	double vRangeDomain = vEndDomain - vStartDomain;

	// We always sample surface at 0.5 in V
	double vParam = 0.5 * vRangeDomain + vStartDomain;

	double step = 1.0 / (sampling-1);
	MDoubleArray sampleLength(sampling);
	MPoint position, previousPosition;
	for (unsigned int i=0; i<sampling; i++){
		double uParam = (step * i) * uRangeDomain + uStartDomain;

		surface.getPointAtParam(uParam, vParam, position, MSpace::Space::kWorld);

		if (i > 0)
			length += position.distanceTo(previousPosition);
		else
			length = 0;

		sampleLength[i] = length;
		previousPosition = position;
	}

	for (unsigned int i=0; i<sampling; i++) 
		samples[i] = sampleLength[i] / length;

	return MS::kSuccess;
}
	




// -------------------------------------------------------------------
// NURBS
// -------------------------------------------------------------------
MStatus getShapeNode(MDagPath& path, bool intermediate) {
  MStatus status;

  if (isShapeNode(path)) {
    // Start at the transform so we can honor the intermediate flag.
    path.pop();
  }

  if (path.hasFn(MFn::kTransform)) {
    unsigned int shapeCount = path.childCount();

    for (unsigned int i = 0; i < shapeCount; ++i) {
      status = path.push(path.child(i));
      CHECK_MSTATUS_AND_RETURN_IT(status);
      if (!isShapeNode(path)) {
        path.pop();
        continue;
      }

      MFnDagNode fnNode(path, &status);
      CHECK_MSTATUS_AND_RETURN_IT(status);
      if ((!fnNode.isIntermediateObject() && !intermediate) ||
          (fnNode.isIntermediateObject() && intermediate)) {
        return MS::kSuccess;
      }
      // Go to the next shape
      path.pop();
    }
  }

  // No valid shape node found.
  return MS::kFailure;
}



bool isShapeNode(MDagPath& path) {
  return path.node().hasFn(MFn::kMesh) ||
         path.node().hasFn(MFn::kNurbsCurve) ||
         path.node().hasFn(MFn::kNurbsSurface);
}





// -------------------------------------------------------------------
// PLUGS
// -------------------------------------------------------------------
bool isConnected(const MPlug& plug, bool upstream, bool downstream){
	MStatus stat;
	MPlugArray plugs;
	plug.connectedTo(plugs, upstream, downstream, &stat);
	return plugs.length() > 0;
}
