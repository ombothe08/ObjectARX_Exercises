#include "pch.h"
#include "framework.h"
#include "arxHeaders.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void print()
{
	acutPrintf(L"HEllO World");
}

extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode Msg, void* pkt)
{
	switch (Msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxDynamicLinker->registerAppMDIAware(pkt);
		acutPrintf(L"\nLoading project...\n");
		acedRegCmds->addCommand(_T("Draw"), _T("Line"), _T("addLine"), ACRX_CMD_MODAL, print);
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

