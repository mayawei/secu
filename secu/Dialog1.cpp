// Dialog1.cpp: 实现文件
//

#include "stdafx.h"
#include "secu.h"
#include "Dialog1.h"
#include "afxdialogex.h"
#include<fstream>
#include<tchar.h>
#include<ostream>
#include<Windows.h>
#include"textDialog1.h"
#include<thread>
using namespace std;
extern void lex(FILE*b, char * &c);
extern std::string lexcfg(FILE * b);
wstring Dialog1::s2ws(const string& str)
{
	int nLen = (int)str.length();
	wstring wstr;
	wstr.resize(nLen, L' ');
	int nResult = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), nLen, (LPWSTR)wstr.c_str(), nLen);
	return wstr;
};

void Tr(const TCHAR * tchar, char * _char)
{
	int i;
	i = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, i, NULL, NULL);
}

int remove_space(char * buf)
{
	int j = 0;
	for (int i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] == ' ' || buf[i] == '\n' || buf[i] == '\t' || buf[i] == '\r') continue;
		else
		{
			buf[j] = buf[i];
			j++;
		}
	}
	buf[j] = '\0';
	return j;
}

void delline(char* buf)
{
	int i = 0;
	for (int j = 0; buf[j] != '\0'; j++)
	{
		if (buf[j] == '\r'&&buf[j + 1] == '\n')
		{
			for (; i <= j - 1; i++)
			{
				buf[i] = ' ';
			}
			break;
		}
	}
}

void remove_sharp(char * buf)
{
	int j = 0;
	int state = 0;
	if (buf[0] == '#')
	{
		delline(buf);
	}
	for (int i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] == '#'&&buf[i - 1] == '\n'&&buf[i - 2] == '\r')
		{
			delline(buf + i);
		}
	}
}

void remove_comment(char *buf, size_t size)
{
	char *p, *end, c;
	char *sq_start, *dq_start;
	char *lc_start, *bc_start;
	size_t len;
	p = buf;
	end = p + size;
	sq_start = dq_start = NULL;
	lc_start = bc_start = NULL;
	while (p < end)
	{
		c = *p;
		switch (c)
		{
		case '\'':
		{
			if (dq_start || lc_start || bc_start)
			{
				p++;
				continue;
			}
			if (sq_start == NULL)
			{
				sq_start = p++;

			}
			else
			{
				len = (p++) - sq_start;
				if (len == 2 && *(sq_start + 1) == '\\')
				{
					continue;
				}
				sq_start = NULL;
			}
			break;
		}
		case '\"':
		{
			if (sq_start || lc_start || bc_start)
			{
				p++;
				continue;
			}
			if (dq_start == NULL)
			{
				dq_start = p++;
			}
			else if (*((p++) - 1) == '\\')
			{
				continue;
			}
			dq_start = NULL;
		}
		case '/':
		{
			if (sq_start || dq_start || lc_start || bc_start)
			{
				p++;
				continue;
			}
			c = *(p + 1);
			if (c == '/')
			{
				lc_start = p;
				p += 2;
			}
			else if (c == '*')
			{
				bc_start = p;
				p += 2;
			}
			else
			{
				p++;
			}
		}
		case '*':
		{


			if (sq_start || dq_start || lc_start || !bc_start)
			{
				p++;
				continue;
			}
			if (*(p + 1) != '/')
			{
				p++;
				continue;
			}
			p += 2;
			memset(bc_start, ' ', p - bc_start);
			bc_start = NULL;
			break;
		}
		case '\n':
		{
			if (lc_start == NULL)
			{
				p++;
				continue;
			}
			c = *(p - 1);
			memset(lc_start, ' ', (c == '\r' ? ((p++) - 1) : p++) - lc_start);
			lc_start = NULL;
			break;
		}
		default:
			p++;
			break;
		}
		if (lc_start)
		{
			memset(lc_start, ' ', p - lc_start);
		}
	}
}

int minn(int one, int two, int three)
{
	return (one = one < two ? one : two) < three ? one : three;
}

struct maxret
{
	int start1;
	int start2;
	int longest;
};

maxret getmaxlength(const string& str1, const string& str2)
{
	size_t size1 = str1.size();
	size_t size2 = str2.size();
	if (size1 == 0 || size2 == 0) return {0,0,0};
	int start1 = -1;
	int start2 = -1;
	int longest = 0;
	int comparisons = 0;
	for (int i = 0; i < size1; ++i)
	{
		int m = i;
		int n = 0;
		int length = 0;
		while (m < size1 && n < size2)
		{
			++comparisons;
			if (str1[m] != str2[n])
			{
				length = 0;
			}
			else
			{
				++length;
				if (longest < length)
				{
					longest = length;
					start1 = m - longest + 1;
					start2 = n - longest + 1;
				}
			}
			++m;
			++n;
		}
	}
	for (int j = 1; j < size2; ++j)
	{
		int m = 0;
		int n = j;
		int length = 0;
		while (m < size1 && n < size2)
		{
			++comparisons;
			if (str1[m] != str2[n])
			{
				length = 0;
			}
			else
			{
				++length;
				if (longest < length)
				{
					longest = length;
					start1 = m - longest + 1;
					start2 = n - longest + 1;
				}
			}
			++m;
			++n;
		}
	}
	maxret r;
	r.start1 = start1;
	r.start2 = start2;
	r.longest = longest;
	return r;
}

int compare(string target, string conststr,wstring * samestr)
{
	int m = conststr.length();
	int n = target.length();
	if (m <= 40 || n <= 40) return 0;
	string a;
	a.assign(conststr);
	string b;
	b.assign(target);
	int same = 0;
	int r;
	
	maxret out=getmaxlength(a,b);
	if (out.longest == 0) return 0;
	int len = out.longest;
	string t;
	t.assign(a, out.start1, out.longest);
	*samestr += Dialog1::s2ws(t);
	return len;
}
std::string deal(const TCHAR * filepath)
{
	FILE * fp;
	fp = _tfopen(filepath, _T("rb"));
	char * c;
	lex(fp,c);
	std::string a(c);
	free(c);
	fclose(fp);
	int length1 = a.length();
	char * buf = new char[length1 + 1];
	memset(buf, 0, length1 * sizeof(char) + 1);
	strcpy(buf, a.c_str());
	remove_sharp(buf);
	remove_comment(buf, length1 + 1);
	length1 = remove_space(buf);
	std::string s(buf);
	free(buf);
	return s;
};

std::string dealcfg(const TCHAR * filepath)
{
	TCHAR filepathcfg[MAX_PATH];
	_tcscpy(filepathcfg, filepath);
	_tcscat(filepathcfg, _T(".cfg"));
	FILE * fp;
	fp = _tfopen(filepathcfg, _T("rb"));
	std::string a = lexcfg(fp);
	fclose(fp);
	CFile TempFile;
	TempFile.Remove(filepathcfg);
	return a;
};

std::string dealcfg_open(const TCHAR * filepath)
{
	TCHAR filepathcfg[MAX_PATH];
	_tcscpy(filepathcfg, filepath);
	_tcscat(filepathcfg, _T(".cfg"));
	FILE * fp;
	fp = _tfopen(filepathcfg, _T("rb"));
	
	ifstream f;
	f.open(filepathcfg, ios::binary || ios::in);
	f.seekg(0, ios::end);
	int length = f.tellg();
	f.seekg(0, ios::beg);
	char * buf = new char[length + 1];
	f.read(buf, length);
	f.close();
	string c(buf);
	string j;
	for (int i = 0; i < c.length(); i++)
	{
		if (c[i] == '\n') j += '\r';
		j += c[i];
	}
	textDialog1 dlg(_T("cfg"), Dialog1::s2ws(j));
	dlg.DoModal();

	std::string a = lexcfg(fp);
	fclose(fp);
	CFile TempFile;
	TempFile.Remove(filepathcfg);
	return a;
};

void cflow(const TCHAR * filepath)
{
	char filepathchar[MAX_PATH];
	Tr(filepath, filepathchar);
	string comm = "C:\\Users\\42287\\Desktop\\cflow.exe ";
	comm += filepathchar;
	comm += " -o ";
	comm += filepathchar;
	comm += ".cfg";
	system(comm.c_str());
}
void cal(int * score,TCHAR filelist[][MAX_PATH],string s,int j,int start,int core, textDialog1 *dlg,int *cc)
{
	wstring *samestr = new wstring;
	string s2;
	wstring r = L"文件";
    dlg = new  textDialog1(r+filelist[0], *samestr);
	dlg->Create(IDD_DIALOG3, NULL);
	dlg->ShowWindow(SW_SHOW);
	dlg->SetWindowPos(NULL, 600 * core, 0, 0, 0, SWP_SHOWWINDOW|SWP_NOSIZE);
	dlg->RedrawWindow();
	for (int i = start; i < start + j; i++)
	{
		s2 = deal(filelist[i]);
		score[i] = 100 * compare(s, s2, samestr) / s.length();
		dlg->settxt(r + filelist[i], *samestr);
		dlg->RedrawWindow();
		(*cc)++;
	}
	delete(samestr);
};
void cal_cfg(int * score, TCHAR filelist[][MAX_PATH], string s, int j, int start, int core, textDialog1 *dlg, int *cc)
{
	wstring *samestr = new wstring;
	string s2;
	wstring r = L"文件";
	dlg = new  textDialog1(r + filelist[0], *samestr);
	dlg->Create(IDD_DIALOG3, NULL);
	dlg->ShowWindow(SW_SHOW);
	dlg->SetWindowPos(NULL, 600 * core, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
	dlg->RedrawWindow();
	for (int i = start; i < start + j; i++)
	{
		cflow(filelist[i]);
		s2 = dealcfg(filelist[i]);
		score[i] = 100 * compare(s, s2, samestr) / s.length();
		dlg->settxt(r + filelist[i], *samestr);
		dlg->RedrawWindow();
		(*cc)++;
	}
	delete(samestr);
};
IMPLEMENT_DYNAMIC(Dialog1, CDialogEx)

Dialog1::Dialog1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

Dialog1::~Dialog1()
{
}

void Dialog1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dialog1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Dialog1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Dialog1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Dialog1::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &Dialog1::OnBnClickedButton4)
END_MESSAGE_MAP()


// Dialog1 消息处理程序


void Dialog1::OnBnClickedButton1()
{
	CEdit* edit1;
	edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	TCHAR filename[MAX_PATH] = { 0 };
	OPENFILENAME filepath = { 0 };
	filepath.lpstrInitialDir = _T(".\\");
	filepath.hwndOwner = NULL;
	filepath.lStructSize = sizeof(filepath);
	filepath.lpstrFile = filename;
	filepath.nMaxFile = sizeof(filename) / sizeof(*filename);
	filepath.nFilterIndex = 0;
	filepath.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
fai:	BOOL bSel = GetOpenFileName(&filepath);
	if (bSel) MessageBox(_T("打开文件成功"));
	else
	{
		MessageBox(_T("打开文件失败"));
		goto fai;
	}
	edit1->SetWindowText(filename);
}


void Dialog1::OnBnClickedButton2()
{
	CEdit* edit2;
	edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
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

void Dialog1::OnBnClickedButton3()
{	
	TCHAR filepath[MAX_PATH] = { 0 };
	TCHAR filepath2[MAX_PATH] = { 0 };
	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	edit1->GetWindowText(filepath, MAX_PATH);
	std::string s = deal(filepath);


	fstream fin2;
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	edit2->GetWindowText(filepath, MAX_PATH);
	_tcscpy(filepath2, filepath);
	_tcscat(filepath, _T("\\"));
	_tcscat(filepath2, _T("/*.*"));
	WIN32_FIND_DATA wfd;
	HANDLE hfind;
	hfind = FindFirstFile(filepath2, &wfd);
	TCHAR filelist[100][MAX_PATH] = { 0 };
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
	i--;
	FindClose(hfind);
	fin2.close();
	int *score = new int[i+1];
	memset(score, 0, (i+1) * sizeof(int));
	char c2;
	int kin = (i + 1) % 3;
	int i1 = (i + 1) / 3;
	int i2 = (i + 1) / 3;
	int i3 = (i + 1) / 3;
	if (kin == 1)
	{
		i1++;
	}
	else if (kin == 2)
	{
		i2++; i1++;
	}
	textDialog1 * dlg1, *dlg2, *dlg3;
	int cc = 0;
	thread *t1 = new thread(cal, score, filelist, s, i1,0,0,dlg1,&cc);
	thread *t2 = new thread(cal, score, filelist, s, i2,i1,1,dlg2,&cc);
	thread *t3 = new thread(cal, score, filelist, s, i3,i1 + i2,2,dlg3,&cc);

	CProgressCtrl *myProCtrl2 = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS1);
	int head, tail;
	myProCtrl2->GetRange(head, tail);
	int lenn = (tail - head);
	wstring tempstr;
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT3);
	while(cc<=i)
	{
		myProCtrl2->SetPos(cc*lenn/i+head);
		myProCtrl2->RedrawWindow();
		for (int j = 0; j <= i; j++)
		{
			if (score[j] == 0) continue;
			tempstr += L"文件";
			tempstr += to_wstring(j);
			tempstr += L"重复率为:";
			tempstr += to_wstring(score[j]);
			tempstr += L"%\r\n";
		}
		LPCTSTR tem = tempstr.c_str();
		edit3->SetWindowText(tem);
		edit3->RedrawWindow();
		tempstr.clear();
		Sleep(100);
	}
	if (t1->joinable()) t1->join();
	if (t2->joinable()) t2->join();
	if (t3->joinable()) t3->join();
	
	for (int j = 0; j <= i; j++)
	{
		tempstr += L"文件";
		tempstr += to_wstring(j);
		tempstr += L"重复率为:";
		tempstr += to_wstring(score[j]);
		tempstr += L"%\r\n";
	}
}

void Dialog1::OnBnClickedButton4()
{
	TCHAR filepath[MAX_PATH] = { 0 };
	TCHAR filepath2[MAX_PATH] = { 0 };
	TCHAR filecfg[MAX_PATH + 10];
	CEdit * edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	edit1->GetWindowText(filepath, MAX_PATH);
	cflow(filepath);
	string s = dealcfg_open(filepath);



	fstream fin2;
	CEdit* edit2 = (CEdit*)GetDlgItem(IDC_EDIT2);
	edit2->GetWindowText(filepath, MAX_PATH);
	_tcscpy(filepath2, filepath);
	_tcscat(filepath, _T("\\"));
	_tcscat(filepath2, _T("/*.*"));
	WIN32_FIND_DATA wfd;
	HANDLE hfind;
	hfind = FindFirstFile(filepath2, &wfd);
	TCHAR filelist[100][MAX_PATH] = { 0 };
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
	i--;
	FindClose(hfind);
	fin2.close();
	
	int *score = new int[i + 1];
	memset(score, 0, (i + 1) * sizeof(int));
	char c2;
	int kin = (i + 1) % 3;
	int i1 = (i + 1) / 3;
	int i2 = (i + 1) / 3;
	int i3 = (i + 1) / 3;
	if (kin == 1)
	{
		i1++;
	}
	else if (kin == 2)
	{
		i2++; i1++;
	}
	textDialog1 * dlg1, *dlg2, *dlg3;
	int cc = 0;
	thread *t1 = new thread(cal_cfg, score, filelist, s, i1, 0, 0, dlg1, &cc);
	thread *t2 = new thread(cal_cfg, score, filelist, s, i2, i1, 1, dlg2, &cc);
	thread *t3 = new thread(cal_cfg, score, filelist, s, i3, i1 + i2, 2, dlg3, &cc);

	CProgressCtrl *myProCtrl2 = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS1);
	int head, tail;
	myProCtrl2->GetRange(head, tail);
	int lenn = (tail - head);
	wstring tempstr;
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT3);
	while (cc <= i)
	{
		myProCtrl2->SetPos(cc*lenn / i + head);
		myProCtrl2->RedrawWindow();
		for (int j = 0; j <= i; j++)
		{
			if (score[j] == 0) continue;
			tempstr += L"文件";
			tempstr += to_wstring(j);
			tempstr += L"重复率为:";
			tempstr += to_wstring(score[j]);
			tempstr += L"%\r\n";
		}
		LPCTSTR tem = tempstr.c_str();
		edit3->SetWindowText(tem);
		edit3->RedrawWindow();
		tempstr.clear();
		Sleep(100);
	}
	if (t1->joinable()) t1->join();
	if (t2->joinable()) t2->join();
	if (t3->joinable()) t3->join();
}
