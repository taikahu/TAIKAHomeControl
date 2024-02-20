return {
	on = {
		devices = {148}
	},
	execute = function(domoticz, device)
		domoticz.log('Aktív objeltum ' .. device.name .. ' was changed', domoticz.LOG_INFO)
		if (device.state=='On') then
	            domoticz.devices(286).switchOn()
	            domoticz.devices(230).switchOn()
	            domoticz.devices(232).switchOn()
	   end;
	end
}