// Dialog2.cpp: 实现文件
//

#include "stdafx.h"
#include "secu.h"
#include "Dialog2.h"
#include "afxdialogex.h"
#include<string>
#include<sstream>
#include<fstream>
#include<iostream>
#include"textDialog1.h"
#include<thread>
#include<windows.h>
int cores;
wstring convert[7] = { L"",L"short",L"unsigned short",L"int",L"unsigned int",L"long",L"unsigned long" };


using namespace std;
int getcores()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
};
struct errint
{
	string s;
	string veri[2] = { "","" };
	int line;
	string length[2] = { "","" };
	int type[2] = { 0,0 };
};
struct retint
{
	errint *a;
	int len;
};
struct err
{
	string s;
	string veri[5] = { "","" ,"","" ,"" };
	int line;
	int vs;
	string length[5] = { "","" ,"","","" };
};
struct retf
{
	err *a;
	int len;
};
struct foerr
{
	string s;
	int line;
};
struct foret
{
	foerr * e;
	int length = 0;
};
extern void of1(FILE * input, retf *a);
extern void of2(FILE * input, retf *a);
extern void ofint(FILE * input, retint *a);
extern void offor(FILE * input, foret *a);
wstring s2ws(const string& str)
{
	int nLen = (int)str.length();
	wstring wstr;
	wstr.resize(nLen, L' ');
	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);
	return wstr;
};

int getname(TCHAR *filepath,TCHAR filelist[][MAX_PATH])
{
	TCHAR filepath2[MAX_PATH] = { 0 };
	fstream fin2;
	_tcscpy(filepath2, filepath);
	_tcscat(filepath, _T("\\"));
	_tcscat(filepath2, _T("/*.*"));
	WIN32_FIND_DATA wfd;
	HANDLE hfind;
	hfind = FindFirstFile(filepath2, &wfd);
	int i = 0;
	_tcscpy(filelist[0], wfd.cFileName);
	for (; ((i <= 99) && FindNextFile(hfind, &wfd)); i++)
	{
		if (wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
		{
			i--;  continue;
		}
		else
			_tcscpy(filelist[i], filepath);
		_tcscat(filelist[i], wfd.cFileName);
	}
	FindClose(hfind);
	fin2.close();
	return i;
}

wstring stackof(TCHAR * filepath,retf* a)
{
	FILE * fp;
	fp = _tfopen(filepath, _T("rb"));
	of1(fp, a);
	wstring tempstr;
	tempstr.clear();
	tempstr += L"文件";
	tempstr += filepath;
	tempstr += L"\r\n";
	for (int j = 0; j < a->len; j++)
	{
		tempstr += L"line ";
		wostringstream li;
		li << a->a[j].line;
		tempstr += li.str();
		tempstr += L":";
		tempstr += s2ws(a->a[j].s);
		tempstr += L"\r\n";
		for (int k = 0; k <= a->a[j].vs; k++)
		{
			if (!atoi(a->a[j].veri[k].c_str()))tempstr += L"变量";
			tempstr += s2ws(a->a[j].veri[k]);
			if (!atoi(a->a[j].veri[k].c_str()) && a->a[j].length[k].length() != 0)tempstr += L"长度为";
			tempstr += s2ws(a->a[j].length[k]);
			tempstr += L"\t";
		}
		tempstr += L"\r\n";
	}
	fclose(fp);
	return tempstr;
}
//void showstackof(int c, int t, TCHAR filelist[][MAX_PATH])
//{
//	int core = c;
//	int tasks = t;
//	TCHAR (*constfile)[MAX_PATH] = new TCHAR[tasks][MAX_PATH];
//	for (int i = 0; i < tasks; i++)
//	{
//		_tcscpy(constfile[i], filelist[i]);
//	}
//	wstring cap= L"线程";
//	wostringstream li;
//	li << core;
//	cap += li.str();
//	cap += L",文件";
//	wstring tempstr=L"";
//	wstring f= L"0" ;
//	textDialog1 *dlgs = new textDialog1(cap + f, tempstr);
//	dlgs->Create(IDD_DIALOG3, NULL);
//	dlgs->ShowWindow(SW_SHOW);
//	dlgs->SetWindowPos(NULL, 400 * core, 0, 400, 200, SWP_SHOWWINDOW);
//	for (int i = 0; i < tasks; i++)
//	{
//		f.clear();
//		wostringstream t;
//		t << i;
//		f.assign(t.str());
//		tempstr += stackof(constfile[i]);
//		dlgs->settxt(cap + f, tempstr);
//		dlgs->RedrawWindow();	
//	}
//	delete(constfile);
//	delete(dlgs);
//}

wstring heapof(TCHAR * filepath,retf *a)
{
	FILE * fp;
	fp = _tfopen(filepath, _T("rb"));
	of2(fp, a);
	wstring tempstr;
	tempstr.clear();
	tempstr.clear();
	tempstr += L"文件";
	tempstr += filepath;
	tempstr += L"\r\n";
	for (int j = 0; j < a->len; j++)
	{
		tempstr += L"line ";
		wostringstream li;
		li << a->a[j].line;
		tempstr += li.str();
		tempstr += L":";
		tempstr += s2ws(a->a[j].s);
		tempstr += L"\r\n";
		for (int k = 0; k <= a->a[j].vs; k++)
		{
			if (!atoi(a->a[j].veri[k].c_str()))tempstr += L"变量";
			tempstr += s2ws(a->a[j].veri[k]);
			if (!atoi(a->a[j].veri[k].c_str()) && a->a[j].length[k].length() != 0)tempstr += L"长度为";
			tempstr += s2ws(a->a[j].length[k]);
			tempstr += L"\t";
		}
		tempstr += L"\r\n";
	}
	return tempstr;
};

wstring intof(TCHAR * filepath, retint* a)
{
	FILE * fp;
	fp = _tfopen(filepath, _T("rb"));
	
	ofint(fp, a);
	wstring tempstr;
	tempstr.clear();
	tempstr.clear();
	tempstr += L"文件";
	tempstr += filepath;
	tempstr += L"\r\n";
	for (int j = 0; j < a->len; j++)
	{
		if (((a->a[j].type[0] + 1) / 2) >= ((a->a[j].type[1] + 1) / 2)) break;
		tempstr += L"line ";
		wostringstream li;
		li << a->a[j].line;
		tempstr += li.str();
		tempstr += L":";
		tempstr += s2ws(a->a[j].s);
		tempstr += L"\r\n";
		for (int k = 0; k <= 1; k++)
		{
			tempstr += convert[a->a[j].type[k]];
			if (!atoi(a->a[j].veri[k].c_str()))tempstr += L"变量";
			tempstr += s2ws(a->a[j].veri[k]);
			if (!atoi(a->a[j].veri[k].c_str()) && a->a[j].length[k].length() != 0)tempstr += L"值为";
			tempstr += s2ws(a->a[j].length[k]);
			tempstr += L"\t";
		}
		tempstr += L"\r\n";
	}
	return tempstr;
};

wstring form(TCHAR * filepath, foret *a)
{
	FILE * fp;
	fp = _tfopen(filepath, _T("rb"));

	offor(fp, a);
	wstring tempstr;
	tempstr.clear();
	tempstr += L"文件";
	tempstr += filepath;
	tempstr += L"\r\n";
	for (int j = 0; j < a->length; j++)
	{
		tempstr += L"line ";
		wostringstream li;
		li << a->e[j].line;
		tempstr += li.str();
		tempstr += L":";
		tempstr += s2ws(a->e[j].s);
		tempstr += L"\r\n";
	}
	return tempstr;
}
IMPLEMENT_DYNAMIC(Dialog2, CDialogEx)

Dialog2::Dialog2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	cores = getcores();
}

Dialog2::~Dialog2()
{
}

void Dialog2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Dialog2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Dialog2::OnBnClickedButton1)
//	ON_BN_CLICKED(IDC_BUTTON2, &Dialog2::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Dialog2::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &Dialog2::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Dialog2::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &Dialog2::OnBnClickedButton6)
END_MESSAGE_MAP()

void Dialog2::OnBnClickedButton1()
{	
	CEdit* edit1;
	edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	TCHAR filename[MAX_PATH] = { 0 };
	OPENFILENAME filepath = { 0 };
	filepath.lpstrInitialDir = _T(".\\");//默认的文件路径 
	filepath.hwndOwner = NULL;
	filepath.lStructSize = sizeof(filepath);
	filepath.lpstrFile = filename;//存放文件的缓冲区 s
	filepath.nMaxFile = sizeof(filename) / sizeof(*filename);
	filepath.nFilterIndex = 0;
	filepath.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT
fai:	BOOL bSel = GetOpenFileName(&filepath);
	if (bSel) MessageBox(_T("打开文件成功"));
	else
	{
		MessageBox(_T("打开文件失败"));
		goto fai;
	}
	edit1->SetWindowText(filename);
}

wstring Dialog2::s2ws(const string& str)
{
	int nLen = (int)str.length();
	wstring wstr;
	wstr.resize(nLen, L' ');
	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);
	return wstr;
};

//void Dialog2::OnBnClickedButton2()
//{
//	TCHAR filepath[MAX_PATH] = { 0 };
//	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT3);
//	edit1->GetWindowText(filepath, MAX_PATH);
//	TCHAR filelist[100][MAX_PATH];
//	int files = getname(filepath, filelist);
//	wstring tempstr;
//	retf a;
//	err c[500];
//	a.a = c;
//	for (int i = 0; i < files; i++)
//	{
//		tempstr += stackof(filelist[i],&a);
//	}
//	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT2);
//	LPCTSTR tem = tempstr.c_str();
//	edit3->SetWindowText(tem);
//	
//}

void Dialog2::OnBnClickedButton3()
{
	TCHAR filepath[MAX_PATH] = { 0 };
	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT3);
	edit1->GetWindowText(filepath, MAX_PATH);
	TCHAR filelist[100][MAX_PATH];
	int files = getname(filepath, filelist);
	wstring tempstr;
	retf a;
	err c[500];
	a.a = c;
	for (int i = 0; i < files; i++)
	{
		tempstr += heapof(filelist[i],&a);
	}
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT2);
	LPCTSTR tem = tempstr.c_str();
	edit3->SetWindowText(tem);
}

void Dialog2::OnBnClickedButton4()
{
	TCHAR filepath[MAX_PATH] = { 0 };
	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT3);
	edit1->GetWindowText(filepath, MAX_PATH);
	TCHAR filelist[100][MAX_PATH];
	int files = getname(filepath, filelist);
	wstring tempstr;
	retint a;
	a.a = new errint[500];
	for (int i = 0; i < files; i++)
	{
		tempstr += intof(filelist[i], &a);
	}
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT2);
	LPCTSTR tem = tempstr.c_str();
	edit3->SetWindowText(tem);
}

void Dialog2::OnBnClickedButton5()
{
	TCHAR filepath[MAX_PATH] = { 0 };
	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT3);
	edit1->GetWindowText(filepath, MAX_PATH);
	TCHAR filelist[100][MAX_PATH];
	int files = getname(filepath, filelist);
	wstring tempstr;
	foret a;
	a.e = new foerr[500];
	for (int i = 0; i < files; i++)
	{
		tempstr += form(filelist[i], &a);
	}
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT2);
	LPCTSTR tem = tempstr.c_str();
	edit3->SetWindowText(tem);
}

void Dialog2::OnBnClickedButton6()
{
	CEdit* edit2;
	edit2 = (CEdit*)GetDlgItem(IDC_EDIT3);
	TCHAR dirname[MAX_PATH] = { 0 };
	BROWSEINFO dir = { 0 };
	dir.hwndOwner = NULL;
	dir.pszDisplayName = dirname;
	dir.lpszTitle = _T("文件夹目录:");
	dir.ulFlags = BIF_RETURNFSANCESTORS;

fai2:	LPITEMIDLIST lpi = SHBrowseForFolder(&dir);
	if (lpi) MessageBox(_T("打开目录成功"));
	else
	{
		MessageBox(_T("打开目录失败"));
		goto fai2;
	}
	SHGetPathFromIDList(lpi, dirname);
	edit2->SetWindowText(dirname);
}
