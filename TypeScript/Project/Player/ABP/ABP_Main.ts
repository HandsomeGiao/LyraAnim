import * as UE from "ue";
import { $ref, $unref, blueprint } from "puerts";

const ucls = UE.Class.Load("/Game/Project/Player/ABP/ABP_Main.ABP_Main_C");

const ABP_Main = blueprint.tojs<typeof UE.Game.Project.Player.ABP.ABP_Main.ABP_Main_C>(ucls);

interface TS_ABP_MainPlaceHold extends UE.Game.Project.Player.ABP.ABP_Main.ABP_Main_C {}

class TS_ABP_MainPlaceHold {}

Object.setPrototypeOf(TS_ABP_MainPlaceHold.prototype, ABP_Main.prototype);

export class TS_ABP_Main extends TS_ABP_MainPlaceHold {
    declare private LastActorYaw: number;
    declare private ActorYaw: number;
    declare private LastActorLoc: UE.Vector;

    override BlueprintInitializeAnimation(): void {
        console.log("TS_ABP_MainPlaceHold");
    }

    CalculateLocomotionDirection(
        vector2D: UE.Vector,
        locomotionAngle: number,
    ): UE.Game.Project.Enums.E_VelocityLocomotionDirection.E_VelocityLocomotionDirection {
        const locomotionDirection = UE.Game.Project.Enums.E_VelocityLocomotionDirection.E_VelocityLocomotionDirection;

        if (vector2D.IsNearlyZero()) {
            return locomotionDirection.None;
        }

        if (locomotionAngle >= this.ForwardMin && locomotionAngle <= this.ForwardMax) {
            return locomotionDirection.Forward;
        }

        if (locomotionAngle <= this.BackwardMin || locomotionAngle >= this.BackwardMax) {
            return locomotionDirection.Backward;
        }

        return locomotionAngle < this.ForwardMin ? locomotionDirection.Left : locomotionDirection.Right;
    }

    override BlueprintUpdateAnimation(DeltaTimeX: number): void {
        const playerChar = this.TryGetPawnOwner();
        if (!playerChar) {
            return;
        }

        const playerCharInterface = playerChar as unknown as UE.Game.Project.Interface.BPI_PlayerChar.BPI_PlayerChar_C;
        if (typeof playerCharInterface.GetbAim !== "function" || typeof playerCharInterface.GetEquipType !== "function") {
            return;
        }

        const actorRotation = playerChar.K2_GetActorRotation();
        const velocity = playerChar.GetVelocity();

        this.Velocity = velocity;
        this.Velocity2D.X = velocity.X;
        this.Velocity2D.Y = velocity.Y;
        this.Velocity2D.Z = 0;
        this.Velocity2DFloat = velocity.Size2D();
        this.VelocityLocomotionAngle = UE.KismetAnimationLibrary.CalculateDirection(this.Velocity2D, actorRotation);
        this.LastVelocityLocomotionDir =
            this.VelocityLocomotionDir ?? UE.Game.Project.Enums.E_VelocityLocomotionDirection.E_VelocityLocomotionDirection.None;
        this.VelocityLocomotionDir = this.CalculateLocomotionDirection(this.Velocity2D, this.VelocityLocomotionAngle);

        const bAim = $ref(false);
        playerCharInterface.GetbAim(bAim);
        this.bAim = $unref(bAim);

        const equipType = $ref<UE.Game.Project.Enums.E_PlayerEquipType.E_PlayerEquipType>();
        playerCharInterface.GetEquipType(equipType);
        this.EquipType = $unref(equipType);

        this.LastActorYaw = this.ActorYaw;
        this.ActorYaw = actorRotation.Yaw;

        this.DeltaActorYaw = this.ActorYaw - this.LastActorYaw;
        this.LeanAngle = UE.KismetMathLibrary.Clamp(this.DeltaActorYaw / (DeltaTimeX == 0 ? 1 : DeltaTimeX) / 5.0, -90, 90);
        if (this.VelocityLocomotionDir == UE.Game.Project.Enums.E_VelocityLocomotionDirection.E_VelocityLocomotionDirection.Backward) {
            this.LeanAngle *= -1;
        }

        this.Acceleration = (playerChar.GetMovementComponent() as UE.CharacterMovementComponent).Acceleration;
        this.LastAcceleration2D = this.Acceleration2D ?? this.Acceleration;
        this.Acceleration2D = new UE.Vector(this.Acceleration.X, this.Acceleration.Y, 0);
        this.Acceleration2DFloat = UE.KismetMathLibrary.VSize(this.Acceleration2D);
        const accelerationLocomotionAngle = UE.KismetAnimationLibrary.CalculateDirection(this.Acceleration2D, actorRotation);
        this.AccelerationDir = this.CalculateLocomotionDirection(this.Acceleration2D, accelerationLocomotionAngle);

        if (this.LastActorLoc) {
            const ActorLoc = playerChar.K2_GetActorLocation();
            this.DeltaLocationFloat = UE.KismetMathLibrary.VSize(
                new UE.Vector(ActorLoc.X - this.LastActorLoc.X, ActorLoc.Y - this.LastActorLoc.Y, ActorLoc.Z - this.LastActorLoc.Z),
            );
            this.LastActorLoc = ActorLoc;
        } else {
            // 第一帧初始化为0即可.
            this.LastActorLoc = playerChar.K2_GetActorLocation();
            this.DeltaLocationFloat = 0;
        }
    }
}

const Mixin_TS_ABP_Main = blueprint.mixin(ABP_Main, TS_ABP_Main, {
    objectTakeByNative: true,
    noMixinedWarning: true,
});
