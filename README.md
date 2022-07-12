SCD41
=====

Copyright (c) 2022 BitBank Software, Inc.<br>
Written by Larry Bank<br>
larry@bitbanksoftware.com

**I optimize other people's code for a living. This library is a good example of the kind of work I do for my commercial clients; it contains many unique and clever optimizations that allows it to perform better than anything else available. I'm happy to contribute optimized libraries to the open source community in addition to working on commercial projects. Whatever platform you're using, I can make significant improvements to your native code. Please contact me so that I can show you how.**

![SCD41](/demo.jpg?raw=true "SCD41")

Features:
---------

- Supports all of the power saving modes of the SCD40/SCD41
- Simple class with a minimum of methods
- Depends on my BitBang_I2C library

I wrote this because the existing SCD41 libraries didn't do what I needed - specifically I wanted to communicate with the sensor on a random set of GPIO pins using bit banging. By using my BitBang_I2C library, this library can talk to the sensor in a more flexible way.

