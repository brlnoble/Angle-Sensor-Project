%Brandon Noble April 2020

if ~isempty(instrfind) %if port is already open, close it so we can use it for serial communication
     fclose(instrfind);
     delete(instrfind);
end

s = serial('COM10', 'BaudRate',38400); %configure COM port 10 to be serially read at 38400 baud rate
fopen(s);

h = animatedline('MaximumNumPoints',30,'Color','r','Linewidth',3); %speeds up execution by deleting older values no longer displayed on graph
xlabel('Time in Seconds');
ylabel('Angle in Degrees')
title('Plot of Angle VS Time')

t = 0.0; %used to increment the time axis on the graph

mode = 0; %0 for X values, 1 for Y values

while 1==1
    read = fscanf(s);
    if read(1) == 'E' %if communication has ended, stop the code from running
        break;
    elseif read(1) == 'S' %ignore communication start up
        continue;
    elseif read(1) == 'Y'
        mode = 1; %change to Y reading mode
        h = animatedline('Color','b','Linewidth',3); %plot Y values in blue
    elseif read(1) == 'X'
        mode = 0; %change to X reading mode
        h = animatedline('Color','r','Linewidth',3); %plot X values in red
    else         
        read = string(read); %x is read as a character array, perform conversions to get it as a double
        read = double(read)
        
        if mode == 0 %X reading mode
            angle = floor((read-2077)/(2483-2077) * 90); %0deg ~ 2077, 90deg ~ 2483. This normalizes the value and multiplies by 90deg to get result
        
        else %Y reading mode
            angle = floor((read-2015)/(2427-2015) *90); %0deg ~ 2015, 90deg ~ 2427. This normalizes the value and multiplies by 90deg to get the result
        end
        
        addpoints(h,t,angle); %add new point to graph
        drawnow
        axis([t-5,t,-100,100]) %keeps the current value centred in the graph
        t = t+0.2; %increment time

        angVal = string(angle); %convert angle to string so it can be displayed on the graph
        delete(findall(gcf,'type','annotation')) %delete previous, and draw the current value as text to the right of the graph
        annotation('textbox',[.9 .5 0 0],'String',angVal)
        
        angle = char(abs(angle)); %convert angle to a character to be sent to the Esduino. Absolute of the angle allows us to also read negative values
        fprintf(s, '%s', angle); %format and send character to the Esduino
    end
end

pause; %allow user to view the plot before clearing it, hit backspace to continue
clearpoints(h); %clear and close the plot
close;
fclose(s); %close the port properly when code is finished running
delete(s);
clc;
clear;
