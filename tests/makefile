TESTS = test_scheduler.c test_stimer.c
SRC = ../src/*.c
INC = ../inc ../tests
BUILD = build
FRAMEWORK = framework.c framework.h

CFLAGS = $(foreach PATH,$(INC),-I $(PATH))

run : build_tests
	@ $(foreach FILE,$(basename $(TESTS)),\
		echo '$(FILE).c:';\
		./$(BUILD)/$(FILE).elf;\
	)

build_tests : $(TESTS) $(FRAMEWORK) $(SRC)
	$(foreach TEST_C,$(TESTS),\
		$(CC) $(CFLAGS) $(TEST_C) $(FRAMEWORK) $(SRC) -o $(BUILD)/$(basename $(TEST_C)).elf;\
	)
