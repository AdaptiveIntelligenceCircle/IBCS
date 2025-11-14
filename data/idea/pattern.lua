function Pattern_reflective_compilation(context)
    if context.confidence > 0.7 then 
        return "COMPILE"
    else 
        return "WAIT"
    end 
end

-- pattern : rollback safety ---// 
function Pattern_rollback_safety(context)
    if context.error_detected == true then 
        return "ROLLBACK"
    else 
        return "CONTINUE"
    end 
end 

-- dispatcher --- // 
function Apply_pattern(pattern_name, context)
    if pattern_name == "Reflection_complitation" then 
        return Pattern_reflective_compilation(context)
    elseif pattern_name == "rollback safety" then
        return Pattern_reflective_compilation(context)
    else
        return "UNKNOWN"
    end 
end 



