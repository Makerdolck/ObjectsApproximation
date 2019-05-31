#ifndef __MYFONT_H
#define __MYFONT_H

#include <wchar.h>      // ����� ������������ � ��������� ������� ����, �� �����
// ������������ ������, ������� �������� � ���������� ���� wchar_t

// ������ FTGL:
#include <ftgl/FTFont.h>
#include <ftgl/FTGLBitmapFont.h>
#include <ftgl/FTGLOutlineFont.h>

class MyFont {

public:
	MyFont();
	MyFont(char* ttf, int FSize, int FDepth);
	~MyFont();

	// ����������� ������ - ������� ��� ��� .ttf-�����, ������ � ������� ������.
	


	// ����������� ��� ����� ��������� �����
	FTFont* Font;
	

	// ������� ������� ������ ������
	void Print(double x, double y, double z, const wchar_t* text);
	void Print(double x, double y, const wchar_t* text);
	void SetFontSize(double size);
	double GetFontSize();
private:
	double fontSize;
};


#endif
