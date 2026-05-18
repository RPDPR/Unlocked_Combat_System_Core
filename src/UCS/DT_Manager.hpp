
namespace GOTHIC_NAMESPACE
{
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

	NpcDTManager dtManager; // -> damage type manager


	// EXTERNALS /////

	int __cdecl Npc_GetLastHitDT()
	{
		oCNpc* npc = (oCNpc*)(parser->GetInstance());

		int result = int(npc != nullptr ? dtManager.GetLastHitDT(npc) : -1);

		parser->SetReturn(result);

		return 0;
	}


	void Game_DefineExternals_DT_Manager()
	{
		parser->DefineExternal("Npc_GetLastHitDT", Npc_GetLastHitDT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
	}
}