# magic-maze
A MATLAB framework for running rat experiments through mazes using Arduino controllers.

## The `MagicMaze` object
The core class is the `MagicMaze` object, which stores useful properties for running experiments.

* `serial_port`: contains the `Serial` object created when MATLAB connects to a serial port.
* `protocol`: a character vector that indicates which procedure to follow. This needs to be specified by the user. Outputs from the Arduino microcontroller are interpreted based on the protocol.
* `exp_state`: contains a human-readable explanation of what state `MagicMaze` thinks the current state of the experiment is. For example, it might be, "Rat running on treadmill".
* `trials`: a vector of doubles which contains values describing the trial type. 10 trials with two different conditions might be notated as a 10x1 vector of binary digits, for instance.
* `outcomes`: a vector of the same size as `trials` which contains the results of the experiment.
* `isDone` and `isTreadmill`: logical flags which give `MagicMaze` some flexibility in how to interpret output from the serial port.

### Initialize a `MagicMaze` object
Simply bind the object to a variable. We will use `mm`.

```matlab
mm = MagicMaze;
```

If `MagicMaze` is instantiated without a variable binding, it will create one (`mm`) and print a warning.

Upon initialization, the experimental state is set up `'startup'`. For ease of experimenters during an ongoing project, the protocol is set by default to `'dmns'`.

A `timer` object is also created. The MATLAB serial port callback API is woefully inadequate, so a timer is used instead to check conditions manually.

## Setting experimental parameters

You can set the number of trials by assigning a vector to the correct property.

```matlab
mm.trials = [0, 1, 1, 0, 1, 0];
```

Alternatively, to set `n` trials with values chosen from `k`, do:

```matlab
mm.setTrials(k, n);
% example
mm.setTrials(1:4, 10);
```

To open up the serial port:

```matlab
mm.setSerial;
```

If a port is not specified, `MagicMaze` will pick the first one. Otherwise, acquire the port by viewing the available ones using `seriallist`.

## Performing an experiment

The `doExperiment` function is used to begin an experiment. It uses the specified `protocol` to determine what to do.

This means that each protocol needs to correspond exactly with the embedded Arduino-flavored C++ code on the microcontroller. For convenience, we have put this code in its own folder within this project.

As more protocols are developed, they can be added here.
