#include "stdafx.h"

#include "CompensationOptionNamesEnum.h"

// ---																										// Constructors
CompensationOptionNamesEnum::CompensationOptionNamesEnum()
{
	Value					= 0;
	SelectedOptionMaxLength = 15;
	SelectedOption			= (wchar_t*)malloc(sizeof(wchar_t) * SelectedOptionMaxLength);
	
	wcscpy_s(SelectedOption, SelectedOptionMaxLength, No);
}

CompensationOptionNamesEnum::~CompensationOptionNamesEnum()
{
	delete SelectedOption;
}
// ---																										// operator= (Set Option)
void CompensationOptionNamesEnum::operator=(wchar_t* option)
{
	wcscpy_s(SelectedOption, SelectedOptionMaxLength, option);
}