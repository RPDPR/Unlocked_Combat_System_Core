
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
	private:

		struct fxProto
		{
			int id;
			int* outerID;

			int damage;
			oEDamageIndex damageIndex;
			int spellID;
			zSTRING strVisualFX;
			int dontKill;
			float loopInterval;
			int exCndFuncIndex;
			int iterCount;
		};

		std::unordered_map<int, fxProto> fxProtoCollection;

		struct fx
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

		struct ctx
		{
			int id;
			CtxType type = CTX_UNKNOWN;
			int fxProtoID = -1;
			int fxID = -1;

			oCNpc* damageSender;
			oCNpc* damageReceiver;

			int damage;
			oEDamageIndex damageIndex;
			int spellID;
			zSTRING strVisualFX;
			int dontKill;
			float loopInterval;
			int exCndFuncIndex;
			int iterCount;

			float lastIterTime = -1.0f;
			int currIter = -1;
			bool isRunning = false;
			bool isApplying = false;
			bool isCompleted = false;
		};

		std::unordered_map<int, ctx> ctxCollection;
		std::vector<int> ctxQueue;

		int nextFxProtoID = 0;
		int nextFxID = 0;
		int nextCtxID = 0;

	public:

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

			if (currFxProto && currFxProto->outerID == outerFxProtoID && isFxProtoValid(currFxProto))
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


		fx* getFx(int fxID)
		{
			auto it = fxCollection.find(fxID);

			if (it == fxCollection.end()) return nullptr;

			return &it->second;
		}

		bool isFxValid(fx* currFx)
		{
			if (currFx == nullptr ||
				currFx->id < 0)
				return false;

			return true;
		}

		void addFx
		(
			int* outerFxID
		)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID && isFxValid(currFx))
			{
				*currFx->outerID = currFx->id; return;
			}


			fx newFx{};

			newFx.id = nextFxID++;
			newFx.outerID = outerFxID;
			*newFx.outerID = newFx.id;

			fxCollection[newFx.id] = newFx;
		}


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

		void addCtx
		(
			int fxID,
			int fxProtoID,
			oCNpc* damageSender,
			oCNpc* damageReceiver
		)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);
			fxProto* currFxProto = getFxProto(fxProtoID);

			if (currCtx && currCtx->isRunning) {
				Union::StringANSI::Format(zSTRING("addCtx returned -1 cuz of this 1 branch")).StdPrintLine();
				return;
			}
			if (!isFxProtoValid(currFxProto)) {
				Union::StringANSI::Format(zSTRING("addCtx returned -1 cuz of this 2 branch")).StdPrintLine();
				return;
			}


			ctx newCtx{};

			newCtx.id = nextCtxID++;
			newCtx.type = CTX_LOOP;
			newCtx.fxProtoID = fxProtoID;
			newCtx.fxID = fxID;

			newCtx.damageSender = damageSender;
			newCtx.damageReceiver = damageReceiver;
			newCtx.damage = currFxProto->damage;
			newCtx.damageIndex = currFxProto->damageIndex;
			newCtx.spellID = currFxProto->spellID;
			newCtx.strVisualFX = currFxProto->strVisualFX;
			newCtx.dontKill = currFxProto->dontKill;
			newCtx.loopInterval = currFxProto->loopInterval;
			newCtx.iterCount = currFxProto->iterCount;
			newCtx.exCndFuncIndex = currFxProto->exCndFuncIndex;

			ctxCollection[newCtx.id] = newCtx;
		}
		void addCtx
		(
			int fxID,
			oCNpc* damageSender,
			oCNpc* damageReceiver,
			int damage,
			oEDamageIndex damageIndex,
			int spellID,
			zSTRING strVisualFX,
			int dontKill,
			float loopInterval = -1.0f,
			int iterCount = -1,
			int exCndFuncIndex = -1
		)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && currCtx->isRunning)
				return;


			ctx newCtx{};

			newCtx.id = nextCtxID++;
			newCtx.type = (loopInterval >= 100.0f) && (iterCount >= 0 || exCndFuncIndex >= 0) ? CTX_LOOP : CTX_REGULAR;
			newCtx.fxID = fxID;

			newCtx.damageSender = damageSender;
			newCtx.damageReceiver = damageReceiver;
			newCtx.damage = damage;
			newCtx.damageIndex = damageIndex;
			newCtx.spellID = spellID;
			newCtx.strVisualFX = strVisualFX;
			newCtx.dontKill = dontKill;
			newCtx.loopInterval = loopInterval;
			newCtx.iterCount = iterCount;
			newCtx.exCndFuncIndex = exCndFuncIndex;

			ctxCollection[newCtx.id] = newCtx;
		}

		bool isCtxValid(ctx* currCtx)
		{
			if (currCtx == nullptr ||
				currCtx->id < 0 ||
				currCtx->type == CTX_UNKNOWN ||
				currCtx->fxProtoID < -1 ||
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
				if (currCtx->loopInterval < 100.0f ||
					currCtx->iterCount < 0 && currCtx->exCndFuncIndex < 0 ||
					(!!currCtx->currIter) != (!!currCtx->lastIterTime))
					return false;
			}

			return true;
		}

		void runCtx(int ctxID)
		{
			ctx* currCtx = getCtx(ctxID);

			if (!isCtxValid(currCtx)) return;

			if (currCtx->isRunning || currCtx->isApplying || currCtx->isCompleted) return;

			currCtx->isRunning = true;

			ctxQueue.push_back(ctxID);

			return;
		}

		void closeCtx(int ctxID)
		{
			auto it = ctxCollection.find(ctxID);

			if (it == ctxCollection.end()) return;

			Union::StringANSI::Format(zSTRING("Ctx with id: {0}, was closed! Params: type: {1}, isCtxValid: {2}, isRn: {3}, isCmp: {4}, dmg: {5}, currIter: {6}, currTime: {7}, lastIterTime: {8}, loopInterval: {9}, "), it->second.id, it->second.type, isCtxValid(getCtx(it->second.id)), getCtxIsRunning(it->second.id, it->second.damageSender, it->second.damageReceiver), getCtxIsCompleted(it->second.id, it->second.damageSender, it->second.damageReceiver), getCtxDamage(it->second.id, it->second.damageSender, it->second.damageReceiver), getCtxCurrIter(it->second.id, it->second.damageSender, it->second.damageReceiver), it->second.currIter, it->second.lastIterTime, it->second.loopInterval).StdPrintLine();

			it->second.isApplying = false;
			it->second.isRunning = false;
			it->second.isCompleted = true;
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

				++it;
			}
		}

		void updateCtxQueue()
		{
			/* for (auto& [id, currFx] : fxProtoCollection)
			{
				Union::StringANSI::Format(zSTRING("fxProtoCollection.id {0}: fxProtoID: {1}"), id, currFx.id).StdPrintLine();
			} */
			/* for (auto& [id, currCtx] : ctxCollection)
			{
				Union::StringANSI::Format(zSTRING("size: {0}, ctxCollection.id {1}: ctxID: {2}"), ctxCollection.size(), id, currCtx.id).StdPrintLine();
			} */

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
				//Union::StringANSI::Format(zSTRING("ctxQueue | id: {0}, type: {1}, isRn: {2}, isCmp: {3}, dmg: {4} currIter: {5}, iterCount: {6}, exCndFuncIdx: {7}, "), currCtx->id, currCtx->type, getCtxIsRunning(currCtx->fxID, currCtx->damageSender, currCtx->damageReceiver), getCtxIsCompleted(currCtx->fxID, currCtx->damageSender, currCtx->damageReceiver), getCtxDamage(currCtx->fxID, currCtx->damageSender, currCtx->damageReceiver), currCtx->currIter, currCtx->iterCount, currCtx->exCndFuncIndex).StdPrintLine();


				if (currCtx->type == CTX_REGULAR)
				{
					// filling damage descriptor
					
					oCNpc::oSDamageDescriptor dd {};

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
						Union::StringANSI::Format(zSTRING("closed in unexpected place. isItersExceeded: {0}, isRecDead: {1}, isRecUnc: {2}"), isItersExceeded, isReceiverDead, isReceiverUnconscious).StdPrintLine();
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

					oCNpc::oSDamageDescriptor dd {};

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

					Union::StringANSI::Format(zSTRING("APPLYING CTX WITH ID {0} AND ITER {1} AND DAMAGE {2}"), currCtx->id, currCtx->currIter, currCtx->damage).StdPrintLine();

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
			Union::StringANSI::Format(zSTRING("ctxQueue was cleared! ctxQueue size: {0}"), ctxQueue.size()).StdPrintLine();
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
			Union::StringANSI::Format(zSTRING("ctxCollection was cleared! ctxCollection size: {0}"), ctxCollection.size()).StdPrintLine();
		}


		// ctx getters

		int getCtxFxProtoID(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->fxProtoID : -1;
		}
		int getCtxID(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->id : -1;
		}
		oCNpc* getCtxDamageSender(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->damageSender : nullptr;
		}
		oCNpc* getCtxDamageReceiver(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->damageReceiver : nullptr;
		}
		int getCtxDamage(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->damage : -1;
		}
		int getCtxDamageIndex(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->damageIndex : -1;
		}
		int getCtxCurrIter(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->currIter : -1;
		}
		float getCtxLoopInterval(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->loopInterval : -1.0f;
		}
		int getCtxIterCount(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->iterCount : -1;
		}
		int getCtxSpellID(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->spellID : -1;
		}
		zSTRING getCtxStrVisualFX(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->strVisualFX : zSTRING("");
		}
		int getCtxDontKill(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->dontKill : -1;
		}
		float getCtxLastIterTime(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->lastIterTime : -1.0f;
		}

		bool getCtxIsRunning(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->isRunning : false;
		}
		bool getCtxIsApplying(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->isApplying : false;
		}
		bool getCtxIsCompleted(int fxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			return currCtx ? currCtx->isCompleted : false;
		}

		// ctx setters

		void setCtxDamage(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDamage)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newDamage >= 0) currCtx->damage = newDamage;
		}
		void setCtxDamageIndex(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, oEDamageIndex newDamageIndex)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newDamageIndex >= 0 && newDamageIndex < oEDamageIndex_MAX)
				currCtx->damageIndex = newDamageIndex;
		}
		void setCtxSpellID(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newSpellID)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newSpellID >= -1) currCtx->spellID = newSpellID;
		}
		void setCtxStrVisualFX(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, zSTRING newStrVisualFX)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newStrVisualFX) currCtx->strVisualFX = newStrVisualFX;
		}
		void setCtxDontKill(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDontKill)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newDontKill >= 0) currCtx->dontKill = newDontKill;
		}
		void setCtxLoopInterval(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, float newLoopInterval)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newLoopInterval >= 100.0f) currCtx->loopInterval = newLoopInterval;
		}
		void setCtxIterCount(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newIterCount)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newIterCount >= -1) currCtx->iterCount = newIterCount;
		}
		void setCtxExCndFuncIndex(int fxID, oCNpc* damageSender, oCNpc* damageReceiver, int newExCndFuncIndex)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);

			if (currCtx && newExCndFuncIndex >= -1) currCtx->exCndFuncIndex = newExCndFuncIndex;
		}

		// fx getters

		int getFxFxProtoID(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxFxProtoID(*outerFxID, damageSender, damageReceiver) : -1;
		}
		oCNpc* getFxDamageSender(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxDamageSender(*outerFxID, damageSender, damageReceiver) : nullptr;
		}
		oCNpc* getFxDamageReceiver(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxDamageReceiver(*outerFxID, damageSender, damageReceiver) : nullptr;
		}
		int getFxDamage(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxDamage(*outerFxID, damageSender, damageReceiver) : -1;
		}
		int getFxDamageIndex(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxDamageIndex(*outerFxID, damageSender, damageReceiver) : -1;
		}
		int getFxCurrIter(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxCurrIter(*outerFxID, damageSender, damageReceiver) : -1;
		}
		float getFxLoopInterval(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxLoopInterval(*outerFxID, damageSender, damageReceiver) : -1.0f;
		}
		int getFxIterCount(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxIterCount(*outerFxID, damageSender, damageReceiver) : -1;
		}
		int getFxSpellID(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxSpellID(*outerFxID, damageSender, damageReceiver) : -1;
		}
		zSTRING getFxStrVisualFX(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxStrVisualFX(*outerFxID, damageSender, damageReceiver) : zSTRING("");
		}
		int getFxDontKill(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxDontKill(*outerFxID, damageSender, damageReceiver) : -1;
		}
		float getFxLastIterTime(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxLastIterTime(*outerFxID, damageSender, damageReceiver) : -1.0f;
		}

		bool getFxIsRunning(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxIsRunning(*outerFxID, damageSender, damageReceiver) : false;
		}
		bool getFxIsApplying(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxIsApplying(*outerFxID, damageSender, damageReceiver) : false;
		}
		bool getFxIsCompleted(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver)
		{
			fx* currFx = getFx(*outerFxID);

			return currFx && currFx->outerID == outerFxID ? getCtxIsCompleted(*outerFxID, damageSender, damageReceiver) : false;
		}

		// fx setters

		void setFxDamage(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDamage)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxDamage(*outerFxID, damageSender, damageReceiver, newDamage);
		}
		void setFxDamageIndex(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, oEDamageIndex newDamageIndex)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxDamageIndex(*outerFxID, damageSender, damageReceiver, newDamageIndex);
		}
		void setFxSpellID(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newSpellID)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxSpellID(*outerFxID, damageSender, damageReceiver, newSpellID);
		}
		void setFxStrVisualFX(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, zSTRING newStrVisualFX)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxStrVisualFX(*outerFxID, damageSender, damageReceiver, newStrVisualFX);
		}
		void setFxDontKill(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newDontKill)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxDontKill(*outerFxID, damageSender, damageReceiver, newDontKill);
		}
		void setFxLoopInterval(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, float newLoopInterval)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxLoopInterval(*outerFxID, damageSender, damageReceiver, newLoopInterval);
		}
		void setFxIterCount(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newIterCount)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxIterCount(*outerFxID, damageSender, damageReceiver, newIterCount);
		}
		void setFxExCndFuncIndex(int* outerFxID, oCNpc* damageSender, oCNpc* damageReceiver, int newExCndFuncIndex)
		{
			fx* currFx = getFx(*outerFxID);

			if (currFx && currFx->outerID == outerFxID) setCtxExCndFuncIndex(*outerFxID, damageSender, damageReceiver, newExCndFuncIndex);
		}

	};

	static UCS_Manager ucsManager;


	// EXTERNAL FUNCTIONS /////

	int __cdecl UCS_CreateFXProto()
	{
		int* outerFxProtoID;
		int damage;
		int damageIndex;
		int spellID;
		zSTRING strVisualFX;
		int dontKill;
		float loopInterval;
		int iterCount;
		int exCndFuncIndex;

		parser->GetParameter(exCndFuncIndex);
		parser->GetParameter(iterCount);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(strVisualFX);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);
		parser->GetParameter(damage);
		outerFxProtoID = parser->PopVarAddress();

		if (outerFxProtoID != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 100.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ucsManager.addFxProto(outerFxProtoID, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);

			Union::StringANSI::Format(zSTRING("created FXProto, id: {0}, outerID: {1}"), outerFxProtoID != nullptr ? *outerFxProtoID : zSTRING("nothing"), ucsManager.getFxProto(outerFxProtoID != nullptr ? *outerFxProtoID : -1)).StdPrintLine();
		}

		return 0;
	};

	int __cdecl UCS_Hit()
	{
		int damage;
		int damageIndex;
		int spellID;
		zSTRING strVisualFX;
		int dontKill;

		parser->GetParameter(dontKill);
		parser->GetParameter(strVisualFX);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);
		parser->GetParameter(damage);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ucsManager.addCtx(-1, damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill);
		}

		return 0;
	};
	int __cdecl UCS_StartFX()
	{
		int* outerFxID;
		int* outerFxProtoID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxProtoID = parser->PopVarAddress();
		outerFxID = parser->PopVarAddress();

		Union::StringANSI::Format(zSTRING("Start FX, outerFxID: {0}, outerFxProtoID: {1}"), outerFxID != nullptr ? *outerFxID : zSTRING("nothing"), outerFxProtoID != nullptr ? *outerFxProtoID : zSTRING("nothing")).StdPrintLine();

		if (outerFxID != nullptr && outerFxProtoID != nullptr && damageSender != nullptr && damageReceiver != nullptr)
		{
			ucsManager.addFx(outerFxID);

			Union::StringANSI::Format(zSTRING("STARTFX ON NPC PAIR {0}, {1}"), damageSender->GetName(0), damageReceiver->GetName(0)).StdPrintLine();
			Union::StringANSI::Format(zSTRING("fxID has been returned with id {0}"), *outerFxID).StdPrintLine();

			ucsManager.addCtx(*outerFxID, *outerFxProtoID, damageSender, damageReceiver);

			Union::StringANSI::Format(zSTRING("fxProtoID has been returned with id {0}"), *outerFxProtoID).StdPrintLine();
		}

		return 0;
	};
	int __cdecl UCS_StartFXEX()
	{
		int* outerFxID;
		int damage;
		int damageIndex;
		int spellID;
		zSTRING strVisualFX;
		int dontKill;
		float loopInterval;
		int iterCount;
		int exCndFuncIndex;

		parser->GetParameter(exCndFuncIndex);
		parser->GetParameter(iterCount);
		parser->GetParameter(loopInterval);
		parser->GetParameter(dontKill);
		parser->GetParameter(strVisualFX);
		parser->GetParameter(spellID);
		parser->GetParameter(damageIndex);
		parser->GetParameter(damage);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 100.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			ucsManager.addFx(outerFxID);
			
			ucsManager.addCtx(*outerFxID, damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);
		}

		return 0;
	};

	int __cdecl UCS_StopFX()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int ctxID = ucsManager.getCtxID(*outerFxID, damageSender, damageReceiver);

		ucsManager.closeCtx(ctxID);

		return 0;
	};

	// external getter functions

	int __cdecl UCS_IsRunning()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int isRunning = ucsManager.getFxIsRunning(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(isRunning); return 0;
	};
	int __cdecl UCS_IsCompleted()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int isCompleted = ucsManager.getFxIsCompleted(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(isCompleted); return 0;
	};
	int __cdecl UCS_GetFxProtoID()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int fxProtoID = ucsManager.getFxFxProtoID(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(fxProtoID); return 0;
	};
	int __cdecl UCS_GetDamage()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int damage = ucsManager.getFxDamage(outerFxID, damageSender, damageReceiver);

		Union::StringANSI::Format(zSTRING("getDamage: {0}"), damage).StdPrintLine();

		parser->SetReturn(damage); return 0;
	};
	int __cdecl UCS_GetDamageIndex()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int damageIndex = ucsManager.getFxDamageIndex(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(damageIndex); return 0;
	};
	int __cdecl UCS_GetCurrentIter()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int currentIter = ucsManager.getFxCurrIter(outerFxID, damageSender, damageReceiver);

		Union::StringANSI::Format(zSTRING("EXTERNAL GET CURRENT ITER {0} OUT OF CTX WITH ID: {1}!"), currentIter, ucsManager.getCtxID(*outerFxID, damageSender, damageReceiver)).StdPrintLine();

		parser->SetReturn(currentIter); return 0;
	};
	int __cdecl UCS_GetLoopInterval()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		float loopInterval = ucsManager.getFxLoopInterval(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(loopInterval); return 0;
	};
	int __cdecl UCS_GetIterCount()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int iterCount = ucsManager.getFxIterCount(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(iterCount); return 0;
	};
	int __cdecl UCS_GetSpellID()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int spellID = ucsManager.getFxSpellID(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(spellID); return 0;
	};
	int __cdecl UCS_GetVisualFX()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		zSTRING visualFX = ucsManager.getFxStrVisualFX(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(visualFX); return 0;
	};
	int __cdecl UCS_GetDontKill()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		int dontKill = ucsManager.getFxDontKill(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(dontKill); return 0;
	};
	int __cdecl UCS_GetLastIterTime()
	{
		int* outerFxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		float lastIterTime = ucsManager.getFxLastIterTime(outerFxID, damageSender, damageReceiver);

		parser->SetReturn(lastIterTime); return 0;
	};

	// external setter functions

	int __cdecl UCS_SetDamage()
	{
		int* outerFxID; int newDamage;

		parser->GetParameter(newDamage);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) { ucsManager.setFxDamage(outerFxID, damageSender, damageReceiver, newDamage); Union::StringANSI::Format(zSTRING("EXTERNAL SET DAMAGE IN CTX WITH ID: {0} TO {1}!"), ucsManager.getCtxID(*outerFxID, damageSender, damageReceiver), newDamage).StdPrintLine(); } return 0;
	};
	int __cdecl UCS_SetDamageIndex()
	{
		int* outerFxID; int newDamageIndex;

		parser->GetParameter(newDamageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) ucsManager.setFxDamageIndex(outerFxID, damageSender, damageReceiver, (oEDamageIndex)newDamageIndex); return 0;
	};
	int __cdecl UCS_SetSpellID()
	{
		int* outerFxID; int newSpellID;

		parser->GetParameter(newSpellID);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) ucsManager.setFxSpellID(outerFxID, damageSender, damageReceiver, newSpellID); return 0;
	};
	int __cdecl UCS_SetVisualFX()
	{
		int* outerFxID; zSTRING newVisualFX;

		parser->GetParameter(newVisualFX);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) ucsManager.setFxStrVisualFX(outerFxID, damageSender, damageReceiver, newVisualFX); return 0;
	};
	int __cdecl UCS_SetDontKill()
	{
		int* outerFxID; int newDontKill;

		parser->GetParameter(newDontKill);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) ucsManager.setFxDontKill(outerFxID, damageSender, damageReceiver, newDontKill); return 0;
	};
	int __cdecl UCS_SetLoopInterval()
	{
		int* outerFxID; float newLoopInterval;

		parser->GetParameter(newLoopInterval);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) ucsManager.setFxLoopInterval(outerFxID, damageSender, damageReceiver, newLoopInterval); return 0;
	};
	int __cdecl UCS_SetIterCount()
	{
		int* outerFxID; int newIterCount;

		parser->GetParameter(newIterCount);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) ucsManager.setFxIterCount(outerFxID, damageSender, damageReceiver, newIterCount); return 0;
	};
	int __cdecl UCS_SetExitCondition()
	{
		int* outerFxID; int newExCndFuncIndex;

		parser->GetParameter(newExCndFuncIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		outerFxID = parser->PopVarAddress();

		if (ucsManager.getFxIsApplying(outerFxID, damageSender, damageReceiver)) ucsManager.setFxExCndFuncIndex(outerFxID, damageSender, damageReceiver, newExCndFuncIndex); return 0;
	};


	void Game_DefineExternals_ApplyDamage()
	{
		parser->DefineExternal("UCS_CreateFXProto", UCS_CreateFXProto, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_Hit", UCS_Hit, zPAR_TYPE_VOID, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartFX", UCS_StartFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_StartFXEX", UCS_StartFXEX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_StopFX", UCS_StopFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_IsRunning", UCS_IsRunning, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_IsCompleted", UCS_IsCompleted, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetFxProtoID", UCS_GetFxProtoID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDamage", UCS_GetDamage, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDamageIndex", UCS_GetDamageIndex, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetCurrentIter", UCS_GetCurrentIter, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetLoopInterval", UCS_GetLoopInterval, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetIterCount", UCS_GetIterCount, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetSpellID", UCS_GetSpellID, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetVisualFX", UCS_GetVisualFX, zPAR_TYPE_STRING, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetDontKill", UCS_GetDontKill, zPAR_TYPE_INT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_GetLastIterTime", UCS_GetLastIterTime, zPAR_TYPE_FLOAT, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_VOID);

		parser->DefineExternal("UCS_SetDamage", UCS_SetDamage, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetDamageIndex", UCS_SetDamageIndex, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetSpellID", UCS_SetSpellID, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetVisualFX", UCS_SetVisualFX, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_STRING, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetDontKill", UCS_SetDontKill, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetLoopInterval", UCS_SetLoopInterval, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FLOAT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetIterCount", UCS_SetIterCount, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_INT, zPAR_TYPE_VOID);
		parser->DefineExternal("UCS_SetExitCondition", UCS_SetExitCondition, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);
		
		// void parser->DefineExternal("UCS_SetExitCondition", UCS_SetExitCondition, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);
	}
}