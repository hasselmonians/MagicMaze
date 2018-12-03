function doExperiment(self)

  % performs the protocol specified in self.protocol
  % records the data in self.outcomes based on the conditions set in self.trials
  % updates the experimental state in self.exp_state

  self.outcomes = NaN(length(self.trials), 1);

  for ii = 1:length(self.trials)

    % experiment is **not** done
    self.isDone = false;

    % send trial information through the serial port
    fprintf(self.serial_port, num2str(uint8(self.trials(ii))));

    % wait for a response
    waitfor(self, 'isDone', logical(1));

    switch self.protocol
    case 'dmns'
      if self.exp_state == 3
        self.outcomes(ii) = 1;
      else
        self.outcomes(ii) = 0;
      end
    otherwise
      disp('[ERROR] unknown protocol')
    end

  end % ii

  disp('[INFO] experiment completed')

  % sing the song of our people
  load handel
  sound(y,Fs)

end % function
