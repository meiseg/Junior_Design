# Define the default target
all: myprogram

# Define the myprogram target
myprogram: $(file)
	g++ $(file) -o main `pkg-config gtkmm-3.0 webkit2gtk-4.0 aubio opencv4 --cflags --libs` -lportaudio -I/home/mrescoba/Junior_Design/
	./main &
debug:
	gdb a.out
# Define the clean target
clean:
	rm -f myprogram
