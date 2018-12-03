function setSerial(self, port)

  if nargin < 2
    port = seriallist;
  end

  self.serial_port = serial(port, 'BaudRate', 9600);

  if self.gui
    % do something
  else
    disp('[INFO] serial port opened')
  end

end % setSerial
