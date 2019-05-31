#ifndef __MYFONT_H
#define __MYFONT_H

#include <wchar.h>      // Чтобы использовать в программе русский язык, мы будем
// использовать юникод, который хранится в переменных типа wchar_t

// Хидеры FTGL:
#include <ftgl/FTFont.h>
#include <ftgl/FTGLBitmapFont.h>
#include <ftgl/FTGLOutlineFont.h>

class MyFont {

public:
	MyFont();
	MyFont(char* ttf, int FSize, int FDepth);
	~MyFont();

	// Конструктор класса - передаём ему имя .ttf-файла, размер и глубину шрифта.
	


	// Создаваемый для нашей программы шрифт
	FTFont* Font;
	

	// Простая функция печати текста
	void Print(double x, double y, double z, const wchar_t* text);
	void Print(double x, double y, const wchar_t* text);
	void SetFontSize(double size);
	double GetFontSize();
private:
	double fontSize;
};


#endif
