import * as UE from "ue";
import {blueprint} from "puerts";

const ucls = UE.Class.Load("/Game/Project/Player/ABP/ABP_Main.ABP_Main_C");

const ABP_Main = blueprint.tojs<typeof UE.Game.Project.Player.ABP.ABP_Main.ABP_Main_C>(ucls);

interface TS_ABP_MainPlaceHold extends UE.Game.Project.Player.ABP.ABP_Main.ABP_Main_C {
}

class TS_ABP_MainPlaceHold {
}

Object.setPrototypeOf(TS_ABP_MainPlaceHold.prototype, ABP_Main.prototype);

export class TS_ABP_Main extends TS_ABP_MainPlaceHold {
    override BlueprintInitializeAnimation(): void {
        console.log("TS_ABP_MainPlaceHold");
    }
}

const Mixin_TS_ABP_Main = blueprint.mixin(ABP_Main, TS_ABP_Main, {
    objectTakeByNative: true,
    noMixinedWarning: true,
});
