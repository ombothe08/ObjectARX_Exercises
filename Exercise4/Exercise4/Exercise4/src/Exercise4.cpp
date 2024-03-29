#include "pch.h"
#include "framework.h"
#include "arxHeaders.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void addLine()
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	// Open the Block Table for read-only
	AcDbBlockTable* pBlockTable;
	pDb->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE,
		pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();
	// Define the points that will be used to define the Line object
	AcGePoint3d startPt(7000.0, 3.0, 0.0);
	AcGePoint3d endPt(11.0, 3.0, 0.0);
	// Create the new Line object in memory
	AcDbLine* pLine = new AcDbLine(startPt, endPt);
	// Add the new Line object to Model space
	pBlockTableRecord->appendAcDbEntity(pLine);
	// Close the Model space block
	pBlockTableRecord->close();
	pLine->close();
}

extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxDynamicLinker->registerAppMDIAware(pkt);
		acutPrintf(_T("\nLoading AU 2014 project...\n"));
		acedRegCmds->addCommand(_T("AUCommands"), _T("AddLine"),_T("AddLine"), ACRX_CMD_MODAL, addLine);
		break;
	case AcRx::kUnloadAppMsg:
		acutPrintf(_T("\nUnloading AU 2014 project...\n"));
		acedRegCmds->removeGroup(_T("AUCommnds"));
		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}