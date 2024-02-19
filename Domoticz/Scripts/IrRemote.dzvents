return {
	on = {
		devices = {
			'IR Command*'
		}
	},
	execute = function(domoticz, device)
		if (device.name=='IR Command 1') then
		    if (domoticz.devices('TV háttér világítás').level==0) then
		        domoticz.devices('TV háttér világítás').setLevel(domoticz.devices('TV háttér világítás').lastLevel);
		    else
		        domoticz.devices('TV háttér világítás').switchOff();
		    end
		end
	
		if (device.name=='IR Command 2') then
            domoticz.devices('Tükör képernyő').toggleSwitch();
		end	
		
		if (device.name=='IR Command 4') then
		    if (domoticz.devices('Akvárium fal világítás').level==0) then
		        domoticz.devices('Akvárium fal világítás').setLevel(domoticz.devices('Akvárium fal világítás').lastLevel);
		    else
		        domoticz.devices('Akvárium fal világítás').switchOff();
		    end
    	end		
		if (device.name=='IR Command 5') then
		    if (domoticz.devices('Akvárium világítás').level==0) then
		        domoticz.devices('Akvárium világítás').setLevel(domoticz.devices('Akvárium világítás').lastLevel);
		    else
		        domoticz.devices('Akvárium világítás').switchOff();
		    end
		end
	end
}