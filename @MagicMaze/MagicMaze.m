% MagicMaze.m
% Alec Hoyland

% serial_port: the serial object that reads and writes to the Arduino controller
% protocol: the name of the code running on the Arduino -- determines how I/O should be translated between MATLAB and the controller

classdef MagicMaze < handle & matlab.mixin.CustomDisplay

  properties

    serial_port@serial
    protocol@char
    exp_state@char

  end % properties

  properties (SetAccess = protected)

end % SetAccess = protected

  properties (Access = protected)

end % Access = protected

  methods

    % constructor
    function self = MagicMaze(make_gui)

      % check to see if a graphical user interface should be made
      % default to the dmnstaskstart protocol
      if nargin == 0
        make_gui = true;
        self.protocol = 'dmnstaskstart';
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
        cprintf('text', 'MagicMaze called without assigning to an object. MagicMaze will create an object named ''mm''.')
        assignin('base', 'mm', self);
      end

      % set the experimental state to "startup"
      self.exp_state = 'startup';

    end % constructor

    function set.exp_state(self, str)

      if self.make_gui
        % update the GUI somehow
      else
        disp(['[INFO] experiment state: ' str]);
      end

    end % set.exp_state

  end % methods

  methods (Static)

  end % static methods


end % classdef
