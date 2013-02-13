BF-FFT-Demo
===========
This small GUI application (based on the [Qt 4.7 Framework](http://qt-project.org)) was used to demonstrate the [FFT algorithm](http://en.wikipedia.org/wiki/Fft) implemented on an [ADSP-BF532 DSP](http://en.wikipedia.org/wiki/Blackfin) in a [student project at ETH Zurich (link in German)](http://www.isi.ee.ethz.ch/teaching/pps/prac_dsp).
The assembly source code used for the algorithm on the DSP is available in another repository.


## Usage
### Important reminder
Please keep in mind that this project is used to communicate with a very specific DSP setup which is not publicly available. Don't expect anything to work out of the box. You may find some inspiration on certain approaches for your own projects here, though.
### Drivers
Communication over USB uses the [FT232R USB UART IC made by FTDI](http://www.ftdichip.com/Products/ICs/FT232R.htm). Make sure you have a current and appropriate version of their [Virtual COM Port drivers](http://www.ftdichip.com/Drivers/VCP.htm) installed.
### Firmware
The core firmware/bootloader that initializes processor, audio codec and USB UART is, in our case, flashed to the board and not publicly available.
Because the connection module makes strong assumptions about the BF firmware, it won't work with your installation without customization unless you own that specific board configuration.
The FFT processing source code, however, is available (as linked in the initial description).


## Configuration

### Display Settings
Pretty self explanatory, adjust to your needs accordingly.

### FFT/Codec Settings
* The *Number of FFT Points* set here must be the same the algorithm computes.
* *Sample Rate* is determined by codec settings (changes must be made in source code).
* *FFT Precision* is computed out of the former two settings.

### Connection Settings
* *Baud Rate* is again hard coded. The default value works well with most systems. However, if you encounter problems adjusting this setting might help.
* *USB device* is specific to your system. Do a `ls -l *usb*` in your system's `/dev` directory and watch out for `tty` devices. Enter the absolute path to the device found.
* *Hex File* points to the (absolute) path of your algorithm hex file.
