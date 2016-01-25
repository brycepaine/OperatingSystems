#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
//for bit numbers
#include <stdint.h>

int _size;
int _length;
int fd;
uint32_t magic_num = 0xDEADD00D;
uint32_t tombstone = 0xB16B00B5;
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
        _size = size;
        _length = length;
        return fd;
    }
    else {
        /* perror("failed to open"); */
        return -1;
    }
}

int delete(char *key) {
	int prober = probe(key);
    // printf("prober: %d\n", prober);
    if(prober == (-1)) {
        printf("Error: Value not in table.\n");
        return(-1);
    }
    else {
        lseek(fd, (prober * _length), SEEK_SET);
        if(write(fd, &tombstone, sizeof(uint32_t)) < 0) {
        	printf("failed to write tombstone\n");
        }
        return(0);
    }
}

int probe (char *key) {
    uint32_t check;
    int check_tomb; //variable to check if tombstone is in slot
    int check_dead; //variable to check if something is in slot
    int i; //iterator through hash table
    int hashkey = strhash(key) % _size;
    char *check_key = (char*) malloc(strlen(key));
   
    for (i = 0; i < _size; ++i) {
       //check if 0xDEAd
       /* printf("initial key: %d\n", hashkey); */
       
       lseek(fd, hashkey * _length, SEEK_SET);
       check = NULL;
       read(fd, &check, sizeof(uint32_t));
       check_dead = memcmp(&magic_num, &check, sizeof(uint32_t));
       check_tomb = memcmp(&tombstone, &check, sizeof(uint32_t));
       //check if item is already there
       read(fd, check_key, strlen(key));
       if(check_dead==0 && (strcmp(check_key, key) == 0)) {
           return hashkey;
       }
       else if (check_dead == 0) {
           /* printf("conflict\n"); */
           hashkey++;
       }
       else if(check_tomb == 0 && (strcmp(check_key, key) == 0)) {
          printf("Value has been deleted from table.\n");
          return (-1);
       }
       else if(check_tomb == 0) {
          hashkey++;
       }
       
       else {
           /* printf("found slot\n"); */
           return (-1);
       }
       /* printf("hashkey: %d, _size: %d\n", hashkey,_size); */
       if(hashkey == _size) {
           hashkey =0;
       }  
       /* printf ("hashkey = %d\n", hashkey); */
    }
    return (-1);
}

int insert (char *key, void *value, int length) {
    uint32_t check;
    int check_dead; //variable to check if something is in slot
    int check_tomb; //variable to check if tombstone is in slot
    int i; //iterator through hash table
    char *check_key = (char*) malloc(strlen(key));
    int hash_dead = -1;
    
    if (_length < (length + strlen(key) + sizeof(magic_num))) {
        printf ("value too big can't insert\n");
        return EXIT_FAILURE;
    }
    
   //Make a hashkey for new item
   int hashkey = strhash(key) % _size;
   // printf("hashkey: %d\n", hashkey);
   
   
   for (i = 0; i < _size; ++i) {
       //check if 0xDEAd
       /* printf("initial key: %d\n", hashkey); */
       
       lseek(fd, hashkey * _length, SEEK_SET);
       check = NULL;
       read(fd, &check, sizeof(uint32_t));
       check_dead = memcmp(&magic_num, &check, sizeof(uint32_t));
       check_tomb = memcmp(&tombstone, &check, sizeof(uint32_t));
       // printf("check_dead: %d, check_tomb: %d\n, hashkey: %d", check_dead, check_tomb, hashkey);
       //check if item is already there
       read(fd, check_key, strlen(key));
       if((strcmp(check_key, key) == 0) && check_dead == 0) {
           printf("already in table\n");
           return hashkey;
       }
       if(check_tomb == 0 && strcmp(check_key,key) == 0) {
       		printf("Inserting into slot where same key has had something removed.\n");
       		break;
       }
       else if (check_tomb == 0) {
       		// printf("check tomb = 0\n");
       		hash_dead = hashkey;
       }
       if (check_dead == 0) {
            // printf("conflict\n"); 
           hashkey++;
       }
       else {
           // printf("found slot\n"); 
           break;
       }
       /* printf("hashkey: %d, _size: %d\n", hashkey,_size); */
       if(hashkey == _size) {
          // printf("here\n");
           hashkey =0;
       }  
       /* printf ("hashkey = %d\n", hashkey); */
   }
    if(i == _size) {
           printf("hash table full. Can't insert.\n");
           return EXIT_FAILURE;
    }  
   
   if(hash_dead >=0) {
		hashkey = hash_dead;
   }
   lseek(fd, hashkey * _length, SEEK_SET);
   if(write(fd, &magic_num, sizeof(uint32_t)) < 0) {
       printf("failed to write magic num\n");
   }
   if(write(fd, key, strlen(key)) < 0) {
       printf("failed to write key\n");
   }
   if(write(fd, &length, sizeof(int)) < 0) {
       printf("failed to write length\n");
   }
   if(write(fd, value, length) < 0) {
       printf("failed to write value\n");
   }
   // printf("key: %s, value: %s, hashkey: %d\n", key, value, hashkey);
   return hashkey;
}

int fetch (char *key, void *value, int *length) {
	// printf("initialval:%s\n", value);
	// value = (void*) malloc(length);
  // printf("key: %s\n", key);
	void *a;
    int prober = probe(key);
    // printf("prober: %d\n", prober);
    if(prober == (-1)) {
        printf("Error: Value not in table.\n");
        return(-1);
    }

    else {
        // printf("hello\n");
        lseek(fd, (prober * _length) + sizeof(uint32_t) + strlen(key), SEEK_SET);
        read(fd, length, sizeof(int));
        read (fd, value, *length);
        // printf("length: %d\n", *length);

		// memcpy(&value, &a, length);
		// printf("a: %s\n", a);
		// value = *a;
        return prober;
    }
}


// int main () {
//     char *key1 = "1425";
//     char *key2 = "5718";
//     char *key3 = "51553";
//     char *key4 = "85179";
//     char *key5 = "5718";
//     char *key6 = "719";
//     char *key7 = "175";
//     char *key8 = "741982";
//     char *key9 = "7181";
	
    
//     /* int hashkey = strhash(key1) % _size;
//     printf("%d\n", hashkey);
//     hashkey = strhash(key2) % _size;
//     printf("%d\n", hashkey);
//     hashkey = strhash(key3) % _size;
//     printf("%d\n", hashkey);
//      */
//     // int new[] = {1,5,124,4,8};
//     initialize("fuck", 25, 5);
    
//     // insert (key6, "FOOOOBE",7 );
//     // delete(key6);
//     // insert (key5, "Bry", 5);
//     insert (key7, "BAR", 3);
//     insert (key8, "BEEP", 4);
//     // insert (key9, "BOOP", 4);
//     // delete(key6);
//     // printf( "hash: %d\n", strhash(key5)%_size);
//     insert (key1, "bryce", 5);
//     insert (key3, "is", 2);
//     insert (key2, "awesome", 7);
//     insert (key4,"here", 4);
//     // insert (key5, "we", 2);
//  	// int prober = probe( key7);
//     // delete (key7);
//     // int prober2 = probe(key7);
//     // printf("prober: %d, prober2: %d\n", prober, prober2);
    
//     // int n;
//     // char a[4000];
//     // char b[4000];
//     // char c[4000];
//     // char d[4000];
//     // char z[4000];
//     // int e;
//     // int f;
//     // int g;
//     // int h;
//     // int y;
//     // fetch (key8, a, &e);
//     // printf("(BEEP): %s\n", a);
//     // fetch (key2, b, &f);
//     // printf("(awesome): %s\n", b);
//     // delete (key8);
//     // fetch (key7, d, &h);
//     // printf("(BAR): %s\n", d);
//     // fetch (key8, c, &g);
//     // printf("(BEEP: DELETED): %s\n", c);
//     // delete(key2);
//     // fetch (key2, z, &y);
//     // printf("(Awesome: DELETED): %s\n", c);
//     close(fd);
//     /* int prober = probe("89");
//     printf("%d\n", prober); */
//     return EXIT_SUCCESS;
// }