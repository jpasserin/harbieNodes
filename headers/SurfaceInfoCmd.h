
#include <maya/MPxCommand.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>

class SurfaceInfoCmd : public MPxCommand
{
  	public:
  		enum CommandMode { kCommandQuery, kCommandHelp};
		SurfaceInfoCmd();
		virtual ~SurfaceInfoCmd();
		static void* creator();
		bool isUndoable() const;
		MStatus doIt(const MArgList&);
		MStatus undoIt();

		static MSyntax newSyntax();

		const static char* kName;

		const static char* kQueryFlagShort;
		const static char* kQueryFlagLong;

		const static char* kLengthFlagShort;
		const static char* kLengthFlagLong;

		const static char* kParamFromPercentageFlagShort;
		const static char* kParamFromPercentageFlagLong;

		const static char* kHelpFlagShort;
		const static char* kHelpFlagLong;

	private:
		MStatus gatherCommandArguments(const MArgList& args);
 		MStatus getGeometryPaths();

		MSelectionList selectionList_;
 		CommandMode command_;

  		MDagPath surfaceQuery_;

  		bool queryLength_;
  		bool queryParam_;
  		double percentage_;
};
