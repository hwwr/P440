-----------------------------------------------------------------------------------
-- Ali provide lua function for data convertion, "device meta <--lua table --> alink json".  --
-- Developer can compose specific script for each desired device by implementing --
-- all of Transaction Functions listed as below.                                 --
--                                                                               --
-- Transaction Functions:                                                        --
--   is_increment():                                                             --
--     @Desc:Design for device which cannot handle sigle-attribute command.      --
--           Lua author need to set "false" as return value and server will      --
--           compose a full-attributes command by integrate other attributes     --
--           recorded on server. Otherwise, set "true".                          --
--     @Para:<null>                                                              --
--     @Rets:true/false                                                          --
--                                                                               --
--   private_cmd_query(json):                                                    --
--     @Desc:Invoked by server when app query device status.                     --
--           Lua author need to compose a "query" cmd for device                 --
--     @Para:<null>                                                              --
--     @Rets:"query" cmd string for device status                                --
--                                                                               --
--   convert_from_private(meta):                                                 --
--     @Desc:Invoked by server, convert device meta data to alink json data.     --
--     @Para:device meta data                                                    --
--     @Rets:alink json data                                                     --
--                                                                               --
--   convert_to_private(json):                                                   --
--     @Desc:Invoked by server, convert alink json data to device meta data.     --
--     @Para:alink json data                                                     --
--     @Rets:device meta data                                                    --
--                                                                               --
-- Basic Functions:                                                              --
--   string.fromhex(str)                                                         --
--   string.tohex(str)                                                           --
--   getbyte(str,pos)                                                            --
--   subbyte(str,start,end)                                                      --
--   arraytostring(tdlist, start, end)                                           --
--   serialize(obj)                                                              --
-----------------------------------------------------------------------------------

---Begin: Transaction Functions---
  function is_increment()
    local ret_sample = false;
    -- do your job here --
    return ret_sample;                   
  end
  
  function private_cmd_query(json)
     local private_cmd_sample = "010101010101"
     -- do your job here --
     return private_cmd_sample
  end
  
  function convert_from_private(meta)
    local jsonDataSample = {
		OnOff_Power= {
	        value= 1
	    },
	    OnOff_Ions= {
	        value= 1
	    },
	    Status_AirQuality= {
	        value= 1
	    },
	    WorkMode= {
	        value= 1
	    },
	    Ventilation_Speed= {
	        value = 1
	    },
		TimeMeter_PowerOn= {
	        value= 1
	    },
	    TimeMeter_PowerOff= {
	        value= 1
	    },
	    LifeTime_Filter= {
	        value= 1
	    },
	    ErrorCode= {
	        value= 1
	    },
	    OnOff_AirQuality= {
	        value = 1
	    }
          }
    -- do your job here --
    local str = string.fromhex(meta)
    local v_OnOff_Power = bit32.band(str:byte(3), 0x3);
--	print(tostring(v_OnOff_Power));
	local v_OnOff_Ions = bit32.band(str:byte(4), 0x3);
--	print(tostring(v_OnOff_Ions));
	local v_Status_AirQuality = bit32.band(str:byte(5), 0xf);
--	print(tostring(v_Status_AirQuality));
	local v_WorkMode = bit32.band(str:byte(6), 0xf);
--	print(tostring(v_WorkMode));
	local v_Ventilation_Speed = bit32.band(str:byte(7), 0xf);
--	print(v_Ventilation_Speed);
    local v_TimeMeter_PowerOn = bit32.band(str:byte(8), 0xff);
--	print(tostring(v_TimeMeter_PowerOn));
	local v_TimeMeter_PowerOff = bit32.band(str:byte(9), 0xff);
--	print(tostring(v_TimeMeter_PowerOff));
	local v_LifeTime_Filter = bit32.band(str:byte(10), 0xff);
--	print(tostring(v_LifeTime_Filter));
	local v_ErrorCode = bit32.band(str:byte(11), 0xf);
--	print(tostring(v_ErrorCode));
	local v_OnOff_AirQuality = bit32.band(str:byte(12), 0x3);
--	print(tostring(v_OnOff_AirQuality));
	jsonDataSample={
		OnOff_Power = {value = tostring(v_OnOff_Power)},
		OnOff_Ions = {value = tostring(v_OnOff_Ions)},
		Status_AirQuality = {value = tostring(v_Status_AirQuality)},
		WorkMode = {value = tostring(v_WorkMode)},
		Ventilation_Speed = {value = tostring(v_Ventilation_Speed)},
		
		TimeMeter_PowerOn = {value = tostring(v_TimeMeter_PowerOn)},
		TimeMeter_PowerOff = {value = tostring(v_TimeMeter_PowerOff)},
		LifeTime_Filter = {value = tostring(v_LifeTime_Filter)},
		ErrorCode = {value = tostring(v_ErrorCode)},
		OnOff_AirQuality = {value = tostring(v_OnOff_AirQuality)},
	}

    -- do your job end --
    return jsonDataSample
  end

  function convert_to_private(t)
    local metaDataListSample = {
		[0] = 0xaa,       --header
		[1] = 0x0C,       --cmd length------------------------------
		[2] = 0x01,       --OnOff_Power
		[3] = 0x01,       --OnOff_Ions
		[4] = 0x01,       --Status_AirQuality
		[5] = 0x00,       --WorkMode
		[6] = 0x00,       --Ventilation_Speed
		[7] = 0x00,       --TimeMeter_PowerOn
		[8] = 0x00,       --TimeMeter_PowerOff
		[9] = 0x01,       --LifeTime_Filter
		--[10] = 0x01,       --LifeTime_Filter
		[10] = 0x00,       --ErrorCode
		[11] = 0x00,       --OnOff_AirQuality
		
		[12] = 0x55,       --end
    }
    local metaDatamaxposSample = metaDataListSample[1]
    -- do your job here --
	for key, value in pairs(t) do
	--print(key)
		if (key == "OnOff_Power") then
			metaDataListSample[2] = tonumber(t['OnOff_Power']['value'])
			--print(key, metaDataListSample[2])
		elseif (key == "OnOff_Ions") then
			metaDataListSample[3] = tonumber(t['OnOff_Ions']['value'])
			--print(key, metaDataListSample[3])
		elseif (key == "Status_AirQuality") then
			metaDataListSample[4] = tonumber(t['Status_AirQuality']['value'])
			--print(key, metaDataListSample[4])
		elseif (key == "WorkMode") then
			metaDataListSample[5] = tonumber(t['WorkMode']['value'])
			--print(key, metaDataListSample[5])
		elseif (key == "Ventilation_Speed") then
			metaDataListSample[6] = tonumber(t['Ventilation_Speed']['value'])
			--print(key, metaDataListSample[6])
		elseif (key == "TimeMeter_PowerOn") then
			metaDataListSample[7] = tonumber(t['TimeMeter_PowerOn']['value'])
			--print(key, metaDataListSample[7])
		elseif (key == "TimeMeter_PowerOff") then
			metaDataListSample[8] = tonumber(t['TimeMeter_PowerOff']['value'])
			--print(key, metaDataListSample[8])
		elseif (key == "LifeTime_Filter") then
			metaDataListSample[9] = tonumber(t['LifeTime_Filter']['value'])
			--print(key, metaDataListSample[9])
		elseif (key == "ErrorCode") then
			metaDataListSample[10] = tonumber(t['ErrorCode']['value'])
			--print(key, metaDataListSample[10])
		elseif (key == "OnOff_AirQuality") then
			metaDataListSample[11] = tonumber(t['OnOff_AirQuality']['value'])
			--print(key, metaDataListSample[11])
		
		
		
		end
	end
    
    -- do your job end --
    local metaData = arraytostring(metaDataListSample,0,metaDatamaxposSample)
    return metaData
  end
---End: Transaction Functions---

---Begin: Basic Functions---
  function string.fromhex(str)
      return (str:gsub('..', function (cc)
          return string.char(tonumber(cc, 16))
      end))
  end
  
  function string.tohex(str)
      return (str:gsub('.', function (c)
          return string.format('%02X', string.byte(c))
      end))
  end
  
  function getbyte(str,pos)
    local b15 = string.sub(str, (pos*2)+1, ((pos+1)*2))
    return b15
  end
  
  function subbyte(str,pos1,pos2)
    local b15 = string.sub(str, ((pos1-1)*2)+1, (pos2*2)+1)  
    return b15
  end
  
  function arraytostring(tdlist, pos1, pos2)
    local res ="";
    for i=pos1,pos2 do
      res = res..string.char(tdlist[i])
    end
    res = string.tohex(res)
    return res;
  end
  
  function serialize(obj)  
    local lua = ""  
    local t = type(obj)  
    if t == "number" then  
      lua = lua .. obj  
    elseif t == "boolean" then  
      lua = lua .. tostring(obj)  
    elseif t == "string" then  
      lua = lua .. string.format("%q", obj)  
    elseif t == "table" then  
      lua = lua .. "{"  
      for k, v in pairs(obj) do  
        lua = lua .. serialize(k) .. ":"..serialize(v) .. ","  
      end  
      local metatable = getmetatable(obj)  
        if metatable ~= nil and type(metatable.__index) == "table" then  
          for k, v in pairs(metatable.__index) do  
            lua = lua .. serialize(k) .. ":"..serialize(v) .. ","  
          end  
        end  
      local luaLen =  string.len(lua)-1;
      lua = string.sub(lua,1,luaLen)
      lua = lua .. "}"  
    elseif t == "nil" then  
      return nil  
    else  
      error("can not serialize a " .. t .. " type.")  
    end  
    return lua  
  end
---End: Basic Functions---

---Begin: lua test---
--[[
  function lua_test()
    local luatableDataSample = {
		OnOff_Power= {
	        value= 1
	    },
	    Color_Temperature= {
	        value= 48
	    },
	    Light_Brightness= {
	        value= 80
	    },
	    TimeDelay_PowerOff= {
	        value= 0
	    },
	    WorkMode_MasterLight= {
	        value = 1
	    }
    }
	local metaDataSample = 'AA07010130500055'
    print(convert_to_private(luatableDataSample))
    print(serialize(convert_from_private(metaDataSample)))
  end
  
  lua_test()
--]]
---END: lua test---

