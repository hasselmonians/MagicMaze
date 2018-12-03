function str = serialInterpret(self, num)

  % interprets a number passed from the Arduino as an experimental state
  % this is essentially a lookup table for experimenter convenience

  % Arguments:
    % num: a number or character vector representing a number
  % Outputs:
    % a character vector meaning what the number represents

  if ischar(num)
    % convert to double if not a double
    num = str2double(num);
  end

  switch self.protocol

  case 'dmns'
    if num == 1
      str = 'Setting up trial...';
      return
    elseif num == 2
      str = 'Treadmill is running...';
      return
    elseif num == 3
      str = 'Rat made correct choice...';
      self.isDone = true;
      return
    elseif num == 4
      str = 'Rat made incorrect choice...';
      self.isDone = true;
      return
    end

  otherwise
    disp('[WARN] protocol not recognized')
    return
  end % switch

end % function
