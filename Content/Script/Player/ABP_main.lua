--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
---@type ABP_Main_C
local M = UnLua.Class("")

-- function M:Initialize(Initializer)
-- end

-- function M:BlueprintInitializeAnimation()
-- end

function M:BlueprintBeginPlay()
    print("begin play in ABP");
end

-- function M:BlueprintUpdateAnimation(DeltaTimeX)
-- end

-- function M:BlueprintPostEvaluateAnimation()
-- end

return M
