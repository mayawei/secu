// Dialog1.cpp: 实现文件
//

#include "stdafx.h"
#include "secu.h"
#include "Dialog1.h"
#include "afxdialogex.h"
#include<fstream>
#include<tchar.h>
#include<string> 
#include<ostream>
extern std::string lex(FILE * b);
using namespace std;



int remove_space(char * buf)
{
	int j = 0;
	for (int i = 0; buf[i]!='\0'; i++)
	{
		if (buf[i] == ' '||buf[i] == '\n' || buf[i] == '\t' || buf[i] == '\r') continue;
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
	for (int j = 0;buf[j]!='\0';j++)
	{
		if (buf[j] == '\r'&&buf[j + 1] == '\n')
		{
			for (; i<=j-1; i++)
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
	char *p, *end, c;          //p-动态移动的字符指针，end-指向文件末尾的字符指针，c-存储没一个p指向的字符
	char *sq_start, *dq_start; //sq_start-单引号开始位置（single），dq_start-双引号开始（double）
	char *lc_start, *bc_start; //lc_start-//的开始位置，bc_start-/*的开始位置
	size_t len;                //记录某符号结束和开始的位置之差（长度，偏移量）

	p = buf;
	end = p + size;
	sq_start = dq_start = NULL;
	lc_start = bc_start = NULL;

	while (p < end) /*当指针没有到达文件末尾 */
	{
		c = *p;     //用字符变量c存储指针指向的字符
		switch (c) //根据c的值做相应处理
		{
		case '\'': /*处理单引号*/
		{
			if (dq_start || lc_start || bc_start) //当遇到过双引号、//或/*的时候，则不需要再判断'//'的情况了。
			{
				p++;
				continue; //继续下一个，对while而言的
			}
			/*******************************以下是没有遇到过双引号或//或/*的时候*******************************/
			if (sq_start == NULL) /****如果未遇到单引号****/
			{
				sq_start = p++; //start指向单引号的开始位置，p指向下一个

			}
			else /*如果遇到过单引号，sq_start指向单引号开始位置*/
			{
				len = (p++) - sq_start;

				if (len == 2 && *(sq_start + 1) == '\\')

				{

					/*若遇到 “  '\''   ”这种情况则两个单引号并未匹配，遇到的“'”是“\' ”中的，而不是与sq_start所指向单引号匹配*/
					continue;
				}

				sq_start = NULL; //否则将sq_start置位为NULL

			}
			/*******************************以上是没有遇到过双引号或//或/*的时候*******************************/
			break;
		}
		
		case '\"': /*处理双引号*/
		{
			if (sq_start || lc_start || bc_start) //当遇到过单引号、//或/*的时候，则不需要处理
			{
				p++;
				continue;
			}
			/*****************以下是没有遇到过单引号或//或/*的时候*****************/
			if (dq_start == NULL) /*如果没有遇到过双引号*/
			{
				dq_start = p++; //标记遇到了双引号
			}
			else if (*((p++) - 1) == '\\')

			{

				/*若遇到 “" ab\''cd"   ”这种情况则两个双引号并未匹配，遇到的“"”是“\"”中的，而不是与dq_start所指向双引号匹配*/
				continue;
			}
			dq_start = NULL; //如果双引号中不是//，标记为NULL
			/*****************以上是没有遇到过单引号或//或/*的时候*****************/
		}

		case '/': //斜杠，注意这个斜杠也可以是'//'，"//",//,/*/中的第二个斜杠，但会在下面第二行代码中被忽略掉
		{
			if (sq_start || dq_start || lc_start || bc_start) //如果是单引号、双引号、斜杠、/*的后面
			{
				p++;
				continue;
			}
			/***********************下面是遇到注释//或/*的时候****************************/
			c = *(p + 1); //否则c取p指向字符的下一个字符
			if (c == '/') //遇到了双斜杠
			{
				lc_start = p; //标记双斜杠的开始
				p += 2; //p指向双斜杠后面的字符
			}
			else if (c == '*') //遇到了/*
			{
				bc_start = p; //标记/*的开始
				p += 2; //p指向/*后面的字符
			}
			/*************************上面是遇到注释//或/*的时候**************************/
			else
			{
				p++;
			}
		}
		case '*': //星号，同斜杠，但少了如果遇到/*的情况，因为遇到这种情况后，要判断是不是遇到结束的地方*/了
		{
			
			
			if (sq_start || dq_start || lc_start||!bc_start) //如果是单引号、双引号、斜杠、/*的后面
			{
				p++;
				continue;
			}
			if (*(p + 1) != '/') //如果星号后面紧跟的不是斜杠，那么忽略过。
			{
				p++;
				continue;
			}
			p += 2; //否则p指向斜杠后面那个字符。注意下面的清空语句，p指向的那个字符并不会被清除。
			memset(bc_start, ' ', p - bc_start); //清空/* …… */中间的内容包括注释符号本身。
			bc_start = NULL;
			break;
		}
		case '\n': /*换行符，主要处理遇到双斜杠时，需要清除双斜杠到\n的前面的字符*/
		{
			if (lc_start == NULL) //如果还没有遇到双斜杠，那么忽略
			{
				p++;
				continue;
			}
			c = *(p - 1);
			/*如果遇到过双斜杠，清空双斜杠本身和到\n前面的那个字符，p指向下一个字符，/r是回车符（光标退回到最前面），这里要判断c == '\r'是因为在UNIX系统下文件结尾的换行只有\n，而windows系统下文件结尾的换行为\r\n   */
			memset(lc_start, ' ', (c == '\r' ? ((p++) - 1) : p++) - lc_start);
			lc_start = NULL;
			break;
		}
		default:
			p++;
			break;
		}
		/****************************************************
		如果遇到双斜杠，这个if语句存在的意义在于万一最后
		一行代码是带有双斜杠但没有给换行符\n的，也要清除掉。
		  *****************************************************/
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

int compare(string str, string target)
{
	int n = str.length();
	int m = target.length();
	int i;                  // 遍历str的
	int j;                  // 遍历target的
	char ch1;               // str的
	char ch2;               // target的
	int temp;               // 记录相同字符,在某个矩阵位置值的增量,不是0就是1
	if (n == 0) { return m; }
	if (m == 0) { return n; }
	int *d = (int*)malloc((n + 1)*(m + 1) * sizeof(int));
	for (i = 0; i <= n; i++)
	{                       // 初始化第一列
		d[i*(m + 1)] = i;
	}

	for (j = 0; j <= m; j++)
	{                       // 初始化第一行
		d[j] = j;
	}

	for (i = 1; i <= n; i++)
	{                       // 遍历str
		ch1 = str[i - 1];
		// 去匹配target
		for (j = 1; j <= m; j++)
		{
		
	ch2 = target[j - 1];
			if (ch1 == ch2 || ch1 == ch2 + 32 || ch1 + 32 == ch2)
			{
				temp = 0;
			}
			else
			{
				temp = 1;
			}
			// 左边+1,上边+1, 左上角+temp取最小
			d[i*(m + 1) + j] = minn(d[(i - 1)*(m + 1) + j] + 1, d[i*(m + 1) + j - 1] + 1, d[(i - 1)*(m + 1) + j - 1] + temp);
		}
	}
	return d[n*(m + 1) + m];
}


float getSimilarityRatio(string str, string target)
{
	return 1 - (float)compare(str, target) / max(str.length(), target.length());
}

std::string deal(TCHAR * filepath)
{
	FILE * fp;
	fp = _tfopen(filepath, _T("rb"));
	std::string a = lex(fp);
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



	//test
	//fstream fout;
	//fout.open("testout.txt", ios::out | ios::binary);
	//int len = length1;
	//char * t = (char*)malloc(len * sizeof(char));
	//strcpy(t, s.c_str());
	//fout.write(t, len);
	//fout.close();
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
	int *score = new int[i];
	char c2;
	for (int j = 0; j <= i; j++)
	{
		string s2 = deal(filelist[j]);
		score[j] = getSimilarityRatio(s, s2) * 100;
	}
	wstring tempstr;
	for (int j = 0; j <= i; j++)
	{
		tempstr += L"文件";
		tempstr += to_wstring(j);
		tempstr += L"重复率为:";
		tempstr += to_wstring(score[j]);
		tempstr += L"%\r\n";
	}
	CEdit* edit3 = (CEdit*)GetDlgItem(IDC_EDIT3);
	LPCTSTR tem = tempstr.c_str();
	edit3->SetWindowText(tem);
}

void Dialog1::OnBnClickedButton4()
{
	
}
