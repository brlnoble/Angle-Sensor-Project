<h1> Angle-Sensor-Project </h1>

<p>This was part of a school project to create an angle sensor using an accelerometer and an old microcontroller called the <em>Esduino Extreme</em>.</p>

<hr>

<p>I was asked to read the angles on both the X and Y axis of the accelerometer, and display this on a laptop screen using another software of my choice. Using serial communication to send and receive data, it was also required to switch between the X and Y axis reading modes. Additionally (not shown here), a BCD display of LEDs was also used to show the angle.</p>

<p>The final result creates a graph such as the following.</p>

<img src="https://github.com/brlnoble/Angle-Sensor-Project/blob/master/resultant-graph.jpg?raw=true" title="Final Result">
<p>This result scrolls by in real time as the accelerometer continuously measures the current angle. The measured value is also displayed to the right side of the screen, seen here as <em>40</em>. Currently the X axis of the accelerometer is being displayed, evident by the <em>red</em> colour shown here. For the Y axis, a blue line is shown instead.</p>

<hr>

<p>Below is a pinout diagram of the Esduino Extreme.</p>
<img src="https://github.com/brlnoble/Angle-Sensor-Project/blob/master/esduino_pinout.jpg?raw=true" width=600px>

<ol type="a">
  <li>3.3V source for accelerometer</li>
  <li>5V source for the interrupt buttons</li>
  <li>Ground for the accelerometer and interrupt buttons</li>
  <li>ADC input for the X axis</li>
  <li>ADC input for the Y axis</li>
  <li>Interrupt input for mode control (X or Y)</li>
  <li>Interrupt input for serial mode control (enable/disable)</li>
  <li>BCD for most significant digit</li>
  <li>BCD for most significant digit</li>
  <li>BCD for most significant digit</li>
  <li>BCD for most significant digit</li>
  <li>BCD for least significant digit</li>
  <li>BCD for least significant digit</li>
  <li>BCD for least significant digit</li>
  <li>BCD for least significant digit</li>
  <li>Ground for LEDs</li>
</ol>

<hr>

<p>When completing the project, there was a lot of noise in the intput signal (likely a result of thermal noise). To avoid this, a high sample rate was used to average 20 distinct readings in quick succession, resulting in a much smoother output graph. The difference can be seen below.</p>

<img src="https://github.com/brlnoble/Angle-Sensor-Project/blob/master/noisy-graph-before-fix.jpg?raw=true" title="Before Fix" width="500px">
<img src="https://github.com/brlnoble/Angle-Sensor-Project/blob/master/noisy-graph-after-fix.jpg?raw=true" title="After Fix" width="500px">

<hr>

<p>In completing this project, I learned a lot about serial communication and working with transducers. My next plans are to create a stabylizing gimbal for a Raspberry Pi camera.</p>
