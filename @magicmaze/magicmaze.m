% magicmaze.m
% Alec Hoyland

% serial_port: the serial object that reads and writes to the Arduino controller
% protocol: the name of the code running on the Arduino -- determines how I/O should be translated between MATLAB and the controller

classdef magicmaze < handle & matlab.mixin.CustomDisplay

  properties

  serial_port@serial
  protocol@char

  end % properties

  properties (SetAccess = protected)

end % SetAccess = protected

  properties (Access = protected)

end % Access = protected

  methods

    % constructor
    function self = magicmaze(make_gui)

      % check to see if a graphical user interface should be made
      % default to the dmnstaskstart protocol
      if nargin == 0
        make_gui = true;
        self.procotol = 'dmnstaskstart';
      end

      if make_gui

        if verLessThan('matlab', '8.0.1')
          error('Need MATLAB 2014b or newer to run')
        end

        self.makeGUI;
      end

      % for stability, automatically assign to a variable if not done so
      if ~nargout
        cprintf('red', '[WARN]')
        cprintf('text', 'magicmaze called without assigning to an object. magicmaze will create an object named ''mm''.')
        assignin('base', 'mm', self);
      end


    end % constructor

    function self = set.serial_port(self, port)

      if nargin < 2
        port = seriallist;
        self.serial_port = serial(port, 'BaudRate', 9600);
      end

    end % set.serial_port

  end % methods

  methods (Static)



  end % static methods


end % classdef