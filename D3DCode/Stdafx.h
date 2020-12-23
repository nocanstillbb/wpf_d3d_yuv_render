// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include "framework.h"
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <assert.h>

#include "CRendererManager.h"
#include "CRenderer.h"
#include "CTriangleRenderer.h"
#include "CYUVRenderer.h"



#define IFC(x) { hr = (x); if (FAILED(hr)) goto Cleanup; }
#define IFCOOM(x) { if ((x) == NULL) { hr = E_OUTOFMEMORY; IFC(hr); } }
#define SAFE_RELEASE(x) { if (x) { x->Release(); x = NULL; } }