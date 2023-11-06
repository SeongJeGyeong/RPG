// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_CameraArm.h"

UPlayer_CameraArm::UPlayer_CameraArm()
{
	TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	bUsePawnControlRotation = true; // Rotate the arm based on the controller
	bEnableCameraLag = true;
	bEnableCameraRotationLag = false;
	CameraLagSpeed = 3.f;
	CameraRotationLagSpeed = 2.f;
	CameraLagMaxDistance = 100.f;
}
