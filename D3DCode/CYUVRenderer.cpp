#pragma once
#include "StdAfx.h"

#pragma region MyRegion
RECT m_rtViewport;


//Width, Height
const int pixel_w = 320, pixel_h = 180;
FILE *fp = NULL;

//Bit per Pixel
const int bpp = 12;



#pragma endregion

CYUVRenderer::CYUVRenderer() : CRenderer(), m_pd3dVB(NULL)
{

}

CYUVRenderer::~CYUVRenderer()
{
	SAFE_RELEASE(m_pd3dVB);
}

HRESULT
CYUVRenderer::Create(IDirect3D9* pD3D, IDirect3D9Ex* pD3DEx, HWND hwnd, UINT uAdapter, CRenderer** ppRenderer)
{
	HRESULT hr = S_OK;

	CYUVRenderer* pRenderer = new CYUVRenderer();
	IFCOOM(pRenderer);

	IFC(pRenderer->Init(pD3D, pD3DEx, hwnd, uAdapter));

	*ppRenderer = pRenderer;
	pRenderer = NULL;

Cleanup:
	delete pRenderer;

	return hr;
}

HRESULT
CYUVRenderer::Init(IDirect3D9* pD3D, IDirect3D9Ex* pD3DEx, HWND hwnd, UINT uAdapter)
{
	HRESULT hr = S_OK;
	{
		//// Call base to create the device and render target
		IFC(CRenderer::Init(pD3D, pD3DEx, hwnd, uAdapter));


		fp = fopen("test_yuv420p_320x180.yuv", "rb+");


	}Cleanup:
	return hr;
}


HRESULT
CYUVRenderer::Render()
{

	HRESULT hr = S_OK;
	{
		IFC(m_pd3dDevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET,
			D3DCOLOR_ARGB(128, 0, 0, 128),  // NOTE: Premultiplied alpha!
			1.0f,
			0
		));
		HRESULT lRet;
		unsigned char buffer[pixel_w*pixel_h*bpp / 8];
		//Read Data
		if (fp != NULL)
		{
			if (fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fp) != pixel_w * pixel_h*bpp / 8) {
				// Loop
				fseek(fp, 0, SEEK_SET);
				fread(buffer, 1, pixel_w*pixel_h*bpp / 8, fp);
			}
		}
		else
		{
			return 0;

		}

		if (m_osrSurface == NULL)
			return -1;
		D3DLOCKED_RECT d3d_rect;
		lRet = m_osrSurface->LockRect(&d3d_rect, NULL, D3DLOCK_DONOTWAIT);
		if (FAILED(lRet))
			return 0;

		byte *pSrc = buffer;
		byte * pDest = (BYTE *)d3d_rect.pBits;
		int stride = d3d_rect.Pitch;
		unsigned long i = 0;

		//Copy Data

		for (i = 0; i < pixel_h; i++) {
			memcpy(pDest + i * stride, pSrc + i * pixel_w, pixel_w);
		}
		for (i = 0; i < pixel_h / 2; i++) {
			memcpy(pDest + stride * pixel_h + i * stride / 2, pSrc + pixel_w * pixel_h + pixel_w * pixel_h / 4 + i * pixel_w / 2, pixel_w / 2);
		}
		for (i = 0; i < pixel_h / 2; i++) {
			memcpy(pDest + stride * pixel_h + stride * pixel_h / 4 + i * stride / 2, pSrc + pixel_w * pixel_h + i * pixel_w / 2, pixel_w / 2);
		}

		lRet = m_osrSurface->UnlockRect();
		if (FAILED(lRet))
			return -1;

		if (m_pd3dDevice == NULL)
			return -1;

		IDirect3DSurface9 * pBackBuffer = NULL;
		m_rtViewport.left = 0;
		m_rtViewport.top = 0;
		m_rtViewport.right = 320;
		m_rtViewport.bottom = 180;

		m_pd3dDevice->StretchRect(m_osrSurface, NULL, m_pd3dRTS, &m_rtViewport, D3DTEXF_LINEAR);

	}
Cleanup:
	return hr;
}