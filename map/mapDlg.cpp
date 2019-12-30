// mapDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "map.h"
#include "mapDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//추가
//#pragma cooment(lib,"odbc32.lib")


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CmapDlg 대화 상자

#include <cstdio>
#include <queue>
#include <map>
#include <stack>
#include <vector>
#include <utility>
#define MAX_VALUE -987654321
using namespace std;

bool draw = false;
class m_Vertex {
public:
	int index;
	int dist;
	int post = 0;
	m_Vertex(int index, int dist, int post) :index(index), dist(dist), post(post) {
	}
	m_Vertex(int index) :index(index) {
		dist = MAX_VALUE;
	}
	void setDist(int d) {
		dist = d;
	}
	bool operator >(const m_Vertex& v) const {
		return dist > v.dist;
	}
	bool operator <(const m_Vertex& v) const {
		return dist < v.dist;
	}
};
class Graph {
public:
	int n = 0;
	vector<int> temp1;
	vector<m_Vertex> vt;
	vector<vector<pair<int, int> > > adj;
	priority_queue<m_Vertex > pq;
	map<int, pair<int, int> > m;

	void addVertex(int index, int dist) {
		vt.push_back(m_Vertex(index, dist, index));
		n = vt.size();
		adj.resize(n + 1);
	}
	void addVertex(int index) {
		vt.push_back(m_Vertex(index));
		n = vt.size();
		adj.resize(n);
	}
	void addAdj(int u, int v, int w) {
		adj[u - 1].push_back(make_pair(v, w));
		adj[v - 1].push_back(make_pair(u, w));

	}
	void setStart(int index) {
		vt[index - 1].setDist(0);
		vt[index - 1].post = index;
	}
	void dijkstra(int s,int g) {
		ofstream ofs("output.txt", ofstream::out | ofstream::trunc);		
		vector<int> dist(n, -1 * MAX_VALUE);
		dist[s-1] = 0;
		setStart(s);
		for (int i = 0; i < n; i++) {
			pq.push(vt[i]);
		}
		m.insert(make_pair(s, make_pair(0, -1)));
		while (!pq.empty()) {
			int index = pq.top().index - 1;
			int cost = -1 * pq.top().dist; pq.pop();
			if (dist[index] < cost) continue;
			for (int i = 0; i < adj[index].size(); i++) {
				int n = adj[index][i].first;
				int v = adj[index][i].second;
				if (dist[n - 1] > dist[index] + v) {
					dist[n - 1] = dist[index] + v;
					m_Vertex newVt = m_Vertex(n, -dist[n - 1], index + 1);
					pq.push(newVt);
					m[n] = make_pair(dist[n - 1], index + 1);

				}
			}
		}
		
		for (int i = 1; i <= n; i++) {			
			if (dist[g-1] != -MAX_VALUE) {
				stack<int> st;
				int next = m[g].second;
				while (next != -1) {
					st.push(next);
					next = m[next].second;
				}
				while (!st.empty()) {
					int t;
					t = st.top() - 1;
					temp1.push_back(t);
					ofs << t << endl;			
					st.pop();
				}				
				temp1.push_back(g - 1);
				ofs << g-1 << endl;
				ofs << "Distance : " << dist[g-1] << endl;
				ofs.close();
				break;
			}
		}
	}
};
CmapDlg::CmapDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAP, m_map);
	//  DDX_Control(pDX, IDC_SCROLLBAR1, m_hscroll);
	//  DDX_Control(pDX, IDC_SCROLLBAR2, m_vscroll);

	DDX_Control(pDX, IDC_MINIMAP, m_minimap);
}

BEGIN_MESSAGE_MAP(CmapDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()	

	ON_BN_CLICKED(IDC_BUTTON1, &CmapDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CmapDlg 메시지 처리기

BOOL CmapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
			
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//ShowWindow(SW_MAXIMIZE);
	//ShowWindow(SW_MINIMIZE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//m_mapView = new CMapView;
	string node_input = "SF_cnode.txt";
	string edge_input = "SF_cedge.txt";
	Load_Nodes(node_input);
	Load_Edges(edge_input);

	

	SCROLLINFO  scrinfo;
	scrinfo.cbSize = sizeof(scrinfo);
	scrinfo.fMask = SIF_ALL;
	scrinfo.nMin = 0;          // 최소값
	scrinfo.nMax = 10000;      // 최대값
	scrinfo.nPage = 10;      // 페이지단위 증가값
	scrinfo.nTrackPos = 0;  // 트랙바가 움직일때의 위치값
	scrinfo.nPos = 0;        // 위치

	this->SetWindowPos(NULL, 0, 0, 1600, 1000, SWP_NOREPOSITION);
	GetDlgItem(IDC_MAP)->SetWindowPos(NULL, 0, 0, 900, 900, SWP_SHOWWINDOW);
	GetDlgItem(IDC_MINIMAP)->SetWindowPos(NULL, 1000, 100, 500, 500,SWP_SHOWWINDOW);


	make_map();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CmapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CmapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{

		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
		
		HDC hdc = ::GetDC(m_map);
		CRect rect_map;
		GetDlgItem(IDC_MAP)->GetClientRect(&rect_map);
		FillRect(hdc, &rect_map, (HBRUSH)GetStockObject(WHITE_BRUSH));
		//::BitBlt(hdc, 0, 0, rect_map.right, rect_map.bottom, backMemDC, (m_HorizonPos * 100), (m_VerticalPos * 100), SRCCOPY);


		if (mapshow_x > 9100) {
			mapshow_x = 9100;
		}
		if (mapshow_y > 9100) {
			mapshow_y = 9100;
		}


		::BitBlt(hdc, 0, 0, rect_map.right, rect_map.bottom, backMemDC, mapshow_x, mapshow_y, SRCCOPY);
		//backMemDC의 완성된 그림을 화면으로 고속복사시킨다.

		HDC hdc_mini = ::GetDC(m_minimap);
		CRect rect_minimap;
		GetDlgItem(IDC_MINIMAP)->GetClientRect(&rect_minimap);
		FillRect(hdc_mini, &rect_minimap, (HBRUSH)GetStockObject(WHITE_BRUSH));
		::BitBlt(hdc_mini, 0, 0, rect_minimap.right, rect_minimap.bottom, back_miniDC, 0, 0, SRCCOPY);
		

		//HBRUSH h_null_brush = CreateSolidBrush(NULL_BRUSH);
		HGDIOBJ h_old_brush = SelectObject(hdc_mini, GetStockObject(NULL_BRUSH));
		HPEN h_pen = CreatePen(PS_SOLID,3,RGB(255, 0, 0));
		HGDIOBJ h_old_pen = SelectObject(hdc_mini, h_pen);
		
		
		if (minimap_x > rect_minimap.right-45) {
			minimap_x = rect_minimap.right - 45;
		}
		if (minimap_y > rect_minimap.bottom - 45) {
			minimap_y = rect_minimap.bottom - 45;
		}
		
		Rectangle(hdc_mini, minimap_x, minimap_y, minimap_x +45, minimap_y + 45);

		SelectObject(hdc_mini, h_old_brush);
		SelectObject(hdc_mini, h_old_pen);
		DeleteObject(h_pen);
		//DeleteObject(h_null_brush);

		


	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CmapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
void CmapDlg::Load_Nodes(const string & nodes_file)
{
	float a, b;

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::Statement* stmt;
		sql::ResultSet* res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://localhost:3306", "root", "apple");
		/* Connect to the MySQL test database */
		con->setSchema("dbmap");

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM NODE ");
		Vertex v;
		while (res->next()) {
		
			v.vertexId = res->getInt("i");
			v.x = res->getInt("x");
			v.y = res->getInt("y");
			vertex.push_back(v);
		}
		this->numNodes = vertex.size();
		delete res;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException & e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
	
}


void CmapDlg::Load_Edges(const string & nodes_file)
{
	int a;
	float b;

	try {
		sql::Driver* driver;
		sql::Connection* con;
		sql::Statement* stmt;
		sql::ResultSet* res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://localhost:3306", "root", "apple");
		/* Connect to the MySQL test database */
		con->setSchema("dbmap");

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM EDGE ");
		Edge e;
		while (res->next()) {			
			e.startId = res->getInt("s");
			e.endId = res->getInt("e");
			e.weight = res->getInt("w");
			edge.push_back(e);
		}
		this->numEdges = edge.size();
		delete res;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException & e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}



BOOL CAboutDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.cx = 15;
	cs.cy = 15;
	return CDialogEx::PreCreateWindow(cs);
}


void CmapDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_VERT, &scrollInfo);
	int fixedValue = 0;
	switch (nSBCode)
	{
	case SB_PAGELEFT:
		m_VerticalPos -= 8;
		break;
	case SB_PAGERIGHT:
		m_VerticalPos += 8;
		break;
	case SB_LINELEFT:
		m_VerticalPos--;
		break;
	case SB_LINERIGHT:
		m_VerticalPos++;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		if (nSBCode == SB_THUMBPOSITION)
			m_VerticalPos = scrollInfo.nPos;
		else
			m_VerticalPos = scrollInfo.nTrackPos;
		break;
	}
	if (m_VerticalPos > scrollInfo.nMax)
		m_VerticalPos = scrollInfo.nMax - 1;
	else if (m_VerticalPos < scrollInfo.nMin)
		m_VerticalPos = scrollInfo.nMin;

	scrollInfo.fMask = SIF_POS;
	scrollInfo.nPos = m_VerticalPos;
	SetScrollInfo(SB_VERT, &scrollInfo);
	
	mapshow_y = (m_VerticalPos * 100);
	minimap_y = (m_VerticalPos * 100) / 20;
	
	Invalidate();

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CmapDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SCROLLINFO scrollInfo;
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	GetScrollInfo(SB_HORZ, &scrollInfo);
	int fixedValue = 0;
	switch (nSBCode)
	{
	case SB_PAGEUP:
		m_HorizonPos -= 8;
		break;
	case SB_PAGEDOWN:
		m_HorizonPos += 8;
		break;
	case SB_LINEUP:
		m_HorizonPos--;
		break;
	case SB_LINEDOWN:
		m_HorizonPos++;
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:	
		if (nSBCode == SB_THUMBPOSITION)
			m_HorizonPos = scrollInfo.nPos;
		else
			m_HorizonPos = scrollInfo.nTrackPos;
		break;
	}
	if (m_HorizonPos > scrollInfo.nMax)
		m_HorizonPos = scrollInfo.nMax - 1;
	else if (m_HorizonPos < scrollInfo.nMin)
		m_HorizonPos = scrollInfo.nMin;

	scrollInfo.fMask = SIF_POS;
	scrollInfo.nPos = m_HorizonPos;
	SetScrollInfo(SB_HORZ, &scrollInfo);
	

	
	mapshow_x = (m_HorizonPos * 100);
	minimap_x = (m_HorizonPos * 100) / 20;
	
	Invalidate();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



bool CmapDlg::SaveMAP(CDC* pDC, CRect BitmapSize, int BitCount, const char * filename)
{
	// TODO: 여기에 구현 코드 추가.

	
	CDC memDC;
	CBitmap Bitmap, *pOldBmp;

	HANDLE handle;
	HANDLE hDIB;
	HPALETTE hPal = NULL;

	BITMAP bm;
	BITMAPINFOHEADER bi;
	LPBITMAPINFOHEADER lpbi;

	DWORD dwLen;

	memDC.CreateCompatibleDC(pDC);
	Bitmap.CreateCompatibleBitmap(pDC, BitmapSize.Width(), BitmapSize.Height());
	pOldBmp = (CBitmap*)memDC.SelectObject(&Bitmap);
	memDC.BitBlt(0, 0, BitmapSize.Width(), BitmapSize.Height(), pDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);

	if (hPal == NULL)
		hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

	GetObject(HBITMAP(Bitmap), sizeof(BITMAP), &bm);

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = bm.bmWidth;
	bi.biHeight = bm.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = BitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = bm.bmWidth * bm.bmHeight * 3;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	int nColors = (1 << bi.biBitCount);
	if (nColors > 256)
		nColors = 0;

	dwLen = bi.biSize + nColors * sizeof(RGBQUAD);

	hPal = SelectPalette(pDC->GetSafeHdc(), hPal, FALSE);

	RealizePalette(pDC->GetSafeHdc());

	hDIB = GlobalAlloc(GMEM_FIXED, dwLen);

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	GetDIBits(pDC->GetSafeHdc(), HBITMAP(Bitmap), 0, (DWORD)bi.biHeight,
		(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	if (bi.biSizeImage == 0)
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) * bi.biHeight;

	dwLen += bi.biSizeImage;

	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	GetDIBits(pDC->GetSafeHdc(), HBITMAP(Bitmap), 0, (DWORD)bi.biHeight,
		(LPBYTE)lpbi + (bi.biSize + nColors * sizeof(RGBQUAD)),
		(LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	BITMAPFILEHEADER hdr;

	hdr.bfType = ((WORD)('M' << 8) | 'B');
	hdr.bfSize = (DWORD)(GlobalSize(hDIB) + sizeof(hdr));
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD)(sizeof(hdr) + lpbi->biSize + nColors * sizeof(RGBQUAD));

	char* pBmpBuf;
	DWORD FileSize;
	FileSize = (DWORD)(sizeof(hdr) + GlobalSize(hDIB));
	pBmpBuf = new char[FileSize];

	memcpy(pBmpBuf, &hdr, sizeof(hdr));
	memcpy(pBmpBuf + sizeof(hdr), lpbi, GlobalSize(hDIB));

	FILE *pFile = NULL;
	errno_t err;
	err = fopen_s(&pFile,filename, "wb");

	bool bResult = false;
	if (pFile != NULL)
	{
		fwrite(pBmpBuf, FileSize, 1, pFile);
		fclose(pFile);
		bResult = true;
	}

	delete[] pBmpBuf;
	if (hDIB) GlobalFree(hDIB);
	SelectPalette(pDC->GetSafeHdc(), hPal, FALSE);

	return bResult;
}


bool CmapDlg::make_map()
{
	// TODO: 여기에 구현 코드 추가.
	HDC hdc;
	hdc = ::GetDC(m_map);
	static HBITMAP backBitmap = NULL;

	static RECT rt;
	SetRect(&rt, 0, 0, 10000, 10000);

	backMemDC = CreateCompatibleDC(hdc);
	//MemDC = CreateCompatibleDC(hdc);

	backBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom); //도화지 준비!
	hOldBitmap = (HBITMAP)SelectObject(backMemDC, backBitmap); //도화지 세팅
	FillRect(backMemDC, &rt, (HBRUSH)GetStockObject(WHITE_BRUSH)); //도화지 색 변경

	for (auto e : edge)
	{
		MoveToEx(backMemDC, vertex[e.startId].x, vertex[e.startId].y, NULL);
		LineTo(backMemDC, vertex[e.endId].x, vertex[e.endId].y);

	}
	if (draw)

	{

		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(backMemDC, hBrush);
		HPEN hPen = CreatePen(PS_DOT, 3, RGB(255, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(backMemDC, hPen);

		for (int r = 0; r < redsun.size() - 1; r++)
		{
			MoveToEx(backMemDC, vertex[redsun.at(r)].x, vertex[redsun.at(r)].y, NULL);
			LineTo(backMemDC, vertex[redsun.at(r + 1)].x, vertex[redsun.at(r + 1)].y);
		}
		SelectObject(backMemDC, oldPen);
		DeleteObject(hPen);
		SelectObject(backMemDC, oldBrush);
		DeleteObject(hBrush);
	}

	CImage image;
	image.Create(rt.right, rt.bottom, 24, 0);
	::BitBlt(image.GetDC(), 0, 0, rt.right, rt.bottom, backMemDC, 0, 0, SRCCOPY);
	
	image.Save(L"test.png", Gdiplus::ImageFormatPNG);// 캡쳐한 이미지를 "test.png" 라는 이름으로 저장한다.
	
	image.ReleaseDC();
	image.Destroy();
	
	DeleteDC(hdc);



	HDC hdc_mini = ::GetDC(m_minimap);
	HBITMAP back_miniBitmap = NULL;

	RECT rt_mini;
	SetRect(&rt_mini, 0, 0, 500, 500);


	FillRect(hdc_mini, &rt_mini, (HBRUSH)GetStockObject(WHITE_BRUSH));
	back_miniDC = CreateCompatibleDC(hdc_mini);

	back_miniBitmap = CreateCompatibleBitmap(hdc_mini, rt_mini.right, rt_mini.bottom); //도화지 준비!
	hOld_miniBitmap = (HBITMAP)SelectObject(back_miniDC, back_miniBitmap); //도화지 세팅
	FillRect(back_miniDC, &rt_mini, (HBRUSH)GetStockObject(WHITE_BRUSH)); //도화지 색 변경

	for (auto e : edge)
	{
		MoveToEx(back_miniDC, vertex[e.startId].x/20, vertex[e.startId].y / 20, NULL);
		LineTo(back_miniDC, vertex[e.endId].x / 20, vertex[e.endId].y / 20);

	}

	DeleteDC(hdc_mini);
	 
	return true;
}


void CmapDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	DeleteObject(SelectObject(backMemDC, hOldBitmap));
	DeleteObject(SelectObject(back_miniDC, hOld_miniBitmap));
	DeleteDC(backMemDC);
	//추가
	/*if (m_connect_flag == 1)
	{
		if (mh_odbc != SQL_NULL_HDBC)
			SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
		if (mh_environment != SQL_NULL_HENV)
			SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
	}*/
	//delete map_image;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CmapDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rt;
	m_minimap.GetClientRect(&rt);
	
	GetCursorPos(&point);
	GetDlgItem(IDC_MINIMAP)->ScreenToClient(&point);
	
	//
	if (rt.PtInRect(point)) {
		//mapshow_x = point.x;
		mapshow_x = point.x*20;
		mapshow_y = point.y*20;
		minimap_x = point.x;
		minimap_y = point.y;
		TRACE("x is &d \n y is &d \n", minimap_x, minimap_y);
		Invalidate();
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


#include <time.h>
void CmapDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStdioFile fileTest;
	TCHAR* pszFileName = _T("elapsed_time.txt");

	// do stuff that may cause exceptions
	CFileException ex;
	if (!fileTest.Open(pszFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &ex))
	{
		ex.ReportError();
		fileTest.Abort();   // close file safely and quietly
	}

	clock_t startTime, endTime;
	startTime = clock();
	//--------------------------------------------------------------------------------------
	// 시간 측정 함수 시작
	//--------------------------------------------------------------------------------------



	ifstream inputs("input.txt", ios::in);
	int start, end;
	while (inputs >> start >> end) {
		s = start + 1;
		g = end + 1;
	}
	n = vertex.size();
	m = edge.size();
	Graph gp;
	for (int i = 0; i < n; i++) {
		gp.addVertex(i + 1);
	}
	for (int i = 0; i < m; i++) {
		int u, v, w;
		u = edge[i].startId + 1;
		v = edge[i].endId + 1;
		w = edge[i].weight;
		gp.addAdj(u, v, w);
	}
	gp.dijkstra(s, g);
	redsun = gp.temp1;
	MessageBox(L"input.txt파일을 읽어 최단 경로를 그립니다.");

	draw = true;



	//--------------------------------------------------------------------------------------
	// 시간 측정 함수 종료
	//--------------------------------------------------------------------------------------
	endTime = clock();

	CString str;
	str.Format(_T("elapsed_time : %6.2f sec \n"), ((float)(endTime - startTime)) / CLOCKS_PER_SEC);

	fileTest.WriteString(str);
	fileTest.Close();

	make_map();
	
	
	Invalidate();
}
