%% Set up the serial port
port        = seriallist;
laserserial = serial(port, 'BaudRate', 9600);

% open the I/O connection
fopen(laserserial);

% determine the trial type
trialtype   = '04';
fprintf(laserserial, trialtype);

% how long should we test?
trialtime   = 2; % minutes

%% Wait for output
while time < (trialtime*60)
    if laserserial.BytesAvailable > 0
        output = fscanf(laserserial, '%d');
        disp(output)
    end
    pause(1);
end
