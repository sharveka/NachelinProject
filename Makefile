####Cybermap project program version 5
#### By cadman12
#

CC = gcc
CC_FLAG=-O2
MYSQL_INC_HOME = /usr/include/mysql
MYSQL_LIB_HOME = /usr/lib64/mysql
INCL_DIR = -I$(MYSQL_INC_HOME)
LIB_DIR = -L$(MYSQL_LIB_HOME)
LIBS = -lmysqlclient -lm -lz

.c.o:
	@echo Compiling $<
	@$(CC) $(INCL_DIR) $(CC_FLAG) -c $<

#Object linkage definition

smnmsd_obj = smnmsd.o clntproc.o

all : smnmsd

smnmsd : $(smnmsd_obj)
	@echo Linking objects to smnmsd
	$(CC) $(CC_FLAG)  -o smnmsd $(smnmsd_obj) $(LIB_DIR) $(LIBS)

clean :
	@echo Cleaning intermediate files
	@rm -f $(smnmsd_obj) 
