#include <stdio.h>
#include <stdlib.h>
#define SQLITE_HAS_CODEC
#include <sqlite3.h>
//#include <sys/queue.h>
#include "list.h"

#include "util_by_wei.h"
#include "RFID_handler.h"

#define UID_COLUMN_INDEX 0
sqlite3 *card_db;
char *zErrMsg = 0;
unsigned int gAalias_index = 0;
int registed_int[10];
int registering_fd;
int rf_nwk;
/**
* after this, open door (or light it)
*/
int RFID_verify(unsigned int id){
	int i ;
	int nrow = 0, ncolumn = 0;  
	unsigned int tmp;
	char **azResult; //二维数组存放结果  
	/*for(i = 0;i < ARRAY_SIZE(registed_ids);i++){
		if(id == registed_ids[i])
			return 1;
	}*/
	char *exec;
	exec = malloc(100);
	id = encrypt(id);		// exam aa -->55
    sprintf(exec,"select * from RFIDcardInfo where RF_ID=%u;",id);

    /* 
 int sqlite3_get_table(sqlite3*, const char *sql,char***result , int *nrow , int *ncolumn ,char **errmsg ); 
 azResult        中是以数组的形式存放你所查询的数据，首先是表名，再是数据。 
 nrow ,ncolumn   分别为查询语句返回的结果集的行数，列数，没有查到结果时返回0 
 */  
	sqlite3_get_table(card_db , exec , &azResult , &nrow , &ncolumn , &zErrMsg );  
	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )  {
		Udbg( "azResult[%d] = %s\n", i , azResult[i] );
		//tmp = atol(azResult[ i + UID_COLUMN_INDEX]);
		//if(tmp ==)
	}
	//释放掉  azResult 的内存空间  
	sqlite3_free_table( azResult );  
	free(exec);
	if(nrow>0)
		return 1;
	else
		return 0;
}

int delete_ID(unsigned int id){
	int ret;
    char *exec;
    exec = malloc (100);
    id = encrypt(id);
    sprintf(exec,"delete from RFIDcardInfo where RF_ID=%u;",id);
    Udbg("%s \n",exec);
    ret = sqlite3_exec( card_db , exec , 0 , 0 , &zErrMsg );  
    free(exec);
    if(ret)
    	Udbg("%s \n",zErrMsg);
    return ret;
}

int get_rows_num(){
	int i ;
	int nrow = 0, ncolumn = 0;  
	unsigned int tmp;
	char **azResult; //二维数组存放结果  
	char *exec = "select * from RFIDcardInfo order by alias_index desc limit 1;";
	sqlite3_get_table(card_db , exec , &azResult , &nrow , &ncolumn , &zErrMsg );
	if(nrow == 0 )
		return 0;  
	Udbg( "azResult[%d] = %s\n", 4 , azResult[3] );
	return atoi(azResult[3]);
}

/**
* register to sql
* and send the registed id to clients
* 保证max_row 总是大于数据库打 alias_id
*/
int register_ID(unsigned int id){
//send id 
	int ret = 0;
	int max_row;
	ret = RFID_verify(id);//
	ULdbg();
	Udbgv(ret);
	if(!ret){// is not registed. return 0 , database hasn't the id
		max_row = get_rows_num();
		max_row ++;
		insert(id,max_row);
		Udbg("register successfully ,id :%u ,rownum:%d \n",id,max_row);
		return max_row;
	}else 
		return -1;
}
/*
client(android,qt) register UID to save own card through sending register command.
*/

sqlite3 *openDB(){
	sqlite3 *db=NULL;
	char *zErrMsg = 0;
	int rc;
	 
	//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件
	rc = sqlite3_open("RFcards.db", &db);
	if( rc )
	{
	  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	  sqlite3_close(db);
	  return NULL;
	}else{
	 	printf("You have opened a sqlite3 database named RFcards.db successfully!\nCongratulations! Have fun !  ^-^ \n");
	 	return db;
 	}
}

int insert(unsigned int id,unsigned int alias_index){
	char *ins;
	int ret;
	//insert into RFIDcardInfo(RF_ID, alias_index) values(2,3);
	ULdbg();
	ins = (char *)malloc(100);
	id = encrypt(id);
	sprintf(ins,"insert into RFIDcardInfo(RF_ID, alias_index) values(%u,%u);",id,alias_index);
	printf("ins : &s \n",ins);
	ULdbg();
	ret = sqlite3_exec( card_db , ins , 0 , 0 , &zErrMsg);
	ULdbg();
	if(ret!=0)
		printf("%s\n",zErrMsg);
	return ret;
}

int create_table(){
	char *sql = "create table RFIDcardInfo(RF_ID,alias_index);";
	return sqlite3_exec(card_db , sql , 0 , 0 , &zErrMsg );
}

unsigned int row_index;

int skimRFIDcardInfo(){
	int i ;
	int nrow = 0, ncolumn = 0;  
	char **azResult; //二维数组存放结果  
	/*for(i = 0;i < ARRAY_SIZE(registed_ids);i++){
		if(id == registed_ids[i])
			return 1;
	}*/
	char *exec;
    exec = "select * from RFIDcardInfo;";
    /* 
 int sqlite3_get_table(sqlite3*, const char *sql,char***result , int *nrow , int *ncolumn ,char **errmsg ); 
 result中是以数组的形式存放你所查询的数据，首先是表名，再是数据。 
 nrow ,ncolumn分别为查询语句返回的结果集的行数，列数，没有查到结果时返回0 
 */  
	sqlite3_get_table(card_db , exec , &azResult , &nrow , &ncolumn , &zErrMsg );  
	for( i=0 ; i<( nrow + 1 ) * ncolumn ; i++ )  
		printf( "azResult[%d] = %s\n", i , azResult[i] );
	//释放掉  azResult 的内存空间  
	row_index = ncolumn;
	sqlite3_free_table( azResult ); 
		return 0;
}
unsigned int encrypt(unsigned int original){
	unsigned int x;
   x=(original>>1)|((original&0x1)<<31);//value的左边15位
   return x;
}

unsigned int decrypt(unsigned int original){
	unsigned int x;
   x=(original<<1)|((original&0x80000000)>>31);//value的左边15位
   return x;
}

int init_sqlite_RFID(){
	int ret = 0;
	unsigned int tmp = 3;
	card_db = openDB();
	//ret += encrypt();
	ret += create_table();
	if(ret)
		printf("%s\n",zErrMsg);
	Udbgv(ret);
	if(card_db == NULL){
		printf("open error\n");
		return 0 ;
	}else
	return 1;
	/*char *ins = "insert into RFIDcardInfo(RF_ID, alias_index) values(2,3)";
	//sprintf(ins,"insert into RFIDcardInfo(RF_ID, alias_index) values(%d, %d);",id,alias_index);
	ret = sqlite3_exec( card_db , ins , 0 , 0 , &zErrMsg);
	register_ID(tmp);*/
	//skimRFIDcardInfo();
	//close_db_RFID();	
//warning!!after the debug .every operation need open and close
}

void close_db_RFID(){
	if(!card_db)
		sqlite3_close(card_db); //关闭数据库
	card_db = NULL;
}

/*
int main( void )
{
	int ret;
	unsigned int tmp = 3;
	card_db = openDB();
	ret += create_table();
	printf("%s\n",zErrMsg);
	Udbgv(ret);
	if(card_db == NULL){
		printf("open error\n");
		exit(1);
	}
	char *ins = "insert into RFIDcardInfo(RF_ID, alias_index) values(2,3)";
	//sprintf(ins,"insert into RFIDcardInfo(RF_ID, alias_index) values(%d, %d);",id,alias_index);
	ret = sqlite3_exec( card_db , ins , 0 , 0 , &zErrMsg);
	register_ID(tmp);
	skimRFIDcardInfo();
	sqlite3_close(card_db); //关闭数据库
	return 0;
}*/
