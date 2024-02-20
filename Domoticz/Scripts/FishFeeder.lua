return {
	on = {
	    devices = {78,71}
	},
	data = {
        counter = { initial = 0 }
    },

	execute = function(domoticz, device)
		domoticz.log('Device ' .. device.name .. ' was changed', domoticz.LOG_INFO)
		if (device.idx==71) then
		    domoticz.data.counter = domoticz.data.counter + 1
		else
		    domoticz.data.counter = 0
		end
		domoticz.log('Counter ' .. domoticz.data.counter, domoticz.LOG_INFO)
		domoticz.devices(82).updatePercentage((33-domoticz.data.counter)/33*100)
	end
}