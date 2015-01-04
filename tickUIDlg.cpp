// tickUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "tickUI.h"
#include "tickUIDlg.h"
#include ".\tickuidlg.h"

#include "mmsystem.h"
#include "MemDC.h"
//#include "apfastTiming.h"
//#include "apThread.h"

#pragma comment(lib, "Winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

	CFont			m_ftUI;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtickUIDlg �Ի���



CtickUIDlg::CtickUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtickUIDlg::IDD, pParent)
	, m_strTick(_T("")), m_delay(5)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bRunning = FALSE;
}

void CtickUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TICK, m_strTick);
}

BEGIN_MESSAGE_MAP(CtickUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CtickUIDlg ��Ϣ�������

BOOL CtickUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//m_ftUI.CreateFont(500, 64, 0,0,0,0,0,0,0,0,0,0,0,"FixSystem");
	m_ftUI.CreateFont(100, 32, 0,0,0,0,0,0,0,0,0,0,0,"FixSystem");
	GetDlgItem(IDC_STATIC_TICK)->SetFont(&m_ftUI);	

	if(__argc > 1) {
		if(__argc != 2) {
			AfxMessageBox("invalid param");
			return FALSE;
		}

		m_delay = atoi(__argv[1]);
	}

	TIMECAPS cap;
	memset(&cap, 0, sizeof cap);
	timeGetDevCaps(&cap, sizeof cap);
	TRACE("%d-%d\n", cap.wPeriodMax, cap.wPeriodMin);

	Toggle();

	//CRect rc;
	//GetWindowRect(&rc);
	//MoveWindow(0, 0, rc.Width(), rc.Height());
	//SetWindowPos(NULL, 0,0,rc.Width(), rc.Height(), 0);
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CtickUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtickUIDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CtickUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtickUIDlg::OnTimer(UINT nIDEvent)
{
	
	SYSTEMTIME stNow;
	GetLocalTime(&stNow);

	m_strTick.Format("%02d:%02d:%02d %03d", stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds);
	//UpdateData(FALSE);
	//SetDlgItemText(IDC_STATIC_TICK, m_strTick);

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC* pDC = GetDC();
	CMemDC memDC(pDC, rcClient);

	HGDIOBJ  old = memDC.SelectObject(m_ftUI.GetSafeHandle());
	memDC.SetBkMode(TRANSPARENT);
	memDC.FillRect(rcClient, NULL);
	memDC.TextOut(0, 0, m_strTick);

	memDC.SelectObject(old);

	//ReleaseDC(pDC);

	CDialog::OnTimer(nIDEvent);
}

void CALLBACK MyTimeProc(  UINT uID,        UINT uMsg,       DWORD dwUser,    DWORD dw1,       DWORD dw2   )
{
	CtickUIDlg* p = (CtickUIDlg*)dwUser;

	SYSTEMTIME stNow;
	GetLocalTime(&stNow);

	CString s;
	s.Format("%02d:%02d:%02d %03d", stNow.wHour, stNow.wMinute, stNow.wSecond, stNow.wMilliseconds);
	//UpdateData(FALSE);
	//SetDlgItemText(IDC_STATIC_TICK, m_strTick);

	CRect rcClient;
	p->GetClientRect(&rcClient);

	CDC* pDC = p->GetDC();
	{
		CMemDC memDC(pDC/*, rcClient*/);

		HGDIOBJ  old = memDC.SelectObject(m_ftUI.GetSafeHandle());
		memDC.SetBkMode(TRANSPARENT);
		memDC.FillRect(rcClient, NULL);
		memDC.TextOut(0, 0, s);

		//added
		int pos;
		pos = stNow.wSecond % 2 * 1000 + stNow.wMilliseconds;

		CPen *oldPen;
		CPen pen1;
		pen1.CreatePen(PS_SOLID, 2, RGB(255,0,0));
		oldPen = memDC.SelectObject(&pen1);

		CBrush *oldBrush;
		CBrush brush1;
		brush1.CreateSolidBrush(RGB(0,0,255));
		oldBrush = memDC.SelectObject(&brush1);

		CFont ftIndex;
		ftIndex.CreateFont(50, 16, 0,0,0,0,0,0,0,0,0,0,0,"FixSystem");
		CFont *oldFont;
		oldFont = memDC.SelectObject(&ftIndex);

		CString str_index;
		for(int i=1;i<=10;i++) {
			memDC.MoveTo(i * 40, 100);
			memDC.LineTo(i * 40, 150);
			str_index.Format("%d", i * 200);
			if(i % 5 == 0)
				memDC.TextOut(i * 40, 150, str_index);
		}

		memDC.MoveTo(0, 125);
		memDC.LineTo(400, 125);

		memDC.Rectangle(pos / 5 - 5, 100, pos / 5 + 10, 150);

		if(stNow.wSecond % 2 == 0 && stNow.wMilliseconds < 50) {
			memDC.Rectangle(0, 150, 500, 300);

			MessageBeep(MB_OK);
			unsigned FREQUENCY[] = {392,392,440,392,523,494,  
				  392,392,440,392,587,523,  
				  392,392,784,659,523,494,440,  
				  689,689,523,587,523};  
 
			unsigned DELAY[] = {375,125,500,500,500,1000,  
					  375,125,500,500,500,1000,  
					  375,125,500,500,500,500,1000,  
					  375,125,500,500,500,1000};  
			/*int i;  
			for (i = 0; i < 25; i++)  
			{  
				Beep(FREQUENCY[i], DELAY[i]);  
			}*/
			//Beep(1000, 50);
		}

		memDC.SelectObject(oldPen);
		memDC.SelectObject(oldBrush);
		memDC.SelectObject(oldFont);
		memDC.SelectObject(old);
	}

	p->ReleaseDC(pDC);
	
}

void CtickUIDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	Toggle();
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CtickUIDlg::Toggle(void)
{
	static MMRESULT tmID = 0;
	m_bRunning = !m_bRunning;
	if(m_bRunning)
	{
		//apHiResElapsedTime tick;

		//SetTimer(0, 10, NULL);

		tmID = timeSetEvent(m_delay, 5, MyTimeProc, (DWORD_PTR)this, TIME_PERIODIC);
	}
	else
	{
		timeKillEvent(tmID);
	}
}
