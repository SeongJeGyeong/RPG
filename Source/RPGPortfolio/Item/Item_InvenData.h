// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Header/Enum.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item_InvenData.generated.h"

/**
 * 
 */
UCLASS()
class RPGPORTFOLIO_API UItem_InvenData : public UObject
{
	GENERATED_BODY()
	
private:
	FString	m_ItemName;
	FString	m_ItemImgPath;
	FString	m_ItemDesc;
	uint16	m_ItemQnt;
	EITEM_TYPE m_ItemType;
	

	float	m_ATK;
	float	m_DEF;
	float	m_Restore_HP;
	float	m_Restore_MP;
	int32	m_Maximum_Stack;
	int32	m_Require_Str;
	int32	m_Require_Dex;
	int32	m_Require_Int;


public:
	const FString& GetItemName() { return m_ItemName; }
	void SetItemName(const FString& _ItemName) { m_ItemName = _ItemName; }

	const FString& GetItemImgPath() { return m_ItemImgPath; }
	void SetItemImgPath(const FString& _ItemImgPath) { m_ItemImgPath = _ItemImgPath; }

	const FString& GetItemDesc() { return m_ItemDesc; }
	void SetItemDesc(const FString& _ItemDesc) { m_ItemDesc = _ItemDesc; }

	const uint16& GetItemQnt() { return m_ItemQnt; }
	void SetItemQnt(const uint16& _ItemQnt) { m_ItemQnt = _ItemQnt; }

	const EITEM_TYPE& GetItemType() { return m_ItemType; }
	void SetItemType(const EITEM_TYPE& _ItemType) { m_ItemType = _ItemType; }

	const float& GetAtkVal() { return m_ATK; }
	void SetAtkVal(const float& _ATK) { m_ATK  = _ATK;}

	const float& GetDefVal() { return m_DEF; }
	void SetDefVal(const float& _DEF) { m_DEF = _DEF; }

	const float& GetRestoreHP() { return m_Restore_HP; }
	void SetRestoreHP(const float& _Restore_HP) { m_Restore_HP = _Restore_HP; }

	const float& GetRestoreMP() { return m_Restore_MP; }
	void SetRestoreMP(const float& _Restore_MP) { m_Restore_MP = _Restore_MP; }

	const int32& GetMaximumStack() { return m_Maximum_Stack; }
	void SetMaximumStack(const int32& _Maximum_Stack) { m_Maximum_Stack = _Maximum_Stack; }

	const int32& GetRequireStr() { return m_Require_Str; }
	void SetRequireStr(const int32& _Require_Str) { m_Require_Str = _Require_Str; }

	const int32& GetRequireDex() { return m_Require_Dex; }
	void SetRequireDex(const int32& _Require_Dex) { m_Require_Dex = _Require_Dex; }

	const int32& GetRequireInt() { return m_Require_Int; }
	void SetRequireInt(const int32& _Require_Int) { m_Require_Int = _Require_Int; }
};
