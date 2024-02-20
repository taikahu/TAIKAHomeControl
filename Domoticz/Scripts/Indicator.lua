return {
	on = {
		devices = {227,228,229}
	},
	execute = function(domoticz, device)
		domoticz.log('Aktív objeltum ' .. device.name .. ' was changed', domoticz.LOG_INFO)
		if (domoticz.devices(233).state=='On') then
		    if (device.state=='On') then
	            domoticz.devices(286).switchOn()
	        end;
	    end;
	end
}