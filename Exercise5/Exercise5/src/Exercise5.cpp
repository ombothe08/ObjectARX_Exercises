#include "pch.h"
#include "framework.h"
#include "arxHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void makeLayer()
{
	AcDbDatabase* pDb = acdbHostApplicationServices()->workingDatabase();
	AcDbLayerTable* pLayerTable;
	pDb->getLayerTable(pLayerTable, AcDb::kForRead);
	// Check to see if the layer exists
	if (pLayerTable->has(_T("OBJ")) == false)
	{
		pLayerTable->upgradeOpen();
		// Create the new layer and assign it the name 'OBJ'
		AcDbLayerTableRecord* pLayerTableRecord =new AcDbLayerTableRecord();
		pLayerTableRecord->setName(_T("OBJ"));
		AcCmColor color;
		color.setColorIndex(4);
		pLayerTableRecord->setColor(color);
		// Add the new layer to the Layer table
		pLayerTable->add(pLayerTableRecord);
		pLayerTable->close();
		pLayerTableRecord->close();
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
		acedRegCmds->addCommand(_T("AUCommands"), _T("MakeLayer"),_T("MakeLayer"), ACRX_CMD_MODAL, makeLayer);
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