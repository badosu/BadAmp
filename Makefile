BUNDLE = amp.lv2
INSTALL_DIR = /usr/local/lib/lv2

$(BUNDLE): manifest.ttl amp.ttl amp.so
	rm -rf $(BUNDLE)
	mkdir $(BUNDLE)
	cp $^ $(BUNDLE)

all: $(BUNDLE) amp_gui_gtk.so amp_gui_fltk.so amp_gui_qt.so

amp.so: amp.c
	gcc -shared -fPIC -DPIC $< -o $@

amp_gui_gtk.so: amp_gui_gtk.c $(BUNDLE)
	gcc -shared -fPIC -DPIC `pkg-config --cflags --libs gtk+-2.0` $< -o $@
	cp $@ $(BUNDLE)

amp_gui_fltk.so: amp_gui_fltk.cpp $(BUNDLE)
	g++ -shared -fPIC -lcairo -I/usr/include/ntk -lntk -Wl,-z,nodelete $< -o $@
	cp $@ $(BUNDLE)

# See: http://qt-project.org/doc/qt-4.8/moc.html#writing-make-rules-for-invoking
amp_gui_qt.o: amp_gui_qt.moc.cpp

amp_gui_qt.moc.cpp: amp_gui_qt.cpp
	moc-qt4 $(DEFINES) $(INCPATH) -i $< -o $@

amp_gui_qt.so: amp_gui_qt.cpp amp_gui_qt.moc.cpp $(BUNDLE)
	g++ -shared -fPIC -Wl,--no-undefined `pkg-config --cflags --libs QtCore QtGui` $< -o $@
	cp $@ $(BUNDLE)

clean:
	rm -rf $(BUNDLE) *.so *.moc.cpp

install: $(BUNDLE)
	mkdir -p $(INSTALL_DIR)
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
	cp -R $(BUNDLE) $(INSTALL_DIR)

uninstall:
	rm -rf $(INSTALL_DIR)/$(BUNDLE)
