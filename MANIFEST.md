title: gr-fcdproplus
brief: A GNU Radio funcube dongle pro+ source
tags:
  - funcube
author:
  - Volker Schroer
copyright_owner:
  - Volker Schroer
dependencies:
  - gnuradio
  - alsa usb drivers activated
  - hidapi-libusb 
repo: https://github.com/dl1ksv/gr-fcdproplus.git
---
gr-fcdproplus is an linux and OSX addon for gnuradio to implement a funcube dongle pro+ source.
On linux it autodetects the correct soundcard from /proc/asound/cards.
This idea was taken from the osmosdr drivers.  To control the device  the hidapi usb version is used.
