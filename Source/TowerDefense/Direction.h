#pragma once

#include "Math/Rotator.h"

class FDirection
{
public:
	enum class EType : uint8
	{
		North,
		East,
		South,
		West,
	};

	enum class EChangeType : uint8
	{
		None,
		TurnRight,
		TurnLeft,
		TurnAround,
	};

public:
	static FRotator GetRotation(EType Dir);
	static EChangeType GetDirectionChangeTo(EType Current, EType Next);
	static float GetAngle(EType Dir);
	static FVector GetHalfVector(EType Dir);

private:
	static const FRotator Rotations[];
	static const FVector HalfVectors[];
};
