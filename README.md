STM32L1 SDCARD Audio Playback Example
=====================================

This is ST application note 4309, ["Playing audio file interfacing STM32L1xx microcontroller with an external I2S audio codec"](http://www.st.com/web/en/catalog/tools/PF259349), patched with missing source files and makefiles for the gcc-arm-embedded toolchain. See [readme.txt](Project/I2S_Audio_Codec/Media/USER/readme.txt) from the original appnote.

Environmen Setup
----------------

* Download and extract the [gcc-arm-embedded](https://launchpad.net/gcc-arm-embedded) toolchain (known to build with `4_8-2014q2`)
* Update your `PATH` environment variable, e.g.

`$ export PATH=/opt/gcc-arm-none-eabi-4_8-2014q2/bin:$PATH`


Building the Code
-----------------

* `cd stm32l1-audio-stsw-stm32135/Projects/I2S_Audio_Codec`
* `make`

Notes
-----

* To see the code changes, grab [stsw-stm32135-make-it-build.patch](http://scottt.tw/stsw-stm32135-make-it-build.patch)
