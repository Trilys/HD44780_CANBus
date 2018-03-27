all: hd44780_sender hd44780_receivePushButtons hd44780_demo

hd44780_sender: hd44780_sender.o can_min.o lib_can.o
	gcc can_min.c lib_can.c hd44780_sender.c -o hd44780_sender

hd44780_receivePushButtons: hd44780_receivePushButtons.o can_min.o lib_can.o
	gcc can_min.c lib_can.c hd44780_receivePushButtons.c -o hd44780_receivePushButtons

hd44780_demo: hd44780_demo.o can_min.o lib_can.o
	gcc can_min.c lib_can.c hd44780_demo.c -o hd44780_demo

clean:
	rm -rf *.o

install:
	cp hd44780_receivePushButtons /usr/local/bin/
	cp hd44780_sender /usr/local/bin/
	cp hd44780_demo /usr/local/bin/

mrproper: clean
	rm -rf hd44780_sender
	rm -rf hd44780_receivePushButtons
	rm -rf hd44780_demo
