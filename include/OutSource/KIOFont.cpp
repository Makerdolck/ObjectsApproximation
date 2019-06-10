#include "stdafx.h"
#include "KIOFont.h"



KIOFont::KIOFont()
{
}


KIOFont::~KIOFont()
{
}

KIOFont::KIOFont(char* badgeTTF, char* russianTTF, int FSize)
{
	// ������ ����� ��������� �����
	this->BadgeFont = new FTGLBitmapFont(badgeTTF);
	this->RussianFont = new FTGLBitmapFont(russianTTF);
	// ������������� ������ ������ ��� ������� � �������
	if (!BadgeFont->FaceSize(FSize)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}
	if (!RussianFont->FaceSize(FSize)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}

	fontSize = FSize;
	// ������������� �������
	BadgeFont->Depth(16);
	//RussianFont->Depth(16);
	// � ��������� - ������
	BadgeFont->CharMap(ft_encoding_unicode);

	
	//RussianFont->CharMap(ft_encoding_unicode);

}


void KIOFont::SetFontSize(double size) {
	
	if (!BadgeFont->FaceSize(size)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}
	if (!RussianFont->FaceSize(size)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}


	fontSize = size;
}

double KIOFont::GetFontSize()
{
	return fontSize;
}

