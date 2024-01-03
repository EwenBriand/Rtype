HomingMissile = HomingMissile or {}

function HomingMissile:Update(id)
    print("HomingMissile:update")
end

function HomingMissile:Start()
    print("HomingMissile:start")
end

function HomingMissile:OnAddComponent(int)
    print("HomingMissile:onAddComponent", int)
end

function HomingMissile.new()
    local self = setmetatable({}, {__index = HomingMissile})
    return self
end