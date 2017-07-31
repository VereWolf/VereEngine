#pragma once

typedef unsigned short int USING;

#define BUFFERUNIT 0.00000005960464478

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													 \
	{															  \
	HRESULT hr = (x);											 \
if (FAILED(hr))													 \
		{														 \
			return false;										\
		}                                                      \
	}
#endif

#ifndef HR2
#define HR2(x)													 \
	{															  \
	HRESULT hr = (x);											 \
if (FAILED(hr))													 \
		{														 \
			return -1;										\
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

#define SafeDelete(x) {if(x != 0) delete x; x = 0; }