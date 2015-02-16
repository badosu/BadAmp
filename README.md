BadAmp - A simple lv2 amplifier plugin
======================================

A shameless fork of Joeboy's
[amp_with_gui](https://github.com/Joeboy/joeboy-lv2-plugins/tree/master/amp_with_gui),
which in turn implements the amp plugin at [LV2 Book](http://lv2plug.in/book/)
with the gtk gui addition.

See a screenshot:

![Screenshot of BadAmp](https://github.com/badosu/BadAmp/raw/master/screenshot.png)

Run it
------

You'll need gtk2 headers and a LV2 host to test it, I recommend
[Jalv](http://drobilla.net/software/jalv/).

Install with: `sudo make install`

Test with:    `jalv.gtk http://bado.so/bad-amp`

Connect it to a sound input and output with [JACK](http://jackaudio.org/) and you're good to go!

![Jack Connection](https://github.com/badosu/BadAmp/raw/master/jack-connection.png)
