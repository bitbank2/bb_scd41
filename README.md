bb_scd41
========

Copyright (c) 2022 BitBank Software, Inc.<br>
Written by Larry Bank<br>
larry@bitbanksoftware.com

![SCD41](/demo.jpg?raw=true "SCD41")

Features:
---------

- Supports all of the power saving modes of the SCD40/SCD41
- Simple class with a minimum of methods
- Depends on my BitBang_I2C library

I wrote this because the existing SCD41 libraries didn't do what I needed - specifically I wanted to communicate with the sensor on a random set of GPIO pins using bit banging. By using my BitBang_I2C library, this library can talk to the sensor in a more flexible way.

