-- adapter.lua
-- Bridge between IBCS introspection protocol and Lua-side logic

local adapter = {}

-- Called when an introspection command arrives
function adapter.handle_request(req)
    -- req = { session_id = "...", payload = "..." }

    if req.payload == "ping" then
        return {
            success = true,
            result = "pong",
            error = ""
        }
    end

    if req.payload == "introspect_state" then
        return {
            success = true,
            result = "STATE:OK",
            error = ""
        }
    end

    -- Execute custom mapping
    if adapter.map and adapter.map[req.payload] then
        return adapter.map[req.payload]()
    end

    return {
        success = false,
        result = "",
        error = "Unknown command: " .. req.payload
    }
end

return adapter
