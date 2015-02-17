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

Run it
------

Dependencies:

- [Gtk2](http://www.gtk.org/)
- [NTK](http://non.tuxfamily.org/ntk/)

You'll also need a LV2 host to test it, I recommend
[Jalv](http://drobilla.net/software/jalv/).

Install with: `sudo make install`

Test with:    `jalv.gtk http://bado.so/bad-amp`

Connect it to a sound input and output with [JACK](http://jackaudio.org/) and you're good to go!

![Jack Connection](https://github.com/badosu/BadAmp/raw/master/screenshots/jack-connection.png)
