#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
//for bit numbers
#include <stdint.h>

int size   = 5;
int _length=15;
int fd;
uint32_t magic_num = 0xDEADD00D;
int hash_global;



unsigned int strhash (char *str) {
    unsigned int hash = 5381;
   int c;
   while (c = *str++)
      hash = ((hash << 5) + hash) + c;    
   return hash;
}


int initialize (char* file, int length, int size) {
    if ((fd = open(file, O_RDWR | O_CREAT, 00700))  != -1) {
        /* lseek(fd, _length, SEEK_CUR);
        write(fd, magic_num, 8); */
        return fd;
    }
    else {
        /* perror("failed to open"); */
        return -1;
    }
}


int probe (char *key) {
    uint32_t check_dead;
    int n; //variable to check if something is in slot
    int i; //iterator through hash table
    int hashkey = strhash(key) % size;
    char *check_key = (char*) malloc(strlen(key));
   
   
    for (i = 0; i < size; ++i) {
       //check if 0xDEAd
       /* printf("initial key: %d\n", hashkey); */
       
       lseek(fd, hashkey * _length, SEEK_SET);
       check_dead = NULL;
       read(fd, &check_dead, sizeof(uint32_t));
       n = memcmp(&magic_num, &check_dead, sizeof(uint32_t));
       //check if item is already there
       read(fd, check_key, strlen(key));
       if(n==0 && (strcmp(check_key, key) == 0)) {
           return hashkey;
       }
       if (n == 0) {
           /* printf("conflict\n"); */
           hashkey++;
       }
       else {
           /* printf("found slot\n"); */
           return (-1);
       }
       /* printf("hashkey: %d, size: %d\n", hashkey,size); */
       if(hashkey == size) {
           hashkey =0;
       }  
       /* printf ("hashkey = %d\n", hashkey); */
   }
   return (-1);
}

int insert (char *key, void *value, int length) {
    uint32_t check_dead;
    int n; //variable to check if something is in slot
    int i; //iterator through hash table
    char *check_key = (char*) malloc(strlen(key));
    
    if (_length < (length + strlen(key) + sizeof(magic_num))) {
        printf ("value too big can't insert\n");
        return EXIT_FAILURE;
    }
    
   //Make a hashkey for new item
   int hashkey = strhash(key) % size;
   // printf("hashkey: %d\n", hashkey);
   
   
   for (i = 0; i < size; ++i) {
       //check if 0xDEAd
       /* printf("initial key: %d\n", hashkey); */
       
       lseek(fd, hashkey * _length, SEEK_SET);
       check_dead = NULL;
       read(fd, &check_dead, sizeof(uint32_t));
       n = memcmp(&magic_num, &check_dead, sizeof(uint32_t));
       //check if item is already there
       read(fd, check_key, strlen(key));
       if(strcmp(check_key, key) == 0) {
           printf ("already in table\n");
           return hashkey;
       }
       if (n == 0) {
           /* printf("conflict\n"); */
           hashkey++;
       }
       else {
           /* printf("found slot\n"); */
           break;
       }
       /* printf("hashkey: %d, size: %d\n", hashkey,size); */
       if(hashkey == size) {
           hashkey =0;
       }  
       /* printf ("hashkey = %d\n", hashkey); */
   }
    if(i == size) {
           printf("hash table full. Can't insert.\n");
           return EXIT_FAILURE;
    }  
   
   lseek(fd, hashkey * _length, SEEK_SET);
   if(write(fd, &magic_num, sizeof(uint32_t)) < 0) {
       printf("failed to write magic num\n");
   }
   if(write(fd, key, strlen(key)) < 0) {
       printf("failed to write key\n");
   }
   if(write(fd, value, length) < 0) {
       printf("failed to write value\n");
   }
   // printf("key: %s, value: %s, hashkey: %d\n", key, value, hashkey);
   return hashkey;
}

int fetch (char *key, void *value, int length) {
    int prober = probe(key);
    value = malloc(sizeof(length));
    printf("prober: %d\n", prober);
    if(prober == (-1)) {
        printf("Error: Value not in table.\n");
        return(-1);
    }

    else {
        // printf("hello\n");
        printf("hashkey: %d\n", strhash(key)%size);
        lseek(fd, prober * _length + sizeof(uint32_t) /*+ strlen(key)*/, SEEK_SET);
    	char *keyb;
        read (fd, keyb, strlen(key));
        read (fd, value, sizeof(length));
        printf("value: %s, keyb: %s\n", value, keyb);
        return prober;
    }
}


int main () {
    char *key1 = "1425";
    char *key2 = "5718";
    char *key3 = "51553";
    char *key4 = "85179";
    char *key5 = "5718";
    char *key6 = "719853";
    char *key7 = "175";
    char *key8 = "741982";
    char *key9 = "567";
    
    /* int hashkey = strhash(key1) % size;
    printf("%d\n", hashkey);
    hashkey = strhash(key2) % size;
    printf("%d\n", hashkey);
    hashkey = strhash(key3) % size;
    printf("%d\n", hashkey);
     */
    // int new[] = {1,5,124,4,8};
    initialize("fuck", _length, size);
    
    // insert (key6, "FOO", 3);
    // insert (key7, "BAR", 3);
    // insert (key8, "BEEP", 4);
    insert (key9, "BOOP", 4);
    // insert (key1, "bryce", 5);
    // insert (key3, "is", 2);
    // insert (key2, "awesome", 7);
    // insert (key4,"here", 4);
    // insert (key5, "we", 2);
 	// int length_fetch;
    // char *suck = malloc (sizeof(4));
    void *poop= malloc(sizeof(4));
    int n = fetch (key9, poop, 4);
    printf("n=%d, poop (should be BOOP): %s\n", n, poop);

    /* int prober = probe("89");
    printf("%d\n", prober); */
    // char *boop = "boop";
    // write(fd, boop, sizeof(boop));
    // lseek(fd, 0, SEEK_SET);
    // read (fd, suck, sizeof(boop));
    // printf("suck: %s\n", suck);
    
}


