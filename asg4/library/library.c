#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
//for bit numbers
#include <stdint.h>
#include "md5.h"

int _size;
int _length;
int fd;
uint32_t magic_num = 0xDEADD00D;
uint32_t tombstone = 0xB16B00B5;
char keydigest[16];

/* Prints message digest buffer in mdContext as 32 hexadecimal digits.
   Order is from low-order byte to high-order byte of digest.
   Each byte is printed with high-order hexadecimal digit first.
 */
MD5_CTX* MDPrint (mdContext)
MD5_CTX *mdContext;
{
   char key_temp[16];
   int i;
   //printf("hello:%02x", keydigest);
   for (i = 0; i < 16; i++){
      sprintf (key_temp, "%02x", mdContext->digest[i]);
      strcat(keydigest, key_temp);
   	  // keydigest[i] = mdContext->digest[i];
   }
   // printf("keydigest: %s\n", keydigest);
   
   return mdContext;
  // memcpy (&keydigest, mdContext->digest[i], 2);
}

void md5 (inString)
char *inString;
{
  MD5_CTX mdContext;
  unsigned int len = strlen (inString);

  MD5Init (&mdContext);
  MD5Update (&mdContext, inString, len);
  MD5Final (&mdContext);
  MDPrint (&mdContext);
  

  // printf (" \"%s\"\n\n", inString);
}

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
   
    char check_key[16];

    md5(key);
    int hashkey = (int)(keydigest[0]^keydigest[2]^keydigest[3]^keydigest[4]^
      keydigest[5]^keydigest[6]^keydigest[7]^keydigest[8]^keydigest[9]^
       keydigest[10]^keydigest[11]^keydigest[12]^keydigest[13]^keydigest[14]^
        keydigest[15]) % _size;



    for (i = 0; i < _size; ++i) {
       //check if 0xDEAd
       /* printf("initial key: %d\n", hashkey); */
       printf("starting for loop\n");
       lseek(fd, hashkey * _length, SEEK_SET);
       check = NULL;
       read(fd, &check, sizeof(uint32_t));
       check_dead = memcmp(&magic_num, &check, sizeof(uint32_t));
       check_tomb = memcmp(&tombstone, &check, sizeof(uint32_t));
       //check if item is already there
       read(fd, &check_key, strlen(keydigest));
       if(check_dead==0 && memcmp(&check_key, &keydigest, 16) == 0) {
            printf("tata\n");
            bzero(keydigest, 16);
           return hashkey;
       }
       else if (check_dead == 0) {
           /* printf("conflict\n"); */
          printf("tafeafta\n");
           hashkey++;
       }
       else if(check_tomb == 0 && memcmp(&check_key, &keydigest, 16) == 0) {
          printf("Value has been deleted from table.\n");
          bzero(keydigest, 16);
          return (-1);
       }
       else if(check_tomb == 0) {
          hashkey++;
       }
       
       else {
           /* printf("found slot\n"); */
          bzero(keydigest, 16);
           return (-1);
       }
       /* printf("hashkey: %d, _size: %d\n", hashkey,_size); */
       if(hashkey == _size) {
           hashkey =0;
       }  
       /* printf ("hashkey = %d\n", hashkey); */
    }
    bzero(keydigest, 16);
    return (-1);
}

int insert (char *key, void *value, int length) {
    uint32_t check;
    int check_dead; //variable to check if something is in slot
    int check_tomb; //variable to check if tombstone is in slot
    int i, z;  //iterator through hash table
    char check_key[16];
    int hash_dead = -1;
    
    if (_length < (length + sizeof(int) + 16 + sizeof(magic_num))) {
        printf ("value too big can't insert\n");
        return EXIT_FAILURE;
    }
    
   //Make a hashkey for new item
   md5(key);
   printf("keydigest: %s\n", keydigest);
   // printf("\n");
   int hashkey = (int)(keydigest[0]^keydigest[2]^keydigest[3]^keydigest[4]^
    keydigest[5]^keydigest[6]^keydigest[7]^keydigest[8]^keydigest[9]^
    keydigest[10]^keydigest[11]^keydigest[12]^keydigest[13]^keydigest[14]^
    keydigest[15]) % _size;
   
   //printf("poop\n");

   // printf("hashkey: %d\n", hashkey);
   
   
   for (i = 0; i < _size; ++i) {
       //check if 0xDEAd
       /* printf("initial key: %d\n", hashkey); */
       
	   lseek(fd, hashkey * _length, SEEK_SET);
	   check = NULL;
	   read(fd, &check, sizeof(uint32_t));
	   check_dead = memcmp(&magic_num, &check, sizeof(uint32_t));
	   check_tomb = memcmp(&tombstone, &check, sizeof(uint32_t));
	   // printf("check_dead: %d, check_tomb: %d\n, hashkey: %d", check_dead, 
	   // 		check_tomb, hashkey);
	   //check if item is already there
	   read(fd, &check_key, strlen(keydigest));
	   //printf("checkkey value:%d\n", check_key);
	   // printf("here:%d\n",(memcmp(&check_key, &keydigest, 16)));
	   // printf("hellloooo%d\n", keydigest[0]);

     // printf("check_key: ");
     // for (z = 0; z<16; z++){
	    //  printf ("%02x", check_key[i]);
     // }
     // printf("\n");


	   if(memcmp(&check_key, &keydigest, 16) == 0 && check_dead == 0) {
	       printf("already in table\n");
         bzero(keydigest, 16);
	       return hashkey;
	   }
	   if(check_tomb == 0 && memcmp(&check_key, &keydigest, 16) == 0) {
	      printf("Inserting into slot where same key has" 
	      	" had something removed.\n");
	      break;
	   }
	   else if (check_tomb == 0) {
	      // printf("check tomb = 0\n");
	      hash_dead = hashkey;
	   }
	   if (check_dead == 0) {
	        printf("conflict\n"); 
	       hashkey++;
	   }
	   else {
	       // printf("found slot\n"); 
	       break;
	   }
	   /* printf("hashkey: %d, _size: %d\n", hashkey,_size); */
	   if(hashkey == _size) {
	      // printf("here\n");
	       hashkey = 0;
	   }  
	   /* printf ("hashkey = %d\n", hashkey); */
   }
    if(i == _size) {
        printf("hash table full. Can't insert.\n");
        bzero(keydigest, 16);
        return EXIT_FAILURE;
    }  
   
   if(hash_dead >=0) {
   		hashkey = hash_dead;
   }
   lseek(fd, hashkey * _length, SEEK_SET);
   printf("about to write\n");
   if(write(fd, &magic_num, sizeof(uint32_t)) < 0) {
       printf("failed to write magic num\n");
   }
   
   if(write(fd, keydigest, 16) < 0 ) {
   		printf("failed to write keydigest\n");
   }
      //printf ("%02x", keydigest[i]);
   if(write(fd, &length, sizeof(int)) < 0) {
       printf("failed to write length\n");
   }

   printf ("value: %s\n", value);
   if(write(fd, value, length) < 0) {
       printf("failed to write value\n");
   }

   bzero(keydigest, 16);
   // printf("key: %s, value: %s, hashkey: %d\n", key, value, hashkey);
   return hashkey;
}


/****
****
***
***

*FETCH IS NOT COMPLETE. are we storing the length of value again*
**
***
***
***
***
**
**/


int fetch (char *key, void *value, int *length) {
  // printf("initialval:%s\n", value);
  // value = (void*) malloc(length);
  // printf("key: %s\n", key);
    int prober = probe(key);
    // printf("prober: %d\n", prober);
    if(prober == (-1)) {
        printf("Error: Value not in table.\n");
        return(-1);
    }

    else {
        // printf("hello\n");
        lseek(fd, (prober * _length) + sizeof(uint32_t) + 16, SEEK_SET);
        read(fd, length, sizeof(int));
        read (fd, value, *length);
        // printf("length: %d\n", *length);

    // memcpy(&value, &a, length);
    // printf("a: %s\n", a);
    // value = *a;
        return prober;
    }
}

//for testing the hash library functions
int main () {
    char *key1 = "1425";
    char *key2 = "5718";
    char *key3 = "51553";
    char *key4 = "85179";
    char *key5 = "525108";
    char *key6 = "719";
    char *key7 = "175";
    char *key8 = "741982";
    char *key9 = "7181";
  
    
    /* int hashkey = strhash(key1) % _size;
    printf("%d\n", hashkey);
    hashkey = strhash(key2) % _size;
    printf("%d\n", hashkey);
    hashkey = strhash(key3) % _size;
    printf("%d\n", hashkey);
     */
    // int new[] = {1,5,124,4,8};
    initialize("fuck", 50, 5);
    
    insert (key6, "FOOOOBE", 7);
    // delete(key6);
    // insert (key5, "Bry", 5);
    insert (key2, "BAR", 3);
    insert (key2, "BOO", 3);
    insert (key8, "BEEP", 4);
    insert (key9, "BOOP", 6);
    // delete(key6);
    // printf( "hash: %d\n", strhash(key5)%_size);
    insert (key1, "bryce", 5);
    insert (key5, "is", 2);
    // insert (key2, "awesome", 7);
    // insert (key4,"here", 4);
    // insert (key5, "we", 2);
    int prober = probe( key2);
    // // delete (key7);
    int prober2 = probe(key7);
    printf("prober: %d, prober2: %d\n", prober, prober2);
    
    // int n;
    char a[4000];
    // char b[4000];
    // char c[4000];
    // char d[4000];
    // char z[4000];
    int e;
    // int f;
    // int g;
    // int h;
    // int y;
    fetch (key8, a, &e);
    printf("(BEEP): %s\n", a);
    // fetch (key2, b, &f);
    // printf("(awesome): %s\n", b);
    // delete (key8);
    // fetch (key7, d, &h);
    // printf("(BAR): %s\n", d);
    // fetch (key8, c, &g);
    // printf("(BEEP: DELETED): %s\n", c);
    // delete(key2);
    // fetch (key2, z, &y);
    // printf("(Awesome: DELETED): %s\n", c);
    close(fd);
    /* int prober = probe("89");
    printf("%d\n", prober); */
    return EXIT_SUCCESS;
}