---
name: puerts-ts-rules
description: 在当前 UE5 项目中编写、修改或审查 puerts TypeScript 代码时使用，尤其适用于 Blueprint Mixin 模式、TS 文件路径映射、启动脚本 import、UE 类型绑定和逻辑变量约束。
---

# puerts TypeScript 规范

## 基本流程

在编写或修改 puerts TypeScript 代码前，先确认目标 Blueprint 路径、对应 TS 文件路径、启动脚本 import 位置，以及现有代码风格。

优先沿用项目现有目录、命名、import 顺序、类型写法和生命周期函数组织方式。

如果遇到了复杂的问题，你可以去项目的puerts插件中查看源代码。

## Mixin 模式

### 固定的分层收集

父、子 Blueprint 的每一层 `blueprint.mixin(...)` 都必须独立注册；父层已注册不能代替子层注册。

```typescript
const Mixin_TS_BP_TestTSActor = blueprint.mixin(BP_TestTSActor, TS_BP_TestTSActor, {
    objectTakeByNative: true,
    noMixinedWarning: true,
});
```

### 基础 Blueprint 的写法

没有 TS Mixin 父类时，使用占位类连接 Blueprint wrapper prototype，再让实际 TS 类继承占位类：

```typescript
import * as UE from "ue";
import { blueprint } from "puerts";

const ucls = UE.Class.Load("/Game/Project/Test/BP_TestTSActor.BP_TestTSActor_C");

const BP_TestTSActor = blueprint.tojs<typeof UE.Game.Project.Test.BP_TestTSActor.BP_TestTSActor_C>(ucls);

interface TS_BP_TestTSActorPlaceHold extends UE.Game.Project.Test.BP_TestTSActor.BP_TestTSActor_C {}

class TS_BP_TestTSActorPlaceHold {}
Object.setPrototypeOf(TS_BP_TestTSActorPlaceHold.prototype, BP_TestTSActor.prototype);

export class TS_BP_TestTSActor extends TS_BP_TestTSActorPlaceHold {
    // 在此实现 TS 逻辑
}

const Mixin_TS_BP_TestTSActor = blueprint.mixin(BP_TestTSActor, TS_BP_TestTSActor, {
    objectTakeByNative: true,
    noMixinedWarning: true,
});
```

将 `BP_TestTSActor`、`TS_BP_TestTSActor`、`TS_BP_TestTSActorPlaceHold`、`Mixin_TS_BP_TestTSActor` 和 `UE.Game.Project.Test.BP_TestTSActor.BP_TestTSActor_C` 替换为目标 Blueprint 对应名称与类型路径。

保持 `blueprint.tojs<typeof ...>(ucls)` 的类型绑定，不要去掉 `typeof`、`ucls` 或 `blueprint.mixin`。

如果该类会被其他 TS Mixin 继承，必须 `export` 实际 TS 类。

### 子 Blueprint 继承父 TS Mixin 的写法

当 Blueprint 与 TS 同时存在父子关系时，两条继承链必须一一对应：

```text
BP_TestTSActor_Child_C extends BP_TestTSActor_C
TS_BP_TestTSActor_Child extends TS_BP_TestTSActor
```

子类文件导入父 TS 类；同名 `interface` 继承子 Blueprint 生成类以补入 UE 类型，同名 `class` 继承父 TS 类以建立 TS 行为继承：

```typescript
import * as UE from "ue";
import { blueprint } from "puerts";
import { TS_BP_TestTSActor } from "./BP_TestTSActor";

const ucls = UE.Class.Load("/Game/Project/Test/BP_TestTSActor_Child.BP_TestTSActor_Child_C");

const BP_TestTSActor_Child = blueprint.tojs<typeof UE.Game.Project.Test.BP_TestTSActor_Child.BP_TestTSActor_Child_C>(ucls);

export interface TS_BP_TestTSActor_Child extends UE.Game.Project.Test.BP_TestTSActor_Child.BP_TestTSActor_Child_C {}

export class TS_BP_TestTSActor_Child extends TS_BP_TestTSActor {
    override ReceiveBeginPlay(): void {
        super.ReceiveBeginPlay();
    }
}

const Mixin_TS_BP_TestTSActor_Child = blueprint.mixin(BP_TestTSActor_Child, TS_BP_TestTSActor_Child, {
    objectTakeByNative: true,
    noMixinedWarning: true,
});
```

每增加一级 Blueprint 子类，就重复子类模式：导入直接父级 TS 类、让同名接口继承当前 Blueprint 生成类、让同名 TS 类继承直接父级 TS 类，并为当前 Blueprint 独立调用一次 `blueprint.mixin(...)`。

`super.Foo()` 调用父级 TS 实现；只在业务逻辑确实需要继续执行父级 TS 行为时调用。

## 变量约束

Mixin模式下,TS中无法实现可复制的变量或者是RPC函数,这一类的变量和函数必须在C++或者蓝图层面实现.

在TS代码中声明类成员变量时,在声明时必须使用declare关键字,且在声明时不赋值,初始化操作必须放在UE声明周期函数中,例如一个Actor的BeginPlay函数,一个GameplayAbility的ActivateAbility函数等等.

## 类型获取

在TS中,如果要获取一个C++类型的UClass,可以使用其StaticClass函数;如果要获取一个蓝图类型的UClass,则必须使用`UE.Class.Load`函数;

## 路径映射

TS 文件保存路径必须和 Blueprint 路径对应。

例如 Blueprint 路径为 `/Game/Project/Test/BP_TestTSActor` 时，TS 文件路径必须为：

```text
TypeScript\Project\Test\BP_TestTSActor.ts
```

新增 Mixin 文件后，必须在 puerts 启动脚本`TypeScript/GameStartup.ts`中 import 该文件。

## 实现约束

1. 保持类型显式且贴近 UE 暴露类型。避免使用 `any`.
2. 不要改变 Blueprint 路径、资源名、生成类后缀 `_C` 或启动脚本加载顺序，除非用户明确要求。
3. 如果 C++ 或蓝图层面缺少必要的函数、变量或类型信息（例如可复制变量、RPC、蓝图资产配置变量或未暴露接口），先修改 C++ 或蓝图，再使用仓库当前可用的 Puerts 类型生成流程更新 `Typing/`。若仓库没有自动生成脚本，则同步修改类型源和当前生成声明，并用 `tsc --noEmit` 验证；不要引用不存在的生成脚本。
4. 写完TS后，不要主动编译为JS，除非用户明确要求。
5. 除了Mixin的蓝图类,在TS代码中,不应该使用路径硬编码其他的蓝图或者资源,因为这种做会让UE Editor丢失蓝图的引用关系,正确的做法是在Mixin的蓝图中添加一个成员变量指向需要的资源.
