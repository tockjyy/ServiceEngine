local gate = require("MgrGate")
local timer = require("cpp_timer")

timer.NewTimer("GateBeat",0,30000)

function GateBeat( ... )
	gate.BeatTimer()
	return true
end