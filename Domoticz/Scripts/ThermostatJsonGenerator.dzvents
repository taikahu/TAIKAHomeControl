return {
	on = {
		timer = {
			'every minute'
	   },
	    devices =  {34,57,38,29,40,58,43,42,41,59,44,45,46,60,49,48,47,61,55,54,53,62,50,51,52,63,37,81}
    },
	execute = function(domoticz, timer)
	    local myFile        = "www/thermostat.json" 
	    local myData = {}, {}  
	    local sortField     = "idx"  
	    local myCollection = domoticz.utils.fromJSON("{" .. domoticz.variables("ThermostatJsonCollection1").value .. domoticz.variables("ThermostatJsonCollection2").value .. "}")

        local function sortTable(t,validSet)
            if validSet[sortField] then
                if sortDirection ~= nil and sortDirection:sub(1,3):lower() == "des" then
                    table.sort(t, function(a,b) return a[sortField] > b[sortField] end)  -- as advised by @devros
                else
                    table.sort(t, function(a,b) return a[sortField] < b[sortField] end)
                end    
            else
                 domoticz.log("Warning !!  " .. sortField .. " is not a valid sortfield. JSON is still unsorted",domoticz.LOG_ERROR) 
            end
        end 
        
        for attribute in pairs(myCollection) do                            -- loop trought the informationTypes 
	        table.insert(myData,{ idx     = myCollection[attribute][1]
                                ,name    = attribute
                                ,temp    = domoticz.utils.round(domoticz.devices(myCollection[attribute][2]).temperature,3)
                                ,status  = domoticz.devices(myCollection[attribute][3]).state
                                ,dtemp   = domoticz.utils.round(domoticz.devices(myCollection[attribute][4]).setPoint,3)
                                ,didx    = myCollection[attribute][4]
                                ,ntemp   = domoticz.utils.round(domoticz.devices(myCollection[attribute][5]).setPoint,3)
                                ,nidx    = myCollection[attribute][5]
                                ,otemp   = domoticz.utils.round(domoticz.devices(myCollection[attribute][6]).setPoint,3)
                                ,oidx    = myCollection[attribute][6]
                                 })
	    end 

        myCollection =domoticz.utils.fromJSON("{" .. domoticz.variables("ThermostatJsonCollection3").value .. "}")
        for attribute in pairs(myCollection) do                            -- loop trought the informationTypes 
	        table.insert(myData,{ idx     = myCollection[attribute][1]
                                ,name    = attribute
                                 ,status  = domoticz.devices(myCollection[attribute][2]).state
                                 ,mode  = domoticz.devices(myCollection[attribute][3]).level
                                 ,modeIdx = myCollection[attribute][3]
                                 })
	    end 

        local validKeys = { idx=true }
        sortTable(myData,validKeys)
	                    
	    file = io.open(myFile, "w+")       -- Open file for (over)write
        file:write("{result:")
        file:write(domoticz.utils.toJSON(myData))
        file:write("}")
        file:close() 
	    
		--domoticz.log('Thermostat JSON generated.', domoticz.LOG_INFO)
		--domoticz.log('Nappali termosztát battery level:.' .. domoticz.devices(81).BatteryLevel, domoticz.LOG_ERROR)
		--domoticz.devices(83).updatePercentage(domoticz.devices(81).batteryLevel)
		
	end
}