
namespace GOTHIC_NAMESPACE
{
	void Game_EntryPoint()
	{

	}

	void Game_Init()
	{

	}

	void Game_Exit()
	{

	}

	void Game_PreLoop()
	{

	}

	void Game_Loop()
	{
		screen->Print(6900, 200, zSTRING("nikita patch is on"));
	}

	void Game_PostLoop()
	{

	}

	void Game_MenuLoop()
	{

	}

	void Game_SaveBegin()
	{

	}

	void Game_SaveEnd()
	{

	}

	void LoadBegin()
	{

	}

	void LoadEnd()
	{

	}

	void Game_LoadBegin_NewGame()
	{
		LoadBegin();
	}

	void Game_LoadEnd_NewGame()
	{
		LoadEnd();
	}

	void Game_LoadBegin_SaveGame()
	{
		LoadBegin();
	}

	void Game_LoadEnd_SaveGame()
	{
		LoadEnd();
	}

	void Game_LoadBegin_ChangeLevel()
	{
		LoadBegin();
	}

	void Game_LoadEnd_ChangeLevel()
	{
		LoadEnd();
	}

	void Game_LoadBegin_TriggerChangeLevel()
	{

	}

	void Game_LoadEnd_TriggerChangeLevel()
	{

	}

	void Game_Pause()
	{

	}

	void Game_Unpause()
	{

	}

	int __cdecl Hlp_MultiplyInt()
	{
		int value;
		float x;

		parser->GetParameter(x);
		parser->GetParameter(value);

		parser->SetReturn(int(value * x));

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

	int __cdecl Hlp_GetItemInstanceID()
	{
		int itemInstanceID;

		oCItem* item = (oCItem*)(parser->GetInstanceAndIndex(itemInstanceID));

		int result = int(item != nullptr ? item->GetInstance() : itemInstanceID >= 0 ? itemInstanceID : -1);

		parser->SetReturn(result);

		return 0;
	}

	int __cdecl Concat5Strings()
	{
		static zSTRING result; result.Clear();
		int stringsQuantity = 5;

		zSTRING i;
		zSTRING ii;
		zSTRING iii;
		zSTRING iv;
		zSTRING v;

		parser->GetParameter(v);
		parser->GetParameter(iv);
		parser->GetParameter(iii);
		parser->GetParameter(ii);
		parser->GetParameter(i);

		for (int b = 0; b < stringsQuantity; ++b)
		{
			zSTRING* cur = nullptr;

			if (b == 0)         cur = &i;
			else if (b == 1)    cur = &ii;
			else if (b == 2)    cur = &iii;
			else if (b == 3)    cur = &iv;
			else if (b == 4)    cur = &v;

			if (!cur) continue;

			if (cur->IsEmpty())
				continue;

			result += *cur;
		}

		parser->SetReturn(result);

		return 0;
	}
	int __cdecl Concat5StringsWithSeparator()
	{
		static zSTRING result; result.Clear();
		int stringsQuantity = 5;

		zSTRING i;
		zSTRING ii;
		zSTRING iii;
		zSTRING iv;
		zSTRING v;
		zSTRING separator;

		parser->GetParameter(separator);
		parser->GetParameter(v);
		parser->GetParameter(iv);
		parser->GetParameter(iii);
		parser->GetParameter(ii);
		parser->GetParameter(i);

		for (int b = 0; b < stringsQuantity; ++b)
		{
			zSTRING* cur = nullptr;

			if (b == 0)         cur = &i;
			else if (b == 1)    cur = &ii;
			else if (b == 2)    cur = &iii;
			else if (b == 3)    cur = &iv;
			else if (b == 4)    cur = &v;

			if (!cur) continue;

			if (cur->IsEmpty())
				continue;

			if (!result.IsEmpty())
				result += separator;

			result += *cur;
		}

		parser->SetReturn(result);

		return 0;
	}
	int __cdecl Concat10Strings()
	{
		static zSTRING result; result.Clear();
		int stringsQuantity = 5;

		zSTRING i;
		zSTRING ii;
		zSTRING iii;
		zSTRING iv;
		zSTRING v;
		zSTRING vi;
		zSTRING vii;
		zSTRING viii;
		zSTRING ix;
		zSTRING x;

		parser->GetParameter(x);
		parser->GetParameter(ix);
		parser->GetParameter(viii);
		parser->GetParameter(vii);
		parser->GetParameter(vi);
		parser->GetParameter(v);
		parser->GetParameter(iv);
		parser->GetParameter(iii);
		parser->GetParameter(ii);
		parser->GetParameter(i);

		for (int b = 0; b < stringsQuantity; ++b)
		{
			zSTRING* cur = nullptr;

			if (b == 0)			cur = &i;
			else if (b == 1)    cur = &ii;
			else if (b == 2)    cur = &iii;
			else if (b == 3)    cur = &iv;
			else if (b == 4)    cur = &v;
			else if (b == 5)	cur = &vi;
			else if (b == 6)    cur = &vii;
			else if (b == 7)    cur = &viii;
			else if (b == 8)    cur = &ix;
			else if (b == 9)    cur = &x;

			if (!cur) continue;

			if (cur->IsEmpty())
				continue;

			result += *cur;
		}

		parser->SetReturn(result);

		return 0;
	}
	int __cdecl Concat10StringsWithSeparator()
	{
		static zSTRING result; result.Clear();
		int stringsQuantity = 10;

		zSTRING i;
		zSTRING ii;
		zSTRING iii;
		zSTRING iv;
		zSTRING v;
		zSTRING vi;
		zSTRING vii;
		zSTRING viii;
		zSTRING ix;
		zSTRING x;
		zSTRING separator;

		parser->GetParameter(separator);
		parser->GetParameter(x);
		parser->GetParameter(ix);
		parser->GetParameter(viii);
		parser->GetParameter(vii);
		parser->GetParameter(vi);
		parser->GetParameter(v);
		parser->GetParameter(iv);
		parser->GetParameter(iii);
		parser->GetParameter(ii);
		parser->GetParameter(i);

		for (int b = 0; b < stringsQuantity; ++b)
		{
			zSTRING* cur = nullptr;

			if (b == 0)         cur = &i;
			else if (b == 1)    cur = &ii;
			else if (b == 2)    cur = &iii;
			else if (b == 3)    cur = &iv;
			else if (b == 4)    cur = &v;
			else if (b == 5)    cur = &vi;
			else if (b == 6)    cur = &vii;
			else if (b == 7)    cur = &viii;
			else if (b == 8)    cur = &ix;
			else if (b == 9)    cur = &x;

			if (!cur) continue;

			if (cur->IsEmpty())
				continue;

			if (!result.IsEmpty())
				result += separator;

			result += *cur;
		}

		parser->SetReturn(result);

		return 0;
	}

	void Game_DefineExternals()
	{
		parser->DefineExternal("Hlp_MultiplyInt", Hlp_MultiplyInt, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_IsItemEquipped", Hlp_IsItemEquipped, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_GetItemInstanceID", Hlp_GetItemInstanceID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("Concat5Strings", Concat5Strings, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_VOID);
		parser->DefineExternal("Concat5StringsWithSeparator", Concat5StringsWithSeparator, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_VOID);
		parser->DefineExternal("Concat10Strings", Concat10Strings, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_VOID);
		parser->DefineExternal("Concat10StringsWithSeparator", Concat10StringsWithSeparator, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_STRING, zPAR_TYPE_VOID);
	}

	void Game_ApplySettings()
	{
	}

	/*int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
	auto Hook_WinMain = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x004F3E10, 0x00506810, 0x005000F0, 0x00502D70)), &WinMain, Union::HookType::Hook_Detours);
	int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	{
		Game_EntryPoint();
		return Hook_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	}*/

	/*void __fastcall oCGame_Init(oCGame* self, void* vtable);
	auto Hook_oCGame_Init = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x00636F50, 0x0065D480, 0x006646D0, 0x006C1060)), &oCGame_Init, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Init(oCGame* self, void* vtable)
	{
		Hook_oCGame_Init(self, vtable);
		Game_Init();
	}*/

	/*void __fastcall CGameManager_Done(CGameManager* self, void* vtable);
	auto Hook_CGameManager_Done = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x00424850, 0x00427310, 0x004251A0, 0x004254E0)), &CGameManager_Done, Union::HookType::Hook_Detours);
	void __fastcall CGameManager_Done(CGameManager* self, void* vtable)
	{
		Game_Exit();
		Hook_CGameManager_Done(self, vtable);
	}*/

	/*void __fastcall oCGame_Render(oCGame* self, void* vtable);
	auto Hook_oCGame_Render = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063DBE0, 0x006648F0, 0x0066B930, 0x006C86A0)), &oCGame_Render, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Render(oCGame* self, void* vtable)
	{
		Game_PreLoop();
		Hook_oCGame_Render(self, vtable);
		Game_PostLoop();
	}*/

	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg);
	auto Partial_zCWorld_Render = Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0063DC76, 0x0066498B, 0x0066BA76, 0x006C87EB)), &oCGame_MainWorld_Render);
	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg)
	{
		Game_Loop();
	}


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

	int GetProtectionIndex(oETypeDamage damageType)
	{
		if (damageType == oEDamageType_Barrier) return oEDamageIndex_Barrier;
		if (damageType == oEDamageType_Blunt) return oEDamageIndex_Blunt;
		if (damageType == oEDamageType_Edge) return oEDamageIndex_Edge;
		if (damageType == oEDamageType_Fire) return oEDamageIndex_Fly;
		if (damageType == oEDamageType_Fly) return oEDamageIndex_Fire;
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

	//0x00664210 public: void __thiscall oCNpc::oSDamageDescriptor::SetFXHit(class oCVisualFX*)

	//void __fastcall oCNpc_oSDamageDescriptor_SetFXHit(oCNpc::oSDamageDescriptor& self, void* vtable, oCVisualFX* vfx);
	//auto Hook_oCNpc_oSDamageDescriptor_SetFXHit = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0072F240, 0x0076DB70, 0x0077AFA0, 0x00664210)), &oCNpc_oSDamageDescriptor_SetFXHit, Union::HookType::Hook_Detours);
	//void __fastcall oCNpc_oSDamageDescriptor_SetFXHit(oCNpc::oSDamageDescriptor& self, void* vtable, oCVisualFX* vfx)
	//{
	//	int damageIndex = GetDamageIndex(reinterpret_cast<oEDamageType&>(self.enuModeDamage)); // damage index by damageType

	//	int customDamage = -1;

	//	oCNpc* targetNpc = nullptr;

	//	if (self.pVobHit->_GetClassDef()->className == "oCNpc")
	//	{
	//		targetNpc = dynamic_cast<oCNpc*>(self.pVobHit);

	//		if (self.pNpcAttacker != nullptr && targetNpc != nullptr && self.nSpellID >= 0)
	//		{
	//			customDamage = Call_PullCustomPureDamage(self.pNpcAttacker->GetInstance(), targetNpc->GetInstance(), damageIndex, self.fDamageTotal, self.nSpellID);
	//		}
	//	}

	//	if (targetNpc != nullptr && self.nSpellID >= 0 && customDamage >= 0)
	//	{
	//		self.fDamageTotal = customDamage;
	//	}

	//	Hook_oCNpc_oSDamageDescriptor_SetFXHit(self, vtable, vfx);
	//}

	static oCNpc::oSDamageDescriptor* gDamageDescriptor = nullptr;

	int attackerInstance = -1;
	int receiverInstance = -1;

	int damageIndex;
	int protectionIndex;
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
		protectionIndex = GetProtectionIndex(reinterpret_cast<oEDamageType&>(dd.enuModeDamage)); // protection index by damageType

		Union::StringANSI(zSTRING("enuModeDamage: ")).StdPrint(); Union::StringANSI(dd.enuModeDamage ? zSTRING(dd.enuModeDamage) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("damageIndex: ")).StdPrint(); Union::StringANSI(damageIndex >= -1 ? zSTRING(damageIndex) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("fDamageTotal: ")).StdPrint(); Union::StringANSI(dd.fDamageTotal ? zSTRING(dd.fDamageTotal) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("AryDamage: ")).StdPrint(); Union::StringANSI(zSTRING(dd.aryDamage[damageIndex])).StdPrintLine();

		pureDamage = Call_PullPureDamage(attackerInstance, receiverInstance, damageIndex, dd.aryDamage[damageIndex], dd.nSpellID);

		if (pureDamage >= 0)
		{
			dd.aryDamage[damageIndex] = pureDamage;
			dd.fDamageTotal = pureDamage;
		}

		Union::StringANSI(zSTRING("ProcessedPureDamage: ")).StdPrint(); Union::StringANSI(zSTRING(dd.aryDamage[damageIndex])).StdPrintLine();
		Union::StringANSI(zSTRING("ProcessedPureDamageInFDamageTotal: ")).StdPrint(); Union::StringANSI(zSTRING(dd.fDamageTotal)).StdPrintLine();

		Hook_oCNpc_OnDamage_Hit(self, vtable, dd);

		// reseting the values
		gDamageDescriptor = nullptr;
		attackerInstance = -1;
		receiverInstance = -1;

		damageIndex = -1;
		protectionIndex = -1;
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
		Union::StringANSI(zSTRING(" ")).StdPrintLine();
		Union::StringANSI(zSTRING("PROT PROCEDURE NEXT::::::: ")).StdPrintLine();
		Union::StringANSI(zSTRING(" ")).StdPrintLine();

		Union::StringANSI(zSTRING("protection: ")).StdPrint(); Union::StringANSI(reg.eax ? zSTRING(reg.eax) : zSTRING("None result")).StdPrintLine();

		protection = Call_PullProtection(attackerInstance, receiverInstance, damageIndex, reg.eax, gDamageDescriptor->nSpellID);

		if (protection >= -1)
		{
			reg.eax = protection;
		}

		Union::StringANSI(zSTRING("processedProtection: ")).StdPrint(); Union::StringANSI(reg.eax ? zSTRING(reg.eax) : zSTRING("None result")).StdPrintLine();

		Union::StringANSI(zSTRING(" ")).StdPrintLine();
	}

	void __fastcall oCNpc_OnDamage_Hit_GetTotalDamage(Union::Registers& reg);
	auto Hook_oCNpc_OnDamage_Hit_GetTotalDamage = CreatePartialHook((void*)0x0066CA6B, &oCNpc_OnDamage_Hit_GetTotalDamage);
	void __fastcall oCNpc_OnDamage_Hit_GetTotalDamage(Union::Registers& reg)
	{
		Union::StringANSI(zSTRING(" ")).StdPrintLine();
		Union::StringANSI(zSTRING("TOTAL PROCEDURE NEXT::::::: ")).StdPrintLine();
		Union::StringANSI(zSTRING(" ")).StdPrintLine();

		int& resultTotalDamage = *(int*)(reg.esp + 0xFC);
		isCrit = *(int*)(reg.esp + 0x11C);

		Union::StringANSI(zSTRING("isCrit: ")).StdPrint(); Union::StringANSI(isCrit >= -1 ? zSTRING(isCrit) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("td: ")).StdPrint(); Union::StringANSI(resultTotalDamage ? zSTRING(resultTotalDamage) : zSTRING("None result")).StdPrintLine();

		totalDamage = Call_PullTotalDamage(attackerInstance, receiverInstance, damageIndex, resultTotalDamage, gDamageDescriptor->nSpellID);

		if (totalDamage >= 0)
		{
			resultTotalDamage = totalDamage;
			reg.edi = totalDamage;

			Union::StringANSI(zSTRING("ProcessedDamageTotal: ")).StdPrint(); Union::StringANSI(resultTotalDamage >= -1 ? zSTRING(resultTotalDamage) : zSTRING("None result")).StdPrintLine();
		}

		multiplier = Call_PullMultiplier(attackerInstance, receiverInstance, damageIndex, isCrit);

		if (multiplier >= 0)
		{
			reg.edi = (int)((float)resultTotalDamage * multiplier); //reg.edi -> also totalDamage

			Union::StringANSI(zSTRING("ProcessedDamageTotalWithCustomMultiplier: ")).StdPrint(); Union::StringANSI(zSTRING(reg.edi)).StdPrintLine();

			reg.eip = (int)0x0066CA8A;
		}

		Union::StringANSI(zSTRING(" ")).StdPrintLine();
	}

	void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg);
	auto Hook_oCNpc_OnDamage_Hit_GetMinimalDamage = CreatePartialHook((void*)0x0066CAA0, &oCNpc_OnDamage_Hit_GetMinimalDamage);
	void __fastcall oCNpc_OnDamage_Hit_GetMinimalDamage(Union::Registers& reg)
	{
		Union::StringANSI(zSTRING(" ")).StdPrintLine();
		Union::StringANSI(zSTRING("MIN PROCEDURE NEXT::::::: ")).StdPrintLine();
		Union::StringANSI(zSTRING(" ")).StdPrintLine();

		minimalDamage = Call_PullMinimalDamage(attackerInstance, receiverInstance, damageIndex, gDamageDescriptor->nSpellID);

		if (minimalDamage >= 0)
		{
			Union::StringANSI(zSTRING("customMinimalDamage: ")).StdPrint(); Union::StringANSI(minimalDamage >= -2 ? zSTRING(minimalDamage) : zSTRING("none")).StdPrintLine();

			reg.eax = minimalDamage;
		}

		Union::StringANSI(zSTRING(" ")).StdPrintLine();
	}

	void __fastcall oCNpc_OnDamage_Hit_GetCustomDamage(Union::Registers& reg);
	auto Hook_oCNpc_OnDamage_Hit_GetCustomDamage = CreatePartialHook((void*)0x0066CAC7, &oCNpc_OnDamage_Hit_GetCustomDamage);
	void __fastcall oCNpc_OnDamage_Hit_GetCustomDamage(Union::Registers& reg)
	{
		Union::StringANSI(zSTRING(" ")).StdPrintLine();
		Union::StringANSI(zSTRING("CUSTOM PROCEDURE NEXT::::::: ")).StdPrintLine();
		Union::StringANSI(zSTRING(" ")).StdPrintLine();

		customDamageIndex = Call_PullCustomDamageType(attackerInstance, receiverInstance, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetInstance() : -1);
		
		Union::StringANSI(zSTRING("customDamageIndex: ")).StdPrint(); Union::StringANSI(customDamageIndex >= -1 ? zSTRING(customDamageIndex) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("initialPureDamage: ")).StdPrint(); Union::StringANSI(gDamageDescriptor->pItemWeapon->GetFullDamage() >= 0 ? zSTRING(gDamageDescriptor->pItemWeapon->GetFullDamage()) : zSTRING("None result")).StdPrintLine();

		if (customDamageIndex >= 0)
		{
			int customDamage = Call_PullCustomDamage(attackerInstance, receiverInstance, customDamageIndex, gDamageDescriptor->pItemWeapon != nullptr ? gDamageDescriptor->pItemWeapon->GetFullDamage() : 0, isCrit, gDamageDescriptor->nSpellID);

			Union::StringANSI(zSTRING("CUSTOMDAMAGE: ")).StdPrint(); Union::StringANSI(customDamage >= 0 ? zSTRING(customDamage) : zSTRING("None result")).StdPrintLine();

			if (customDamage >= 0)
			{
				reg.edi = customDamage;
			}
			else
			{
				reg.edi = 0;
			}
		}

		Union::StringANSI(zSTRING(" ")).StdPrintLine();
	}

	/*void __fastcall zCMenu_Render(zCMenu* self, void* vtable);
	auto Hook_zCMenu_Render = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x004D0DA0, 0x004E14E0, 0x004DB270, 0x004DDC20)), &zCMenu_Render, Union::HookType::Hook_Detours);
	void __fastcall zCMenu_Render(zCMenu* self, void* vtable)
	{
		Hook_zCMenu_Render(self, vtable);
		Game_MenuLoop();
	}*/

	/*void __fastcall oCGame_WriteSaveGame(oCGame* self, void* vtable, int slot, zBOOL saveGlobals);
	auto Hook_oCGame_WriteSaveGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063AD80, 0x00661680, 0x006685D0, 0x006C5250)), &oCGame_WriteSaveGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_WriteSaveGame(oCGame* self, void* vtable, int slot, zBOOL saveGlobals)
	{
		Game_SaveBegin();
		Hook_oCGame_WriteSaveGame(self, vtable, slot, saveGlobals);
		Game_SaveEnd();
	}*/

	/*void __fastcall oCGame_LoadGame(oCGame* self, void* vtable, int slot, const zSTRING& levelPath);
	auto Hook_oCGame_LoadGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063C070, 0x00662B20, 0x00669970, 0x006C65A0)), &oCGame_LoadGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_LoadGame(oCGame* self, void* vtable, int slot, const zSTRING& levelPath)
	{
		Game_LoadBegin_NewGame();
		Hook_oCGame_LoadGame(self, vtable, slot, levelPath);
		Game_LoadEnd_NewGame();
	}*/

	/*void __fastcall oCGame_LoadSaveGame(oCGame* self, void* vtable, int slot, zBOOL loadGlobals);
	auto Hook_oCGame_LoadSaveGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063C2A0, 0x00662D60, 0x00669BA0, 0x006C67D0)), &oCGame_LoadSaveGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_LoadSaveGame(oCGame* self, void* vtable, int slot, zBOOL loadGlobals)
	{
		Game_LoadBegin_SaveGame();
		Hook_oCGame_LoadSaveGame(self, vtable, slot, loadGlobals);
		Game_LoadEnd_SaveGame();
	}*/

	/*void __fastcall oCGame_ChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint);
	auto Hook_Game_Load_ChangeLevel = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063CD60, 0x00663950, 0x0066A660, 0x006C7290)), &oCGame_ChangeLevel, Union::HookType::Hook_Detours);
	void __fastcall oCGame_ChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint)
	{
		Game_LoadBegin_ChangeLevel();
		Hook_Game_Load_ChangeLevel(self, vtable, levelpath, startpoint);
		Game_LoadEnd_ChangeLevel();
	}*/

	/*void __fastcall oCGame_TriggerChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint);
	auto Hook_oCGame_TriggerChangeLevel = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063D480, 0x00664100, 0x0066AD80, 0x006C7AF0)), &oCGame_TriggerChangeLevel, Union::HookType::Hook_Detours);
	void __fastcall oCGame_TriggerChangeLevel(oCGame* self, void* vtable, const zSTRING& levelpath, const zSTRING& startpoint)
	{
		Game_LoadBegin_TriggerChangeLevel();
		Hook_oCGame_TriggerChangeLevel(self, vtable, levelpath, startpoint);
		Game_LoadEnd_TriggerChangeLevel();
	}*/

	/*#if ENGINE <= Engine_G1A
		void __fastcall oCGame_Pause_G1(oCGame* self, void* vtable);
		auto Hook_oCGame_Pause = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063DF50, 0x00664CD0, 0, 0)), &oCGame_Pause_G1, Union::HookType::Hook_Detours);
		void __fastcall oCGame_Pause_G1(oCGame* self, void* vtable)
		{
			Hook_oCGame_Pause(self, vtable);
			Game_Pause();
		}
	#else
		void __fastcall oCGame_Pause_G2(oCGame* self, void* vtable, zBOOL sessionPaused);
		auto Hook_oCGame_Pause = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0, 0, 0x0066BD50, 0x006C8AD0)), &oCGame_Pause_G2, Union::HookType::Hook_Detours);
		void __fastcall oCGame_Pause_G2(oCGame* self, void* vtable, zBOOL sessionPaused)
		{
			Hook_oCGame_Pause(self, vtable, sessionPaused);
			Game_Pause();
		}
	#endif*/

	/*void __fastcall oCGame_Unpause(oCGame* self, void* vtable);
	auto Hook_oCGame_Unpause = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063E1D0, 0x00664F80, 0x0066BFD0, 0x006C8D50)), &oCGame_Unpause, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Unpause(oCGame* self, void* vtable)
	{
		Hook_oCGame_Unpause(self, vtable);
		Game_Unpause();
	}*/

	void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser);
	auto Hook_oCGame_DefineExternals_Ulfi = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x006495B0, 0x006715F0, 0x00677A00, 0x006D4780)), &oCGame_DefineExternals_Ulfi, Union::HookType::Hook_Detours);
	void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser)
	{
		Hook_oCGame_DefineExternals_Ulfi(self, vtable, parser);
		Game_DefineExternals();
	}

	/*void __fastcall CGameManager_ApplySomeSettings(CGameManager* self, void* vtable);
	auto Hook_CGameManager_ApplySomeSettings = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x004267C0, 0x004291E0, 0x00427370, 0x004276B0)), &CGameManager_ApplySomeSettings, Union::HookType::Hook_Detours);
	void __fastcall CGameManager_ApplySomeSettings(CGameManager* self, void* vtable)
	{
		Hook_CGameManager_ApplySomeSettings(self, vtable);
		Game_ApplySettings();
	}*/
}