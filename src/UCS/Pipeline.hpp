
namespace GOTHIC_NAMESPACE
{
	int Call_PullCustomDamageType(int itemInstance_ID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullCustomDamageType")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, itemInstance_ID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 8) return -1;

		return dataValue;
	}
	int Call_PullCustomDamage(int damageType, int initialPureDamage, int isCrit, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullCustomDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, damageType, initialPureDamage, isCrit, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	float Call_PullMultiplier(int damageType, int isCrit)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullMultiplier")); if (funcIndex < 0) return -1.0f;

		zCPar_Symbol* sym = parser->GetSymbol(funcIndex); if (sym == nullptr) return -1.0f;
		void* pRet = parser->CallFunc(funcIndex, damageType, isCrit);

		float dataValue = -1.0f;

		if (sym->GetOffset() == zPAR_TYPE_FLOAT)
		{
			dataValue = *reinterpret_cast<float*>(pRet);
		}
		else if (sym->GetOffset() == zPAR_TYPE_INT)
		{
			dataValue = float(*reinterpret_cast<int*>(pRet)) / 1000.0f;
		}

		if (dataValue < 0.0f) return -1.0f;

		return dataValue;
	}

	int Call_PullMinimalDamage(int damageType, int initialMinimalDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullMinimalDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, damageType, initialMinimalDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullPureDamage(int damageType, int initialPureDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullPureDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, damageType, initialPureDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullTotalDamage(int damageType, int initialTotalDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullTotalDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, damageType, initialTotalDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullProtection(int damageType, int initialProtection, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullProtection")); if (funcIndex < 0) return -2;

		void* pRet = parser->CallFunc(funcIndex, damageType, initialProtection, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < -1) return -2;

		return dataValue;
	}

	void Call_PullPre(int damageType, int initialPureDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullPre")); if (funcIndex < 0) return;

		parser->CallFunc(funcIndex, damageType, initialProtection, spellID);
	}

	void Call_PullPre(int damageType, int initialProtection, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullPre")); if (funcIndex < 0) return;

		parser->CallFunc(funcIndex, damageType, initialProtection, spellID);
	}


	// UCS Pipeline /////
	static oCNpc::oSDamageDescriptor* gDamageDescriptor = nullptr;

	oCNpc* damageSender = nullptr;
	oCNpc* damageReceiver = nullptr;

	int damageIndex = -1;
	int customDamageIndex = -1;

	int isCrit = -1;
	int minimalDamage = -1;
	float multiplier = -1;
	int protection = -2;
	int pureDamage = -1;
	int totalDamage = -1;

	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd);
	auto Hook_oCNpc_OnDamage_Hit = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x00731410, 0x00666610)), &oCNpc_OnDamage_Hit, Union::HookType::Hook_Detours);
	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd)
	{
		gDamageDescriptor = &dd;

		damageSender = dd.pNpcAttacker != nullptr ? dd.pNpcAttacker : dd.pVobAttacker != nullptr ? (oCNpc*)dd.pVobAttacker : nullptr;
		damageReceiver = self != nullptr ? self : dd.pVobHit != nullptr ? (oCNpc*)dd.pVobHit : nullptr;

		auto damageIndexArr = GetDamageIndexArr(dd.enuModeDamage);

		damageIndex =
			damageIndexArr.empty()
			? -1
			: damageIndexArr[0];

		customDamageIndex = ucsManager.


		pureDamage = Call_PullPureDamage(damageIndex, dd.aryDamage[damageIndex], dd.nSpellID);

		if (pureDamage >= 0)
		{
			dd.aryDamage[damageIndex] = pureDamage;
			dd.fDamageTotal = pureDamage;
		}


		Call_PullPre(damageIndex);

		Hook_oCNpc_OnDamage_Hit(self, vtable, dd);

		Call_PullPost(damageIndex)


		// reseting the values
		if (customDamageIndex >= 0)
		{
			dtManager.SetLastHitDT(self, customDamageIndex);
		}
		else if (damageIndex >= 0)
		{
			dtManager.SetLastHitDT(self, damageIndex);
		}

		gDamageDescriptor = nullptr;
		damageSender = nullptr;
		damageReceiver = nullptr;

		damageIndex = -1;
		customDamageIndex = -1;

		isCrit = -1;
		minimalDamage = -1;
		multiplier = -1;
		protection = -2;
		pureDamage = -1;
		totalDamage = -1;
	}

	void __fastcall oCNpc_OnDamage_Hit_GetProtection(Union::Registers& reg);
	auto Hook_oCNpc_OnDamage_Hit_GetProtection = CreatePartialHook((void*)zSwitch(0x00736510, 0x0066B733), &oCNpc_OnDamage_Hit_GetProtection);
	void __fastcall oCNpc_OnDamage_Hit_GetProtection(Union::Registers& reg)
	{
		protection = Call_PullProtection(damageIndex, reg.eax, gDamageDescriptor->nSpellID);

		if (protection != -1 && protection >= 0)
		{
			#if ENGINE == Engine_G1

				reg.eax = protection;

			#elif ENGINE == Engine_G2A

				reg.eax = protection;

			#endif
		}
	}

	#if ENGINE == Engine_G1

		void __fastcall oCNpc_OnDamage_Hit_GetIsCrit(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_GetIsCrit = CreatePartialHook((void*)0x007328B7, &oCNpc_OnDamage_Hit_GetIsCrit);
		void __fastcall oCNpc_OnDamage_Hit_GetIsCrit(Union::Registers& reg)
		{
			if ((int)(reg.edi) <= (int)(reg.ebp))
			{
				isCrit = 1;
			}
			else
			{
				isCrit = 0;
			}
		}

		void __fastcall oCNpc_OnDamage_Hit_ActivateMultiplierLoop(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_ActivateMultiplierLoop = CreatePartialHook((void*)0x00733589, &oCNpc_OnDamage_Hit_ActivateMultiplierLoop);
		void __fastcall oCNpc_OnDamage_Hit_ActivateMultiplierLoop(Union::Registers& reg)
		{
			reg.eip = (int)0x00733596;
		}

		void __fastcall oCNpc_OnDamage_Hit_GetMultiplier(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_GetMultiplier = CreatePartialHook((void*)0x00733BC9, &oCNpc_OnDamage_Hit_GetMultiplier);
		void __fastcall oCNpc_OnDamage_Hit_GetMultiplier(Union::Registers& reg)
		{
			float& previousMultiplier = *(float*)(reg.eax + 0x4C);

			multiplier = Call_PullMultiplier(reg.ebp, isCrit);

			if (multiplier >= 0.0 && multiplier != previousMultiplier)
			{
				reg.eax = (int)((float)reg.edx * multiplier);

				reg.eip = (int)0x00733BEA;
			}

		}

		void __fastcall oCNpc_OnDamage_Hit_SaveTotalDamage(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_SaveTotalDamage = CreatePartialHook((void*)0x0073774A, &oCNpc_OnDamage_Hit_SaveTotalDamage);
		void __fastcall oCNpc_OnDamage_Hit_SaveTotalDamage(Union::Registers& reg)
		{
			totalDamage = *(int*)(reg.esp + 0x108); // saving total damage

			reg.eip = (int)0x0073775F;
		}

		void __fastcall oCNpc_OnDamage_Hit_GetTotalDamage(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_GetTotalDamage = CreatePartialHook((void*)0x007377E1, &oCNpc_OnDamage_Hit_GetTotalDamage);
		void __fastcall oCNpc_OnDamage_Hit_GetTotalDamage(Union::Registers& reg)
		{
			int initialTotalDamage = totalDamage > 0 ? totalDamage : 0;
			int initialMinimalDamage = reg.eax > 0 ? reg.eax : 0;

			totalDamage = Call_PullTotalDamage(damageIndex, initialTotalDamage, gDamageDescriptor->nSpellID);

			if (totalDamage >= 0)
			{
				reg.eax = totalDamage;
			}
			else
			{
				reg.eax = initialTotalDamage;
			}

			minimalDamage = Call_PullMinimalDamage(damageIndex, initialMinimalDamage, gDamageDescriptor->nSpellID);

			if (minimalDamage >= 0 && reg.eax < minimalDamage)
			{
				reg.eax = minimalDamage;
			}
			else if (reg.eax < initialMinimalDamage)
			{
				reg.eax = initialMinimalDamage;
			}
		}

	#elif ENGINE == Engine_G2A

		void __fastcall oCNpc_OnDamage_Hit_GetTotalDamage(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_GetTotalDamage = CreatePartialHook((void*)0x0066CA6B, &oCNpc_OnDamage_Hit_GetTotalDamage);
		void __fastcall oCNpc_OnDamage_Hit_GetTotalDamage(Union::Registers& reg)
		{
			int& resultTotalDamage = *(int*)(reg.esp + 0xFC);
			isCrit = *(int*)(reg.esp + 0x11C);

			totalDamage = Call_PullTotalDamage(damageIndex, resultTotalDamage, gDamageDescriptor->nSpellID);

			if (totalDamage >= 0)
			{
				resultTotalDamage = totalDamage;
				reg.edi = totalDamage;
			}

			multiplier = Call_PullMultiplier(damageIndex, isCrit);

			if (multiplier >= 0.0 && ((!isCrit && multiplier != 0.1) || (isCrit && multiplier != 1.0)))
			{
				reg.edi = (int)((float)resultTotalDamage * multiplier); //reg.edi -> also totalDamage

				reg.eip = (int)0x0066CA8A;
			}
		}

		void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_GetMinimalDamage = CreatePartialHook((void*)0x0066CAA0, &oCNpc_OnDamage_Hit_GetMinimalDamage);
		void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg)
		{
			minimalDamage = Call_PullMinimalDamage(damageIndex, reg.eax, gDamageDescriptor->nSpellID);

			if (minimalDamage >= 0)
			{
				reg.eax = minimalDamage;
			}
		}

	#endif

	void __fastcall oCNpc_OnDamage_Hit_GetCustomDamage(Union::Registers& reg);
	auto Hook_oCNpc_OnDamage_Hit_GetCustomDamage = CreatePartialHook((void*)zSwitch(0x007377E1, 0x0066CAB2), &oCNpc_OnDamage_Hit_GetCustomDamage);
	void __fastcall oCNpc_OnDamage_Hit_GetCustomDamage(Union::Registers& reg)
	{
		#if ENGINE == Engine_G1

			if (protection == -1)
			{
				reg.eax = 0;
			};

			customDamageIndex = customDamageIndex >= oEDamageIndex_MAX ? customDamageIndex : Call_PullCustomDamageType(gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetInstance() : -1);

			if (customDamageIndex >= 0)
			{
				int customDamage = Call_PullCustomDamage(customDamageIndex, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetFullDamage() : 0, isCrit, gDamageDescriptor->nSpellID);

				if (customDamage >= 0)
				{
					reg.eax = customDamage;
				}
				else
				{
					reg.eax = 0;
				}
			}

		#elif ENGINE == Engine_G2A

			if (protection == -1)
			{
				reg.edi = 0;
			};

			customDamageIndex = customDamageIndex >= 8 ? customDamageIndex : Call_PullCustomDamageType(gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetInstance() : -1);

			if (customDamageIndex >= 8)
			{
				int customDamage = Call_PullCustomDamage(customDamageIndex, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetFullDamage() : 0, isCrit, gDamageDescriptor->nSpellID);

				if (customDamage >= 0)
				{
					reg.edi = customDamage;
				}
				else
				{
					reg.edi = 0;
				}
			}

		#endif
	}


	// EXTERNALS /////

	int __cdecl UCS_GetDamageSender()
	{
		parser->SetReturn(damageSender != nullptr ? damageSender : gDamageDescriptor->pNpcAttacker != nullptr ? gDamageDescriptor->pNpcAttacker : gDamageDescriptor->pVobAttacker != nullptr ? (oCNpc*)gDamageDescriptor->pVobAttacker : nullptr);
		
		return 0;
	};

	int __cdecl UCS_GetDamageReceiver()
	{
		parser->SetReturn(damageReceiver != nullptr ? damageReceiver : gDamageDescriptor->pVobHit != nullptr ? (oCNpc*)gDamageDescriptor->pVobHit : nullptr);
		
		return 0;
	};


	void Game_DefineExternals_Pipeline()
	{
		parser->DefineExternal("UCS_GetDamageSender", UCS_GetDamageSender, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_GetDamageReceiver", UCS_GetDamageReceiver, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
	}
}
