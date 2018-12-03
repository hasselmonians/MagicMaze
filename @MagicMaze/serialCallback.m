function serialCallback(self, event)

  % callback function to run whenever there are at least four bytes in the buffer
  % calls serialInterpret and displays to the GUI

  num = fscanf(self.serial_port, '%d');
  self.exp_state = self.serialInterpret(num);
  self.exp_num = num;

end % function
