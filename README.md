BadAmp - A simple lv2 amplifier plugin
======================================

An implementation of the LV2 amp plugin at [LV2 Book](http://lv2plug.in/book/).

Many different GUIs are implemented, the idea is to compare the different
GUI implementations for each toolkit.

See some screenshots:

NTK
---

Uses code from [a simple lv2 sin synth](https://github.com/harryhaaren/ntk-lv2-ui)

![BadAmp on NTK](https://github.com/badosu/BadAmp/raw/master/screenshots/ntk.png)

GTK
---

Based on [amp_with_gui](https://github.com/Joeboy/joeboy-lv2-plugins/tree/master/amp_with_gui)

![BadAmp on GTK](https://github.com/badosu/BadAmp/raw/master/screenshots/gtk.png)

QT4
---

![BadAmp on QT](https://github.com/badosu/BadAmp/raw/master/screenshots/qt4.png)

Setup your LV2 bundle
---------------------

Create the DSP plugin and bundle directory with `make`.

If you want you can make any of the interface binaries available, check
if you meet the dependency requirements first.

Bear in mind that hosts usually have an internal logic to decide which
UI they should use, so if you want to check a particular interface
you'll have to make just that one.

You can always `sudo make uninstall` later and make a different gui,
or you can `make all` to make all of them at once.

### [Gtk2](http://www.gtk.org/)

`make amp_gui_gtk.so`

### [Qt4](http://www.qt.io/)

`make amp_gui_qt.so`

### [NTK](http://non.tuxfamily.org/ntk/) and [Cairo](http://cairographics.org/)

`make amp_gui_fltk.so`

Install and Test
----------------

After setting up your bundle, use: `sudo make install` to install it.

If you have problems you can check if your installation was successful
by checking if `http://bado.so/bad-amp` appears on the list provided by
the command `lv2ls`.

You'll need a LV2 host to test it, I recommend
[Jalv](http://drobilla.net/software/jalv/).

Test with:    `jalv.gtk http://bado.so/bad-amp`

Connect it to a sound input and output with [JACK](http://jackaudio.org/) and you're good to go!

![Jack Connection](https://github.com/badosu/BadAmp/raw/master/screenshots/jack-connection.png)
