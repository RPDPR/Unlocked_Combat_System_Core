
#include <unordered_map>
#include <vector>

#include "UCS/DT_Manager.hpp"
#include "UCS/UCS_Manager.hpp"
#include "UCS/Pipeline.hpp"
#include "UCS/Externals/UCS_Packet.hpp"
#include "UCS/Externals/Hlp_Packet.hpp"

namespace GOTHIC_NAMESPACE
{
	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg);
	auto Partial_zCWorld_Render = Union::CreatePartialHook(reinterpret_cast<void*>(zSwitch(0x0063DC76, 0x006C87EB)), &oCGame_MainWorld_Render);
	void __fastcall oCGame_MainWorld_Render(Union::Registers& reg)
	{
		ucsManager.updateCtxCollection();
		ucsManager.updateCtxQueue();
		ucsManager.filterCtxCollection();
	}
	void __fastcall oCGame_LoadGame(oCGame* self, void* vtable, int slot, const zSTRING& levelPath);
	auto Hook_oCGame_LoadGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063C070, 0x006C65A0)), &oCGame_LoadGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_LoadGame(oCGame* self, void* vtable, int slot, const zSTRING& levelPath)
	{
		ucsManager.clearCtxQueue();
		ucsManager.clearCtxCollection();

		Hook_oCGame_LoadGame(self, vtable, slot, levelPath);
	}

	void __fastcall oCGame_LoadSaveGame(oCGame* self, void* vtable, int slot, zBOOL loadGlobals);
	auto Hook_oCGame_LoadSaveGame = Union::CreateHook(reinterpret_cast<void*>(zSwitch(0x0063C2A0, 0x006C67D0)), &oCGame_LoadSaveGame, Union::HookType::Hook_Detours);
	void __fastcall oCGame_LoadSaveGame(oCGame* self, void* vtable, int slot, zBOOL loadGlobals)
	{
		ucsManager.clearCtxQueue();
		ucsManager.clearCtxCollection();

		Hook_oCGame_LoadSaveGame(self, vtable, slot, loadGlobals);
	}


	// EXTERNALS /////

	void Game_DefineExternals()
	{
		Game_DefineExternals_UCS_Packet();
		Game_DefineExternals_Pipeline();
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