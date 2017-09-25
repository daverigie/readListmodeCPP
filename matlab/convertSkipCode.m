function [skipcode] = convertSkipCode(skipevents)

    packetCodes.Event                 = 1;
    packetCodes.TimeMarkerDeadTime    = 2;
    packetCodes.TimeMarkerElapsed     = 4;
    packetCodes.GantryMotion          = 8;
    packetCodes.GatingExpandedRespOn  = 16;
    packetCodes.GatingExpandedRespOff = 32;
    packetCodes.GatingOther           = 64;
    packetCodes.ControlAcq            = 128;
    
    skipcode = 0;
    
    for i = 1:1:numel(skipevents)
        if isfield(packetCodes, skipevents{i})
            skipcode = skipcode + getfield(packetCodes, skipevents{i});
        end
    end
    
end

