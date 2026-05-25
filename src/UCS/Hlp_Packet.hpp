
namespace GOTHIC_NAMESPACE
{
	// EXTERNALS /////

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
				oCItem* currItem = npc->inventory2.contents->Get(i);

				if (currItem->GetInstance() == item->GetInstance() && currItem->HasFlag(ITM_FLAG_ACTIVE))
				{
					parser->SetReturn(1);

					return 0;
				}
			}
		}

		parser->SetReturn(0);

		return 0;
	}


	// EXTERNALS /////

	void Game_DefineExternals_HlpPacket()
	{
		parser->DefineExternal("Hlp_MultInt", Hlp_MultInt, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_MultInt_F", Hlp_MultInt_F, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_IsItemEquipped", Hlp_IsItemEquipped, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("Hlp_GetItemInstanceID", Hlp_GetItemInstanceID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
	}
}