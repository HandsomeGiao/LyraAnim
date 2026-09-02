import * as UE from "ue";
import {blueprint} from "puerts";

const ucls = UE.Class.Load("/Game/Project/Gameplay/BP_PlayerChar.BP_PlayerChar_C");

const BP_PlayerChar = blueprint.tojs<typeof UE.Game.Project.Gameplay.BP_PlayerChar.BP_PlayerChar_C>(ucls);

interface TS_BP_PlayerCharPlaceHold extends UE.Game.Project.Gameplay.BP_PlayerChar.BP_PlayerChar_C {
}

class TS_BP_PlayerCharPlaceHold {
}

Object.setPrototypeOf(TS_BP_PlayerCharPlaceHold.prototype, BP_PlayerChar.prototype);

export class TS_BP_PlayerChar extends TS_BP_PlayerCharPlaceHold {
}

const Mixin_TS_BP_PlayerChar = blueprint.mixin(BP_PlayerChar, TS_BP_PlayerChar, {
    objectTakeByNative: true,
    noMixinedWarning: true,
});
