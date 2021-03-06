% Add the directory containing the utility functions to the MATLAB path.
utilpath = fullfile(matlabroot, 'toolbox', 'imaq', 'imaqdemos', ...
    'html', 'applications', 'laserTracking');
addpath(utilpath);

global END_PROGRAM
global cam
global cam_config
global img
END_PROGRAM = 0;
% Detect keypresses in the figure
gcf
set(gcf, 'KeyPressFcn', @myKeyPressFcn)

% Access and configure a device. Type imaqhwinfo('winvideo') for more
% information on the cameras.

% This is the configuration for the Logitech quick cam 
cam = videoinput('winvideo', 1, 'RGB24_640x480');
% This configuration is for your own webcam.
% cam = videoinput('winvideo', 1, 'MJPG_640x480');
cam_config = getselectedsource(cam);
% configure camera settings
get(cam_config)
set(cam_config, 'Exposure', -8);
set(cam_config, 'BacklightCompensation', 'off');
set(cam_config, 'ExposureMode', 'manual');
% cam_config.Exposure = -9;
% cam_config.BacklightCompensation = 'off';
% cam_config.Exposure = -8;
% cam_config.ExposureMode = 'manual';
% cam_config.WhiteBalanceMode = 'manual';

% configures camera to repeat triggers and have 1 frame per trigger. doing
% so does not store frames to memory so the memory won't get fillede up
cam.FramesPerTrigger = 1;
cam.TriggerRepeat = Inf;
triggerconfig(cam,'manual');
view = figure(1);

start(cam);
trigger(cam)
while ~END_PROGRAM
    img = getsnapshot(cam);
    imshow(img);
    drawnow;
end
close(view)
delete(cam)
clear cam

function myKeyPressFcn(hObject, event)
global cam
global cam_config
disp('key is pressed:')
disp(event.Key)
if event.Key == 's'
    % Stop the program
    global END_PROGRAM
    END_PROGRAM = 1;
    stop(cam)
    delete(cam)
    disp('program stopped.')
elseif event.Key == 'c'
    % Display current camera config
    disp(cam_config)
elseif event.Key == 'w'
    % Save current frame as png with capture-date-# convention
    global img
    num = 0;
    name = strcat('capture-', date, '-',num2str(num), '.png');
    while exist(name,'file')
        num = num + 1
        name = strcat('capture-', date, '-',num2str(num), '.png');
    end
    imwrite(img, name)
end

end