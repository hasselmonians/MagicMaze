% MagicMaze.m
% Alec Hoyland

% serial_port: the serial object that reads and writes to the Arduino controller
% protocol: the name of the code running on the Arduino -- determines how I/O should be translated between MATLAB and the controller

classdef MagicMaze < handle & matlab.mixin.CustomDisplay

  properties

    serial_port@serial
    protocol@char
    exp_state@char
    trials@double
    outcomes@double
    isDone@logical

  end % properties

  properties (SetAccess = protected)

  end % SetAccess = protected

  properties (Access = protected)
    gui@logical

  end % Access = protected

  methods

    % constructor
    function self = MagicMaze(make_gui)

      % check to see if a graphical user interface should be made
      % default to the dmnstaskstart protocol
      if nargin == 0
        make_gui = false;
      end

      if make_gui

        if verLessThan('matlab', '8.0.1')
          error('Need MATLAB 2014b or newer to run')
        end

        self.gui = true;
        self.makeGUI;
      else
        self.gui = false;
      end

      % for stability, automatically assign to a variable if not done so
      if ~nargout
        cprintf('red', '[WARN]')
        cprintf('text', 'MagicMaze called without assigning to an object. MagicMaze will create an object named ''mm''.')
        assignin('base', 'mm', self);
      end

      % set the experimental state to "startup"
      self.exp_state = 'startup';
      % set default protocol
      self.protocol = 'dmns';
      % set asynchronous waiting to not done
      self.isDone = false;

    end % constructor

    function delete(self)

      if ~isempty(self.serial_port)
        fclose(self.serial_port)
      end

    end % delete

    function set.exp_state(self, str)

      self.exp_state = str;

      if self.gui
        % update the GUI somehow
      else
        disp(['[INFO] experiment state: ' str]);
      end

    end % set.exp_state

    function set.protocol(self, str)

      protocols = {'dmns'};
      if any(strcmp(str, protocols))
        self.protocol = str;
      else
        disp('[ERROR] protocol not recognized')
      end

    end % set.protocol

  end % methods

  methods (Static)

  end % static methods


end % classdef
