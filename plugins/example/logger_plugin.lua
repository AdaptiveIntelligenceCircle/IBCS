local M = {}

function M.info()
    return {
        name = "builtins.logger", 
        version = "1.0.0", 
        author = "IBCS" 
    }
end 

function M.on_load()
    print("Logger - loaded")
    return true 
end 

function M.on_unload()
    print("logger  - unloaded")
    return true 
end

function M.capabilities()
    print("Logger - capabilities", "trace emit")
    return true 
end

-- optional : exported API for core to call 
function M.emit(severity, msg)
    print(string.format("Plugin [%s] %s", severity, msg))
end 

Plugin = M 
return Plugin 

