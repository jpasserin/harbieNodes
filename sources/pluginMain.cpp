
#include <maya/MFnPlugin.h>
#include "../headers/AbsRotationTrackerNode.h"
#include "../headers/CurveMultiAttachNode.h"
#include "../headers/FkIk2BonesNode.h"
#include "../headers/InverseRotOrderNode.h"
#include "../headers/PointAtBlendedAxisNode.h"
#include "../headers/PointAtDoubleAxisNode.h"
#include "../headers/PreviewPlaneNode.h"
#include "../headers/rotationToSliderNode.h"
#include "../headers/RotationTrackerNode.h"
#include "../headers/SpinePointAtNode.h"
#include "../headers/MeshMultiAttachNode.h"
#include "../headers/SurfaceMultiAttachNode.h"
#include "../headers/SurfaceInfoCmd.h"
#include "../headers/BeanRemoteNode.h"
#include "../headers/PoseConstraintNode.h"
#include "../headers/CorneaBulgeInitNode.h"
#include "../headers/CorneaBulgeNode.h"
#include "../headers/SplitMatrixNode.h"


MStatus initializePlugin(MObject obj)
{ 
	MStatus   status;
	MFnPlugin plugin(obj, "Blur Studio", "1.0", "Any");

	// NODES
	status = plugin.registerNode("AbsRotationTracker", AbsRotationTracker::id, AbsRotationTracker::creator, AbsRotationTracker::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("FkIk2Bones", FkIk2Bones::id, FkIk2Bones::creator, FkIk2Bones::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("InverseRotOrder", InverseRotOrder::id, InverseRotOrder::creator, InverseRotOrder::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("PointAtBlendedAxis", PointAtBlendedAxis::id, PointAtBlendedAxis::creator, PointAtBlendedAxis::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("PointAtDoubleAxis", PointAtDoubleAxis::id, PointAtDoubleAxis::creator, PointAtDoubleAxis::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("PreviewPlane", PreviewPlane::id, PreviewPlane::creator, PreviewPlane::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("RotationToSlider", RotationToSlider::id, RotationToSlider::creator, RotationToSlider::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("RotationTracker", RotationTracker::id, RotationTracker::creator, RotationTracker::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("SpinePointAt", SpinePointAt::id, SpinePointAt::creator, SpinePointAt::initialize);
		if (!status) {status.perror("registerNode"); return status;}

	status = plugin.registerNode("CurveMultiAttach", CurveMultiAttach::id, CurveMultiAttach::creator, CurveMultiAttach::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("MeshMultiAttach", MeshMultiAttach::id, MeshMultiAttach::creator, MeshMultiAttach::initialize);
		if (!status) {status.perror("registerNode"); return status;}
	status = plugin.registerNode("SurfaceMultiAttach", SurfaceMultiAttach::id, SurfaceMultiAttach::creator, SurfaceMultiAttach::initialize);
		if (!status) { status.perror("registerNode"); return status; }
		
	status = plugin.registerNode("PoseConstraint", PoseConstraint::id, PoseConstraint::creator, PoseConstraint::initialize);
		if (!status) { status.perror("registerNode"); return status; }

	status = plugin.registerNode("BeanRemote", BeanRemote::id, BeanRemote::creator, BeanRemote::initialize);
		if (!status) { status.perror("registerNode"); return status; }

	status = plugin.registerNode("CorneaBulgeInit", CorneaBulgeInit::id, CorneaBulgeInit::creator, CorneaBulgeInit::initialize);
		if (!status) { status.perror("registerNode"); return status; }
	status = plugin.registerNode("CorneaBulge", CorneaBulge::id, CorneaBulge::creator,CorneaBulge::initialize, MPxNode::kDeformerNode);
		if (!status) { status.perror("registerNode"); return status; }

	status = plugin.registerNode("SplitMatrix", SplitMatrix::id, SplitMatrix::creator, SplitMatrix::initialize);
		if (!status) { status.perror("registerNode"); return status; }

	// COMMANDS
	status = plugin.registerCommand(SurfaceInfoCmd::kName, SurfaceInfoCmd::creator, SurfaceInfoCmd::newSyntax);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	// NODES
	status = plugin.deregisterNode(AbsRotationTracker::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(FkIk2Bones::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(InverseRotOrder::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(PointAtBlendedAxis::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(PointAtDoubleAxis::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(PreviewPlane::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(RotationToSlider::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(RotationTracker::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(SpinePointAt::id);
		if (!status) {status.perror("deregisterNode"); return status;}

	status = plugin.deregisterNode(CurveMultiAttach::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(MeshMultiAttach::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(SurfaceMultiAttach::id);
		if (!status) {status.perror("deregisterNode"); return status;}
		
	status = plugin.deregisterNode(PoseConstraint::id);
		if (!status) {status.perror("deregisterNode"); return status;}

	status = plugin.deregisterNode(BeanRemote::id);
		if (!status) {status.perror("deregisterNode"); return status;}

	status = plugin.deregisterNode(CorneaBulgeInit::id);
		if (!status) {status.perror("deregisterNode"); return status;}
	status = plugin.deregisterNode(CorneaBulge::id);
		if (!status) {status.perror("deregisterNode"); return status;}

	status = plugin.deregisterNode(SplitMatrix::id);
		if (!status) {status.perror("deregisterNode"); return status;}

	// COMMANDS
	status = plugin.deregisterCommand(SurfaceInfoCmd::kName);

	return status;
}
