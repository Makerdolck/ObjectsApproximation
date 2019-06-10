#ifndef __KIOFONT_H
#define __KIOFONT_H

#include <wchar.h>      // Чтобы использовать в программе русский язык, мы будем
// использовать юникод, который хранится в переменных типа wchar_t

// Хидеры FTGL:
#include <ftgl/FTFont.h>
#include <ftgl/FTGLBitmapFont.h>
#include <ftgl/FTGLOutlineFont.h>

class KIOFont {
public:
	KIOFont();
	// Конструктор класса - передаём ему имя .ttf-файла, размер и глубину шрифта.
	KIOFont(char* badgeTTF, char* russianTTF, int FSize);
	~KIOFont();

	FTFont* BadgeFont; // Английский шрифт
	FTFont* RussianFont; // Русский шрифт
	
	void SetFontSize(double size);
	double GetFontSize();
private:
	double fontSize; // Текущий размер шрифта
};

#endif
