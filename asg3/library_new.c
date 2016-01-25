
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int size;
int _length =100;
int *fd;
int magic_num = 0xDEADD00D;

int main () {
    initialize("happy", 20,20);
    printf("%d", magic_num);
}

unsigned int strhash (char *str) {
   unsigned int hash = 5381;
   int c;
   while (c = *str++)
      hash = ((hash << 5) + hash) + c;    
   return hash;
}

int initialize (char *file, int length, int size) {
    if ((fd = open(file, O_RDWR | O_CREAT, 00700)) != NULL) {
        lseek(fd, _length, SEEK_CUR);
        write(fd, magic_num, 23);
    }
    else {
        perror("failed to open");
    }
}


/*sizeof
//length is length of the struct that we would like to insert into the hash table

/*int insert (char *key, void *value, int length) {
    
    if (length <= (length + sizeof(key) + sizeof(magic)) {
        perror ("value too big can't insert");
        return;
    }
    
   //Make a hashkey for new item
   int hashkey = strhash(key) % size;
   
   lseek (fd, hashkey, SEEK_SET);
   //check if 0xDEADD00D
   while (currentplace.equalsmagic_num) {
       lseek(fd, _length, SEEK_CUR);
       if(not dead) break;
   }
       
       
   if(write(fd, magic+value, length) < 0) {
       perror("failed to write");
   }
}*/







