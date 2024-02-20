return {
	on = {
		devices = {
			'*Hőmérséklet / Páratartalom'
		}
	},
	execute = function(domoticz, device)
		domoticz.log('Device ' .. device.name .. ' was changed', domoticz.LOG_INFO)
		local hom=device.temperature
		domoticz.log('Hömérséklet:' .. hom, domoticz.LOG_INFO)
		local sensor=domoticz.utils.stringSplit(device.name,' - ')
		domoticz.log(sensor[1])
		domoticz.devices(sensor[1] .. ' termosztát').updateTemperature(domoticz.utils.round(hom,1))
	end
}