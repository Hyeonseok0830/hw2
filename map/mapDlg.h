
// mapDlg.h: 헤더 파일
//

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include<string>
//추가
//#include <odbcinst.h>
//#include <sqlext.h>

using namespace std;
// CmapDlg 대화 상자

typedef int Coordinate;

typedef pair<Coordinate, Coordinate> CoordinatePair;
typedef unsigned int EdgeWeight;
typedef unsigned int NodeID;
typedef unsigned int EdgeID;
typedef pair<NodeID, EdgeWeight> NodeEdgeWeightPair;
typedef pair<NodeID, NodeEdgeWeightPair> edge_info;


struct Vertex
{
	unsigned int vertexId;
	unsigned int x;
	unsigned int y;
};
struct Edge
{
	unsigned int startId;
	unsigned int endId;
	unsigned int weight;
};

class CmapDlg : public CDialogEx
{
	//추가
	//SQLHANDLE mh_environment;
	//SQLHDBC mh_odbc;
	//char m_connect_flag = 0;
// 생성입니다.
public:
	CmapDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	// 구현
	vector<CoordinatePair> coordinates; // 각 노드의 x,y좌표
	vector<edge_info> edges;	// 간선 정보( 출발 노드, 도착노드, 거리), 인접리스트
	vector<unsigned int> firstEdgeIndex;
	NodeID node_id;
	unsigned int numNodes;
	unsigned int numEdges;

	vector<Vertex> vertex;
	vector<Edge> edge,test;
	
	int minimap_x = 0;
	int minimap_y = 0;

	int mapshow_x = 0;
	int mapshow_y = 0;

	int m_HorizonPos;
	int m_VerticalPos;
	void Load_Nodes(const string & nodes_file);
	void Load_Edges(const string & nodes_file);
public:
	CStatic m_map;
	vector <int> redsun;
	int n, m, s,g;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool SaveMAP(CDC* pDC, CRect BitmapSize, int BitCount, const char * filename);
	
	HDC backMemDC;
	HDC back_miniDC;
	HBITMAP hOldBitmap;
	HBITMAP hOld_miniBitmap;
	bool make_map();
	afx_msg void OnDestroy();
	CStatic m_minimap;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	

	afx_msg void OnBnClickedButton1();
};
