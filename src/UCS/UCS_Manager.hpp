
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
	std::vector<int> GetDamageIndexArr(unsigned long damageMask)
	{
		std::vector<int> result;

		if (damageMask & oEDamageType_Barrier)
			result.push_back(oEDamageIndex_Barrier);

		if (damageMask & oEDamageType_Blunt)
			result.push_back(oEDamageIndex_Blunt);

		if (damageMask & oEDamageType_Edge)
			result.push_back(oEDamageIndex_Edge);

		if (damageMask & oEDamageType_Fire)
			result.push_back(oEDamageIndex_Fire);

		if (damageMask & oEDamageType_Fly)
			result.push_back(oEDamageIndex_Fly);

		if (damageMask & oEDamageType_Magic)
			result.push_back(oEDamageIndex_Magic);

		if (damageMask & oEDamageType_Point)
			result.push_back(oEDamageIndex_Point);

		if (damageMask & oEDamageType_Fall)
			result.push_back(oEDamageIndex_Fall);

		return result;
	}


	class UCS_Manager
	{
		// giving access to hooks

		friend void __fastcall oCGame_MainWorld_Render(Union::Registers& reg);
		friend void __fastcall oCGame_LoadGame(oCGame* self, void* vtable, int slot, const zSTRING& levelPath);
		friend void __fastcall oCGame_LoadSaveGame(oCGame* self, void* vtable, int slot, zBOOL loadGlobals);


		private:

			//  S T R U C T U R E S  /////

			struct FxData
			{
				int damage;
				oEDamageIndex damageIndex;
				int spellID;
				zSTRING strVisualFX;
				int dontKill;
				float loopInterval;
				int exCndFuncIndex;
				int iterCount;
			};

			struct fxProto : FxData
			{
				int id;
				int* outerID;
			};

			std::unordered_map<int, fxProto> fxProtoCollection;

			struct fx : FxData
			{
				int id;
				int* outerID;
			};

			std::unordered_map<int, fx> fxCollection;

			enum CtxType
			{
				CTX_UNKNOWN,
				CTX_REGULAR,
				CTX_LOOP,
			};

			struct ctx : FxData
			{
				int id;
				CtxType type = CTX_UNKNOWN;
				int fxID = -1;

				oCNpc* damageSender;
				oCNpc* damageReceiver;

				float lastIterTime = -1.0f;
				int currIter = -1;

				bool isRunning = false;
				bool isApplying = false;
				bool isCompleted = false;

				bool shouldClose = false;
				bool shouldRestart = false;
			};

			std::unordered_map<int, ctx> ctxCollection;
			std::vector<int> ctxQueue;

			int nextFxProtoID = 0;
			int nextFxID = 0;
			int nextCtxID = 0;


			//  M E T H O D S  /////

			// fx prototype

			fxProto* getFxProto(int fxProtoID)
			{
				auto it = fxProtoCollection.find(fxProtoID);

				if (it == fxProtoCollection.end()) return nullptr;

				return &it->second;
			}

			bool isFxProtoValid(fxProto* currFxProto)
			{
				if (currFxProto == nullptr ||
					currFxProto->id < 0 ||
					currFxProto->damage < 0 ||
					currFxProto->damageIndex < 0 ||
					currFxProto->damageIndex >= oEDamageIndex_MAX ||
					currFxProto->spellID < -1 ||
					currFxProto->dontKill < 0 ||
					currFxProto->loopInterval < 100.0f ||
					currFxProto->iterCount < 0 && currFxProto->exCndFuncIndex < 0)
					return false;

				return true;
			}

			void addFxProto
			(
				int* outerFxProtoID,
				int damage,
				oEDamageIndex damageIndex,
				int spellID,
				zSTRING strVisualFX,
				int dontKill,
				float loopInterval,
				int iterCount,
				int exCndFuncIndex
			)
			{
				fxProto* currFxProto = getFxProto(*outerFxProtoID);

				if (isFxProtoValid(currFxProto) && currFxProto->outerID == outerFxProtoID)
				{
					*currFxProto->outerID = currFxProto->id; return;
				}


				fxProto newFxProto{};

				newFxProto.id = nextFxProtoID++;
				newFxProto.outerID = outerFxProtoID;
				*newFxProto.outerID = newFxProto.id;

				newFxProto.damage = damage;
				newFxProto.damageIndex = damageIndex;
				newFxProto.spellID = spellID;
				newFxProto.strVisualFX = strVisualFX;
				newFxProto.dontKill = dontKill;
				newFxProto.loopInterval = loopInterval;
				newFxProto.iterCount = iterCount;
				newFxProto.exCndFuncIndex = exCndFuncIndex;

				fxProtoCollection[newFxProto.id] = newFxProto;
			}


			// fx instance

			fx* getFx(int fxID)
			{
				auto it = fxCollection.find(fxID);

				if (it == fxCollection.end()) return nullptr;

				return &it->second;
			}

			bool isFxValid(fx* currFx)
			{
				if (currFx == nullptr ||
					currFx->id < 0 ||
					currFx->damage < 0 ||
					currFx->damageIndex < 0 ||
					currFx->damageIndex >= oEDamageIndex_MAX ||
					currFx->spellID < -1 ||
					currFx->dontKill < 0 ||
					currFx->loopInterval < 100.0f ||
					currFx->iterCount < 0 && currFx->exCndFuncIndex < 0)
					return false;

				return true;
			}

			void addFx
			(
				int* outerFxID,
				int damage,
				oEDamageIndex damageIndex,
				int spellID,
				zSTRING strVisualFX,
				int dontKill,
				float loopInterval,
				int iterCount,
				int exCndFuncIndex
			)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID)
				{
					*currFx->outerID = currFx->id; return;
				}


				fx newFx{};

				newFx.id = nextFxID++;
				newFx.outerID = outerFxID;
				*newFx.outerID = newFx.id;

				newFx.damage = damage;
				newFx.damageIndex = damageIndex;
				newFx.spellID = spellID;
				newFx.strVisualFX = strVisualFX;
				newFx.dontKill = dontKill;
				newFx.loopInterval = loopInterval;
				newFx.iterCount = iterCount;
				newFx.exCndFuncIndex = exCndFuncIndex;

				fxCollection[newFx.id] = newFx;
			}


			// fx context

			ctx* getCtx(int ctxID)
			{
				auto it = ctxCollection.find(ctxID);

				if (it == ctxCollection.end()) return nullptr;

				return &it->second;
			}
			ctx* getCtx(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				for (auto& el : ctxCollection)
				{
					if (el.second.fxID == fxID && el.second.damageSender == damageSender && el.second.damageReceiver == damageReceiver)
					{
						return &el.second;
					}
				}

				return nullptr;
			}

			bool isCtxValid(ctx* currCtx)
			{
				if (currCtx == nullptr ||
					currCtx->id < 0 ||
					currCtx->type == CTX_UNKNOWN ||
					currCtx->fxID < -1 ||
					currCtx->damageSender == nullptr ||
					currCtx->damageReceiver == nullptr ||
					currCtx->damage < 0 ||
					currCtx->damageIndex < 0 ||
					currCtx->damageIndex >= oEDamageIndex_MAX ||
					currCtx->spellID < -1 ||
					currCtx->dontKill < 0 ||
					(currCtx->isRunning || currCtx->isApplying) && currCtx->isCompleted)
					return false;

				if (currCtx->type == CTX_LOOP)
				{
					if ((currCtx->loopInterval < 100.0f) ||
						(currCtx->iterCount < 0 && currCtx->exCndFuncIndex < 0) ||
						((!!currCtx->currIter) != (!!currCtx->lastIterTime)))
						return false;
				}

				return true;
			}

			void addCtx
			(
				int fxID = -1,
				oCNpc* damageSender = nullptr,
				oCNpc* damageReceiver = nullptr,
				int damage = -1,
				oEDamageIndex damageIndex = oEDamageIndex_Barrier,
				int spellID = -1,
				zSTRING strVisualFX = zSTRING(""),
				int dontKill = -1,
				float loopInterval = -1.0f,
				int iterCount = -1,
				int exCndFuncIndex = -1
			)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);
				fx* currFx = getFx(fxID);

				if (isCtxValid(currCtx) && currCtx->isRunning) return;

				if (!isFxValid(currFx)) return;


				ctx newCtx{};

				newCtx.id = nextCtxID++;
				newCtx.type =
				(
					(currFx ? currFx->loopInterval : loopInterval) >= 100.0f &&
					(
						(currFx ? currFx->iterCount : iterCount) >= 0 ||
						(currFx ? currFx->exCndFuncIndex : exCndFuncIndex) >= 0
					)
					? CTX_LOOP : CTX_REGULAR
				);
				newCtx.fxID = fxID;

				newCtx.damageSender = damageSender;
				newCtx.damageReceiver = damageReceiver;

				newCtx.damage = currFx ? currFx->damage : damage;
				newCtx.damageIndex = currFx ? currFx->damageIndex : damageIndex;
				newCtx.spellID = currFx ? currFx->spellID : spellID;
				newCtx.strVisualFX = currFx ? currFx->strVisualFX : strVisualFX;
				newCtx.dontKill = currFx ? currFx->dontKill : dontKill;
				newCtx.loopInterval = currFx ? currFx->loopInterval : loopInterval;
				newCtx.iterCount = currFx ? currFx->iterCount : iterCount;
				newCtx.exCndFuncIndex = currFx ? currFx->exCndFuncIndex : exCndFuncIndex;

				ctxCollection[newCtx.id] = newCtx;
			}


			void runCtx(int ctxID)
			{
				ctx* currCtx = getCtx(ctxID);

				if (!isCtxValid(currCtx)) return;

				if (currCtx->isRunning || currCtx->isApplying || currCtx->isCompleted) return;

				currCtx->isRunning = true;

				ctxQueue.push_back(ctxID);
			}

			void closeCtx(int ctxID)
			{
				ctx* currCtx = getCtx(ctxID);

				if (!isCtxValid(currCtx)) return;

				if (!currCtx->isRunning || currCtx->isApplying || currCtx->isCompleted) return;

				currCtx->isRunning = false;
				currCtx->isCompleted = true;

				currCtx->shouldClose = false;
			}

			void restartCtx(int ctxID)
			{
				ctx* currCtx = getCtx(ctxID);

				if (!isCtxValid(currCtx)) return;

				if (!currCtx->isRunning || currCtx->isApplying || currCtx->isCompleted) return;

				fx* currFx = getFx(currCtx->fxID);

				if (!isFxValid(currFx)) return;

				currCtx->damage = currFx->damage;
				currCtx->damageIndex = currFx->damageIndex;
				currCtx->spellID = currFx->spellID;
				currCtx->strVisualFX = currFx->strVisualFX;
				currCtx->dontKill = currFx->dontKill;
				currCtx->loopInterval = currFx->loopInterval;
				currCtx->iterCount = currFx->iterCount;
				currCtx->exCndFuncIndex = currFx->exCndFuncIndex;

				currCtx->currIter = -1;
				currCtx->lastIterTime = -1.0f;

				currCtx->shouldRestart = false;
			}


			void updateCtxCollection()
			{
				for (auto it = ctxCollection.begin(); it != ctxCollection.end();)
				{
					ctx* currCtx = getCtx(it->first);

					if (!currCtx->isRunning && !currCtx->isCompleted && !currCtx->isApplying)
					{
						runCtx(currCtx->id);
					}
					if (currCtx->shouldClose && currCtx->isRunning && !currCtx->isCompleted && !currCtx->isApplying)
					{
						closeCtx(currCtx->id);
					}
					if (currCtx->shouldRestart && currCtx->isRunning && !currCtx->isCompleted && !currCtx->isApplying)
					{
						restartCtx(currCtx->id);
					}

					++it;
				}
			}

			void filterCtxCollection()
			{
				for (auto it = ctxCollection.begin(); it != ctxCollection.end();)
				{
					ctx* currCtx = getCtx(it->first);

					if (currCtx->isCompleted || !isCtxValid(currCtx))
					{
						it = ctxCollection.erase(it); continue;
					}

					++it;
				}
			}

			void clearCtxCollection()
			{
				ctxCollection.clear();
			}


			void updateCtxQueue()
			{
				for (auto it = ctxQueue.begin(); it != ctxQueue.end();)
				{
					ctx* currCtx = getCtx(*it);

					if (!isCtxValid(currCtx) || !currCtx->isRunning || currCtx->isCompleted)
					{
						closeCtx(*it);
						it = ctxQueue.erase(it); continue;
					}


					if (currCtx->exCndFuncIndex >= 0)
					{
						parser->SetInstance("SELF", currCtx->damageReceiver);
						parser->SetInstance("OTHER", currCtx->damageSender);

						void* pRet = parser->CallFunc(currCtx->exCndFuncIndex, currCtx->fxID);
						int isFuncTrue = *reinterpret_cast<int*>(pRet);

						if (isFuncTrue)
						{
							closeCtx(*it);
							it = ctxQueue.erase(it); continue;
						}
					}


					if (currCtx->type == CTX_REGULAR)
					{
						// filling damage descriptor

						oCNpc::oSDamageDescriptor dd{};

						dd.dwFieldsValid =
							oCNpc::oEDamageDescFlag_Damage |
							oCNpc::oEDamageDescFlag_DamageType |
							oCNpc::oEDamageDescFlag_Attacker |
							oCNpc::oEDamageDescFlag_Npc |
							oCNpc::oEDamageDescFlag_VisualFX |
							oCNpc::oEDamageDescFlag_SpellID |
							oCNpc::oEDamageDescFlag_HitLocation |
							oCNpc::oEDamageDescFlag_FlyDirection;

						dd.pVobAttacker = currCtx->damageSender;
						dd.pNpcAttacker = currCtx->damageSender;
						dd.pVobHit = currCtx->damageReceiver;
						dd.enuModeDamage = GetDamageType(currCtx->damageIndex);
						dd.aryDamage[currCtx->damageIndex] = currCtx->damage;
						dd.fDamageTotal = currCtx->damage;
						dd.nSpellID = currCtx->spellID;
						dd.strVisualFX = currCtx->strVisualFX;

						#if ENGINE == Engine_G2A
							dd.nSpellCat = 2;
							dd.bDamageDontKill = currCtx->dontKill;
						#endif

						dd.fDamageMultiplier = 1.0f;
						dd.vecLocationHit = currCtx->damageReceiver->GetPositionWorld();
						dd.vecDirectionFly =
						(
							currCtx->damageReceiver->GetPositionWorld() -
							currCtx->damageSender->GetPositionWorld()
						).Normalize();

						// damage applying
						currCtx->isApplying = true;
						currCtx->damageReceiver->OnDamage(dd);
						currCtx->isApplying = false;
						// damage applying

						closeCtx(*it);
						it = ctxQueue.erase(it); continue;
					}
					if (currCtx->type == CTX_LOOP)
					{
						float currTime = ogame->GetWorldTimer()->GetFullTime();

						bool isFirstIter =
						(
							currCtx->currIter == -1 &&
							currCtx->lastIterTime == -1.0f
						);

						bool isCooldownPassed =
						(
							currTime >= (currCtx->lastIterTime + currCtx->loopInterval)
						);

						bool isItersExceeded =
						(
							currCtx->iterCount >= 0 && currCtx->currIter >= currCtx->iterCount
						);

						bool isReceiverUnconscious =
						(
							!currCtx->damageReceiver->IsMonster() && currCtx->damageReceiver->IsUnconscious()
						);

						bool isReceiverDead =
						(
							currCtx->damageReceiver->IsDead()
						);


						if (isItersExceeded || isReceiverUnconscious || isReceiverDead)
						{
							closeCtx(*it);
							it = ctxQueue.erase(it); continue;
						}

						if (!isFirstIter && !isCooldownPassed)
						{
							++it; continue;
						}

						if (isFirstIter)
						{
							currCtx->currIter = 0;
						}

						currCtx->currIter++;
						currCtx->lastIterTime = currTime;


						// filling damage descriptor

						oCNpc::oSDamageDescriptor dd{};

						dd.dwFieldsValid =
							oCNpc::oEDamageDescFlag_Damage |
							oCNpc::oEDamageDescFlag_DamageType |
							oCNpc::oEDamageDescFlag_Attacker |
							oCNpc::oEDamageDescFlag_Npc |
							oCNpc::oEDamageDescFlag_VisualFX |
							oCNpc::oEDamageDescFlag_SpellID |
							oCNpc::oEDamageDescFlag_HitLocation |
							oCNpc::oEDamageDescFlag_FlyDirection;

						dd.pVobAttacker = currCtx->damageSender;
						dd.pNpcAttacker = currCtx->damageSender;
						dd.pVobHit = currCtx->damageReceiver;
						dd.enuModeDamage = GetDamageType(currCtx->damageIndex);
						dd.aryDamage[currCtx->damageIndex] = currCtx->damage;
						dd.fDamageTotal = currCtx->damage;
						dd.nSpellID = currCtx->spellID;
						dd.strVisualFX = currCtx->strVisualFX;

						#if ENGINE == Engine_G2A
							dd.nSpellCat = 2;
							dd.bDamageDontKill = currCtx->dontKill;
						#endif

						dd.fDamageMultiplier = 1.0f;
						dd.vecLocationHit = currCtx->damageReceiver->GetPositionWorld();
						dd.vecDirectionFly =
						(
							currCtx->damageReceiver->GetPositionWorld() -
							currCtx->damageSender->GetPositionWorld()
						).Normalize();

						// damage applying
						currCtx->isApplying = true;
						currCtx->damageReceiver->OnDamage(dd);
						currCtx->isApplying = false;
						// damage applying
					}

					++it;
				}
			}

			void clearCtxQueue()
			{
				ctxQueue.clear();
			}


			// ctx getters

			int getCtxID(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->id : -1;
			}
			oCNpc* getCtxDamageSender(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->damageSender : nullptr;
			}
			oCNpc* getCtxDamageReceiver(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->damageReceiver : nullptr;
			}
			int getCtxDamage(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->damage : -1;
			}
			int getCtxDamageIndex(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->damageIndex : -1;
			}
			int getCtxCurrIter(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->currIter : -1;
			}
			float getCtxLoopInterval(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->loopInterval : -1.0f;
			}
			int getCtxIterCount(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->iterCount : -1;
			}
			int getCtxSpellID(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->spellID : -1;
			}
			zSTRING getCtxStrVisualFX(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->strVisualFX : zSTRING("");
			}
			int getCtxDontKill(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->dontKill : -1;
			}
			float getCtxLastIterTime(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->lastIterTime : -1.0f;
			}

			bool getCtxIsRunning(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->isRunning : false;
			}
			bool getCtxIsApplying(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->isApplying : false;
			}
			bool getCtxIsCompleted(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				return isCtxValid(currCtx) ? currCtx->isCompleted : false;
			}

			// ctx setters

			void setCtxDamage(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDamage)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newDamage >= 0) currCtx->damage = newDamage;
			}
			void setCtxDamageIndex(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, oEDamageIndex newDamageIndex)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newDamageIndex >= 0 && newDamageIndex < oEDamageIndex_MAX)
					currCtx->damageIndex = newDamageIndex;
			}
			void setCtxSpellID(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newSpellID)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newSpellID >= -1) currCtx->spellID = newSpellID;
			}
			void setCtxStrVisualFX(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, zSTRING newStrVisualFX)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newStrVisualFX) currCtx->strVisualFX = newStrVisualFX;
			}
			void setCtxDontKill(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDontKill)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newDontKill >= 0) currCtx->dontKill = newDontKill;
			}
			void setCtxLoopInterval(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, float newLoopInterval)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newLoopInterval >= 100.0f) currCtx->loopInterval = newLoopInterval;
			}
			void setCtxIterCount(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newIterCount)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newIterCount >= -1) currCtx->iterCount = newIterCount;
			}
			void setCtxExCndFuncIndex(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newExCndFuncIndex)
			{
				ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

				if (isCtxValid(currCtx) && currCtx->isApplying && newExCndFuncIndex >= -1) currCtx->exCndFuncIndex = newExCndFuncIndex;
			}


		public:

			void createFxProto
			(
				int* outerFxProtoID,
				int damage,
				oEDamageIndex damageIndex,
				int spellID,
				zSTRING strVisualFX,
				int dontKill,
				float loopInterval,
				int iterCount,
				int exCndFuncIndex
			)
			{
				if (outerFxProtoID == nullptr) return;

				if (damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 100.0f && iterCount >= -1 && exCndFuncIndex >= -1)
				{
					addFxProto(outerFxProtoID, damage, damageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);
				}
			}

			void startFx
			(
				int* outerFxID,
				int* outerFxProtoID,
				oCNpc* damageSender,
				oCNpc* damageReceiver
			)
			{
				if (outerFxID == nullptr || outerFxProtoID == nullptr) return;

				fxProto* currFxProto = getFxProto(*outerFxProtoID);

				if (!isFxProtoValid(currFxProto)) return;

				if (damageSender != nullptr && damageReceiver != nullptr && currFxProto->damage >= 0 && currFxProto->damageIndex >= 0 && currFxProto->spellID >= -1 && currFxProto->strVisualFX && currFxProto->dontKill >= 0 && currFxProto->loopInterval >= 100.0f && currFxProto->iterCount >= -1 && currFxProto->exCndFuncIndex >= -1)
				{
					addFx(outerFxID, currFxProto->damage, currFxProto->damageIndex, currFxProto->spellID, currFxProto->strVisualFX, currFxProto->dontKill, currFxProto->loopInterval, currFxProto->iterCount, currFxProto->exCndFuncIndex);

					addCtx(*outerFxID, damageSender, damageReceiver);
				}
			}

			void startFxEx
			(
				int* outerFxID,
				oCNpc* damageSender,
				oCNpc* damageReceiver,
				int damage,
				oEDamageIndex damageIndex,
				int spellID,
				zSTRING strVisualFX,
				int dontKill,
				float loopInterval,
				int iterCount,
				int exCndFuncIndex
			)
			{
				if (outerFxID == nullptr) return;

				if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 100.0f && iterCount >= -1 && exCndFuncIndex >= -1)
				{
					addFx(outerFxID, damage, damageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);

					addCtx(*outerFxID, damageSender, damageReceiver);
				}
			}

			void startInstantFx
			(
				oCNpc* damageSender,
				oCNpc* damageReceiver,
				int damage,
				oEDamageIndex damageIndex,
				int spellID,
				zSTRING strVisualFX,
				int dontKill
			)
			{
				if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0)
				{
					addCtx(-1, damageSender, damageReceiver, damage, damageIndex, spellID, strVisualFX, dontKill);
				}
			}

			void stopFx
			(
				int* outerFxID,
				oCNpc* damageSender,
				oCNpc* damageReceiver
			)
			{
				if (outerFxID == nullptr) return;

				ctx* currCtx = getCtx(*outerFxID, damageSender, damageReceiver);

				if (currCtx) currCtx->shouldClose = true;
			}

			void restartFx
			(
				int* outerFxID,
				oCNpc* damageSender,
				oCNpc* damageReceiver
			)
			{
				if (outerFxID == nullptr) return;

				ctx* currCtx = getCtx(*outerFxID, damageSender, damageReceiver);

				if (currCtx) currCtx->shouldRestart = true;
			}


			// fx getters

			oCNpc* getFxDamageSender(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxDamageSender(*outerFxID, damageSender, damageReceiver) : nullptr;
			}
			oCNpc* getFxDamageReceiver(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxDamageReceiver(*outerFxID, damageSender, damageReceiver) : nullptr;
			}
			int getFxDamage(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxDamage(*outerFxID, damageSender, damageReceiver) : -1;
			}
			int getFxDamageIndex(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxDamageIndex(*outerFxID, damageSender, damageReceiver) : -1;
			}
			int getFxCurrIter(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxCurrIter(*outerFxID, damageSender, damageReceiver) : -1;
			}
			float getFxLoopInterval(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxLoopInterval(*outerFxID, damageSender, damageReceiver) : -1.0f;
			}
			int getFxIterCount(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxIterCount(*outerFxID, damageSender, damageReceiver) : -1;
			}
			int getFxSpellID(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxSpellID(*outerFxID, damageSender, damageReceiver) : -1;
			}
			zSTRING getFxStrVisualFX(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxStrVisualFX(*outerFxID, damageSender, damageReceiver) : zSTRING("");
			}
			int getFxDontKill(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxDontKill(*outerFxID, damageSender, damageReceiver) : -1;
			}
			float getFxLastIterTime(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxLastIterTime(*outerFxID, damageSender, damageReceiver) : -1.0f;
			}

			bool getFxIsRunning(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxIsRunning(*outerFxID, damageSender, damageReceiver) : false;
			}
			bool getFxIsApplying(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxIsApplying(*outerFxID, damageSender, damageReceiver) : false;
			}
			bool getFxIsCompleted(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
			{
				fx* currFx = getFx(*outerFxID);

				return isFxValid(currFx) && currFx->outerID == outerFxID ? getCtxIsCompleted(*outerFxID, damageSender, damageReceiver) : false;
			}

			// fx setters

			void setFxDamage(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDamage)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxDamage(*outerFxID, damageSender, damageReceiver, newDamage);
			}
			void setFxDamageIndex(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, oEDamageIndex newDamageIndex)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxDamageIndex(*outerFxID, damageSender, damageReceiver, newDamageIndex);
			}
			void setFxSpellID(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newSpellID)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxSpellID(*outerFxID, damageSender, damageReceiver, newSpellID);
			}
			void setFxStrVisualFX(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, zSTRING newStrVisualFX)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxStrVisualFX(*outerFxID, damageSender, damageReceiver, newStrVisualFX);
			}
			void setFxDontKill(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDontKill)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxDontKill(*outerFxID, damageSender, damageReceiver, newDontKill);
			}
			void setFxLoopInterval(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, float newLoopInterval)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxLoopInterval(*outerFxID, damageSender, damageReceiver, newLoopInterval);
			}
			void setFxIterCount(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newIterCount)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxIterCount(*outerFxID, damageSender, damageReceiver, newIterCount);
			}
			void setFxExCndFuncIndex(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newExCndFuncIndex)
			{
				fx* currFx = getFx(*outerFxID);

				if (isFxValid(currFx) && currFx->outerID == outerFxID) setCtxExCndFuncIndex(*outerFxID, damageSender, damageReceiver, newExCndFuncIndex);
			}
	};

	static UCS_Manager ucsManager;
}