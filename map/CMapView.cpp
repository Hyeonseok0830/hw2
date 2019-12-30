// CMapView.cpp: 구현 파일
//

#include "stdafx.h"
#include "map.h"
#include "CMapView.h"


// CMapView

IMPLEMENT_DYNCREATE(CMapView, CScrollView)

CMapView::CMapView()
{

}

CMapView::~CMapView()
{
}


BEGIN_MESSAGE_MAP(CMapView, CScrollView)
END_MESSAGE_MAP()


// CMapView 그리기

void CMapView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CMapView 진단

#ifdef _DEBUG
void CMapView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMapView 메시지 처리기
