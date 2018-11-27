function self = makeGUI(self)

    % check to make sure the GUI doesn't already exist
    f = get(0, 'Children');
    for ii = 1:length(f)
        assert(~strcmp(f(i).Tag, 'magicmaze_main_window'), 'magicmaze main window already exists, refusing to make a new GUI');
    end

    % make the master figure
    handles.main_fig = figure('Position', get(groot, 'ScreenSize'), ...
    'Toolbar', 'figure', 'Menubar', 'none', 'Name', 'Magic Maze', ...
    'NumberTitle', 'off', 'IntegerHandle', 'off', 'WindowButtonDownFcn', @self.mouseCallback, ...
    'WindowScrollWheelFcn', @self.scroll, 'CloseRequestFcn', @self.close, ...
    'Color', 'w', 'Tag', 'magicmaze_main_window', 'ResizeFcn', @self.resize);

end % function
