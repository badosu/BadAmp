BUNDLE = amp.lv2
INSTALL_DIR = /usr/local/lib/lv2

$(BUNDLE): manifest.ttl amp.ttl amp.so amp_gui_gtk.so amp_gui_fltk.so amp_gui_qt.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

amp.so: amp.c
	gcc -shared -fPIC -DPIC amp.c -o amp.so

amp_gui_gtk.so: amp_gui_gtk.c
	gcc -shared -fPIC -DPIC amp_gui_gtk.c `pkg-config --cflags --libs gtk+-2.0` -o amp_gui_gtk.so

amp_gui_fltk.so: amp_gui_fltk.cxx
	g++ -shared -fPIC -lcairo -I/usr/include/ntk -lntk -Wl,-z,nodelete amp_gui_fltk.cxx -o amp_gui_fltk.so

# See: http://qt-project.org/doc/qt-4.8/moc.html#writing-make-rules-for-invoking
amp_gui_qt.o: amp_gui_qt.moc.cpp

amp_gui_qt.moc.cpp: amp_gui_qt.cxx
	moc-qt4 $(DEFINES) $(INCPATH) -i $< -o $@

amp_gui_qt.so: amp_gui_qt.cxx amp_gui_qt.moc.cpp
	g++ -shared -fPIC -Wl,--no-undefined `pkg-config --cflags --libs QtCore QtGui` amp_gui_qt.cxx -o amp_gui_qt.so

clean:
	rm -rf $(BUNDLE) *.so

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
