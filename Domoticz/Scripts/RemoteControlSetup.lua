return {
	on = {
		devices = {
			'Setup Távirányító Gomb*'
		}
	},
	execute = function(domoticz, device)
		domoticz.log('Device ' .. device.name .. ' was changed', domoticz.LOG_INFO)
		local bA='N'
		if (domoticz.devices('Setup Távirányító Gomb D').bState) then bA='V' end
		local bB='N'
		if (domoticz.devices('Setup Távirányító Gomb C').bState) then bB='V' end
		local bC='N'
		if (domoticz.devices('Setup Távirányító Gomb B').bState) then bC='V' end
		local bD='N'
		if (domoticz.devices('Setup Távirányító Gomb A').bState) then bD='V' end
		domoticz.devices('Távirányító Setup').updateText(bA .. bB .. bC .. bD)
	end
}