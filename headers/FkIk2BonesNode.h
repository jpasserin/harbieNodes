#pragma once

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 
#include <maya/MTransformationMatrix.h>

 
class FkIk2Bones : public MPxNode {
public:
	FkIk2Bones();
	virtual	~FkIk2Bones();
	virtual MPxNode::SchedulingType schedulingType() const override { return MPxNode::kParallel; }
	virtual MStatus	compute( const MPlug &plug, MDataBlock &data );
	static void *creator();
	static MStatus initialize();
	void computeFK(const MTransformationMatrix &fkA_tfm, const MTransformationMatrix &fkB_tfm,
                   const MTransformationMatrix &fkC_tfm,
                   double &lengthA, double &lengthB, const bool &negate, MTransformationMatrix &boneA_tfm,
                   MTransformationMatrix &boneB_tfm, MTransformationMatrix &boneC_tfm);

	void computeIK(const MTransformationMatrix &rootTfm, const MTransformationMatrix &effector,
                   const MTransformationMatrix &upVector, const double &lengthA, const double &lengthB,
                   const double &roll, const double &scaleA, const double &scaleB, const double &maxStretch,
                   const double &slide, const double &reverse, const bool &negate, double &softness,
                   MTransformationMatrix &boneA_tfm, MTransformationMatrix &boneB_tfm,
                   MTransformationMatrix &boneC_tfm);


    void computeIkStretch(const double &lengthA, const double &lengthB, const double &scaleA, const double &scaleB,
		const double &maxStretch, double &softness, double &distance,
		const double &slide, const double &reverse, const double globalScale,
		double &outA, double &outB) const;


    double computeIkAngle(const double distance, const double boneA_length, const double boneB_length, const bool invert) ;




public:
	static MTypeId id;
	static MObject blend;
	static MObject lengthA;
	static MObject lengthB;
	static MObject roll;
	static MObject scaleA;
	static MObject scaleB;
	static MObject maxStretch;
	static MObject softness;
	static MObject slide;
	static MObject reverse;
	static MObject negate;

	static MObject fkA;
	static MObject fkB;
	static MObject fkC;
	static MObject root;
	static MObject effector;
	static MObject upv;

    static MObject outBoneA;
	static MObject outBoneB;
	static MObject outBoneC;

	static MObject boneATfm;
	static MObject boneBTfm;
	static MObject boneCTfm;

	static MObject boneAPos;
	static MObject boneAPosX;
	static MObject boneAPosY;
	static MObject boneAPosZ;

	static MObject boneARot;
	static MObject boneARotX;
	static MObject boneARotY;
	static MObject boneARotZ;

	static MObject boneAScl;
	static MObject boneASclX;
	static MObject boneASclY;
	static MObject boneASclZ;

	static MObject boneBPos;
	static MObject boneBPosX;
	static MObject boneBPosY;
	static MObject boneBPosZ;

	static MObject boneBRot;
	static MObject boneBRotX;
	static MObject boneBRotY;
	static MObject boneBRotZ;

	static MObject boneBScl;
	static MObject boneBSclX;
	static MObject boneBSclY;
	static MObject boneBSclZ;

	static MObject boneCPos;
	static MObject boneCPosX;
	static MObject boneCPosY;
	static MObject boneCPosZ;

	static MObject boneCRot;
	static MObject boneCRotX;
	static MObject boneCRotY;
	static MObject boneCRotZ;

	static MObject boneCScl;
	static MObject boneCSclX;
	static MObject boneCSclY;
	static MObject boneCSclZ;

};
