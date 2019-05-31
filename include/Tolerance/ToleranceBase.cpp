#include "stdafx.h"
#include "ToleranceBase.h"


int ToleranceBase::baseCounter = 26;
bool ToleranceBase::isStackInit = false;

std::vector <char> *ToleranceBase::charStack = new std::vector<char>;


ToleranceBase::ToleranceBase()
{
	TRACE("WORK\n");
	if(ToleranceBase::isStackInit == false){
		while (baseCounter >= 0) {
			ToleranceBase::charStack->push_back(baseCharArray[baseCounter]);
			ToleranceBase::baseCounter--;
		}
		ToleranceBase::isStackInit = true;
		TRACE("Stack init!\n");
	}

}

ToleranceBase::ToleranceBase(ObjectApprox* obj)
{
	//baseChar = baseCharArray[baseCounter];
	//baseCounter++;
	this->objMath = obj->objMath;
	this->baseName = obj->Name.c_str();
}

ToleranceBase::ToleranceBase(LineSegmentApprox* obj)
{
	//baseChar = baseCharArray[baseCounter];
	//baseCounter++;
	this->objMath = obj;
}

ToleranceBase::ToleranceBase(PlaneApprox* obj)
{
	//baseChar = baseCharArray[baseCounter];
	//baseCounter++;
	this->objMath = obj;
	PointStart = obj->Line.Point;

}



ToleranceBase::ToleranceBase(AxialLine* obj)
{
	ToleranceBase();
	/*while (baseCounter >= 0) {
		charStack.push(baseCharArray[baseCounter]);
		baseCounter--;
	}*/
	baseChar = charStack->back();
	charStack->pop_back();
	//baseChar = 'A';
	//TRACE("baseChar: %c\n", charStack->top());
	//baseChar = baseCharArray[baseCounter];
	//baseCounter++;
	this->toleranceObject = obj;
	this->objMath = obj->objMath;
	PointStart = obj->startPoint;
}

ToleranceBase::ToleranceBase(ToleranceObject* obj)
{
	//baseChar = baseCharArray[baseCounter];
	//baseCounter++;
	this->objMath = obj->objMath;
	
}




ToleranceBase::~ToleranceBase()
{
	charStack->push_back(baseChar);
}

ToleranceObject::ToleranceObjectType ToleranceBase::getType()
{
	return ToleranceObjectType::BASE_LINE;
}


