#include "pch.h"
#include "framework.h"
#include "arxHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void listObjects()
{

	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbBlockTableRecord* pBlockTableRecord;
	Acad::ErrorStatus es = acdbOpenObject(pBlockTableRecord,
		pDb->currentSpaceId(),
		AcDb::kForRead);

	AcDbBlockTableRecordIterator* pItr;
	pBlockTableRecord->newIterator(pItr);

	AcDbEntity* pEnt;
	// Step through each object in the current space
	for (pItr->start(); !pItr->done(); pItr->step())
	{
		pItr->getEntity(pEnt, AcDb::kForRead);
		acutPrintf(_T("\nClass name: %s"), pEnt->isA()->name());
		pEnt->close();
	}

	pBlockTableRecord->close();
	delete pItr;
	// Display the AutoCAD Text Window
	acedTextScr();
}

extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode Msg, void* pkt)
{
	switch (Msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxDynamicLinker->registerAppMDIAware(pkt);
		acutPrintf(L"\nLoading project...\n");
		acedRegCmds->addCommand(_T("AUCommands"), _T("ListObjects"),_T("ListObjects"), ACRX_CMD_MODAL, listObjects);
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


