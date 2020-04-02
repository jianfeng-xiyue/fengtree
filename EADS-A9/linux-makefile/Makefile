TARGET       := rio
SOURCE       := ./*.c

$(TARGET):$(SOURCE) 
	gcc -g $^ -o $@ -lpthread -L$(MODBUS_LIB) -lmodbus
clean:
	rm -rf ./*.o
install:
	cp ./*.o $(INSTALL_PATH)

