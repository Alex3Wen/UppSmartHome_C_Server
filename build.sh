arm-linux-gcc *.c -o ev_server -I./libev/include/ -I./ -I./sqlite3/include/ -L./sqlite3/lib/ -L./libev/lib/ -lev -lsqlite3 -lpthread && cp ev_server /home/gerrie/up
