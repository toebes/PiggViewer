// DDrawSystem.cpp: implementation of the CDDrawSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DDrawSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDDrawSystem::CDDrawSystem()
{
	m_pDD = NULL;
	m_pddsFrontBuffer = NULL;
	m_pddsStoreBuffer = NULL;
    pcClipper = NULL;
}

CDDrawSystem::~CDDrawSystem()
{
	Terminate();
}

// old DirectDraw Initialization stuff. Set a window mode DirectDraw Display.
BOOL CDDrawSystem::Init(HWND hWnd)
{
	HRESULT hRet;

	this->hWnd = hWnd;

	hRet = DirectDrawCreateEx(NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL);
	if(hRet != DD_OK)
	{
		AfxMessageBox("Failed to create directdraw object.");
		return FALSE;
	}

	hRet = m_pDD->SetCooperativeLevel(hWnd, DDSCL_NORMAL);
	if(hRet != DD_OK)
	{
		AfxMessageBox("Failed to set directdraw display behavior.");
		return FALSE;
	}

	HRESULT hr;

	DDSURFACEDESC2 ddsd;
    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize         = sizeof( ddsd );
    ddsd.dwFlags        = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

    if(FAILED(hr = m_pDD->CreateSurface(&ddsd, &m_pddsFrontBuffer, NULL)))
	{
		AfxMessageBox("Failed to create primary surface.");
		return FALSE;		
	}


    // Create the backbuffer surface
    ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
    ddsd.dwWidth        = 800;
    ddsd.dwHeight       = 600;

    if(FAILED(hr = m_pDD->CreateSurface(&ddsd, &m_pddsStoreBuffer, NULL)))
	{
		AfxMessageBox("Failed to create back buffer surface.");
		return FALSE;		
	}

    if(FAILED(hr = m_pDD->CreateClipper(0, &pcClipper, NULL)))
	{
		AfxMessageBox("Failed to create clipper.");
		return FALSE;		
	}

    if(FAILED(hr = pcClipper->SetHWnd(0, hWnd)))
    {
        pcClipper->Release();
		AfxMessageBox("Failed to create primary surface.");
		return FALSE;		
    }

    if(FAILED(hr = m_pddsFrontBuffer->SetClipper(pcClipper)))
    {
        pcClipper->Release();
		AfxMessageBox("Failed to create primary surface.");
		return FALSE;		
    }

	return TRUE;
}

// make sure all stuff are terminated. and set to NULL when application ends.
void CDDrawSystem::Terminate()
{
	if (m_pDD != NULL)
	{
		if (m_pddsFrontBuffer != NULL)
		{
			if (m_pddsStoreBuffer != NULL)
			{
				m_pddsStoreBuffer->Release();
				m_pddsStoreBuffer = NULL;
			}

			if (pcClipper != NULL)
			{
				pcClipper->Release();
				pcClipper = NULL;
			}

			m_pddsFrontBuffer->Release();
			m_pddsFrontBuffer = NULL;
		}
		m_pDD->Release();
		m_pDD = NULL;
	}
}

// clear both off csreen buffer and primary buffer.
void CDDrawSystem::Clear()
{
	HRESULT hRet;
	DDBLTFX fx;
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = 0x000000;

	while (1)
	{
		hRet = m_pddsFrontBuffer->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &fx);
		if (hRet == DD_OK)
			break;
		else if (hRet == DDERR_SURFACELOST)
		{
			m_pddsFrontBuffer->Restore();
		}
		else if (hRet != DDERR_WASSTILLDRAWING)
			break;
	}

	while (1)
	{
		hRet = m_pddsStoreBuffer->Blt(NULL, NULL, NULL, DDBLT_COLORFILL, &fx);
		if (hRet == DD_OK)
			break;
		else if (hRet == DDERR_SURFACELOST)
		{
			m_pddsStoreBuffer->Restore();
		}
		else if (hRet != DDERR_WASSTILLDRAWING)
			break;
	}

}

// a test:
// The conclusion is: Under no circumstance, draw directly to primary Surface!
// doesn't work that way.
// ...
// ...
// This is just a simple test function. It has shit use in this project.
void CDDrawSystem::TestDraw(int x, int y)
{
	HRESULT hRet;
	HDC dc;
	hRet = m_pddsStoreBuffer->GetDC(&dc);
	if (hRet != DD_OK)
		return;

	POINT p = {0 + x, 0 + y};
	ClientToScreen(hWnd, &p);

	SetTextColor(dc, RGB(255, 0, 0));
	TextOut(dc, 20, 20, "This is a stinky App", lstrlen("This is a stinky App"));

	Ellipse(dc, x-50, y-50, x+50,y+50);

	m_pddsStoreBuffer->ReleaseDC(dc);
}

// Load images from offscteen buffer to primary buffer and for display.
void CDDrawSystem::Display()
{
	HRESULT hRet;

	RECT rt;
	POINT p = {0, 0};
	ClientToScreen(hWnd, &p);
	rt.left = 0 + p.x; rt.top = 0 + p.y; rt.right = 800 + p.x; rt.bottom = 600 + p.y;

    while( 1 )
    {
		hRet = m_pddsFrontBuffer->Blt(&rt, m_pddsStoreBuffer, NULL, DDBLT_WAIT, NULL);
		if (hRet == DD_OK)
			break;
        else if(hRet == DDERR_SURFACELOST)
        {
            m_pddsFrontBuffer->Restore();
            m_pddsStoreBuffer->Restore();
        }
        else if(hRet != DDERR_WASSTILLDRAWING)
            return;
	}
}

