function serialCallback(self, event)

  % callback function to run whenever there are at least four bytes in the buffer
  % calls serialInterpret and displays to the GUI

  self.exp_state = self.serialInterpret(fscanf(self.serial_port, '%d'));

end % function
