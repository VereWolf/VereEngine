// pch.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <wrl.h>
#include <wrl/client.h>
//#include <d3d11.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d3dcompiler.h>
#include "d3dx11effect.h"
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include  <Commctrl.h>

#include  <Xaudio2.h>

//InitCommonControlsEx(ICC_TAB_CLASSES);
#include <fstream>
#include <iostream> 
using namespace DirectX;
using namespace std;
using namespace DirectX;

#define LUA_LIB
#define BT_USE_DOUBLE_PRECISION

#ifdef BT_USE_DOUBLE_PRECISION
#define btInt INT64;
#define btUInt UINT64;
#else
#define btInt INT32;
#define btUInt UINT32;
#endif

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <limits>
#include "Resource.h"
#include "lua.h"
#include "lauxlib.h"
#include "btBulletDynamicsCommon.h"

#include "VereMath.h"
#include "VereTextureFloat.h"
#include "VereTextureByte4.h"
#include "VereQueue.h"
#include "d3dUtil.h"
#include "LuaBinding.h"
#include "LightHelper.h"
#include "RenderState.h"
#include "PLanetCoordinate.h"
//#include "Effects.h"
//#include "Body.h"
//#include "LoadVMS.h"
//#include "DeviceResources.h"
//#include "GameTimer.h"
//#include "GameObject.h"
//#include "inputclass.h"

#include "App.xaml.h"

// TODO: reference additional headers your program requires here
