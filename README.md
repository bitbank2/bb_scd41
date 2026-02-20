bb_scd41
========

Copyright (c) 2022-2026 BitBank Software, Inc.<br>
Written by Larry Bank<br>
larry@bitbanksoftware.com

![SCD41](/demo.jpg?raw=true "SCD41")

Features:
---------

- Supports all of the power saving modes of the SCD40/SCD41/SCD43
- Simple C++ class with a minimum of methods
- Portable code runs on Arduino, Linux and esp-idf

I wrote this because the existing SCD41 libraries didn't do what I needed - specifically I wanted to communicate with the sensor on a random set of GPIO pins using bit banging. By using my BitBang_I2C library, this library can talk to the sensor in a more flexible way.

## Power Savings
The SCD41 and SCD43 include a 'single shot' mode which allows triggering a single sample from idle mode. The sample takes 5 seconds to capture after being triggered. By triggering samples on an infrequent basis, the average power draw can drop well below 1mA. In idle mode the sensor draws on average 2mA. This is way too much for multi-month battery life, so it's necessary to put it in shutdown mode. NB: the last captured sample data will be discarded in shutdown mode. So... for lowest power usage it is necessary to trigger the sample, wait 5 seconds, then save the sample on the host CPU before shutting down the SCD4x.


If you find this code useful, please consider becoming a sponsor or sending a donation.

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SR4F44J2UR8S4)

## Funding

This project received funding through [NGI Zero Core](https://nlnet.nl/core), a fund established by [NLnet](https://nlnet.nl) with financial support from the European Commission's [Next Generation Internet](https://ngi.eu) program. Learn more at the [NLnet project page](https://nlnet.nl/project/ImageCodes-Optimised).

[<img src="https://nlnet.nl/logo/banner.png" alt="NLnet foundation logo" width="20%" />](https://nlnet.nl)
[<img src="https://nlnet.nl/image/logos/NGI0_tag.svg" alt="NGI Zero Logo" width="20%" />](https://nlnet.nl/core)

