classdef meArm_Coordinate_App < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)
        figure1              matlab.ui.Figure
        TimesEditField       matlab.ui.control.NumericEditField
        TimesEditFieldLabel  matlab.ui.control.Label
        SetPointButton       matlab.ui.control.Button
        text9                matlab.ui.control.Label
        Reset                matlab.ui.control.Button
        inversebutton        matlab.ui.control.Button
        text6                matlab.ui.control.Label
        text5                matlab.ui.control.Label
        slider3              matlab.ui.control.Slider
        slider2              matlab.ui.control.Slider
        slider1              matlab.ui.control.Slider
        text1                matlab.ui.control.Label
        text2                matlab.ui.control.Label
        text3                matlab.ui.control.Label
        text4                matlab.ui.control.Label
        textEucE             matlab.ui.control.Label
        axes1                matlab.ui.control.UIAxes
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
            fileID=fopen('robot_coordinates.txt', 'w');
            % Create GUIDE-style callback args - Added by Migration Tool
            [hObject, eventdata, handles] = convertToGUIDECallbackArguments(app, event); %#ok<ASGLU>
            
            pos_start=evalin('base','pos_start');
            pos_target=evalin('base','pos_target');
            jstart=evalin('base','jstart');
            
            j1=jstart(1,1);
            j2=jstart(2,1);
            j3=jstart(3,1);
            if pos_target==pos_start
               errordlg('Set target position first','No Movement');
               movement=0;
               assignin('base','movement',movement);
            else
               movement=1;
               assignin('base','movement',movement);
               delta=divelo(pos_start,pos_target);
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
            delta=divelo(pos_new,pos_target);
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
            
            
            %dEucXY=delta(4,1);
            %dEucXYZ=delta(5,1);
            
            end
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
          time_seconds = app.TimesEditField.Value;
          time = datestr(seconds(time_seconds), 'HH:MM:SS');
          jstart=evalin('base','jstart');
          j1=round(jstart(1,1));
          j2=round(jstart(2,1));
          j3=round(jstart(3,1));
          formatSpec = '%s;%i;%i;%i;*\n';
          fprintf(fileID, formatSpec, time, j1, j2, j3);
        end

        % Value changed function: TimesEditField
        function TimesEditFieldValueChanged(app, event)

        end
    end

    % Component initialization
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create figure1 and hide until all components are created
            app.figure1 = uifigure('Visible', 'off');
            app.figure1.Colormap = [0 0 0.5625;0 0 0.625;0 0 0.6875;0 0 0.75;0 0 0.8125;0 0 0.875;0 0 0.9375;0 0 1;0 0.0625 1;0 0.125 1;0 0.1875 1;0 0.25 1;0 0.3125 1;0 0.375 1;0 0.4375 1;0 0.5 1;0 0.5625 1;0 0.625 1;0 0.6875 1;0 0.75 1;0 0.8125 1;0 0.875 1;0 0.9375 1;0 1 1;0.0625 1 1;0.125 1 0.9375;0.1875 1 0.875;0.25 1 0.8125;0.3125 1 0.75;0.375 1 0.6875;0.4375 1 0.625;0.5 1 0.5625;0.5625 1 0.5;0.625 1 0.4375;0.6875 1 0.375;0.75 1 0.3125;0.8125 1 0.25;0.875 1 0.1875;0.9375 1 0.125;1 1 0.0625;1 1 0;1 0.9375 0;1 0.875 0;1 0.8125 0;1 0.75 0;1 0.6875 0;1 0.625 0;1 0.5625 0;1 0.5 0;1 0.4375 0;1 0.375 0;1 0.3125 0;1 0.25 0;1 0.1875 0;1 0.125 0;1 0.0625 0;1 0 0;0.9375 0 0;0.875 0 0;0.8125 0 0;0.75 0 0;0.6875 0 0;0.625 0 0;0.5625 0 0];
            app.figure1.Position = [520 214 624 586];
            app.figure1.Name = 'IK';
            app.figure1.Resize = 'off';
            app.figure1.HandleVisibility = 'callback';
            app.figure1.Tag = 'figure1';

            % Create axes1
            app.axes1 = uiaxes(app.figure1);
            app.axes1.Colormap = [0 0 0.5625;0 0 0.625;0 0 0.6875;0 0 0.75;0 0 0.8125;0 0 0.875;0 0 0.9375;0 0 1;0 0.0625 1;0 0.125 1;0 0.1875 1;0 0.25 1;0 0.3125 1;0 0.375 1;0 0.4375 1;0 0.5 1;0 0.5625 1;0 0.625 1;0 0.6875 1;0 0.75 1;0 0.8125 1;0 0.875 1;0 0.9375 1;0 1 1;0.0625 1 1;0.125 1 0.9375;0.1875 1 0.875;0.25 1 0.8125;0.3125 1 0.75;0.375 1 0.6875;0.4375 1 0.625;0.5 1 0.5625;0.5625 1 0.5;0.625 1 0.4375;0.6875 1 0.375;0.75 1 0.3125;0.8125 1 0.25;0.875 1 0.1875;0.9375 1 0.125;1 1 0.0625;1 1 0;1 0.9375 0;1 0.875 0;1 0.8125 0;1 0.75 0;1 0.6875 0;1 0.625 0;1 0.5625 0;1 0.5 0;1 0.4375 0;1 0.375 0;1 0.3125 0;1 0.25 0;1 0.1875 0;1 0.125 0;1 0.0625 0;1 0 0;0.9375 0 0;0.875 0 0;0.8125 0 0;0.75 0 0;0.6875 0 0;0.625 0 0;0.5625 0 0];
            app.axes1.FontSize = 13.3333333333333;
            app.axes1.NextPlot = 'replace';
            app.axes1.Tag = 'axes1';
            app.axes1.Position = [25 188 567 384];

            % Create textEucE
            app.textEucE = uilabel(app.figure1);
            app.textEucE.Tag = 'textEucE';
            app.textEucE.HorizontalAlignment = 'center';
            app.textEucE.VerticalAlignment = 'top';
            app.textEucE.WordWrap = 'on';
            app.textEucE.FontSize = 10.6666666666667;
            app.textEucE.Position = [133 129 79 17];
            app.textEucE.Text = '0';

            % Create text4
            app.text4 = uilabel(app.figure1);
            app.text4.Tag = 'text4';
            app.text4.HorizontalAlignment = 'center';
            app.text4.VerticalAlignment = 'top';
            app.text4.WordWrap = 'on';
            app.text4.FontSize = 10.6666666666667;
            app.text4.Position = [331 98.0000000000001 52 22];
            app.text4.Text = '0';

            % Create text3
            app.text3 = uilabel(app.figure1);
            app.text3.Tag = 'text3';
            app.text3.HorizontalAlignment = 'center';
            app.text3.VerticalAlignment = 'top';
            app.text3.WordWrap = 'on';
            app.text3.FontSize = 10.6666666666667;
            app.text3.Position = [45 43 24 20];
            app.text3.Text = 'z';

            % Create text2
            app.text2 = uilabel(app.figure1);
            app.text2.Tag = 'text2';
            app.text2.HorizontalAlignment = 'center';
            app.text2.VerticalAlignment = 'top';
            app.text2.WordWrap = 'on';
            app.text2.FontSize = 10.6666666666667;
            app.text2.Position = [45 70 24 20];
            app.text2.Text = 'y';

            % Create text1
            app.text1 = uilabel(app.figure1);
            app.text1.Tag = 'text1';
            app.text1.HorizontalAlignment = 'center';
            app.text1.VerticalAlignment = 'top';
            app.text1.WordWrap = 'on';
            app.text1.FontSize = 10.6666666666667;
            app.text1.Position = [45 101 24 20];
            app.text1.Text = 'x';

            % Create slider1
            app.slider1 = uislider(app.figure1);
            app.slider1.Limits = [-20 20];
            app.slider1.MajorTicks = [];
            app.slider1.ValueChangedFcn = createCallbackFcn(app, @slider1_Callback, true);
            app.slider1.MinorTicks = [];
            app.slider1.Tag = 'slider1';
            app.slider1.FontSize = 10.6666666666667;
            app.slider1.Position = [69 102 263 3];

            % Create slider2
            app.slider2 = uislider(app.figure1);
            app.slider2.Limits = [2 20];
            app.slider2.MajorTicks = [];
            app.slider2.ValueChangedFcn = createCallbackFcn(app, @slider2_Callback, true);
            app.slider2.MinorTicks = [];
            app.slider2.Tag = 'slider2';
            app.slider2.FontSize = 10.6666666666667;
            app.slider2.Position = [69 71 262 3];
            app.slider2.Value = 12;

            % Create slider3
            app.slider3 = uislider(app.figure1);
            app.slider3.Limits = [0 20];
            app.slider3.MajorTicks = [];
            app.slider3.ValueChangedFcn = createCallbackFcn(app, @slider3_Callback, true);
            app.slider3.MinorTicks = [];
            app.slider3.Tag = 'slider3';
            app.slider3.FontSize = 10.6666666666667;
            app.slider3.Position = [69 42 263 3];
            app.slider3.Value = 2;

            % Create text5
            app.text5 = uilabel(app.figure1);
            app.text5.Tag = 'text5';
            app.text5.HorizontalAlignment = 'center';
            app.text5.VerticalAlignment = 'top';
            app.text5.WordWrap = 'on';
            app.text5.FontSize = 10.6666666666667;
            app.text5.Position = [332 67 52 22];
            app.text5.Text = '12';

            % Create text6
            app.text6 = uilabel(app.figure1);
            app.text6.Tag = 'text6';
            app.text6.HorizontalAlignment = 'center';
            app.text6.VerticalAlignment = 'top';
            app.text6.WordWrap = 'on';
            app.text6.FontSize = 10.6666666666667;
            app.text6.Position = [332 38 52 22];
            app.text6.Text = '2';

            % Create inversebutton
            app.inversebutton = uibutton(app.figure1, 'push');
            app.inversebutton.ButtonPushedFcn = createCallbackFcn(app, @inversebutton_Callback, true);
            app.inversebutton.Tag = 'inversebutton';
            app.inversebutton.FontSize = 10.6666666666667;
            app.inversebutton.Position = [393 85 192 41];
            app.inversebutton.Text = 'PseudoInverse Jacobian IK';

            % Create Reset
            app.Reset = uibutton(app.figure1, 'push');
            app.Reset.ButtonPushedFcn = createCallbackFcn(app, @Reset_Callback, true);
            app.Reset.Tag = 'Reset';
            app.Reset.FontSize = 10.6666666666667;
            app.Reset.Position = [393 40 191 40];
            app.Reset.Text = 'Reset';

            % Create text9
            app.text9 = uilabel(app.figure1);
            app.text9.Tag = 'text9';
            app.text9.HorizontalAlignment = 'center';
            app.text9.VerticalAlignment = 'top';
            app.text9.WordWrap = 'on';
            app.text9.FontSize = 10.6666666666667;
            app.text9.Position = [76 126 74 20];
            app.text9.Text = 'Position Error :';

            % Create SetPointButton
            app.SetPointButton = uibutton(app.figure1, 'push');
            app.SetPointButton.ButtonPushedFcn = createCallbackFcn(app, @SetPointButtonPushed, true);
            app.SetPointButton.Position = [484 137 100 22];
            app.SetPointButton.Text = 'Set Point';

            % Create TimesEditFieldLabel
            app.TimesEditFieldLabel = uilabel(app.figure1);
            app.TimesEditFieldLabel.HorizontalAlignment = 'right';
            app.TimesEditFieldLabel.Position = [194 137 49 22];
            app.TimesEditFieldLabel.Text = 'Time (s)';

            % Create TimesEditField
            app.TimesEditField = uieditfield(app.figure1, 'numeric');
            app.TimesEditField.RoundFractionalValues = 'on';
            app.TimesEditField.ValueChangedFcn = createCallbackFcn(app, @TimesEditFieldValueChanged, true);
            app.TimesEditField.Position = [258 137 100 22];

            % Show the figure after all components are created
            app.figure1.Visible = 'on';
        end
    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = meArm_Coordinate_App(varargin)

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
            fclose(app.fileID)
            % Delete UIFigure when app is deleted
            delete(app.figure1)
        end
    end
end