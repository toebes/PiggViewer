// PiggViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PiggViewer.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "PiggViewerDoc.h"
#include "LeftView.h"
#include "VersionInfo.h"
#include "OptionsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerApp

BEGIN_MESSAGE_MAP(CPiggViewerApp, CWinApp)
	//{{AFX_MSG_MAP(CPiggViewerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerApp construction

CPiggViewerApp::CPiggViewerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPiggViewerApp object

CPiggViewerApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {14ABA651-FC01-49F9-9E68-79072ADED8A8}
static const CLSID clsid =
{ 0x14aba651, 0xfc01, 0x49f9, { 0x9e, 0x68, 0x79, 0x7, 0x2a, 0xde, 0xd8, 0xa8 } };

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerApp initialization

BOOL CPiggViewerApp::InitInstance()
{
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	SetRegistryKey(IDS_REGISTRYKEY);

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_PIGGVITYPE,
		RUNTIME_CLASS(CPiggViewerDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CLeftView));
	AddDocTemplate(pDocTemplate);

	// Connect the COleTemplateServer to the document template.
	//  The COleTemplateServer creates new documents on behalf
	//  of requesting OLE containers by using information
	//  specified in the document template.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);

	// Register all OLE server factories as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleTemplateServer::RegisterAll();
		// Note: MDI applications register all server objects without regard
		//  to the /Embedding or /Automation on the command line.

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// Check to see if launched as OLE server
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	// When a server application is launched stand-alone, it is a good idea
	//  to update the system registry in case it has been damaged.
	m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
	COleObjectFactory::UpdateRegistryAll();

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

    //
    // TODO: MARYELLEN
    // This is where you initialize Direct3D (Store/add member variables to the CPiggViewerApp class)
    //
    // Create the D3D object.
	HRESULT hRet;
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;
  
     // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
	ZeroMemory( &m_d3dpp, sizeof(m_d3dpp));
    m_d3dpp.Windowed               = TRUE; 
    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat       = D3DFMT_UNKNOWN;
    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    m_d3dpp.EnableAutoDepthStencil = TRUE;
    m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
	hRet = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,  // Adapter = default primary adapter
		                         D3DDEVTYPE_HAL,      // DeviceType = Hardware Rasterization
								 (this->GetMainWnd()->m_hWnd),                // hFoucusWindow = may be NULL for windowed mode (not full screen)
                                 D3DCREATE_SOFTWARE_VERTEXPROCESSING,
								 &m_d3dpp,            // presentationParamters = input/output structure
								 &m_pDD );            // IDirect3DDevice9 interface pointer returned.

    if(FAILED(hRet))
		return FALSE;

    // Turn off culling
    m_pDD->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting
    m_pDD->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Turn on the zbuffer
    m_pDD->SetRenderState( D3DRS_ZENABLE, TRUE );

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    if (!pDX->m_bSaveAndValidate)
    {
        CVersionInfo versioninfo;
        versioninfo.UpdateDlgVersionStrings(this, IDC_VERSION1, IDC_VERSION5);
    }
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CPiggViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPiggViewerApp message handlers


void CPiggViewerApp::StartProgress(DWORD dwMax)
{
    CMainFrame* pMainFrame = (CMainFrame* )m_pMainWnd;
    pMainFrame->StartProgress(dwMax);
}

void CPiggViewerApp::StepProgress(DWORD dwCurrent)
{
    CMainFrame* pMainFrame = (CMainFrame* )m_pMainWnd;
    pMainFrame->StepProgress(dwCurrent);
}

void CPiggViewerApp::EndProgress()
{
    CMainFrame* pMainFrame = (CMainFrame* )m_pMainWnd;
    pMainFrame->EndProgress();
}

void CPiggViewerApp::OnToolsOptions() 
{
	// TODO: Add your command handler code here
    COptionsDialog dlgOptions;
    dlgOptions.DoModal();	
}

const char szSection[] = "Preferences";
const char szDefaultDirectory[] = "DefaultDirectory";
const char szLastDirectory[] = "LastDirectory";

CString CPiggViewerApp::GetDefaultDirectory()
{
    return GetProfileString(szSection, szDefaultDirectory, "");
}

void CPiggViewerApp::SetDefaultDirectory(CString strDefaultDirectory)
{
    WriteProfileString(szSection, szDefaultDirectory, strDefaultDirectory);
}
CString CPiggViewerApp::GetLastDirectory()
{
    return GetProfileString(szSection, szLastDirectory, "");
}

void CPiggViewerApp::SetLastDirectory(CString strLastDirectory)
{
    WriteProfileString(szSection, szLastDirectory, strLastDirectory);
}
