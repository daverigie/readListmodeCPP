function [] = dumptags(lmpath, outpath, skipevents)

    %{
        lmpath is the filepath to the .BF listmode file
        outpath is the filepath of the resulting text file
        skipevents is a cellarray of strings or an integer 

        Event                 = 1
        TimeMarkerDeadTime    = 2
        TimeMarkerElapsed     = 4
        GantryMotion          = 8
        GatingExpandedRespOn  = 16
        GatingExpandedRespOff = 32
        GatingOther           = 64         
        ControlAcq            = 128

        to skip multiple packets types, simply add the integers, otherwise
        just expand the cell array of strings
    %}

    % =====================================================================
    %                       ARGUMENT CHECKING
    %======================================================================

    if nargin < 3
        skipevents = 1;
    end
    
    if nargin < 1
        [filename, dirname] = uigetfile('*.*');
        lmpath = fullfile(dirname, filename);
    end
    
    if nargin < 2
       outpath = uiputfile('*.txt');
    end
    
    % Convert skipevents to integer code
    if iscell(skipevents)
        skipevents = convertSkipCode(skipevent);
    end
    
    % =====================================================================
    %                           CALL BINARY
    % =====================================================================
    
    binpath = getbinpath();
    
    commandstr = sprintf('%s %s %s %i',binpath, lmpath, outpath, skipevents)
    
    tic;
    fprintf('\nDumping LM tags ...');
    system(commandstr, '-echo');
    fprintf(' Done.');
    toc;
    
    fprintf('\n\n');
end