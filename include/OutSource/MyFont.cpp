#include "stdafx.h"
#include "MyFont.h"


MyFont::MyFont()
{
}


MyFont::~MyFont()
{
}

MyFont::MyFont(char* ttf, int FSize, int FDepth)
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
	Font->Depth(FDepth);
	// � ��������� - ������
	Font->CharMap(ft_encoding_unicode);

}

void MyFont::Print(double x, double y, double z, const wchar_t* text)
{
	// ������ � ����� ������� � �������� �
	glPushMatrix();
	glLoadIdentity();
	// ��������� ��������
	//glDisable(GL_TEXTURE_2D);
	// ������������ �� ��������� ����������
	glTranslatef(x, y, z);
	glRasterPos3f(-1, 0.5, 0);
	//glRasterPos3f(x, y, z);
	Font->Render(text); // � �������� �����
	//glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}


void MyFont::Print(double x, double y, const wchar_t* text)
{
	// ������ � ����� ������� � �������� �
	glPushMatrix();
	glLoadIdentity();
	// ��������� ��������
	glDisable(GL_TEXTURE_2D);
	// ������������ �� ��������� ����������
	glTranslatef(x, y, -1);
	glRasterPos2f(-1, 0.5);
	Font->Render(text); // � �������� �����
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void MyFont::SetFontSize(double size) {
	
	if (!Font->FaceSize(size)) {
		MessageBox(NULL, L"Can't set font FaceSize", L"Error", MB_OK);
		exit(1);
	}
	fontSize = size;
}

double MyFont::GetFontSize()
{
	return fontSize;
}

