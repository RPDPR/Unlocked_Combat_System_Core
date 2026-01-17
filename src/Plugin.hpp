
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


	int Call_PullIsCustomDamageType(int senderNpc_ID, int receiverNpc_ID, int itemInstance_ID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullIsCustomDamageType")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, itemInstance_ID);
		int dataValue = *reinterpret_cast<int*>(pRet);

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

	int Call_PullCustomDamage(int senderNpc_ID, int receiverNpc_ID, int isTotalDamage, int damageType, int initialDamage, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullCustomDamage")); if (funcIndex < 0) return -1;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, isTotalDamage, damageType, initialDamage, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < 0) return -1;

		return dataValue;
	}

	int Call_PullCustomProtection(int senderNpc_ID, int receiverNpc_ID, int damageType, int initialProtection, int spellID)
	{
		int funcIndex = parser->GetIndex(zSTRING("PullCustomProtection")); if (funcIndex < 0) return -2;

		void* pRet = parser->CallFunc(funcIndex, senderNpc_ID, receiverNpc_ID, damageType, initialProtection, spellID);
		int dataValue = *reinterpret_cast<int*>(pRet);

		if (dataValue < -1) return -2;

		return dataValue;
	}

	//0x00664210 public: void __thiscall oCNpc::oSDamageDescriptor::SetFXHit(class oCVisualFX*)

	void __fastcall oCNpc_oSDamageDescriptor_SetFXHit(oCNpc::oSDamageDescriptor& self, void* vtable, oCVisualFX* vfx);
	auto Hook_oCNpc_oSDamageDescriptor_SetFXHit = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0072F240, 0x0076DB70, 0x0077AFA0, 0x00664210)), &oCNpc_oSDamageDescriptor_SetFXHit, Union::HookType::Hook_Detours);
	void __fastcall oCNpc_oSDamageDescriptor_SetFXHit(oCNpc::oSDamageDescriptor& self, void* vtable, oCVisualFX* vfx)
	{
		int damageIndex = GetDamageIndex(reinterpret_cast<oEDamageType&>(self.enuModeDamage)); // damage index by damageType

		int customDamage = -1;

		oCNpc* targetNpc = nullptr;

		if (self.pVobHit->_GetClassDef()->className == "oCNpc")
		{
			targetNpc = dynamic_cast<oCNpc*>(self.pVobHit);

			if (self.pNpcAttacker != nullptr && targetNpc != nullptr && self.nSpellID >= 0)
			{
				customDamage = Call_PullCustomDamage(self.pNpcAttacker->GetInstance(), targetNpc->GetInstance(), 1, damageIndex, self.fDamageTotal, self.nSpellID);
			}
		}

		if (targetNpc != nullptr && self.nSpellID >= 0 && customDamage >= 0)
		{
			self.fDamageTotal = customDamage;
		}

		Hook_oCNpc_oSDamageDescriptor_SetFXHit(self, vtable, vfx);
	}

	//0x00666610 public: void __thiscall oCNpc::OnDamage_Hit(struct oCNpc::oSDamageDescriptor&)

	/*void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd);
	auto Hook_oCNpc_OnDamage_Hit = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0072F280, 0x0076DBC0, 0x0077D390, 0x00666610)), &oCNpc_OnDamage_Hit, Union::HookType::Hook_Detours);
	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd)
	{
		//Union::StringANSI(zSTRING("PreHookDamageTotal: ")).StdPrint(); Union::StringANSI(dd.fDamageTotal >= -2 ? zSTRING(dd.fDamageTotal) : zSTRING("None result")).StdPrintLine();

		int damageIndex = GetDamageIndex(reinterpret_cast<oEDamageType&>(dd.enuModeDamage)); // damage index by damageType
		int protectionIndex = GetProtectionIndex(reinterpret_cast<oEDamageType&>(dd.enuModeDamage)); // protection index by damageType

		int savedReceiverProtection = self->protection[protectionIndex];

		int savedAttackerStrength = dd.pNpcAttacker->attribute[NPC_ATR_STRENGTH];
		int savedAttackerDexterity = dd.pNpcAttacker->attribute[NPC_ATR_DEXTERITY];
		int savedWeaponDamage = dd.aryDamage[damageIndex];

		//// custom custom //
		//int customDamageType = -1;

		//if (dd.pNpcAttacker != nullptr && self != nullptr)
		//{
		//	customDamageType = Call_PullIsCustomDamageType(dd.pNpcAttacker->GetInstance(), self->GetInstance(), dd.pItemWeapon->GetInstance());
		//};

		//if (customDamageType)
		//{
		//	dd.enuModeDamage = customDamageType;
		//}
		//// custom custom //

		int customProtection = -2;

		if (dd.pNpcAttacker != nullptr && self != nullptr)
		{
			customProtection = Call_PullCustomProtection(dd.pNpcAttacker->GetInstance(), self->GetInstance(), damageIndex, self->protection[protectionIndex], dd.nSpellID);
		};

		if (customProtection >= -1)
		{
			if (self != nullptr)
			{
				self->protection[protectionIndex] = customProtection;
			}
		}

		Union::StringANSI(zSTRING("PreAttackerStrength: ")).StdPrint(); Union::StringANSI(savedAttackerStrength ? zSTRING(savedAttackerStrength) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("PreAttackerDexterity: ")).StdPrint(); Union::StringANSI(savedAttackerDexterity ? zSTRING(savedAttackerDexterity) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("PreReceiverProtection: ")).StdPrint(); Union::StringANSI(savedReceiverProtection ? zSTRING(savedReceiverProtection) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("enuModeDamage: ")).StdPrint(); Union::StringANSI(dd.enuModeDamage ? zSTRING(dd.enuModeDamage) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("damageIndex: ")).StdPrint(); Union::StringANSI(damageIndex ? zSTRING(damageIndex) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("fDamageTotal: ")).StdPrint(); Union::StringANSI(dd.fDamageTotal ? zSTRING(dd.fDamageTotal) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("AryDamage: ")).StdPrint(); Union::StringANSI(dd.aryDamage[damageIndex] ? zSTRING(dd.aryDamage[damageIndex]) : zSTRING("None result")).StdPrintLine();

		int customPureDamage = -1;

		if (dd.pNpcAttacker != nullptr && self != nullptr)
		{
			customPureDamage = Call_PullCustomDamage(dd.pNpcAttacker->GetInstance(), self->GetInstance(), 0, damageIndex, dd.aryDamage[damageIndex], dd.nSpellID);
		};

		if (customPureDamage >= 0)
		{
			dd.aryDamage[damageIndex] = customPureDamage;
		}

		int customTotalDamage = -1;

		if (dd.pNpcAttacker != nullptr && self != nullptr)
		{
			int customIntermidiateDamage = -1;

			if (dd.enuModeDamage == oEDamageType_Blunt || dd.enuModeDamage == oEDamageType_Edge)
			{
				customIntermidiateDamage = dd.aryDamage[damageIndex] + dd.pNpcAttacker->GetAttribute(NPC_ATR_STRENGTH);
			}
			if (dd.enuModeDamage == oEDamageType_Point)
			{
				customIntermidiateDamage = dd.aryDamage[damageIndex] + dd.pNpcAttacker->GetAttribute(NPC_ATR_DEXTERITY);
			}

			Union::StringANSI(zSTRING("AryDamage1: ")).StdPrint(); Union::StringANSI(dd.aryDamage[damageIndex] ? zSTRING(dd.aryDamage[damageIndex]) : zSTRING("None result")).StdPrintLine();
			Union::StringANSI(zSTRING("IntermidiateDamage: ")).StdPrint(); Union::StringANSI(customIntermidiateDamage >= -2 ? zSTRING(customIntermidiateDamage) : zSTRING("None result")).StdPrintLine();

			customTotalDamage = Call_PullCustomDamage(dd.pNpcAttacker->GetInstance(), self->GetInstance(), 1, damageIndex, customIntermidiateDamage >= 0 ? customIntermidiateDamage : dd.aryDamage[damageIndex], dd.nSpellID);
		};

		if (customTotalDamage >= 0)
		{
			if (dd.enuModeDamage == oEDamageType_Blunt || dd.enuModeDamage == oEDamageType_Edge)
			{
				dd.pNpcAttacker->attribute[NPC_ATR_STRENGTH] = customTotalDamage - savedWeaponDamage;
				dd.aryDamage[damageIndex] = 0;
			}
			else if (dd.enuModeDamage == oEDamageType_Point)
			{
				dd.pNpcAttacker->attribute[NPC_ATR_DEXTERITY] = customTotalDamage - savedWeaponDamage;
				dd.aryDamage[damageIndex] = 0;
			}
			else
			{
				dd.aryDamage[damageIndex] = customTotalDamage;
			}
		}

		Union::StringANSI(zSTRING("PreHookTotalDamage: ")).StdPrint(); Union::StringANSI(customTotalDamage >= -2 ? zSTRING(customTotalDamage) : zSTRING("None result")).StdPrintLine();

		Hook_oCNpc_OnDamage_Hit(self, vtable, dd);

		Union::StringANSI(zSTRING("PostHookTotalDamage: ")).StdPrint(); Union::StringANSI(customTotalDamage >= -2 ? zSTRING(customTotalDamage) : zSTRING("None result")).StdPrintLine();
		//Union::StringANSI(zSTRING("PostHookDamageTotal: ")).StdPrint(); Union::StringANSI(dd.fDamageTotal >= -2 ? zSTRING(dd.fDamageTotal) : zSTRING("None result")).StdPrintLine();
		
		self->protection[protectionIndex] = savedReceiverProtection;

		dd.pNpcAttacker->attribute[NPC_ATR_STRENGTH] = savedAttackerStrength;
		dd.pNpcAttacker->attribute[NPC_ATR_DEXTERITY] = savedAttackerDexterity;

		Union::StringANSI(zSTRING("PostAttackerStrength: ")).StdPrint(); Union::StringANSI(dd.pNpcAttacker->attribute[NPC_ATR_STRENGTH] ? zSTRING(dd.pNpcAttacker->attribute[NPC_ATR_STRENGTH]) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("PostAttackerDexterity: ")).StdPrint(); Union::StringANSI(dd.pNpcAttacker->attribute[NPC_ATR_DEXTERITY] ? zSTRING(dd.pNpcAttacker->attribute[NPC_ATR_DEXTERITY]) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("PostReceiverProtection: ")).StdPrint(); Union::StringANSI(self->protection[protectionIndex] ? zSTRING(self->protection[protectionIndex]) : zSTRING("None result")).StdPrintLine();
	}*/


	static oCNpc::oSDamageDescriptor* gDamageDescriptor = nullptr;

	oCNpc* attackerNpc = nullptr;
	oCNpc* receiverNpc = nullptr;

	int damageIndex;
	int protectionIndex;

	int customMinimalDamage = -1;
	int customProtection = -2;
	int customPureDamage = -1;
	int customTotalDamage = -1;

	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd);
	auto Hook_oCNpc_OnDamage_Hit = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0072F280, 0x0076DBC0, 0x0077D390, 0x00666610)), &oCNpc_OnDamage_Hit, Union::HookType::Hook_Detours);
	void __fastcall oCNpc_OnDamage_Hit(oCNpc* self, void* vtable, oCNpc::oSDamageDescriptor& dd)
	{
		gDamageDescriptor = &dd;

		attackerNpc = dd.pNpcAttacker;
		receiverNpc = self;

		damageIndex = GetDamageIndex(reinterpret_cast<oEDamageType&>(dd.enuModeDamage)); // damage index by damageType
		protectionIndex = GetProtectionIndex(reinterpret_cast<oEDamageType&>(dd.enuModeDamage)); // protection index by damageType

		// protection //
		int savedReceiverProtection = receiverNpc->protection[protectionIndex];

		if (attackerNpc != nullptr && receiverNpc != nullptr)
		{
			customProtection = Call_PullCustomProtection(attackerNpc->GetInstance(), receiverNpc->GetInstance(), damageIndex, savedReceiverProtection, dd.nSpellID);
		};

		if (customProtection >= -1)
		{
			if (receiverNpc != nullptr)
			{
				receiverNpc->protection[protectionIndex] = customProtection;
			}
		}
		// protection //

		Union::StringANSI(zSTRING("Protection: ")).StdPrint(); Union::StringANSI(savedReceiverProtection ? zSTRING(savedReceiverProtection) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("enuModeDamage: ")).StdPrint(); Union::StringANSI(dd.enuModeDamage ? zSTRING(dd.enuModeDamage) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("damageIndex: ")).StdPrint(); Union::StringANSI(damageIndex ? zSTRING(damageIndex) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("fDamageTotal: ")).StdPrint(); Union::StringANSI(dd.fDamageTotal ? zSTRING(dd.fDamageTotal) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("AryDamage: ")).StdPrint(); Union::StringANSI(zSTRING(dd.aryDamage[damageIndex])).StdPrintLine();

		if (attackerNpc != nullptr && receiverNpc != nullptr)
		{
			customPureDamage = Call_PullCustomDamage(attackerNpc->GetInstance(), receiverNpc->GetInstance(), 0, damageIndex, dd.aryDamage[damageIndex], dd.nSpellID);
		};

		Union::StringANSI(zSTRING("AryDamage2: ")).StdPrint(); Union::StringANSI(zSTRING(dd.aryDamage[damageIndex])).StdPrintLine();

		if (customPureDamage >= 0)
		{
			dd.aryDamage[damageIndex] = customPureDamage;
			dd.fDamageTotal = customPureDamage;

			Union::StringANSI(zSTRING("DamageTotal: ")).StdPrint(); Union::StringANSI(zSTRING(dd.fDamageTotal)).StdPrintLine();
			Union::StringANSI(zSTRING("AryDamage3: ")).StdPrint(); Union::StringANSI(zSTRING(dd.aryDamage[damageIndex])).StdPrintLine();
		}

		Union::StringANSI(zSTRING("ProcessedCustomPureDamage: ")).StdPrint(); Union::StringANSI(customPureDamage >= -10 ? zSTRING(customPureDamage) : zSTRING("None result")).StdPrintLine();
		Union::StringANSI(zSTRING("ProcessedPureDamage: ")).StdPrint(); Union::StringANSI(zSTRING(dd.aryDamage[damageIndex])).StdPrintLine();
		Union::StringANSI(zSTRING("ProcessedProtection: ")).StdPrint(); Union::StringANSI(receiverNpc->protection[protectionIndex] >= -1 ? zSTRING(receiverNpc->protection[protectionIndex]) : zSTRING("None result")).StdPrintLine();

		Hook_oCNpc_OnDamage_Hit(self, vtable, dd);

		receiverNpc->protection[protectionIndex] = savedReceiverProtection;

		// reseting the values
		gDamageDescriptor = nullptr;
		customMinimalDamage = -1;
		customTotalDamage = -1;
		customPureDamage = -1;
		customProtection = -2;
	}

	void __fastcall partialHookProc(Union::Registers& reg);
	auto partialHook = CreatePartialHook((void*)0x0066CA6B, &partialHookProc);
	void __fastcall partialHookProc(Union::Registers& reg)
	{
		Union::StringANSI(zSTRING(" ")).StdPrintLine();
		Union::StringANSI(zSTRING("LAST PROCEDURE NEXT::::::: ")).StdPrintLine();
		Union::StringANSI(zSTRING(" ")).StdPrintLine();

		if (attackerNpc != nullptr && receiverNpc != nullptr && gDamageDescriptor != nullptr)
		{
			customTotalDamage = Call_PullCustomDamage(attackerNpc->GetInstance(), receiverNpc->GetInstance(), 1, damageIndex, gDamageDescriptor->fDamageTotal, gDamageDescriptor->nSpellID);
		};

		if (customTotalDamage >= 0)
		{
			Union::StringANSI(zSTRING("ProcessedDamageTotal: ")).StdPrint(); Union::StringANSI(customTotalDamage >= -1 ? zSTRING(customTotalDamage) : zSTRING("None result")).StdPrintLine();

			reg.edi = customTotalDamage;
		}

		Union::StringANSI(zSTRING(" ")).StdPrintLine();
	}

	void __fastcall partialHookProc1(Union::Registers& reg);
	auto partialHook1 = CreatePartialHook((void*)0x0066CAA0, &partialHookProc1);
	void __fastcall partialHookProc1(Union::Registers& reg)
	{
		Union::StringANSI(zSTRING(" ")).StdPrintLine();
		Union::StringANSI(zSTRING("MIN PROCEDURE NEXT::::::: ")).StdPrintLine();
		Union::StringANSI(zSTRING(" ")).StdPrintLine();

		if (attackerNpc != nullptr && receiverNpc != nullptr && gDamageDescriptor != nullptr)
		{
			customMinimalDamage = Call_PullMinimalDamage(attackerNpc->GetInstance(), receiverNpc->GetInstance(), damageIndex, gDamageDescriptor->nSpellID);
		}

		if (customMinimalDamage >= 0)
		{
			Union::StringANSI(zSTRING("customMinimalDamage: ")).StdPrint(); Union::StringANSI(customMinimalDamage >= -2 ? zSTRING(customMinimalDamage) : zSTRING("none")).StdPrintLine();

			reg.eax = customMinimalDamage;
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