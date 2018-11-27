
% communicating through serial
temp=seriallist

laserserial = serial(temp,'BaudRate',9600);
fopen(laserserial);
fprintf(laserserial,'04');


temp=fscanf(laserserial);


for i=1:1000
   % this asks whether the arduino sent any bytes
    if laserserial.BytesAvailable>0
        % this reads those bytes
        fscanf(laserserial)
        fprintf('\n');
    end
    pause(2);
end
