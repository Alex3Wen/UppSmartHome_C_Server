#ifndef __RFID_HANDLER
#define __RFID_HANDLER
extern int RFID_verify(unsigned int id);
extern int delete_ID(unsigned int id);
extern int get_rows_num();
extern int register_ID(unsigned int id);
extern int init_sqlite_RFID();
extern unsigned int encrypt(unsigned int original);
extern unsigned int decrypt(unsigned int original);
extern int registering_fd;
extern int rf_nwk;
#endif