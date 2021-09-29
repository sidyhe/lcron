local _M = {}
local core = require("lcronc")

local os = os
local error = error
local pairs = pairs
local table = table

local items = {}

_M.add = function(name, expr, callback)
    if items[name] then
        error("cron already exist: " .. name)
    end

    local obj = core.expr(expr)
    local item = {
        expr = obj,
        name = name,
        next = obj:next(os.time()),
        callback = callback
    }

    items[name] = item
end

_M.update = function()
    local now = os.time()
    -- ref all to avoid delete in callback
    local refs = {}

    for _, v in pairs(items) do
        table.insert(refs, v)
    end

    for i, v in ipairs(refs) do
        if now >= v.next then
            v.next = v.expr:next(now)

            v.callback()
        end
    end
end

_M.delete = function(name)
    items[name] = nil
end

return _M
