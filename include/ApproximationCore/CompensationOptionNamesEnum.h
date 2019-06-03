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

	wchar_t*	No			= L"���";
	wchar_t*	Shaft		= L"���";
	wchar_t*	Hole		= L"���������";
	wchar_t*	PositiveX	= L"���������� - X";
	wchar_t*	PositiveY	= L"���������� - Y";
	wchar_t*	PositiveZ	= L"���������� - Z";
	wchar_t*	NegativeX	= L"���������� - X";
	wchar_t*	NegativeY	= L"���������� - Y";
	wchar_t*	NegativeZ	= L"���������� - Z";

//	--- --- --- --- --- --- --- --- ---	// Methods
public:
	CompensationOptionNamesEnum();
	~CompensationOptionNamesEnum();

	void			operator=(wchar_t* option);				// Save Option;
	
	operator		wchar_t* () { return SelectedOption; }	// Conversation operator
};

#endif /* __COMPENSATION_OPTION_NAMES_ENUM_H__ */