#include "stdafx.h"

#include "ObjectApprox.h"
#include "_ALLincludesCore.h"

// ---																										// Constructors
ObjectApprox::ObjectApprox()
{
	flagReady			= false;
	flagSelected		= false;
	Name				= L"Name";
	objMath				= nullptr;
	objID				= 1;
	CompensationOption	= CompensationOption.No;
}

ObjectApprox::~ObjectApprox()
{
	delete objMath;
}
// ---																										// Update Compensation Option
void ObjectApprox::UpdateCompensationOption(wchar_t* option, float value)
{
	if ((wcscmp(CompensationOption, option) == 0) && (value == CompensationOption.Value))
		return;

	if (objMath->GetName() == PointApprox().GetName())
	{
		PointApprox* point = (PointApprox*)objMath;

		if (wcscmp(CompensationOption,CompensationOption.PositiveX) == 0)
			point->X -= CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.PositiveY) == 0)
			point->Y -= CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.PositiveZ) == 0)
			point->Z -= CompensationOption.Value;

		if (wcscmp(CompensationOption,CompensationOption.NegativeX) == 0)
			point->X += CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.NegativeY) == 0)
			point->Y += CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.NegativeZ) == 0)
			point->Z += CompensationOption.Value;

		CompensationOption			= option;
		CompensationOption.Value	= value;

		if (wcscmp(CompensationOption,CompensationOption.PositiveX) == 0)
			point->X += CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.PositiveY) == 0)
			point->Y += CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.PositiveZ) == 0)
			point->Z += CompensationOption.Value;

		if (wcscmp(CompensationOption,CompensationOption.NegativeX) == 0)
			point->X -= CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.NegativeY) == 0)
			point->Y -= CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.NegativeZ) == 0)
			point->Z -= CompensationOption.Value;

	}
	else if (objMath->GetName() == LineSegmentApprox().GetName())
	{
		LineSegmentApprox* line = (LineSegmentApprox*)objMath;

		if (wcscmp(CompensationOption,CompensationOption.PositiveX) == 0)
		{
			line->Point.X		-= CompensationOption.Value;
			line->PointStart.X	-= CompensationOption.Value;
			line->PointEnd.X	-= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveY) == 0)
		{
			line->Point.Y		-= CompensationOption.Value;
			line->PointStart.Y	-= CompensationOption.Value;
			line->PointEnd.Y	-= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveZ) == 0)
		{
			line->Point.Z		-= CompensationOption.Value;
			line->PointStart.Z	-= CompensationOption.Value;
			line->PointEnd.Z	-= CompensationOption.Value;
		}

		if (wcscmp(CompensationOption,CompensationOption.NegativeX) == 0)
		{
			line->Point.X		+= CompensationOption.Value;
			line->PointStart.X	+= CompensationOption.Value;
			line->PointEnd.X	+= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeY) == 0)
		{
			line->Point.Y		+= CompensationOption.Value;
			line->PointStart.Y	+= CompensationOption.Value;
			line->PointEnd.Y	+= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeZ) == 0)
		{
			line->Point.Z		+= CompensationOption.Value;
			line->PointStart.Z	+= CompensationOption.Value;
			line->PointEnd.Z	+= CompensationOption.Value;
		}

		CompensationOption			= option;
		CompensationOption.Value	= value;

		if (wcscmp(CompensationOption,CompensationOption.PositiveX) == 0)
		{
			line->Point.X		+= CompensationOption.Value;
			line->PointStart.X	+= CompensationOption.Value;
			line->PointEnd.X	+= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveY) == 0)
		{
			line->Point.Y		+= CompensationOption.Value;
			line->PointStart.Y	+= CompensationOption.Value;
			line->PointEnd.Y	+= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveZ) == 0)
		{
			line->Point.Z		+= CompensationOption.Value;
			line->PointStart.Z	+= CompensationOption.Value;
			line->PointEnd.Z	+= CompensationOption.Value;
		}

		if (wcscmp(CompensationOption,CompensationOption.NegativeX) == 0)
		{
			line->Point.X		-= CompensationOption.Value;
			line->PointStart.X	-= CompensationOption.Value;
			line->PointEnd.X	-= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeY) == 0)
		{
			line->Point.Y		-= CompensationOption.Value;
			line->PointStart.Y	-= CompensationOption.Value;
			line->PointEnd.Y	-= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeZ) == 0)
		{
			line->Point.Z		-= CompensationOption.Value;
			line->PointStart.Z	-= CompensationOption.Value;
			line->PointEnd.Z	-= CompensationOption.Value;
		}
		
	}
	else if (objMath->GetName() == RectangleApprox().GetName())
	{
		RectangleApprox* plane = (RectangleApprox*)objMath;

		VectorGeometric tmpVector;

		if (wcscmp(CompensationOption,CompensationOption.PositiveX) == 0)
		{
			tmpVector = VectorGeometric(1, 0, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveY) == 0)
		{
			tmpVector = VectorGeometric(0, 1, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveZ) == 0)
		{
			tmpVector = VectorGeometric(0, 0, 1);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
		}

		if (wcscmp(CompensationOption,CompensationOption.NegativeX) == 0)
		{
			tmpVector = VectorGeometric(1, 0, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeY) == 0)
		{
			tmpVector = VectorGeometric(0, 1, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeZ) == 0)
		{
			tmpVector = VectorGeometric(0, 0, 1);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
		}

		CompensationOption			= option;
		CompensationOption.Value	= value;

		if (wcscmp(CompensationOption,CompensationOption.PositiveX) == 0)
		{
			tmpVector = VectorGeometric(1, 0, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveY) == 0)
		{
			tmpVector = VectorGeometric(0, 1, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
		}
		else if (wcscmp(CompensationOption,CompensationOption.PositiveZ) == 0)
		{
			tmpVector = VectorGeometric(0, 0, 1);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
		}

		if (wcscmp(CompensationOption,CompensationOption.NegativeX) == 0)
		{
			tmpVector = VectorGeometric(1, 0, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeY) == 0)
		{
			tmpVector = VectorGeometric(0, 1, 0);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
		}
		else if (wcscmp(CompensationOption,CompensationOption.NegativeZ) == 0)
		{
			tmpVector = VectorGeometric(0, 0, 1);

			if (tmpVector * plane->Line.Vector > 0)
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value, false);
			else
				plane->Line.Point = plane->Line.CreatePointOnDistance(CompensationOption.Value);
		}

	}
	else if (objMath->GetName() == CircleApprox().GetName())
	{
		CircleApprox* circle = (CircleApprox*)objMath;

		if (wcscmp(CompensationOption,CompensationOption.Hole) == 0)
			circle->Radius -= CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.Shaft) == 0)
			circle->Radius += CompensationOption.Value;
		
		CompensationOption			= option;
		CompensationOption.Value	= value;

		if (wcscmp(CompensationOption,CompensationOption.Hole) == 0)
			circle->Radius += CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.Shaft) == 0)
			circle->Radius -= CompensationOption.Value;

	}
	else if (objMath->GetName() == CylinderApprox().GetName())
	{
		CylinderApprox* cylinder = (CylinderApprox*)objMath;

		if (wcscmp(CompensationOption,CompensationOption.Hole) == 0)
			cylinder->Radius -= CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.Shaft) == 0)
			cylinder->Radius += CompensationOption.Value;

		CompensationOption			= option;
		CompensationOption.Value	= value;

		if (wcscmp(CompensationOption, CompensationOption.Hole) == 0)
		{
			cylinder->Radius += CompensationOption.Value;
			cylinder->IsHole = true;
		}
		else if (wcscmp(CompensationOption, CompensationOption.Shaft) == 0)
		{
			cylinder->Radius -= CompensationOption.Value;
			cylinder->IsHole = false;
		}

	}
	else if (objMath->GetName() == ConeApprox().GetName())
	{
		ConeApprox* cone = (ConeApprox*)objMath;

		if (wcscmp(CompensationOption,CompensationOption.Hole) == 0)
		{
			cone->RadiusSmaller -= CompensationOption.Value;
			cone->Radius		-= CompensationOption.Value;
		}
		else if (wcscmp(CompensationOption,CompensationOption.Shaft) == 0)
		{
			cone->RadiusSmaller += CompensationOption.Value;
			cone->Radius		+= CompensationOption.Value;
		}

		CompensationOption			= option;
		CompensationOption.Value	= value;

		if (wcscmp(CompensationOption,CompensationOption.Hole) == 0)
		{
			cone->RadiusSmaller += CompensationOption.Value;
			cone->Radius		+= CompensationOption.Value;
			cone->IsHole		= true;
		}
		else if (wcscmp(CompensationOption,CompensationOption.Shaft) == 0)
		{
			cone->RadiusSmaller -= CompensationOption.Value;
			cone->Radius		-= CompensationOption.Value;
			cone->IsHole		= false;
		}

	}
	else if (objMath->GetName() == SphereApprox().GetName())
	{
		SphereApprox* sphere = (SphereApprox*)objMath;

		if (wcscmp(CompensationOption,CompensationOption.Hole) == 0)
			sphere->Radius -= CompensationOption.Value;
		else if (wcscmp(CompensationOption,CompensationOption.Shaft) == 0)
			sphere->Radius += CompensationOption.Value;

		CompensationOption			= option;
		CompensationOption.Value	= value;

		if (wcscmp(CompensationOption,CompensationOption.Hole) == 0)
		{
			sphere->Radius += CompensationOption.Value;
			sphere->IsHole = true;
		}
		else if (wcscmp(CompensationOption,CompensationOption.Shaft) == 0)
		{
			sphere->Radius -= CompensationOption.Value;
			sphere->IsHole = false;
		}
	}
}