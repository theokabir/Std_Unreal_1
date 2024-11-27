// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorHeroController.h"

AWarriorHeroController::AWarriorHeroController()
{
	HeroTeamId = FGenericTeamId(0);
}

FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
