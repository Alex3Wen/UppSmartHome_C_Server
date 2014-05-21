arm-linux-gcc -g *.c -o ev_server_debug -I./libev/include/ -I./ -L./libev/lib/ -lev -lpthread
cp ev_server_debug /home/ftp/ftp
