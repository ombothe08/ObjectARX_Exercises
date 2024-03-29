#include "pch.h"
#include "framework.h"
#include "arxHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void inputLine()
{
	// Get the current space block
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTableRecord* pBlockTableRecord;
	Acad::ErrorStatus es = acdbOpenObject(pBlockTableRecord,
		pDb->currentSpaceId(),
		AcDb::kForWrite);
	ads_point pt1, pt2;
	// Prompt for the first point
	if (RTNORM == acedGetPoint(NULL, _T("\nSpecify first point: "), pt1))
	{
		AcGePoint3d startPt(pt1[0], pt1[1], pt1[2]);
		// Prompt for the second point
		if (RTNORM == acedGetPoint(pt1,
			_T("\nSpecify second point: "), pt2))
		{
			AcGePoint3d endPt(pt2[0], pt2[1], pt2[2]);
			// Create and append the new Line object
			AcDbLine* pLine = new AcDbLine(startPt, endPt);
			pBlockTableRecord->appendAcDbEntity(pLine);
			// Close the block table record and the Line object
			pBlockTableRecord->close();
			pLine->close();
		}
	}
}

extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode Msg, void* pkt)
{
	switch (Msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxDynamicLinker->registerAppMDIAware(pkt);
		acutPrintf(L"\nLoading project...\n");
		acedRegCmds->addCommand(_T("AUCommands"), _T("InputLine"), _T("InputLine"), ACRX_CMD_MODAL, inputLine);
		break;

	case AcRx::kUnloadAppMsg:
		acutPrintf(_T("\nUnloading createLine project...\n"));
		acedRegCmds->removeGroup(_T("AUCommnds"));
		break;

	default:
		break;
	}
	return AcRx::kRetOK;
}
