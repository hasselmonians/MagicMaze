mydaq=daq.createSession('NI');

addDigitalChannel(mydaq,'dev1','port0/line1:7','OutputOnly')
addDigitalChannel(mydaq,'dev1','port0/line0','InputOnly')

outputSingleScan(mydaq,[1 1 1 1 1 1 1]);

outputSingleScan(mydaq,[0 0 1 1 1 1 1]);

test=inputSingleScan(mydaq);
%%

% find servos


%%
a=arduino;


% one microsecond
%    0.0001
% this is for hte micro servos
s=servo(a,'D2','MinPulseDuration',0.0009,'MaxPulseDuration',0.0021);

% lets see if this is correct
%% add motor library, servo, and rotartEncoder
a=arduino('COM4','Mega2560','Libraries',{'Adafruit/MotorShieldV2', 'I2C', 'SPI', 'Servo','rotaryEncoder'});
% pulses per rev=cycles per rev=lines per rev=counts per rev/4
% this is a quadrature encoder, so it will be 1 pulse per cycle

% need 2 so you can do direction
encoder=rotaryEncoder(a,'D2','D3',100);



% max is 3 encoders per arduino
%%
for i=1:1000
    yyaxis left;
    speedy(i)=readSpeed(encoder);
    plot(i,speedy(i),'b*'); hold on; drawnow;
    yyaxis right;
    [distancy(i),times(i)]=readCount(encoder);
    plot(i,distancy(i),'r*'); hold on; drawnow;
    if abs(speedy(i))>25
        writeDigitalPin(a,'D13',1);
    else
        writeDigitalPin(a,'D13',0);
    end
    %pause(.2);
end

% for these, if you put 100

%%
multiplyer=((distancy(38)-distancy(37))/times(38)-times(37))/speedy(37)
%% super heavy duty servo
a=arduino;
s2=servo(a,'D2','MinPulseDuration',0.0005,'MaxPulseDuration',0.00025);

%% running a stepper using the motor shield, wayyyy better btw

a=arduino();
shield=addon(a,'Adafruit/MotorShieldV2');
% shield name, motor number is 1, 200 steps per revolution
% rpm is important and it can go real fast, and steptype is always double
sm=stepper(shield,1,200,'RPM',50,'StepType','Double');

% this moves it 50 steps
move(sm,50);
%%
a=arduino;
readpin='D21';
writepin='D24';
configurePin(a,readpin,'Pullup');

for i=1:1000
    sensorstat(i)=readDigitalPin(a,readpin);
    plot(i,sensorstat(i),'r*');
    hold on; drawnow;
    if sensorstat(i)<1
        writeDigitalPin(a,writepin,1);
    else
        writeDigitalPin(a,writepin,1);
    end
end

%% lets see if we can build a camera listener



imaqhwinfo()

vidobj=imaq.VideoDevice('winvideo',1);
for i=1:20
    
    
    
end

myobj=videoinput('winvideo',1);
myobj.SelectedSourceName='input1';
preview(myobj); myframe=[];
for i=1:20
    
    frame=getsnapshot(myobj);
    myframe(:,:,i)=frame(:,:,1);
end
%%
a=arduino;

%%

PinMap={'D2','D3','D4','D5','D6'};
ColorMap={'r*','g*','b*','c*','k*','o*'};
for j=1:5, configurePin(a,PinMap{j},'pullup'); end
figure;
for i=1:10000
    for j=1:5
        plot((i),readDigitalPin(a,PinMap{j})+.05*j,ColorMap{j}); hold on;
        drawnow;
    end
end

%%
% communicating through serial
temp=seriallist;

mymaze = serial(temp,'BaudRate',9600);
fopen(mymaze);
fprintf(mymaze,'04');


temp=fscanf(mymaze);


for i=1:1000
    if mymaze.BytesAvailable>0
        fscanf(mymaze)
        fprintf('\n');
    end
    pause(2);
end
%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%% run with firmware


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
temp=seriallist;
temp'

mymaze = serial(temp{7},'BaudRate',9600);


% generate trial list
triallist{1}='1';
for i=2:25
    triallist{i}=num2str(randi(4));
    while triallist{i}==triallist{i-1}
        triallist{i}=num2str(randi(4));
    end
end


% %for 1,2 then 3,4
% triallist{1}='1';
% triallist{i}=randi([1,2]);
% while any(diff(triallist,3)==0)
%     repeat=find(diff(triallist,3)==0,1,'first')+1;
% end
% end
% 
% while triallist{i}==triallist{i-1};
%     triallist{i}=randi([3,4])
%     

%%
if strcmpi(mymaze.status,'open')
    fclose(mymaze);
end
pause(1);
fopen(mymaze);
pause(5);

trialperf=[];
for i=1:length(triallist)
    
   trialperf=[0 0 0];
   %the 3 is for number of trials correct
   while sum(trialperf(end-2:end))<3
        fwrite(mymaze,triallist{i});
        pause(1);
        trialtype=str2num(char(fread(mymaze,1)));
        
        while mymaze.BytesAvailable==0
        end
        received= str2num(char(fread(mymaze,1)));
        fprintf('   rat sampled\n');
        pause(.1);
        while mymaze.BytesAvailable==0
        end
        received= str2num(char(fread(mymaze,1)));
        fprintf('   rat got on treadmill\n');
        pause(.1);
        while mymaze.BytesAvailable==0
        end
        received= str2num(char(fread(mymaze,1)));
        if received==3
            fprintf('Correct!\n');
            trialperf=[trialperf 1];
        else
            fprintf('Incorrect!\n');
            trialperf=[trialperf 0];
        end
        
        pause(.2);
    end
    fprintf('Moving on to another trial type \n');
    
    pause(.1);
end








%%
configurePin(a,'A2','AnalogInput');
figure;
for i=1:1000
    plot(i,log(4-readVoltage(a,'A2')),'*');
    hold on;
    drawnow;
    pause(.1);
end

%% An up and down servo, where its only two circuits:
% 1. the servo
% 2. two interrupts, where at each end the pullup goes to gnd

% initialize
a=arduino;
s1=servo(a,'D2');
configurePin(a,'D3','Pullup');

% the intial code
% go up
writePosition(s1,.45);
pause(.2);
while readDigitalPin(a,'D3')>0
    
end
writePosition(s1,.5);

% go down
writePosition(s1,.6);
pause(.2);
while readDigitalPin(a,'D3')>0
    
end
writePosition(s1,.5);
%%
% the linear servo
s1=servo(a,'D2');
writePosition(s1,1); % slow the servo
writeDigitalPin(a,'D3',1); writeDigitalPin(a,'D3',0);
writeDigitalPin(a,'D4',1); writeDigitalPin(a,'D4',0);