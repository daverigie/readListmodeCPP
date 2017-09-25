function binpath = getbinpath()

    basedir = updir(mfilename('fullpath'),2);
    
   
    if ispc
        binpath = fullfile(basedir, 'bin', 'Win64', 'dumptags.exe');
    else
        binpath = fullfile(basedir, 'bin', 'dumptags');
    end

end


function p = updir(p, levels)

    while levels > 0
        p = fileparts(p);
        levels = levels - 1;
    end

end