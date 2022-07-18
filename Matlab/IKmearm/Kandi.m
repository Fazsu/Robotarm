classdef Kandi < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        figure1                  matlab.ui.Figure
        textTime                 matlab.ui.control.Label
        AutomaticTimeValue       matlab.ui.control.NumericEditField
        PointsPanel              matlab.ui.container.Panel
        TextArea                 matlab.ui.control.TextArea
        ClearButton              matlab.ui.control.Button
        SortButton               matlab.ui.control.Button
        CloseButton              matlab.ui.control.Button
        ViewPointsButton         matlab.ui.control.Button
        ConnectButton            matlab.ui.control.Button
        textStatus               matlab.ui.control.Label
        text7                    matlab.ui.control.Label
        TimeSettingsButtonGroup  matlab.ui.container.ButtonGroup
        AutomaticButton          matlab.ui.control.RadioButton
        ManualButton             matlab.ui.control.RadioButton
        SendtoRobotButton        matlab.ui.control.Button
        ManualTimeValue          matlab.ui.control.NumericEditField
        TimesEditFieldLabel      matlab.ui.control.Label
        SetPointButton           matlab.ui.control.Button
        text9                    matlab.ui.control.Label
        Reset                    matlab.ui.control.Button
        inversebutton            matlab.ui.control.Button
        text6                    matlab.ui.control.Label
        text5                    matlab.ui.control.Label
        slider3                  matlab.ui.control.Slider
        slider2                  matlab.ui.control.Slider
        slider1                  matlab.ui.control.Slider
        text1                    matlab.ui.control.Label
        text2                    matlab.ui.control.Label
        text3                    matlab.ui.control.Label
        text4                    matlab.ui.control.Label
        textEucE                 matlab.ui.control.Label
        axes1                    matlab.ui.control.UIAxes
        ContextMenu              matlab.ui.container.ContextMenu
        Menu                     matlab.ui.container.Menu
        Menu2                    matlab.ui.container.Menu
        ContextMenu2             matlab.ui.container.ContextMenu
        Menu_2                   matlab.ui.container.Menu
        Menu2_2                  matlab.ui.container.Menu
    end

    
    properties (Access = private)
        %Timer
       t1 = timetic;
       
       %ArduinoObject
       arduinoObj;
    end

    % Callbacks that handle component events
    methods (Access = private)

        % Code that executes after component creation
        function IK_OpeningFcn(app, varargin)
            % Create GUIDE-style callback args - Added by Migration Tool
            [hObject, eventdata, handles] = convertToGUIDECallbackArguments(app); %#ok<ASGLU>

            % This function has no output args, see OutputFcn.
            % hObject    handle to figure
            % eventdata  reserved - to be defined in a future version of MATLAB
            % handles    structure with handles and user data (see GUIDATA)
            % varargin   command line arguments to IK (see VARARGIN)
            
            % Choose default command line output for IK
            handles.output = hObject;
            
            % Update handles structure
            guidata(hObject, handles);
            
            % UIWAIT makes IK wait for user response (see UIRESUME)
            % uiwait(handles.figure1);

            jinit=[90;60;-120];
            assignin('base','jinit',jinit);
            assignin('base','jstart',jinit);
            axes(handles.axes1);
            FKdraw(jinit(1,1),jinit(2,1),jinit(3,1));
            ax_properties = get(gca);
            assignin('base','pov',ax_properties.View);
            FK=evalin('base','FK');
            pos_start=[FK(1,16);FK(2,16);FK(3,16)];
            pos_target=[FK(1,16);FK(2,16);FK(3,16)];
            assignin('base','pos_start',pos_start);
            assignin('base','pos_target',pos_target);
        end

        % Button pushed function: Reset
        function Reset_Callback(app, event)
            % Create GUIDE-style callback args - Added by Migration Tool
            [hObject, eventdata, handles] = convertToGUIDECallbackArguments(app, event); %#ok<ASGLU>
            tic(app.t1);
            pause(app.t1);
            app.AutomaticTimeValue.Value = 0;
            axes(handles.axes1);
            jinit=evalin('base','jinit');
            assignin('base','jstart',jinit);
            FKdraw(jinit(1,1),jinit(2,1),jinit(3,1));
            FK=evalin('base','FK');
            set( handles.text4,'String', num2str(0));
            set( handles.text5,'String', num2str(12));
            set( handles.text6,'String', num2str(2));
            set(handles.slider1, 'value', 0);
            set(handles.slider2, 'value', 12);
            set(handles.slider3, 'value', 2);
            assignin('base','movement',0);
            
        end

        % Button pushed function: inversebutton
        function inversebutton_Callback(app, event)
            % Create GUIDE-style callback args - Added by Migration Tool
            [hObject, eventdata, handles] = convertToGUIDECallbackArguments(app, event);
            
            % Starts timer if the automatic button is chosen
            if(app.TimeSettingsButtonGroup.SelectedObject == app.AutomaticButton)
                start(app.t1);
            end
            
            % Disables Time Setting buttons for duration of IK
            app.TimeSettingsButtonGroup.Enable = 'Off';
            
            % Calculate Matlab base workspace for start and target
            pos_start=evalin('base','pos_start');
            pos_target=evalin('base','pos_target');
            
            % Get joint 1, 2 and 3 values
            jstart=evalin('base','jstart');
            j1=jstart(1,1);
            j2=jstart(2,1);
            j3=jstart(3,1);
            
            % Check movement status
            if pos_target==pos_start
               errordlg('Set target position first','No Movement');
               movement=0;
               assignin('base','movement',movement);
            else
               movement=1;
               assignin('base','movement',movement);
               
               % Calculate delta using Euclidic distance
               delta=dVelocity(pos_start,pos_target);
            end
            
            while evalin('base','movement')==1
            FK=evalin('base','FK');
            Jac=Jacobian(FK);
            Jacinv=pinv(Jac);
            dXYZ=delta(1:3,1)/10;
            dTheta=Jacinv*dXYZ;
            dTheta1=radtodeg(dTheta(1,1));
            dTheta2=radtodeg(dTheta(2,1));
            dTheta3=radtodeg(dTheta(3,1));
            j1=j1+dTheta1;
            j2=j2+dTheta2;
            j3=j3+dTheta3;
            FKdraw(j1,j2,j3)
            pos_new=[FK(1,16);FK(2,16);FK(3,16)];
            delta=dVelocity(pos_new,pos_target);
            EucError=delta(5,1)^2;
            OrinError=delta(6,1)^2;
            set( handles.textEucE,'String', num2str(EucError,3));
            
            if EucError <10^-2
               movement=0;
               assignin('base','movement',movement);
               assignin('base','pos_start',pos_new);
               jstart=[j1;j2;j3];
               assignin('base','jstart',jstart);
            end
            
            if all(delta(:) <= 0.1)
               movement=0;
               assignin('base','movement',movement);
               assignin('base','pos_start',pos_new);
               assignin('base','jstart',jstart);
            end
            
            if(app.TimeSettingsButtonGroup.SelectedObject == app.AutomaticButton)
                timeElapsed = toc(app.t1);
                app.AutomaticTimeValue.Value = timeElapsed;
            end
            %dEucXY=delta(4,1);
            %dEucXYZ=delta(5,1);
            
            end
            pause(app.t1);
            app.SetPointButton.Enable = 'on';
            app.TimeSettingsButtonGroup.Enable = 'On';
        end

        % Value changed function: slider1
        function slider1_Callback(app, event)
            % Create GUIDE-style callback args - Added by Migration Tool
            [hObject, eventdata, handles] = convertToGUIDECallbackArguments(app, event); %#ok<ASGLU>
            
            val2=str2num(get(handles.text5,'String'));
            val3=str2num(get(handles.text6,'String'));
            val1 = get(hObject,'Value') ;
            val=[val1;val2;val3];
            assignin('base','pos_target',val);
            set( handles.text4,'String', num2str(val1,3) );
            axes(handles.axes1);
        end

        % Value changed function: slider2
        function slider2_Callback(app, event)
            % Create GUIDE-style callback args - Added by Migration Tool
            [hObject, eventdata, handles] = convertToGUIDECallbackArguments(app, event); %#ok<ASGLU>
            
            val1=str2num(get(handles.text4,'String'));
            val3=str2num(get(handles.text6,'String'));
            val2 = get(hObject,'Value') ;
            val=[val1;val2;val3];
            assignin('base','pos_target',val);
            set( handles.text5,'String', num2str(val2,3) );
            axes(handles.axes1);
        end

        % Value changed function: slider3
        function slider3_Callback(app, event)
            % Create GUIDE-style callback args - Added by Migration Tool
            [hObject, eventdata, handles] = convertToGUIDECallbackArguments(app, event); %#ok<ASGLU>
            
            val1=str2num(get(handles.text4,'String'));
            val2=str2num(get(handles.text5,'String'));
            val3 = get(hObject,'Value') ;
            val=[val1;val2;val3];
            assignin('base','pos_target',val);
            set( handles.text6,'String', num2str(val3,3) );
            axes(handles.axes1);
        end

        % Button pushed function: SetPointButton
        function SetPointButtonPushed(app, event)
          fileID = fopen('robot_coordinates.txt', 'a');
          
          if(app.TimeSettingsButtonGroup.SelectedObject == app.AutomaticButton)
             time_sec = app.AutomaticTimeValue.Value;
          end
          if(app.TimeSettingsButtonGroup.SelectedObject == app.ManualButton)
             time_sec = app.ManualTimeValue.Value;
          end
          
          time = datestr(seconds(time_sec), 'HH:MM:SS');
          jstart=evalin('base','jstart');
          j1=round(jstart(1,1));
          j2=round(jstart(2,1));
          j3=round(jstart(3,1));
          formatSpec = '\n%s;%i;%i;%i;*';
          fprintf(fileID,formatSpec,time,j1,j2,j3);
          fclose(fileID);
        end

        % Value changed function: ManualTimeValue
        function ManualTimeValueValueChanged(app, event)
            
        end

        % Button pushed function: SendtoRobotButton
        function SendtoRobotButtonPushed(app, event)
            try 
                filename = "robot_coordinates.txt";
                headerlinesIn = 1;
                data = importdata(filename,headerlinesIn);
                formatSpec = '\n%s;%i;%i;%i;*';
                fprintf(app.arduinoObj,formatSpec,data(2:end,:));
                app.textStatus.Text = 'Data Sent';
                pause(3);
                app.textStatus.Text = 'Online';
            catch ME
                app.textStatus.Text = 'Send Failed';
            end
        end

        % Selection changed function: TimeSettingsButtonGroup
        function TimeSettingsButtonGroupSelectionChanged(app, event)
            selectedButton = app.TimeSettingsButtonGroup.SelectedObject;
            if(selectedButton == app.AutomaticButton )
                app.inversebutton.Text = "Start Timing";
                app.ManualTimeValue.Enable = 'Off';
                app.SetPointButton.Enable = 'Off';
                app.AutomaticTimeValue.Visible = 'On';
                app.textTime.Visible = 'On';
            end
            if(selectedButton == app.ManualButton)
                app.inversebutton.Text = "Calculate next point";
                app.ManualTimeValue.Enable = 'On';
                app.SetPointButton.Enable = 'On';
                app.AutomaticTimeValue.Visible = 'Off';
                app.textTime.Visible = 'Off';
            end
        end

        % Button pushed function: ConnectButton
        function ConnectButtonPushed(app, event)
            % creating arduino object
            try 
                app.arduinoObj = serialport("COM1",9600)
                fopen(app.arduinoObj); % Opens connection between arduino board and matlab
                app.textStatus.Text = 'Online';
                app.connected = 1;
                app.SendtoRobotButton.Enable = 'on';
            catch ME
                app.textStatus.Text = 'Failed to connect';
                app.connected = 0;
                app.SendtoRobotButton.Enable = 'off';
            end
        end

        % Button pushed function: ViewPointsButton
        function ViewPointsButtonPushed(app, event)
            app.PointsPanel.Visible = 'On';
            app.ViewPointsButton.Text = 'Refresh';
            points = readlines('robot_coordinates.txt');
            app.TextArea.Value = points;
        end

        % Button pushed function: CloseButton
        function CloseButtonPushed(app, event)
            app.PointsPanel.Visible = 'off';
            app.ViewPointsButton.Text = 'View Points';
        end

        % Button pushed function: SortButton
        function SortButtonPushed(app, event)
          try
              fid = readtimetable('robot_coordinates.txt',"Delimiter",';');
              newData = sortrows(fid);
              writetimetable(newData,'robot_coordinates.txt','Delimiter',';');
          catch
              
          end
        end

        % Button pushed function: ClearButton
        function ClearButtonPushed(app, event)
          fileID = fopen('robot_coordinates.txt', 'w');
          fprintf(fileID,'%c','T;S1;S2;S3;LB');
          fclose(fileID);
        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create figure1 and hide until all components are created
            app.figure1 = uifigure('Visible', 'off');
            app.figure1.Colormap = [0 0 0.5625;0 0 0.625;0 0 0.6875;0 0 0.75;0 0 0.8125;0 0 0.875;0 0 0.9375;0 0 1;0 0.0625 1;0 0.125 1;0 0.1875 1;0 0.25 1;0 0.3125 1;0 0.375 1;0 0.4375 1;0 0.5 1;0 0.5625 1;0 0.625 1;0 0.6875 1;0 0.75 1;0 0.8125 1;0 0.875 1;0 0.9375 1;0 1 1;0.0625 1 1;0.125 1 0.9375;0.1875 1 0.875;0.25 1 0.8125;0.3125 1 0.75;0.375 1 0.6875;0.4375 1 0.625;0.5 1 0.5625;0.5625 1 0.5;0.625 1 0.4375;0.6875 1 0.375;0.75 1 0.3125;0.8125 1 0.25;0.875 1 0.1875;0.9375 1 0.125;1 1 0.0625;1 1 0;1 0.9375 0;1 0.875 0;1 0.8125 0;1 0.75 0;1 0.6875 0;1 0.625 0;1 0.5625 0;1 0.5 0;1 0.4375 0;1 0.375 0;1 0.3125 0;1 0.25 0;1 0.1875 0;1 0.125 0;1 0.0625 0;1 0 0;0.9375 0 0;0.875 0 0;0.8125 0 0;0.75 0 0;0.6875 0 0;0.625 0 0;0.5625 0 0];
            app.figure1.Position = [520 214 701 609];
            app.figure1.Name = 'Kandi';
            app.figure1.Resize = 'off';
            app.figure1.HandleVisibility = 'callback';
            app.figure1.Tag = 'figure1';

            % Create axes1
            app.axes1 = uiaxes(app.figure1);
            app.axes1.Colormap = [0 0 0.5625;0 0 0.625;0 0 0.6875;0 0 0.75;0 0 0.8125;0 0 0.875;0 0 0.9375;0 0 1;0 0.0625 1;0 0.125 1;0 0.1875 1;0 0.25 1;0 0.3125 1;0 0.375 1;0 0.4375 1;0 0.5 1;0 0.5625 1;0 0.625 1;0 0.6875 1;0 0.75 1;0 0.8125 1;0 0.875 1;0 0.9375 1;0 1 1;0.0625 1 1;0.125 1 0.9375;0.1875 1 0.875;0.25 1 0.8125;0.3125 1 0.75;0.375 1 0.6875;0.4375 1 0.625;0.5 1 0.5625;0.5625 1 0.5;0.625 1 0.4375;0.6875 1 0.375;0.75 1 0.3125;0.8125 1 0.25;0.875 1 0.1875;0.9375 1 0.125;1 1 0.0625;1 1 0;1 0.9375 0;1 0.875 0;1 0.8125 0;1 0.75 0;1 0.6875 0;1 0.625 0;1 0.5625 0;1 0.5 0;1 0.4375 0;1 0.375 0;1 0.3125 0;1 0.25 0;1 0.1875 0;1 0.125 0;1 0.0625 0;1 0 0;0.9375 0 0;0.875 0 0;0.8125 0 0;0.75 0 0;0.6875 0 0;0.625 0 0;0.5625 0 0];
            app.axes1.FontSize = 13.3333333333333;
            app.axes1.NextPlot = 'replace';
            app.axes1.Tag = 'axes1';
            app.axes1.Position = [45 180 614 416];

            % Create textEucE
            app.textEucE = uilabel(app.figure1);
            app.textEucE.Tag = 'textEucE';
            app.textEucE.HorizontalAlignment = 'center';
            app.textEucE.WordWrap = 'on';
            app.textEucE.FontSize = 10.6666666666667;
            app.textEucE.Position = [399 10 79 17];
            app.textEucE.Text = '0';

            % Create text4
            app.text4 = uilabel(app.figure1);
            app.text4.Tag = 'text4';
            app.text4.HorizontalAlignment = 'center';
            app.text4.VerticalAlignment = 'top';
            app.text4.WordWrap = 'on';
            app.text4.FontSize = 10.6666666666667;
            app.text4.Position = [425 89 52 22];
            app.text4.Text = '0';

            % Create text3
            app.text3 = uilabel(app.figure1);
            app.text3.Tag = 'text3';
            app.text3.HorizontalAlignment = 'center';
            app.text3.VerticalAlignment = 'top';
            app.text3.WordWrap = 'on';
            app.text3.FontSize = 10.6666666666667;
            app.text3.Position = [140 30 24 20];
            app.text3.Text = 'z';

            % Create text2
            app.text2 = uilabel(app.figure1);
            app.text2.Tag = 'text2';
            app.text2.HorizontalAlignment = 'center';
            app.text2.VerticalAlignment = 'top';
            app.text2.WordWrap = 'on';
            app.text2.FontSize = 10.6666666666667;
            app.text2.Position = [140 59 24 20];
            app.text2.Text = 'y';

            % Create text1
            app.text1 = uilabel(app.figure1);
            app.text1.Tag = 'text1';
            app.text1.HorizontalAlignment = 'center';
            app.text1.VerticalAlignment = 'top';
            app.text1.WordWrap = 'on';
            app.text1.FontSize = 10.6666666666667;
            app.text1.Position = [140 90 24 20];
            app.text1.Text = 'x';

            % Create slider1
            app.slider1 = uislider(app.figure1);
            app.slider1.Limits = [-20 20];
            app.slider1.MajorTicks = [];
            app.slider1.ValueChangedFcn = createCallbackFcn(app, @slider1_Callback, true);
            app.slider1.MinorTicks = [];
            app.slider1.Tag = 'slider1';
            app.slider1.FontSize = 10.6666666666667;
            app.slider1.Position = [164 99 263 3];

            % Create slider2
            app.slider2 = uislider(app.figure1);
            app.slider2.Limits = [2 20];
            app.slider2.MajorTicks = [];
            app.slider2.ValueChangedFcn = createCallbackFcn(app, @slider2_Callback, true);
            app.slider2.MinorTicks = [];
            app.slider2.Tag = 'slider2';
            app.slider2.FontSize = 10.6666666666667;
            app.slider2.Position = [164 68 262 3];
            app.slider2.Value = 12;

            % Create slider3
            app.slider3 = uislider(app.figure1);
            app.slider3.Limits = [0 20];
            app.slider3.MajorTicks = [];
            app.slider3.ValueChangedFcn = createCallbackFcn(app, @slider3_Callback, true);
            app.slider3.MinorTicks = [];
            app.slider3.Tag = 'slider3';
            app.slider3.FontSize = 10.6666666666667;
            app.slider3.Position = [164 39 263 3];
            app.slider3.Value = 2;

            % Create text5
            app.text5 = uilabel(app.figure1);
            app.text5.Tag = 'text5';
            app.text5.HorizontalAlignment = 'center';
            app.text5.VerticalAlignment = 'top';
            app.text5.WordWrap = 'on';
            app.text5.FontSize = 10.6666666666667;
            app.text5.Position = [425 58 52 22];
            app.text5.Text = '12';

            % Create text6
            app.text6 = uilabel(app.figure1);
            app.text6.Tag = 'text6';
            app.text6.HorizontalAlignment = 'center';
            app.text6.VerticalAlignment = 'top';
            app.text6.WordWrap = 'on';
            app.text6.FontSize = 10.6666666666667;
            app.text6.Position = [425 29 52 22];
            app.text6.Text = '2';

            % Create inversebutton
            app.inversebutton = uibutton(app.figure1, 'push');
            app.inversebutton.ButtonPushedFcn = createCallbackFcn(app, @inversebutton_Callback, true);
            app.inversebutton.Tag = 'inversebutton';
            app.inversebutton.FontSize = 10.6666666666667;
            app.inversebutton.Position = [488 70 192 41];
            app.inversebutton.Text = 'Calculate next point';

            % Create Reset
            app.Reset = uibutton(app.figure1, 'push');
            app.Reset.ButtonPushedFcn = createCallbackFcn(app, @Reset_Callback, true);
            app.Reset.Tag = 'Reset';
            app.Reset.FontSize = 10.6666666666667;
            app.Reset.Position = [488 20 191 40];
            app.Reset.Text = 'Reset';

            % Create text9
            app.text9 = uilabel(app.figure1);
            app.text9.Tag = 'text9';
            app.text9.HorizontalAlignment = 'center';
            app.text9.WordWrap = 'on';
            app.text9.FontSize = 10.6666666666667;
            app.text9.Position = [337 8 74 20];
            app.text9.Text = 'Position Error :';

            % Create SetPointButton
            app.SetPointButton = uibutton(app.figure1, 'push');
            app.SetPointButton.ButtonPushedFcn = createCallbackFcn(app, @SetPointButtonPushed, true);
            app.SetPointButton.Position = [350 130 84 22];
            app.SetPointButton.Text = 'Set Point';

            % Create TimesEditFieldLabel
            app.TimesEditFieldLabel = uilabel(app.figure1);
            app.TimesEditFieldLabel.HorizontalAlignment = 'right';
            app.TimesEditFieldLabel.Position = [163 130 49 22];
            app.TimesEditFieldLabel.Text = 'Time (s)';

            % Create ManualTimeValue
            app.ManualTimeValue = uieditfield(app.figure1, 'numeric');
            app.ManualTimeValue.RoundFractionalValues = 'on';
            app.ManualTimeValue.ValueChangedFcn = createCallbackFcn(app, @ManualTimeValueValueChanged, true);
            app.ManualTimeValue.Position = [227 130 100 22];

            % Create SendtoRobotButton
            app.SendtoRobotButton = uibutton(app.figure1, 'push');
            app.SendtoRobotButton.ButtonPushedFcn = createCallbackFcn(app, @SendtoRobotButtonPushed, true);
            app.SendtoRobotButton.Enable = 'off';
            app.SendtoRobotButton.Position = [491 119 188 43];
            app.SendtoRobotButton.Text = 'Send to Robot';

            % Create TimeSettingsButtonGroup
            app.TimeSettingsButtonGroup = uibuttongroup(app.figure1);
            app.TimeSettingsButtonGroup.SelectionChangedFcn = createCallbackFcn(app, @TimeSettingsButtonGroupSelectionChanged, true);
            app.TimeSettingsButtonGroup.Title = 'Time Settings';
            app.TimeSettingsButtonGroup.Position = [18 95 123 71];

            % Create ManualButton
            app.ManualButton = uiradiobutton(app.TimeSettingsButtonGroup);
            app.ManualButton.Text = 'Manual';
            app.ManualButton.Position = [11 25 61 22];
            app.ManualButton.Value = true;

            % Create AutomaticButton
            app.AutomaticButton = uiradiobutton(app.TimeSettingsButtonGroup);
            app.AutomaticButton.Text = 'Automatic';
            app.AutomaticButton.Position = [11 3 75 22];

            % Create text7
            app.text7 = uilabel(app.figure1);
            app.text7.Tag = 'text9';
            app.text7.HorizontalAlignment = 'center';
            app.text7.VerticalAlignment = 'top';
            app.text7.WordWrap = 'on';
            app.text7.FontSize = 10.6666666666667;
            app.text7.Position = [18 57 96 24];
            app.text7.Text = 'Connection status:';

            % Create textStatus
            app.textStatus = uilabel(app.figure1);
            app.textStatus.Tag = 'textEucE';
            app.textStatus.HorizontalAlignment = 'center';
            app.textStatus.VerticalAlignment = 'top';
            app.textStatus.WordWrap = 'on';
            app.textStatus.FontSize = 10.6666666666667;
            app.textStatus.Position = [6 47 121 22];
            app.textStatus.Text = 'Offline';

            % Create ConnectButton
            app.ConnectButton = uibutton(app.figure1, 'push');
            app.ConnectButton.ButtonPushedFcn = createCallbackFcn(app, @ConnectButtonPushed, true);
            app.ConnectButton.Tag = 'Reset';
            app.ConnectButton.FontSize = 10.6666666666667;
            app.ConnectButton.Position = [22 29 90 22];
            app.ConnectButton.Text = 'Connect';

            % Create ViewPointsButton
            app.ViewPointsButton = uibutton(app.figure1, 'push');
            app.ViewPointsButton.ButtonPushedFcn = createCallbackFcn(app, @ViewPointsButtonPushed, true);
            app.ViewPointsButton.Position = [28 7 80 22];
            app.ViewPointsButton.Text = 'View Points';

            % Create PointsPanel
            app.PointsPanel = uipanel(app.figure1);
            app.PointsPanel.Title = 'Points';
            app.PointsPanel.Visible = 'off';
            app.PointsPanel.Position = [42 180 617 416];

            % Create CloseButton
            app.CloseButton = uibutton(app.PointsPanel, 'push');
            app.CloseButton.ButtonPushedFcn = createCallbackFcn(app, @CloseButtonPushed, true);
            app.CloseButton.Position = [508 10 100 22];
            app.CloseButton.Text = 'Close';

            % Create SortButton
            app.SortButton = uibutton(app.PointsPanel, 'push');
            app.SortButton.ButtonPushedFcn = createCallbackFcn(app, @SortButtonPushed, true);
            app.SortButton.Position = [24 10 100 22];
            app.SortButton.Text = 'Sort';

            % Create ClearButton
            app.ClearButton = uibutton(app.PointsPanel, 'push');
            app.ClearButton.ButtonPushedFcn = createCallbackFcn(app, @ClearButtonPushed, true);
            app.ClearButton.Position = [135 10 100 22];
            app.ClearButton.Text = 'Clear';

            % Create TextArea
            app.TextArea = uitextarea(app.PointsPanel);
            app.TextArea.Editable = 'off';
            app.TextArea.WordWrap = 'off';
            app.TextArea.Position = [17 44 587 348];

            % Create AutomaticTimeValue
            app.AutomaticTimeValue = uieditfield(app.figure1, 'numeric');
            app.AutomaticTimeValue.Editable = 'off';
            app.AutomaticTimeValue.Visible = 'off';
            app.AutomaticTimeValue.Position = [196 7 81 22];

            % Create textTime
            app.textTime = uilabel(app.figure1);
            app.textTime.Tag = 'text9';
            app.textTime.HorizontalAlignment = 'center';
            app.textTime.WordWrap = 'on';
            app.textTime.FontSize = 10.6666666666667;
            app.textTime.Visible = 'off';
            app.textTime.Position = [139 7 74 22];
            app.textTime.Text = 'Time:';

            % Create ContextMenu
            app.ContextMenu = uicontextmenu(app.figure1);

            % Create Menu
            app.Menu = uimenu(app.ContextMenu);
            app.Menu.Text = 'Menu';

            % Create Menu2
            app.Menu2 = uimenu(app.ContextMenu);
            app.Menu2.Text = 'Menu2';
            
            % Assign app.ContextMenu
            app.ManualButton.ContextMenu = app.ContextMenu;

            % Create ContextMenu2
            app.ContextMenu2 = uicontextmenu(app.figure1);

            % Create Menu_2
            app.Menu_2 = uimenu(app.ContextMenu2);
            app.Menu_2.Text = 'Menu';

            % Create Menu2_2
            app.Menu2_2 = uimenu(app.ContextMenu2);
            app.Menu2_2.Text = 'Menu2';
            
            % Assign app.ContextMenu2
            app.ViewPointsButton.ContextMenu = app.ContextMenu2;

            % Show the figure after all components are created
            app.figure1.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = Kandi(varargin)

            runningApp = getRunningApp(app);

            % Check for running singleton app
            if isempty(runningApp)

                % Create UIFigure and components
                createComponents(app)

                % Register the app with App Designer
                registerApp(app, app.figure1)

                % Execute the startup function
                runStartupFcn(app, @(app)IK_OpeningFcn(app, varargin{:}))
            else

                % Focus the running singleton app
                figure(runningApp.figure1)

                app = runningApp;
            end

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)

            % Delete UIFigure when app is deleted
            delete(app.figure1)
        end
    end
end