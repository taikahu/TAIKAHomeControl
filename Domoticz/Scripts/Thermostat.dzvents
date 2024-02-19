return {
	on = {
	    devices =  {'*(Nappal)','*(Távol)','*(Éjszaka)','*Hőmérséklet / Páratartalom','Termosztát Mód'}
    },
	execute = function(domoticz, device)
		local Kazan = "Off"
		if domoticz.devices(37).levelName=='Ki' and device.name=='Termosztát Mód' then
		    -- Ki
		    domoticz.devices(57).switchOff()
		    domoticz.devices(58).switchOff()
		    domoticz.devices(59).switchOff()
		    domoticz.devices(60).switchOff()
		    domoticz.devices(61).switchOff()
		    domoticz.devices(62).switchOff()
		    domoticz.devices(63).switchOff()
		elseif domoticz.devices(37).levelName=='Nappal' or domoticz.devices(37).levelName=='Éjszaka' or  domoticz.devices(37).levelName=='Távol' then
	        local myCollection = domoticz.utils.fromJSON("" .. domoticz.variables("ThermostatJson").value .. "")
	        for attribute in pairs(myCollection) do                            -- loop trought the informationTypes 
                domoticz.log('Szoba:' .. myCollection[attribute] , domoticz.LOG_INFO)
                if domoticz.devices(myCollection[attribute] .. " Alapérték (" .. domoticz.devices(37).levelName .. ")").setPoint > 
                   domoticz.devices(myCollection[attribute] .. " - Hőmérséklet / Páratartalom").temperature then
                    Kazan = "On"
                    if domoticz.devices(myCollection[attribute] .. " - Státusz").state == "Off" then
                        domoticz.devices(myCollection[attribute] .. " - Státusz").switchOn()
                    end
                else 
                     if domoticz.devices(myCollection[attribute] .. " - Státusz").state == "On" then
                        domoticz.devices(myCollection[attribute] .. " - Státusz").switchOff()
                    end                      
                end
                
            end 
		end
		if Kazan == "On" then
		    if domoticz.devices("Kazán").state=="Off" then
		        domoticz.devices("Kazán").switchOn()
		    end
    	else
    	    if domoticz.devices("Kazán").state=="On" then
		        domoticz.devices("Kazán").switchOff()
		    end
        end
    end
}