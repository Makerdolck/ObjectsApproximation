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
	// Создаём новый битмапный шрифт
	this->Font = new FTGLBitmapFont(ttf);
	// Устанавливаем размер шрифта или выходим с ошибкой
	if (!Font->FaceSize(FSize)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}
	fontSize = FSize;
	// Устанавливаем глубину
	//Font->Depth(FDepth);
	Font->Depth(16);
	// И кодировку - юникод
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

