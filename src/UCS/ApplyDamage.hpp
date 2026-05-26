
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
				currFxProto->loopInterval < 250.0f ||
				currFxProto->iterCount < 0 && currFxProto->exCndFuncIndex < 0)
				return false;

			return true;
		}

		int addFxProto
		(
			int outerFxProtoID,
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
			fxProto* currFxProto = getFxProto(outerFxProtoID);

			if (currFxProto && isFxProtoValid(currFxProto))
				return outerFxProtoID;


			fxProto newFxProto{};

			newFxProto.id = nextFxProtoID++;

			newFxProto.damage = damage;
			newFxProto.damageIndex = damageIndex;
			newFxProto.spellID = spellID;
			newFxProto.strVisualFX = strVisualFX;
			newFxProto.dontKill = dontKill;
			newFxProto.loopInterval = loopInterval;
			newFxProto.iterCount = iterCount;
			newFxProto.exCndFuncIndex = exCndFuncIndex;

			fxProtoCollection[newFxProto.id] = newFxProto;

			return newFxProto.id;
		}


		fx* getFx(int fxID)
		{
			auto it = fxCollection.find(fxID);

			if (it == fxCollection.end()) return nullptr;

			return &it->second;
		}

		int addFx
		(
			int outerFxID
		)
		{
			fx* currFx = getFx(outerFxID);

			if (currFx)
				return outerFxID;


			fx newFx{};

			newFx.id = nextFxID++;

			fxCollection[newFx.id] = newFx;

			return newFx.id;
		}

		bool isFxValid(fx* currFx)
		{
			if (currFx == nullptr ||
				currFx->id < 0)
				return false;

			return true;
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

		int addCtx
		(
			int fxID,
			int fxProtoID,
			oCNpc* damageSender,
			oCNpc* damageReceiver
		)
		{
			ctx* currCtx = getCtx(fxID, damageSender, damageReceiver);
			fxProto* currFxProto = getFxProto(fxProtoID);

			if (currCtx && currCtx->isRunning)
				return -1;
			if (!isFxProtoValid(currFxProto))
				return -1;


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

			return newCtx.id;
		}
		int addCtx
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
				return -1;


			ctx newCtx{};

			newCtx.id = nextCtxID++;
			newCtx.type = (loopInterval >= 250.0f) && (iterCount >= 0 || exCndFuncIndex >= 0) ? CTX_LOOP : CTX_REGULAR;
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

			return newCtx.id;
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
				if (currCtx->loopInterval < 250.0f ||
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

					Union::StringANSI::Format(zSTRING("IS EXIT FUNC TRUE? Well: {0}"), (bool)isFuncTrue).StdPrintLine();

					if (isFuncTrue)
					{
						closeCtx(*it);
						it = ctxQueue.erase(it); continue;
					}
				}
				//Union::StringANSI::Format(zSTRING("ctxQueue | id: {0}, type: {1}, isRn: {2}, isCmp: {3}, dmg: {4} currIter: {5}, iterCount: {6}, exCndFuncIdx: {7}, "), currCtx->id, currCtx->type, getCtxIsRunning(currCtx->id), getCtxIsCompleted(currCtx->id), getCtxDamage(currCtx->id), currCtx->currIter, currCtx->iterCount, currCtx->exCndFuncIndex).StdPrintLine();


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
					Union::StringANSI::Format(zSTRING("Sender here is {0} AND damageReceiverAdr is {1}"), currCtx->damageSender->GetName(0), (int)currCtx->damageReceiver).StdPrintLine();

					// damage applying
					currCtx->isApplying = true;
					Union::StringANSI::Format(zSTRING("filedsValid: {0}, dd.aryDamage {1} AND dd.fDamageTotal {2} AND damageType {3}"), dd.dwFieldsValid, dd.aryDamage[dd.enuModeDamage], dd.fDamageTotal, dd.enuModeDamage).StdPrintLine();
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


		//getters

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


		//setters

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

			if (currCtx && newLoopInterval >= 250.0f) currCtx->loopInterval = newLoopInterval;
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

		if (damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 250.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int fxProtoID = ucsManager.addFxProto(*outerFxProtoID, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);

			*outerFxProtoID = fxProtoID;
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

			int ctxID = ucsManager.addCtx(-1, damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill);

			ucsManager.runCtx(ctxID);
		}

		return 0;
	};
	int __cdecl UCS_StartFX()
	{
		int* outerFxID;
		int fxProtoID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxProtoID);
		outerFxID = parser->PopVarAddress();

		if (damageSender != nullptr && damageReceiver != nullptr && fxProtoID >= 0)
		{
			int fxID = ucsManager.addFx(*outerFxID);

			int ctxID = ucsManager.addCtx(fxID, fxProtoID, damageSender, damageReceiver);

			ucsManager.runCtx(ctxID);

			*outerFxID = fxID;
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

		if (damageSender != nullptr && damageReceiver != nullptr && damage >= 0 && damageIndex >= 0 && spellID >= -1 && strVisualFX && dontKill >= 0 && loopInterval >= 250.0f && iterCount >= -1 && exCndFuncIndex >= -1)
		{
			oEDamageIndex resultDamageIndex = damageIndex < oEDamageIndex_MAX ? (oEDamageIndex)damageIndex : (oEDamageIndex)0;

			int fxID = ucsManager.addFx(*outerFxID);
			
			int ctxID = ucsManager.addCtx(fxID, damageSender, damageReceiver, damage, resultDamageIndex, spellID, strVisualFX, dontKill, loopInterval, iterCount, exCndFuncIndex);

			ucsManager.runCtx(ctxID);

			*outerFxID = fxID;
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
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int isRunning = ucsManager.getCtxIsRunning(fxID, damageSender, damageReceiver);

		parser->SetReturn(isRunning); return 0;
	};
	int __cdecl UCS_IsCompleted()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int isCompleted = ucsManager.getCtxIsCompleted(fxID, damageSender, damageReceiver);

		parser->SetReturn(isCompleted); return 0;
	};
	int __cdecl UCS_GetFxProtoID()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int fxProtoID = ucsManager.getCtxFxProtoID(fxID, damageSender, damageReceiver);

		parser->SetReturn(fxProtoID); return 0;
	};
	int __cdecl UCS_GetDamage()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int damage = ucsManager.getCtxDamage(fxID, damageSender, damageReceiver);

		Union::StringANSI::Format(zSTRING("getDamage: {0}"), damage).StdPrintLine();

		parser->SetReturn(damage); return 0;
	};
	int __cdecl UCS_GetDamageIndex()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int damageIndex = ucsManager.getCtxDamageIndex(fxID, damageSender, damageReceiver);

		parser->SetReturn(damageIndex); return 0;
	};
	int __cdecl UCS_GetCurrentIter()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int currentIter = ucsManager.getCtxCurrIter(fxID, damageSender, damageReceiver);

		Union::StringANSI::Format(zSTRING("EXTERNAL GET CURRENT ITER {0} OUT OF CTX WITH ID: {1}!"), currentIter, ucsManager.getCtxID(fxID, damageSender, damageReceiver)).StdPrintLine();

		parser->SetReturn(currentIter); return 0;
	};
	int __cdecl UCS_GetLoopInterval()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		float loopInterval = ucsManager.getCtxLoopInterval(fxID, damageSender, damageReceiver);

		parser->SetReturn(loopInterval); return 0;
	};
	int __cdecl UCS_GetIterCount()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int iterCount = ucsManager.getCtxIterCount(fxID, damageSender, damageReceiver);

		parser->SetReturn(iterCount); return 0;
	};
	int __cdecl UCS_GetSpellID()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int spellID = ucsManager.getCtxSpellID(fxID, damageSender, damageReceiver);

		parser->SetReturn(spellID); return 0;
	};
	int __cdecl UCS_GetVisualFX()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		zSTRING visualFX = ucsManager.getCtxStrVisualFX(fxID, damageSender, damageReceiver);

		parser->SetReturn(visualFX); return 0;
	};
	int __cdecl UCS_GetDontKill()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		int dontKill = ucsManager.getCtxDontKill(fxID, damageSender, damageReceiver);

		parser->SetReturn(dontKill); return 0;
	};
	int __cdecl UCS_GetLastIterTime()
	{
		int fxID;

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		float lastIterTime = ucsManager.getCtxLastIterTime(fxID, damageSender, damageReceiver);

		parser->SetReturn(lastIterTime); return 0;
	};

	// external setter functions

	int __cdecl UCS_SetDamage()
	{
		int fxID; int newDamage;

		parser->GetParameter(newDamage);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) { ucsManager.setCtxDamage(fxID, damageSender, damageReceiver, newDamage); Union::StringANSI::Format(zSTRING("EXTERNAL SET DAMAGE IN CTX WITH ID: {0} TO {1}!"), ucsManager.getCtxID(fxID, damageSender, damageReceiver), newDamage).StdPrintLine(); } return 0;
	};
	int __cdecl UCS_SetDamageIndex()
	{
		int fxID; int newDamageIndex;

		parser->GetParameter(newDamageIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) ucsManager.setCtxDamageIndex(fxID, damageSender, damageReceiver, (oEDamageIndex)newDamageIndex); return 0;
	};
	int __cdecl UCS_SetSpellID()
	{
		int fxID; int newSpellID;

		parser->GetParameter(newSpellID);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) ucsManager.setCtxSpellID(fxID, damageSender, damageReceiver, newSpellID); return 0;
	};
	int __cdecl UCS_SetVisualFX()
	{
		int fxID; zSTRING newVisualFX;

		parser->GetParameter(newVisualFX);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) ucsManager.setCtxStrVisualFX(fxID, damageSender, damageReceiver, newVisualFX); return 0;
	};
	int __cdecl UCS_SetDontKill()
	{
		int fxID; int newDontKill;

		parser->GetParameter(newDontKill);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) ucsManager.setCtxDontKill(fxID, damageSender, damageReceiver, newDontKill); return 0;
	};
	int __cdecl UCS_SetLoopInterval()
	{
		int fxID; float newLoopInterval;

		parser->GetParameter(newLoopInterval);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) ucsManager.setCtxLoopInterval(fxID, damageSender, damageReceiver, newLoopInterval); return 0;
	};
	int __cdecl UCS_SetIterCount()
	{
		int fxID; int newIterCount;

		parser->GetParameter(newIterCount);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) ucsManager.setCtxIterCount(fxID, damageSender, damageReceiver, newIterCount); return 0;
	};
	int __cdecl UCS_SetExitCondition()
	{
		int fxID; int newExCndFuncIndex;

		parser->GetParameter(newExCndFuncIndex);

		oCNpc* damageReceiver = (oCNpc*)(parser->GetInstance());
		oCNpc* damageSender = (oCNpc*)(parser->GetInstance());

		parser->GetParameter(fxID);

		if (ucsManager.getCtxIsApplying(fxID, damageSender, damageReceiver)) ucsManager.setCtxExCndFuncIndex(fxID, damageSender, damageReceiver, newExCndFuncIndex); return 0;
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
		
		//parser->DefineExternal("UCS_SetExitCondition", UCS_SetExitCondition, zPAR_TYPE_VOID, zPAR_TYPE_INT, zPAR_TYPE_INSTANCE, zPAR_TYPE_INSTANCE, zPAR_TYPE_FUNC, zPAR_TYPE_VOID);
	}
}