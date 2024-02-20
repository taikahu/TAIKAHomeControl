return {
	on = { 
		devices = { 'Nappali - Hőmérséklet / Páratartalom'} , 
        	timer = { 'every minute' },      
        	httpResponses = { 'setBatterylevel' }
    },
	
    execute = function(domoticz, trigger)

	
	if trigger.isHTTPResponse then
		if (trigger.ok) then -- statusCode == 2xx
               		--domoticz.log('Response: ' ..  trigger.statusText, domoticz.LOG_ERROR)
               		local batteryLevel=trigger.json["result"][1]["BatteryLevel"]
               		local batteryLevelOld=domoticz.devices(201).sValue
               		domoticz.log('Result: ' ..  batteryLevel, domoticz.LOG_INFO)
               		domoticz.log('Value: ' ..  batteryLevelOld, domoticz.LOG_INFO)
               		
               		local message = 'Device Nappali - Hőmérséklet / Páratartalom is low on batteries (' .. batteryLevel .. ')'
                    if not (batteryLevelOld==batteryLevel) then
                        domoticz.devices(201).updateCustomSensor(batteryLevel)
                        
                        if (batteryLevel<10) then
                            domoticz.devices(201).setIcon(146) -- emty
                        end
                        if (batteryLevel>20) then
                            domoticz.devices(201).setIcon(144) -- low
                        end
                        if (batteryLevel>50) then
                            domoticz.devices(201).setIcon(143) -- ok
                        end
                        if (batteryLevel>80) then
                            domoticz.devices(201).setIcon(145) -- full
                        end
                end
                
		            --if (batteryLevel<80) then
			        --    domoticz.notify('Low battery warning', message, domoticz.PRIORITY_NORMAL)
			        --    domoticz.log('Low battery warning: ' .. message, domoticz.LOG_INFO)
		            --end
         	end
	else
        	local myUrl           = "http://192.168.0.200:8080/json.htm?type=devices&rid=283"
              
               domoticz.openURL({
                	url = myUrl,
                    callback = "setBatterylevel"
            	})
        end
    end
}