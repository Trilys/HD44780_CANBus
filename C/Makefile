hd44780_sender: hd44780_sender.o can_min.o lib_can.o
	gcc can_min.c lib_can.c hd44780_sender.c -o hd44780_sender

clean:
	rm -rf *.o

mrproper: clean
	rm -rf hd44780_sender
