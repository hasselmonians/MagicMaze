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

  switch self.procotol

  case 'dmnstaskstart'
    if num == 1
      str = 'Rat is doing nose poke...';
      return
    elseif num == 2
      str = 'Treadmill is running...';
      return
    elseif num == 10
      str = 'Rat made correct choice...';
      return
    elseif num == 20
      str = 'Rat made incorrect choice...';
      return
    end

  otherwise
    disp('[WARN] procotol not recognized')
    return
  end % switch

end % function