--
-- DESCRIPTION
--
-- @COMPANY **
-- @AUTHOR **
-- @DATE ${date} ${time}
--
---@type ABP_Layer_C
local M = UnLua.Class()

-- function M:Initialize(Initializer)
-- end

-- function M:BlueprintInitializeAnimation()
-- end

-- function M:BlueprintBeginPlay()
-- end

-- function M:BlueprintUpdateAnimation(DeltaTimeX)
-- end

-- function M:BlueprintPostEvaluateAnimation()
-- end

---@param Context FAnimUpdateContext
---@param Node FAnimNodeReference
function M:IdleOnUpdate(Context, Node)
    ---@type EAnimNodeReferenceConversionResult
    local result;
    local Sq = UE.USequencePlayerLibrary.ConvertToSequencePlayer(Node, result);

    if result == UE.EAnimNodeReferenceConversionResult.Succeeded then
      
    else
      error("Node Convert Failed!");
    end
end

return M
