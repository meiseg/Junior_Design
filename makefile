# Define the default target
all: myprogram

# Define the myprogram target
myprogram: $(file)
	
	
	 g++ $(file).cpp -o main `pkg-config gtkmm-3.0 opencv4 --cflags --libs` -I/usr/include/gtmm-3.0/ -I/usr/include/opencv4/opencv2
	./main &
debug:
	gdb a.out
# Define the clean target
clean:
	rm -f myprogram
