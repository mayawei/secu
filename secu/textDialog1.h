#pragma once
#include<string>
using namespace std;
// textDialog1 对话框

class textDialog1 : public CDialogEx
{
	DECLARE_DYNAMIC(textDialog1)

public:
	textDialog1(wstring a,wstring b,CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~textDialog1();
	std::wstring caption,text;
	void settxt(wstring a, wstring b);


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
