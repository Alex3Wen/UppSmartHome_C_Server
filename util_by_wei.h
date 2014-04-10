
#ifndef __WEI_UTIL
#define __WEI_UTIL

#define KDEBUG_ENABLE
#ifdef KDEBUG_ENABLE
#define Kdbg(x...) {printk("\nkernel# fun: ");printk(x);printk("\n");}while(0)
//#define Kdbgv(x...) {printk("\nkernel# x:%08x %d ",x,x);printk("\n");}while(0)
#define Kdbgln(x...) {printk("kernel # ");printk(x);printk("\n");}while(0)
#define Kdbgv(v) {printk("kernel # ");printk(#v);printk("\t:%08x %d \n",v , v);}while(0)
#define KLdbg() printk("line :%d fun:%s \n",__LINE__,__FUNCTION__)
#else
#define Kdbg(x...) 
#define Kdbgv(x...) 
#define Ldbg() 
#endif

#define USBdebug(...)	printk(__VA_ARGS__)


#define USER_DBG_EN
#ifdef USER_DBG_EN
#define Udbg(x...) {printf("\nkernel# fun: ");printf(x);printf("\n");}while(0)
#define Udbgln(x...) {printf("kernel # ");printf(x);printf("\n");}while(0)
#define Udbgv(v) {printf("kernel # ");printf(#v);printf("\t:%08x %d \n",v , v);}while(0)
#define ULdbg() printf("line :%d fun:%s \n",__LINE__,__FUNCTION__)
#else
#define Udbg(x...) 
#define Udbgv(x...) 
#define ULdbg() 
#endif



#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#endif