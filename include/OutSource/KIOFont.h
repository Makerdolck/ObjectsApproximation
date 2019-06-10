#ifndef __KIOFONT_H
#define __KIOFONT_H

#include <wchar.h>      // ����� ������������ � ��������� ������� ����, �� �����
// ������������ ������, ������� �������� � ���������� ���� wchar_t

// ������ FTGL:
#include <ftgl/FTFont.h>
#include <ftgl/FTGLBitmapFont.h>
#include <ftgl/FTGLOutlineFont.h>

class KIOFont {
public:
	KIOFont();
	// ����������� ������ - ������� ��� ��� .ttf-�����, ������ � ������� ������.
	KIOFont(char* badgeTTF, char* russianTTF, int FSize);
	~KIOFont();

	FTFont* BadgeFont; // ���������� �����
	FTFont* RussianFont; // ������� �����
	
	void SetFontSize(double size);
	double GetFontSize();
private:
	double fontSize; // ������� ������ ������
};

#endif
