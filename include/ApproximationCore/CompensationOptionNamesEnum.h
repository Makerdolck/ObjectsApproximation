#pragma once

#ifndef __COMPENSATION_OPTION_NAMES_ENUM_H__
#define __COMPENSATION_OPTION_NAMES_ENUM_H__


class CompensationOptionNamesEnum
{
//	--- --- --- --- --- --- --- --- ---	// Properties
private:
	int			SelectedOptionMaxLength;
	wchar_t*	SelectedOption;

public:
	float		Value;

	wchar_t*	No			= L"Нет";
	wchar_t*	Shaft		= L"Вал";
	wchar_t*	Hole		= L"Отверстие";
	wchar_t*	PositiveX	= L"Позитивная - X";
	wchar_t*	PositiveY	= L"Позитивная - Y";
	wchar_t*	PositiveZ	= L"Позитивная - Z";
	wchar_t*	NegativeX	= L"Негативная - X";
	wchar_t*	NegativeY	= L"Негативная - Y";
	wchar_t*	NegativeZ	= L"Негативная - Z";

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CompensationOptionNamesEnum();
	~CompensationOptionNamesEnum();

	void			operator=(wchar_t* option);				// Save Option;
	
	operator		wchar_t* () { return SelectedOption; }	// Conversation operator
};

#endif /* __COMPENSATION_OPTION_NAMES_ENUM_H__ */