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
	// Создаём новый битмапный шрифт
	this->BadgeFont = new FTGLBitmapFont(badgeTTF);
	this->RussianFont = new FTGLBitmapFont(russianTTF);
	// Устанавливаем размер шрифта или выходим с ошибкой
	if (!BadgeFont->FaceSize(FSize)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}
	if (!RussianFont->FaceSize(FSize)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}

	fontSize = FSize;
	// Устанавливаем глубину
	BadgeFont->Depth(16);
	//RussianFont->Depth(16);
	// И кодировку - юникод
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

