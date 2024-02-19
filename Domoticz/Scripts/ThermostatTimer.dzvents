return {
	on = {
		devices = {
			'Nappali/Éjszakai időzítő'
		}
	},
	execute = function(domoticz, device)
	    domoticz.log('Device ' .. domoticz.devices(37).name .. ' :' .. domoticz.devices(37).level , domoticz.LOG_INFO)
	    if domoticz.devices(37).level==10 or domoticz.devices(37).level==20 then
	        if device.state=="On" then
	            domoticz.devices(37).switchSelector(10)
	        else
	            domoticz.devices(37).switchSelector(20)
	        end 
	    end
		domoticz.log('Device ' .. device.name .. ' was changed', domoticz.LOG_INFO)
	end
}