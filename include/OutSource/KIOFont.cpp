#include "stdafx.h"
#include "KIOFont.h"


KIOFont::KIOFont()
{
}


KIOFont::~KIOFont()
{
}

KIOFont::KIOFont(char* ttf, int FSize)
{
	// ������ ����� ��������� �����
	this->Font = new FTGLBitmapFont(ttf);
	// ������������� ������ ������ ��� ������� � �������
	if (!Font->FaceSize(FSize)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}
	fontSize = FSize;
	// ������������� �������
	//Font->Depth(FDepth);
	Font->Depth(16);
	// � ��������� - ������
	Font->CharMap(ft_encoding_unicode);

}

void KIOFont::SetFontSize(double size) {
	
	if (!Font->FaceSize(size)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}
	fontSize = size;
}

double KIOFont::GetFontSize()
{
	return fontSize;
}

