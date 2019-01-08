# magic-maze
A MATLAB framework for running rat experiments through mazes using Arduino controllers.

## The `MagicMaze` object
The core class is the `MagicMaze` object, which stores useful properties for running experiments.

* `serial_port`: contains the `Serial` object created when MATLAB connects to a serial port.
* `protocol`: a character vector that indicates which procedure to follow. This needs to be specified by the user.
* `exp_state`: contains a human-readable explanation of what state `MagicMaze` thinks the 
