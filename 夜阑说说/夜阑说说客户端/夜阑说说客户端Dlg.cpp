// ҹ��˵˵�ͻ���Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ҹ��˵˵�ͻ���.h"
#include "ҹ��˵˵�ͻ���Dlg.h"
//��ͼ�ļ�
#include "SkinH.h"
#pragma comment(lib,"SkinH.lib")
//-----------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT recv_thd(LPVOID p);
SOCKET sock;
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
	ON_BN_CLICKED(IDC_btnConnect, OnbtnConnect)
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
	

//---------------ҹ��˵˵--------------
	//ָ����ť
	edit1 = (CEdit *)GetDlgItem(IDC_EDIT1);
	send_edit = (CEdit *)GetDlgItem(IDC_EDIT2);
	btnconn = (CButton *)GetDlgItem(IDC_btnConnect);
	ip_edit = (CEdit *)GetDlgItem(IDC_EDIT3);
	getname = (CEdit *)GetDlgItem(IDC_NAME);
	//��ʼ��IP
	ip_edit->SetWindowText("127.0.0.1");
	//���������Ϊ����
	send_edit->SetFocus();
	
	//��ͼ��������
    SkinH_Attach();
	return FALSE;  // return TRUE  unless you set the focus to a control
}
//---------------ҹ��˵˵--------------
	//�ػ��������
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
	//---------------ҹ��˵˵--------------
UINT recv_thd(LPVOID p)
{
	int res;
	char msg[1024];
	CString s;	
	CMyDlg * dlg = (CMyDlg *) AfxGetApp()->GetMainWnd();
	dlg->update("in thd");
	
	while(1)
	{
		if( (res = recv(sock, msg, 1024, 0)) == -1)
		{
			dlg->update("ʧȥ����");
			break;
		}
		else
		{
			msg[res] = '\0';
			dlg->update("Server:" + CString(msg));
		}
	}
	
	//closesocket(sock);
	return 0;
}
	//---------------ҹ��˵˵--------------
void CMyDlg::update(CString s)
{
	edit1->ReplaceSel(s + "\r\n");
}
	//---------------ҹ��˵˵--------------
void CMyDlg::OnbtnConnect() 
{
	// TODO: Add your control notification handler code here
	WSADATA wsaData;
	SOCKADDR_IN server_addr;
	WSAStartup(0x0202, &wsaData);
	CString ip;
	ip_edit->GetWindowText(ip);//ȡ�÷�������IP��ַ
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5150);
	if( (sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		edit1->ReplaceSel("create socket error\r\n");
	}
	if( connect(sock, (struct sockaddr *) &server_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		edit1->ReplaceSel("����ʧ��\r\n");
	}
	else
	{
		edit1->ReplaceSel("���ӳɹ�\r\n");
		AfxBeginThread(&recv_thd, 0);
		btnconn->EnableWindow(FALSE);//��ť���
	}
}


void CMyDlg::OnbtnSEND() 
{
	// TODO: Add your control notification handler code here
	CString s;
	CString name;
	char * namemsg;
	char * msg;
	send_edit->GetWindowText(s);
	getname->GetWindowText(name);
	msg = s.GetBuffer(s.GetLength());
	namemsg = name.GetBuffer(name.GetLength());
	
	if(send(sock, msg, strlen(msg), 0)&&send(sock, namemsg,strlen(namemsg),0) == SOCKET_ERROR)
	{
		update("����ʧ��");
	}
	else if(s == "")
	{
		MessageBox("��������Ϣ");
	}
	else if(name =="")
	{
		MessageBox("���������Ĵ���");
	}
	else
	{
		update(name +"˵:" +s);//��Ϣ������������룬���ػ񽹵�
		send_edit->SetWindowText("");
		send_edit->SetFocus();
	}
}


void CMyDlg::OnOK()
{
	OnbtnSEND();//��дONOK���س�ΪĬ�ϰ�ť�����ڷ�����Ϣ
}

