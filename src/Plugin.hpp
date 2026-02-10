
#include <unordered_map>

namespace GOTHIC_NAMESPACE
{
	/* void Game_Loop()
	{

	} */

	/* void __fastcall oCGame_MainWorld_Render(Union::Registers& reg);
	auto Partial_zCWorld_Render = Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0063DC76, 0x0066498B, 0x0066BA76, 0x006C87EB)), &oCGame_MainWorld_Render);
	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg)
	{
		Game_Loop();
	} */


	int GetDamageIndex(oETypeDamage damageType)
	{
		if (damageType == oEDamageType_Barrier) return oEDamageIndex_Barrier;
		if (damageType == oEDamageType_Blunt) return oEDamageIndex_Blunt;
		if (damageType == oEDamageType_Edge) return oEDamageIndex_Edge;
		if (damageType == oEDamageType_Fire) return oEDamageIndex_Fire;
		if (damageType == oEDamageType_Fly) return oEDamageIndex_Fly;
		if (damageType == oEDamageType_Magic) return oEDamageIndex_Magic;
		if (damageType == oEDamageType_Point) return oEDamageIndex_Point;
		if (damageType == oEDamageType_Fall) return oEDamageIndex_Fall;

		return -1;
	}

	int Call_PullCustomDamageType(int senderNpc_ID, int receiverNpc_ID, int itemInstance_ID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullCustomDamageType")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, itemInstance_ID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

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
		int funcIndex = parser->GetIndex(zSTRING("PullMultiplier")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, isCrit);
		float dataValue = *reinterpret_cast<float*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullMinimalDamage(int senderNpc_ID, int receiverNpc_ID, int damageType, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullMinimalDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullPureDamage(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullPureDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullTotalDamage(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullTotalDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialDamage, spellID);
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
	auto Hook_oCNpc_OnDamage_Hit = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0072F280, 0x0076DBC0, 0x0077D390, 0x00666610)), &oCNpc_OnDamage_Hit, Union::HookType::Hook_Detours);
	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd)
	{
		gDamageDescriptor = &dd;

		attackerInstance = dd.pNpcAttacker != nullptr ? dd.pNpcAttacker->GetInstance() : -1;
		receiverInstance = self != nullptr ? self->GetInstance() : -1;

		damageIndex = GetDamageIndex(reinterpret_cast<oEDamageType&>(dd.enuModeDamage)); // damage index by damageType

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
	auto Hook_oCNpc_OnDamage_Hit_GetProtection = CreatePartialHook((void*)0x0066B733, &oCNpc_OnDamage_Hit_GetProtection);
	void __fastcall oCNpc_OnDamage_Hit_GetProtection(Union::Registers& reg)
	{
		protection = Call_PullProtection(attackerInstance, receiverInstance, damageIndex, reg.eax, gDamageDescriptor->nSpellID);

		if (protection != -1 && protection >= 0)
		{
			reg.eax = protection;
		}
	}

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
	}

	void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg);
	auto Hook_oCNpc_OnDamage_Hit_GetMinimalDamage = CreatePartialHook((void*)0x0066CAA0, &oCNpc_OnDamage_Hit_GetMinimalDamage);
	void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg)
	{
		minimalDamage = Call_PullMinimalDamage(attackerInstance, receiverInstance, damageIndex, gDamageDescriptor->nSpellID);

		if (minimalDamage >= 0)
		{
			reg.eax = minimalDamage;
		}
	}

	void __fastcall oCNpc_OnDamage_Hit_GetCustomDamage(Union::Registers& reg);
	auto Hook_oCNpc_OnDamage_Hit_GetCustomDamage = CreatePartialHook((void*)0x0066CAC7, &oCNpc_OnDamage_Hit_GetCustomDamage);
	void __fastcall oCNpc_OnDamage_Hit_GetCustomDamage(Union::Registers& reg)
	{
		customDamageIndex = Call_PullCustomDamageType(attackerInstance, receiverInstance, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetInstance() : -1);

		if (customDamageIndex >= 0)
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

		if (protection == -1)
		{
			reg.edi = 0;
		};
	}


	// EXTERNAL FUNCTIONS /////

	int __cdecl Hlp_MultiplyInt()
	{
		int value;
		float x;

		parser->GetParameter(x);
		parser->GetParameter(value);

		parser->SetReturn(int(value * x));

		return 0;
	}

	int __cdecl Hlp_GetItemInstanceID()
	{
		int itemInstanceID;

		oCItem* item = (oCItem*)(parser->GetInstanceAndIndex(itemInstanceID));

		int result = int(item != nullptr ? item->GetInstance() : itemInstanceID >= 0 ? itemInstanceID : -1);

		parser->SetReturn(result);

		return 0;
	}

	int __cdecl Hlp_IsItemEquipped()
	{
		int npcInstanceID;
		int itemInstanceID;

		oCNpc* npc = (oCNpc*)(parser->GetInstanceAndIndex(npcInstanceID));
		oCItem* item = (oCItem*)(parser->GetInstanceAndIndex(itemInstanceID));

		if (npc != nullptr)
		{
			for (int i = 0; i < npc->inventory2.contents->GetNumInList(); ++i)
			{
				oCItem* currentItem = npc->inventory2.contents->Get(i);

				if (currentItem->GetInstance() == itemInstanceID && currentItem->HasFlag(ITM_FLAG_ACTIVE))
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
		int npcInstanceID;

		oCNpc* npc = (oCNpc*)(parser->GetInstanceAndIndex(npcInstanceID));

		int result = int(npc != nullptr ? dtManager.GetLastHitDT(npc) : -1);

		parser->SetReturn(result);

		return 0;
	}

	void Game_DefineExternals()
	{
		parser->DefineExternal("Hlp_MultiplyInt", Hlp_MultiplyInt, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);

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