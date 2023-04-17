# Define the default target
all: myprogram


c: $(c)
	gcc $(c)
	./a.out
# Define the myprogram target
myprogram: $(file)
	
	
	 g++ $(file) -o main `pkg-config gtkmm-3.0 --cflags --libs` -I/usr/include/gtmm-3.0/
	./main &
debug:
	gdb a.out
# Define the clean target
clean:
	rm -f myprogram