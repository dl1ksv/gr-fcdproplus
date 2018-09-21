title: gr-fcdproplus
brief: A GNU Radio funcube dongle pro+ source
tags:
  - funcube
author:
  - Volker Schroer
copyright_owner:
  - Volker Schroer
dependencies:
  - gnuradio (>= 3.8.0 )
  - alsa usb drivers activated
  - hidapi-libusb
repo: https://github.com/dl1ksv/gr-fcdproplus.git
---
gr-fcdproplus is an linux and OSX addon for gnuradio to implement a funcube dongle pro+ source.
On linux it autodetects the correct soundcard from /proc/asound/cards.
This idea was taken from the osmosdr drivers.  To control the device  the hidapi usb version is used.

It contains two components:

Funcube dongle Pro+         - The complete source block 

Funcube dongle Pro+ control - The control block, to set frequency, gain, etc.

This block is used inside the source block but can be used standalone, too.

You find these blocks in

custom - fcd in grc


