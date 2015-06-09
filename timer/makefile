INCLUDE_PATH = -I../boost -I../libevent/include
LIB_PATH = -L../boost/lib -L../libevent/lib
LDFLAGS = -lpthread -lrt
TARGET = timer

timer: timers_container.o main.o
	g++ timers_container.o main.o -o $(TARGET) $(LIB_PATH) $(LDFLAGS) 

timers_container.o: timers_container.cpp
	g++ -c timers_container.cpp $(INCLUDE_PATH)

main.o: main.cpp
	g++ -c main.cpp $(INCLUDE_PATH)

clean:
	rm -rf $(OBJS) $(TARGET)
