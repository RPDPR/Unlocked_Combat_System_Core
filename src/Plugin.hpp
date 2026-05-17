
#include <unordered_map>

namespace GOTHIC_NAMESPACE
{
	int GetDamageIndex(oEDamageType damageType)
	{
		if (damageType == oEDamageType_Barrier) return oEDamageIndex_Barrier;
		if (damageType == oEDamageType_Blunt)   return oEDamageIndex_Blunt;
		if (damageType == oEDamageType_Edge)    return oEDamageIndex_Edge;
		if (damageType == oEDamageType_Fire)    return oEDamageIndex_Fire;
		if (damageType == oEDamageType_Fly)     return oEDamageIndex_Fly;
		if (damageType == oEDamageType_Magic)   return oEDamageIndex_Magic;
		if (damageType == oEDamageType_Point)   return oEDamageIndex_Point;
		if (damageType == oEDamageType_Fall)    return oEDamageIndex_Fall;

		return -1;
	}
	oEDamageType GetDamageType(oEDamageIndex damageIndex)
	{
		if (damageIndex == oEDamageIndex_Barrier) return oEDamageType_Barrier;
		if (damageIndex == oEDamageIndex_Blunt)   return oEDamageType_Blunt;
		if (damageIndex == oEDamageIndex_Edge)    return oEDamageType_Edge;
		if (damageIndex == oEDamageIndex_Fire)    return oEDamageType_Fire;
		if (damageIndex == oEDamageIndex_Fly)     return oEDamageType_Fly;
		if (damageIndex == oEDamageIndex_Magic)   return oEDamageType_Magic;
		if (damageIndex == oEDamageIndex_Point)   return oEDamageType_Point;
		if (damageIndex == oEDamageIndex_Fall)    return oEDamageType_Fall;

		return oEDamageType_Unknown;
	}

	int Call_PullCustomDamageType(int senderNpc_ID, int receiverNpc_ID, int itemInstance_ID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullCustomDamageType")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, itemInstance_ID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 8) return -1;

		return dataValue;
	}
	int Call_PullCustomDamage(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialPureDamage, int isCrit, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullCustomDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialPureDamage, isCrit, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	float Call_PullMultiplier(int senderNpc_ID, int receiverNpc_ID, int damageType, int isCrit)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullMultiplier")); if (funcIndex < 0) return -1.0f;

		zCPar_Symbol* sym = parser->GetSymbol(funcIndex); if (sym == nullptr) return -1.0f;
		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, isCrit);
		
		float dataValue = -1.0f; 
		
		if (sym->GetOffset() == zPAR_TYPE_FLOAT)
		{
			dataValue = *reinterpret_cast<float*>(pRet);
		} else if (sym->GetOffset() == zPAR_TYPE_INT)
		{
			dataValue = float(*reinterpret_cast<int*>(pRet)) / 1000.0f;
		}
		
		if (dataValue < 0.0f) return -1.0f;

		return dataValue;
	}

	int Call_PullMinimalDamage(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialMinimalDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullMinimalDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialMinimalDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullPureDamage(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialPureDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullPureDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialPureDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullTotalDamage(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialTotalDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullTotalDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialTotalDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullProtection(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialProtection, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullProtection")); if (funcIndex < 0) return -2;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialProtection, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < -1) return -2;

		return dataValue;
	}

	class NpcDTManager {
	private:
		std::unordered_map<oCNpc*, int> lastHitDT;

	public:
		void SetLastHitDT(oCNpc* npc, int damageType) {
			if (npc) {
				lastHitDT[npc] = damageType;
			}
		}

		int GetLastHitDT(oCNpc* npc) {
			if (!npc) return -1;
			auto it = lastHitDT.find(npc);
			return (it != lastHitDT.end()) ? it->second : -1;
		}

		void Remove(oCNpc* npc) {
			lastHitDT.erase(npc);
		}
	};

	NpcDTManager dtManager;

	static oCNpc::oSDamageDescriptor* gDamageDescriptor = nullptr;
	static bool gOnDamageIsRunning = false;
	static int gID = -1;

	int attackerInstance = -1;
	int receiverInstance = -1;

	int damageIndex;
	int customDamageIndex;

	int isCrit = -1;
	int minimalDamage = -1;
	float multiplier = -1;
	int protection = -2;
	int pureDamage = -1;
	int totalDamage = -1;

	//0x00666610 public: void __thiscall oCNpc::OnDamage_Hit(struct oCNpc::oSDamageDescriptor&)

	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd);
	auto Hook_oCNpc_OnDamage_Hit = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x00731410, 0x00666610)), &oCNpc_OnDamage_Hit, Union::HookType::Hook_Detours);
	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd)
	{
		gDamageDescriptor = &dd;

		attackerInstance = dd.pNpcAttacker != nullptr ? dd.pNpcAttacker->GetInstance() : -1;
		receiverInstance = self != nullptr ? self->GetInstance() : -1;

		damageIndex = GetDamageIndex((oEDamageType)dd.enuModeDamage); // damage index by damageType

		pureDamage = Call_PullPureDamage(attackerInstance, receiverInstance, damageIndex, dd.aryDamage[damageIndex], dd.nSpellID);

		if (pureDamage >= 0)
		{
			dd.aryDamage[damageIndex] = pureDamage;
			dd.fDamageTotal = pureDamage;
		}

		Hook_oCNpc_OnDamage_Hit(self, vtable, dd);

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
		attackerInstance = -1;
		receiverInstance = -1;

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
		protection = Call_PullProtection(attackerInstance, receiverInstance, damageIndex, reg.eax, gDamageDescriptor->nSpellID);

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

			multiplier = Call_PullMultiplier(attackerInstance, receiverInstance, reg.ebp, isCrit);

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

			totalDamage = Call_PullTotalDamage(attackerInstance, receiverInstance, damageIndex, initialTotalDamage, gDamageDescriptor->nSpellID);

			if (totalDamage >= 0)
			{
				reg.eax = totalDamage;
			}
			else
			{
				reg.eax = initialTotalDamage;
			}

			minimalDamage = Call_PullMinimalDamage(attackerInstance, receiverInstance, damageIndex, initialMinimalDamage, gDamageDescriptor->nSpellID);

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

			totalDamage = Call_PullTotalDamage(attackerInstance, receiverInstance, damageIndex, resultTotalDamage, gDamageDescriptor->nSpellID);

			if (totalDamage >= 0)
			{
				resultTotalDamage = totalDamage;
				reg.edi = totalDamage;
			}

			multiplier = Call_PullMultiplier(attackerInstance, receiverInstance, damageIndex, isCrit);

			if (multiplier >= 0.0 && ((!isCrit && multiplier != 0.1) || (isCrit && multiplier != 1.0)))
			{
				reg.edi = (int)((float)resultTotalDamage * multiplier); //reg.edi -> also totalDamage

				reg.eip = (int)0x0066CA8A;
			}

			gID++;

			Union::StringANSI(zSTRING(" ")).StdPrintLine();
			Union::StringANSI(zSTRING("ID: ")).StdPrint(); 
			Union::StringANSI(gID >= 0 ? zSTRING(gID) : zSTRING("nothing")).StdPrintLine();
			Union::StringANSI(zSTRING("ISRN:: ")).StdPrint();
			Union::StringANSI(gOnDamageIsRunning >= 0 ? zSTRING(gOnDamageIsRunning) : zSTRING("nothing")).StdPrintLine();
			Union::StringANSI(zSTRING("TD::: ")).StdPrint();
			Union::StringANSI(zSTRING(reg.edi >= 0 ? reg.edi : zSTRING("nothing"))).StdPrintLine(); Union::StringANSI(zSTRING(" ")).StdPrintLine();
		}

		void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg);
		auto Hook_oCNpc_OnDamage_Hit_GetMinimalDamage = CreatePartialHook((void*)0x0066CAA0, &oCNpc_OnDamage_Hit_GetMinimalDamage);
		void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg)
		{
			minimalDamage = Call_PullMinimalDamage(attackerInstance, receiverInstance, damageIndex, reg.eax, gDamageDescriptor->nSpellID);

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
			
			customDamageIndex = Call_PullCustomDamageType(attackerInstance, receiverInstance, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetInstance() : -1);

			if (customDamageIndex >= 0)
			{
				int customDamage = Call_PullCustomDamage(attackerInstance, receiverInstance, customDamageIndex, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetFullDamage() : 0, isCrit, gDamageDescriptor->nSpellID);

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
			
			customDamageIndex = customDamageIndex >= 8 ? customDamageIndex : Call_PullCustomDamageType(attackerInstance, receiverInstance, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetInstance() : -1);
			
			if (customDamageIndex >= 8)
			{
				int customDamage = Call_PullCustomDamage(attackerInstance, receiverInstance, customDamageIndex, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetFullDamage() : 0, isCrit, gDamageDescriptor->nSpellID);

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


	// EXTERNAL FUNCTIONS /////

	int __cdecl Hlp_MultInt()
	{
		int value;
		float x;

		parser->GetParameter(x);
		parser->GetParameter(value);

		parser->SetReturn(int(value * x));

		return 0;
	}

	int __cdecl Hlp_MultInt_F()
	{
		int value;
		float x;

		parser->GetParameter(x);
		parser->GetParameter(value);

		parser->SetReturn(float(value * x));

		return 0;
	}

	int __cdecl Hlp_GetItemInstanceID()
	{
		oCItem* item = (oCItem*)(parser->GetInstance());

		int result = int(item != nullptr ? item->GetInstance() : -1);

		parser->SetReturn(result);

		return 0;
	}

	int __cdecl Hlp_IsItemEquipped()
	{
		oCNpc* npc = (oCNpc*)(parser->GetInstance());
		oCItem* item = (oCItem*)(parser->GetInstance());

		if (npc != nullptr)
		{
			for (int i = 0; i < npc->inventory2.contents->GetNumInList(); ++i)
			{
				oCItem* currentItem = npc->inventory2.contents->Get(i);

				if (currentItem->GetInstance() == item->GetInstance() && currentItem->HasFlag(ITM_FLAG_ACTIVE))
				{
					parser->SetReturn(1);

					return 0;
				}
			}
		}

		parser->SetReturn(0);

		return 0;
	}

	int __cdecl Npc_GetLastHitDT()
	{
		oCNpc* npc = (oCNpc*)(parser->GetInstance());

		int result = int(npc != nullptr ? dtManager.GetLastHitDT(npc) : -1);

		parser->SetReturn(result);

		return 0;
	}

	int __cdecl UCS_ApplyDamage()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;

		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (!gOnDamageIsRunning && damageReceiver != nullptr && damageSender != nullptr && damageIndex >= 0 && spellID >= 0 && damage >= 0 && dontKill >= 0)
		{
			oCNpc::oSDamageDescriptor dd{};
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			dd.pVobAttacker = damageSender;
			dd.pNpcAttacker = damageSender;
			dd.pVobHit = damageReceiver;
			dd.enuModeDamage = GetDamageType(resultDamageIndex);
			dd.nSpellID = spellID;
			dd.nSpellCat = 2;
			dd.aryDamage[resultDamageIndex] = damage;
			dd.fDamageTotal = damage;
			dd.bDamageDontKill = dontKill;

			dd.fDamageMultiplier = 1.0f;
			dd.bOnce = 1;
			dd.bFinished = 1;
			dd.dwFieldsValid = 0;

			gOnDamageIsRunning = true;

			damageReceiver->OnDamage(dd);

			Union::StringANSI(zSTRING("Call THIS FUNC")).StdPrintLine();

			Union::StringANSI(zSTRING("Attacker: ")).StdPrint(); Union::StringANSI(zSTRING(dd.pNpcAttacker != nullptr ? dd.pNpcAttacker->GetName(0) : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("Receiver: ")).StdPrint(); Union::StringANSI(zSTRING(damageReceiver != nullptr ? damageReceiver->GetName(0) : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("dt: ")).StdPrint(); Union::StringANSI(zSTRING(dd.enuModeDamage >= 0 ? dd.enuModeDamage : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("aryDamage: ")).StdPrint(); Union::StringANSI(zSTRING(resultDamageIndex && dd.aryDamage[resultDamageIndex] >= 0 ? dd.aryDamage[resultDamageIndex] : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bDamageDontKill: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bDamageDontKill >= 0 ? dd.bDamageDontKill : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bIsUnconscious: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bIsUnconscious >= 0 ? dd.bIsUnconscious : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bIsDead: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bIsDead >= 0 ? dd.bIsDead : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("dwFieldsValid: ")).StdPrint(); Union::StringANSI(dd.dwFieldsValid >= 0 ? zSTRING(dd.dwFieldsValid) : zSTRING("nothing")).StdPrintLine();
		
			gOnDamageIsRunning = false;
		}

		return 0;
	};

	int __cdecl UCS_ApplyLoopDamage()
	{
		int damageIndex;
		int spellID;
		int damage;
		int dontKill;

		parser->GetParameter(dontKill);
		parser->GetParameter(damage);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageReceiver != nullptr && damageSender != nullptr && damageIndex >= 0 && spellID >= 0 && damage >= 0 && dontKill >= 0)
		{
			oCNpc::oSDamageDescriptor dd{};
			oEDamageIndex resultDamageIndex = damageIndex < 8 ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			dd.pNpcAttacker = damageSender;
			dd.enuModeDamage = GetDamageType(resultDamageIndex);
			dd.nSpellID = spellID;
			dd.aryDamage[resultDamageIndex] = damage;
			dd.fDamageTotal = damage;
			dd.bDamageDontKill = dontKill;

			dd.fDamageMultiplier = 1.0f;
			//dd.bIsUnconscious = damageReceiver->IsUnconscious();
			dd.bIsDead = damageReceiver->IsDead();
			dd.bOnce = 1;
			dd.dwFieldsValid = 0x38F;

			//damageReceiver->OnDamage(dd);

			Union::StringANSI(zSTRING("Call THIS FUNC")).StdPrintLine();

			Union::StringANSI(zSTRING("Attacker: ")).StdPrint(); Union::StringANSI(zSTRING(dd.pNpcAttacker != nullptr ? dd.pNpcAttacker->GetName(0) : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("Receiver: ")).StdPrint(); Union::StringANSI(zSTRING(damageReceiver != nullptr ? damageReceiver->GetName(0) : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("dt: ")).StdPrint(); Union::StringANSI(zSTRING(dd.enuModeDamage >= 0 ? dd.enuModeDamage : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("aryDamage: ")).StdPrint(); Union::StringANSI(zSTRING(resultDamageIndex && dd.aryDamage[resultDamageIndex] >= 0 ? dd.aryDamage[resultDamageIndex] : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bDamageDontKill: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bDamageDontKill >= 0 ? dd.bDamageDontKill : zSTRING("nothing"))).StdPrintLine();
			//Union::StringANSI(zSTRING("bIsUnconscious: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bIsUnconscious >= 0 ? dd.bIsUnconscious : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("bIsDead: ")).StdPrint(); Union::StringANSI(zSTRING(dd.bIsDead >= 0 ? dd.bIsDead : zSTRING("nothing"))).StdPrintLine();
			Union::StringANSI(zSTRING("dwFieldsValid: ")).StdPrint(); Union::StringANSI(dd.dwFieldsValid >= 0 ? zSTRING(dd.dwFieldsValid) : zSTRING("nothing")).StdPrintLine();
		}

		return 0;
	};


	void Game_DefineExternals()
	{
		parser->DefineExternal("UCS_ApplyDamage", UCS_ApplyDamage, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_MultInt", Hlp_MultInt, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_MultInt_F", Hlp_MultInt_F, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_IsItemEquipped", Hlp_IsItemEquipped, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_GetItemInstanceID", Hlp_GetItemInstanceID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("Npc_GetLastHitDT", Npc_GetLastHitDT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
	}

	void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser);
	auto Hook_oCGame_DefineExternals_Ulfi = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x006495B0, 0x006715F0, 0x00677A00, 0x006D4780)), &oCGame_DefineExternals_Ulfi, Union::HookType::Hook_Detours);
	void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser)
	{
		Hook_oCGame_DefineExternals_Ulfi(self, vtable, parser);
		Game_DefineExternals();
	}
}