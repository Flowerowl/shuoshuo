// 夜阑说说服务端Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "夜阑说说服务端.h"
#include "夜阑说说服务端Dlg.h"
#include "SkinH.h"
#pragma comment(lib,"SkinH.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//监听
SOCKET listen_sock;
SOCKET sock;
UINT server_thd(LPVOID p);

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
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btnSEND, OnbtnSEND)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
//---------------------夜阑----------------------
	show_edit = (CEdit *)GetDlgItem(IDC_EDIT1);
	send_edit = (CEdit *)GetDlgItem(IDC_EDIT2);
	getname = (CEdit *)GetDlgItem(IDC_NAME);
	send_edit->SetFocus();


	char name[80];
	CString IP;
	hostent* pHost; 
	gethostname(name, 128);//获得主机名 
	pHost = gethostbyname(name);//获得主机结构 
	IP = inet_ntoa(*(in_addr *)pHost->h_addr);
	update("本机IP地址：" + IP);
	AfxBeginThread(&server_thd, 0);
	
//---------------------夜阑----------------------
	SkinH_Attach();//贴图美化程序
	return 	FALSE;  // return TRUE  unless you set the focus to a control
	
}
void CMyDlg::update(CString s)
{
	show_edit->ReplaceSel(s + "\r\n");
}

UINT server_thd(LPVOID p)
{
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	SOCKADDR_IN local_addr;
	SOCKADDR_IN client_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);
	int res;
	char msg[1024];
	CMyDlg * dlg = (CMyDlg *)AfxGetApp()->GetMainWnd();
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(5150);
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if( (listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET )
	{
		dlg->update("创建监听失败");
	}
	if( bind(listen_sock, (struct sockaddr*) &local_addr, sizeof(SOCKADDR_IN)) )
	{
		dlg->update("绑定错误");
	}
	listen(listen_sock, 1);
	if( (sock = accept(listen_sock, (struct sockaddr *)&client_addr, &iaddrSize)) == INVALID_SOCKET)
	{
		dlg->update("accept 失败");
	}
	else
	{
		CString port;
		port.Format("%d", int(ntohs(client_addr.sin_port)));
		dlg->update( "已连接来自：" + CString(inet_ntoa(client_addr.sin_addr)) + "  端口：" + port);
	}

	////////////接收数据
	while(1)
	{
		if( (res = recv(sock, msg, 1024, 0)) == -1 )
		{
			dlg->update("失去连接");
			break;
		}
		else
		{
			msg[res] = '\0';
			dlg->update(name + CString(msg));
		}
	}
	return 0;
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMyDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMyDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
	OnbtnSEND();
}

void CMyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CMyDlg::OnbtnSEND() 
{
	// TODO: Add your control notification handler code here
	CString s;
	CString name;
	char * namemsg;
	char * msg;
	getname->GetWindowText(name);
	send_edit->GetWindowText(s);
	namemsg = name.GetBuffer(name.GetLength());
	msg = s.GetBuffer(s.GetLength());
	if(send(sock, msg, strlen(msg), 0)&&send(sock,namemsg,strlen(namemsg),0)== SOCKET_ERROR)
	{
		show_edit->ReplaceSel("发送失败\r\n");
	}
	else if(s == "")
	{
		MessageBox("请输入信息");
	}
	else if(name =="")
	{
		MessageBox("请输入您的大名");
	}
	else
	{
		show_edit->ReplaceSel(name+"说:"+ s + "\r\n");//消息上屏，清空输入，并重获焦点
		send_edit->SetWindowText("");
		send_edit->SetFocus();
	}
}

void CMyDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnCancel();
}

void CMyDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

