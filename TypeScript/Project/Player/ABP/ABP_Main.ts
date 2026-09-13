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
    
    UpdateCharVars(): void {
        const playerChar = this.TryGetPawnOwner() as UE.Game.Project.Gameplay.BP_PlayerChar.BP_PlayerChar_C;
        if (!playerChar) {
            return;
        }
        
        const velocity = playerChar.GetVelocity();
        
        this.Velocity = velocity;
        this.Velocity2D.X = velocity.X;
        this.Velocity2D.Y = velocity.Y;
        this.Velocity2D.Z = 0;
        this.Velocity2DFloat = velocity.Size2D();
        this.EquipType = playerChar.EquipType;
    }
    
    override BlueprintUpdateAnimation(DeltaTimeX: number): void {
        this.UpdateCharVars();
    }
}

const Mixin_TS_ABP_Main = blueprint.mixin(ABP_Main, TS_ABP_Main, {
    objectTakeByNative: true,
    noMixinedWarning: true,
});
