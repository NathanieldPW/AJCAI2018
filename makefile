COMPILE=g++ -std=c++11 -c -O2

all: main

main: train.o grid_world.o experiments.o grid_world_tabular_agent.o
	g++ -std=c++11 -O2 main.cpp experiments.o grid_world_tabular_agent.o grid_world.o position.o train.o counter.o logger.o log.o utils.o -o main

#map_generator: map_generator.cpp
#	g++ -std=c++11 -O2 map_generator.cpp -o map_generator

clean:
	rm -f *.o
	rm -f main

grid_world_tabular_agent.o: logger.o grid_world.o grid_world_tabular_agent.h grid_world_tabular_agent.cpp
	$(COMPILE) grid_world_tabular_agent.cpp

experiments.o: grid_world.o experiments.h experiments.cpp
	$(COMPILE) experiments.cpp

grid_world.o: position.o grid_world.h grid_world.cpp
	$(COMPILE) grid_world.cpp

position.o: position.h position.cpp
	$(COMPILE) position.cpp

train.o: counter.o logger.o utils.o train.h train.cpp
	$(COMPILE) train.cpp

counter.o: counter.h counter.cpp
	$(COMPILE) counter.cpp

logger.o: log.o logger.h logger.cpp
	$(COMPILE) logger.cpp

log.o: utils.o log.h log.cpp
	$(COMPILE) log.cpp

utils.o: utils.h utils.cpp
	$(COMPILE) utils.cpp

#counter.o: counter.h
#	$(COMPILE) counter.h

#logger.o: log.o logger.h
#	$(COMPILE) logger.h

#log.o: log.h
#	$(COMPILE) log.h

#grid_world.o: grid_world.h
#	$(COMPILE) grid_world.h

#grid_world_tabular_agent.o: grid_world.o grid_world_tabular_agent.h
#	$(COMPILE) grid_world_tabular_agent.h

