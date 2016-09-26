OUTPUT = SajeMonitor
CXX = g++-4.7
CXX_FLAGS = -std=c++11 -Wall -Werror
PROJECT_DIR = .
TESTS_DIR = $(PROJECT_DIR)/tests
GTEST_DIR = $(PROJECT_DIR)/gtest-1.7.0
MODELS_DIR = $(PROJECT_DIR)/models
SOURCES = SajeMonitor.cpp AlsaSource.cpp AlsaSink.cpp PocketSphinxKWS.cpp
TEST_SOURCES = $(TESTS_DIR)/test_SajeMonitor.cpp
LIBS = -lwiringPi -lpocketsphinx -lsphinxbase -lsphinxad -lasound -lpthread
INCLUDES = -I/usr/local/include/sphinxbase -I/usr/local/include/pocketsphinx
TEST_INCLUDES = -I$(PROJECT_DIR) -isystem $(GTEST_DIR)/include -I$(GTEST_DIR)


SajeMonitor: $(SOURCES) main.cpp
	$(CXX) $(CXX_FLAGS) $(INCLUDES) $(SOURCES) main.cpp -o $(OUTPUT) $(LIBS)

run: SajeMonitor
	./SajeMonitor -hmm $(MODELS_DIR)/hub4wsj_sc_8k/ -dict $(MODELS_DIR)/cmu07a.dic -lm $(MODELS_DIR)/wsj0vp.5000.DMP

runtests: tests
	./testrunner

tests: $(TEST_SOURCES) gtest-all.o gtest_main.o
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) $(TEST_SOURCES) -pthread $(LIBS) gtest_main.o gtest-all.o -o testrunner

AlsaSourceRecorder: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) ./tests/AlsaSourceRecorder.cpp -o AlsaSourceRecorder $(LIBS)

AlsaSinkPlayer: $(SOURCES)
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) $(INCLUDES) $(SOURCES) ./tests/AlsaSinkPlayer.cpp -o AlsaSinkPlayer $(LIBS)

gtest-all.o:
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) -pthread -c $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o:
	$(CXX) $(CXX_FLAGS) $(TEST_INCLUDES) -pthread -c $(GTEST_DIR)/src/gtest_main.cc

clean:
	rm -f *.o
	rm -f $(OUTPUT)
	rm -f testrunner
	rm -f AlsaSourceRecorder
	rm -f AlsaSinkPlayer
