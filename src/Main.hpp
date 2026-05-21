
#include <unordered_map>
#include <vector>

#include "UCS/DT_Manager.hpp"
#include "UCS/Pipeline.hpp"
#include "UCS/ApplyDamage.hpp"
#include "UCS/Hlp_Packet.hpp"

namespace GOTHIC_NAMESPACE
{
	void Call_UCS_Init()
	{
		int funcIndex = parser->GetIndex(zSTRING("UCS_Init")); if (funcIndex < 0) return;

		parser->CallFunc(funcIndex);

		return;
	}

	void __fastcall oCGame_Init(oCGame* self, void* vtable);
	auto Hook_oCGame_Init = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x00636F50, 0x006C1060)), &oCGame_Init, Union::HookType::Hook_Detours);
	void __fastcall oCGame_Init(oCGame* self, void* vtable)
	{
		Hook_oCGame_Init(self, vtable);
		Call_UCS_Init();
	}

	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg);
	auto Partial_zCWorld_Render = Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0063DC76, 0x006C87EB)), &oCGame_MainWorld_Render);
	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg)
	{
		ucsManager.updateCtxQueue();
	}


	// EXTERNALS /////

	void Game_DefineExternals()
	{
		Game_DefineExternals_ApplyDamage();
		Game_DefineExternals_DT_Manager();
		Game_DefineExternals_HlpPacket();
	}

	void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser);
	auto Hook_oCGame_DefineExternals_Ulfi = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x006495B0, 0x006715F0, 0x00677A00, 0x006D4780)), &oCGame_DefineExternals_Ulfi, Union::HookType::Hook_Detours);
	void __fastcall oCGame_DefineExternals_Ulfi(oCGame* self, void* vtable, zCParser* parser)
	{
		Hook_oCGame_DefineExternals_Ulfi(self, vtable, parser);
		Game_DefineExternals();
	}
}