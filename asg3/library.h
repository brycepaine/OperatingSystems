#ifndef __LIBRARY_H__
#define __LIBRARY_H__

int initialize (char* file, int length, int size);

int delete(char *key);

int probe (char *key);

int insert (char *key, void *value, int length);

int fetch (char *key, void *value, int *length);

#endif