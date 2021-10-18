#include "Direction.h"

const FRotator FDirection::Rotations[] =
{
	FRotator{ FQuat::Identity },
	FRotator::MakeFromEuler(FVector{ 0.0f, 0.0f, 90.0f }),
	FRotator::MakeFromEuler(FVector{ 0.0f, 0.0f, 180.0f }),
	FRotator::MakeFromEuler(FVector{ 0.0f, 0.0f, 270.0f }),
};

const FVector FDirection::HalfVectors[] =
{
	0.5f * FVector::ForwardVector,
	0.5f * FVector::RightVector,
	0.5f * FVector::BackwardVector,
	0.5f * FVector::LeftVector,
};

FRotator FDirection::GetRotation(EType Dir)
{
	return Rotations[static_cast<int>(Dir)];
}

FDirection::EChangeType FDirection::GetDirectionChangeTo(EType Current, EType Next)
{
	if (Current == Next)
	{
		return EChangeType::None;
	}
	else if ((static_cast<EType>(static_cast<int>(Current) + 1) == Next) || (static_cast<EType>(static_cast<int>(Current) - 3) == Next))
	{
		return EChangeType::TurnRight;
	}
	else if ((static_cast<EType>(static_cast<int>(Current) - 1) == Next) || (static_cast<EType>(static_cast<int>(Current) + 3) == Next))
	{
		return EChangeType::TurnLeft;
	}

	return EChangeType::TurnAround;
}

float FDirection::GetAngle(EType Dir)
{
	return static_cast<int>(Dir) * 90.0f;
}

FVector FDirection::GetHalfVector(EType Dir)
{
	return HalfVectors[static_cast<int>(Dir)];
}
