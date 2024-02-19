return {
	on = {
		devices = {
			'Távirányító Gomb*'
		}
	},
	execute = function(domoticz, device)
		domoticz.log('Device ' .. device.name .. ' was changed', domoticz.LOG_INFO)

		if (domoticz.devices('Setup ' .. device.name).state=="Off") then 
	        if (device.state=='On') then
	            device.switchOff().afterSec(1)
	        end;
	    end
	end
}