
#include "../headers/_Utils.h"
#include "../headers/SurfaceInfoCmd.h"


#include <maya/MArgList.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MGlobal.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>

const char* SurfaceInfoCmd::kName = "surfaceInfo";
const char* SurfaceInfoCmd::kQueryFlagShort = "-q";
const char* SurfaceInfoCmd::kQueryFlagLong = "-query";
const char* SurfaceInfoCmd::kLengthFlagShort = "-l";
const char* SurfaceInfoCmd::kLengthFlagLong = "-length";
const char* SurfaceInfoCmd::kParamFromPercentageFlagShort = "-pfp";
const char* SurfaceInfoCmd::kParamFromPercentageFlagLong = "-paramFromPercentage";
const char* SurfaceInfoCmd::kHelpFlagShort = "-h";
const char* SurfaceInfoCmd::kHelpFlagLong = "-help";

void DisplayHelp() {
  MString help;
  help += "Flags:\n"; 
  help += "-query (-q):           N/A        Query mode.\n";
  help += "-length (-l):     	    N/A        Return the u length of the surface\n";
  help += "-paramFromPercentage (-pfp):     	    N/A        Return the u param for given length percentage\n";
  help += "-help (-h)             N/A        Display this text.\n";
  MGlobal::displayInfo(help);
}

MSyntax SurfaceInfoCmd::newSyntax() {
  MSyntax syntax;
  syntax.addFlag(kQueryFlagShort, kQueryFlagLong);
  syntax.addFlag(kLengthFlagShort, kLengthFlagLong);
  syntax.addFlag(kParamFromPercentageFlagShort, kParamFromPercentageFlagLong, MSyntax::kDouble);  
  syntax.addFlag(kHelpFlagShort, kHelpFlagLong);
  syntax.setObjectType(MSyntax::kSelectionList, 0, 255);
  syntax.useSelectionAsDefault(true);
  return syntax;
}

SurfaceInfoCmd::SurfaceInfoCmd()
    : 
      command_(kCommandQuery),
    queryLength_(false),
	queryParam_(false),
    percentage_(0.0)
{}
SurfaceInfoCmd::~SurfaceInfoCmd(){}

void* SurfaceInfoCmd::creator(){
   return new SurfaceInfoCmd;
}

bool SurfaceInfoCmd::isUndoable() const{
   return false;
}
MStatus SurfaceInfoCmd::undoIt(){
	MStatus stat;
	return stat;// undo not implemented
}

MStatus SurfaceInfoCmd::doIt(const MArgList& args){
	MStatus stat = MStatus::kSuccess;

	stat = gatherCommandArguments(args);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = getGeometryPaths();
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	if (queryLength_){
		MFnNurbsSurface surface(surfaceQuery_);
		MDoubleArray samples(1000);
		double length;
		sampleSurface(surface, samples, length);
		
		setResult(static_cast<float>(length));
	}
	else if (queryParam_){
    MFnNurbsSurface surface(surfaceQuery_);
    MDoubleArray samples(1000);
    double length;
    sampleSurface(surface, samples, length);
    unsigned int startIndex = 0;
    double param = findParamFromPercentage(percentage_, samples, startIndex);

    setResult(static_cast<float>(param));
  }

	return stat;
}

MStatus SurfaceInfoCmd::gatherCommandArguments(const MArgList& args) {
  MStatus stat;
  MArgDatabase argData(syntax(), args);
  argData.getObjects(selectionList_);
  if (argData.isFlagSet(kHelpFlagShort)) {
    command_ = kCommandHelp;
    DisplayHelp();
    return MS::kSuccess;
  }

  if (argData.isFlagSet(kQueryFlagShort))
	  command_ = kCommandQuery;

  if (argData.isFlagSet(kLengthFlagShort))
	  queryLength_ = true;
  else if (argData.isFlagSet(kParamFromPercentageFlagShort)) {
    argData.getFlagArgument(kParamFromPercentageFlagShort, 0, percentage_);
    queryParam_ = true;
  }
  return MS::kSuccess;
}

MStatus SurfaceInfoCmd::getGeometryPaths() {
  MStatus stat;
  if (command_ == kCommandHelp)
   return MS::kSuccess;

  if (selectionList_.length() == 0 ) {
	  MGlobal::displayError("select at least a surface");
	  return MS::kFailure;
  }

  if (command_ == kCommandQuery)
  {	 
	  // The driver is selected last
	  stat = selectionList_.getDagPath(0, surfaceQuery_);
	  stat = getShapeNode(surfaceQuery_);
	  // The driver must be a mesh for this specific algorithm.
	  if (!surfaceQuery_.hasFn(MFn::kNurbsSurface)) {
		  MGlobal::displayError("query works only on surface.");
		  return MS::kFailure;
	  }
  }

  return MS::kSuccess;
}