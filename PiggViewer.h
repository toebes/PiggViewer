// PiggViewer.h : main header file for the PIGGVIEWER application
//

#if !defined(AFX_PIGGVIEWER_H__EB8C5818_DC0E_41D6_BF4A_C1F025573D95__INCLUDED_)
#define AFX_PIGGVIEWER_H__EB8C5818_DC0E_41D6_BF4A_C1F025573D95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include <d3dx9.h>

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerApp:
// See PiggViewer.cpp for the implementation of this class
//

class CPiggViewerApp : public CWinApp
{
public:
	void SetDefaultDirectory(CString strDefaultDirectory);
	CString GetDefaultDirectory();
	void SetLastDirectory(CString strLastDirectory);
	CString GetLastDirectory();
	void EndProgress();
	void StepProgress(DWORD dwCurrent);
	void StartProgress(DWORD dwMax);
	CPiggViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPiggViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	struct IDirect3D9      *m_pD3D;
	IDirect3DDevice9       *m_pDD; // Used to create the D3DDevice
	D3DPRESENT_PARAMETERS   m_d3dpp;

	COleTemplateServer m_server;
		// Server object for document creation
	//{{AFX_MSG(CPiggViewerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnToolsOptions();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIGGVIEWER_H__EB8C5818_DC0E_41D6_BF4A_C1F025573D95__INCLUDED_)
