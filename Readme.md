gr-fcdproplus is an linux and OSX addon for gnuradio to implement a funcube dongle pro+ source.
On linux it autodetects the correct soundcard from /proc/asound/cards.
This idea was taken from the osmosdr drivers.

To control the device the hidapi usb version is used.




1.   Dependencies:

  - gnuradio
  - alsa usb drivers activated
  - hidapi-libusb 


If the hidapi lib of your distro is installed then this hidapi version is used. Otherwise bundled code will be used.
In case of MAC OSX this is hidmac.c otherwise hid.c
( This new MAC Version is untested, due to a lack of access to MAC OSX. So reports of success or bugs are welcome. )


2.   Installation:

get the latest code from https://github.com/dl1ksv/gr-fcdproplus

The build process is cmake based. So change to code directory.
There
$mkdir build
$cd build
$cmake -DCMAKE_INSTALL_PREFIX=<where gnuradio is installed> ../
$make
$su
$make install

After that you'll find in grc in Sources
Funcube Dongle Pro+ source


3.   Important

Don't forget the udev rules:
For instance:

  # Udev rules for the Funcube Dongle Pro (0xfb56) and Pro+ (0xfb31)

  # HIDAPI/libusb:
  SUBSYSTEMS=="usb" ATTRS{idVendor}=="04d8" ATTRS{idProduct}=="fb56" MODE:="0666"
  SUBSYSTEMS=="usb" ATTRS{idVendor}=="04d8" ATTRS{idProduct}=="fb31" MODE:="0666"

  # HIDAPI/hidraw:
  KERNEL=="hidraw*", ATTRS{busnum}=="1", ATTRS{idVendor}=="04d8", ATTRS{idProduct}=="fb56", MODE="0666"
  KERNEL=="hidraw*", ATTRS{busnum}=="1", ATTRS{idVendor}=="04d8", ATTRS{idProduct}=="fb31", MODE="0666"

4.    Usage

Two modules are available:

   - fcdpp_control
   - fcdproplus

The first module only controls the dongle, while the second module controls the dongle by fcdpp_control and provides the IQ samples.
Both modules accept messages to control the frequency. That was the reason to introduce the fcdpp_control module.

See the examples of a simple fm receiver in the examples directory or see a more complex example in in gr-display


5.    Credits

Some code is taken from qthid-4.1 by Alexandru Csete, OZ9AEC.
The hid part is taken from
 http://github.com/signal11/hidapi

Thanks to Micael Dickens, who gave valuable hints for implementing message in hier2_blocks.
