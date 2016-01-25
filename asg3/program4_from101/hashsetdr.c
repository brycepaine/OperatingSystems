/*Taoh Green (tmgreen) Sam Fields (sefields
*NAME: hashsetdr.c
*DESCRIPTION: Used to test the functions of the hashset.c
*adt.
*/
#include "hashset.h"

int main(void) {
   hashset *H = new_hashset(2);
   // put_hashset(H, "Harry Potter", 155135);
   // put_hashset(H, "faefea", 15143);
   // put_hashset(H, "sam fields", 1515);
   // put_hashset(H, "taoh green", 797);
   // put_hashset(H, "faefea", 187);
   // put_hashset(H, "Haageatter", 159);
   // put_hashset(H, "Harageagotter", 9015);
   // put_hashset(H, "sam fields", 1875);
   // print_hashset(H);
   // has_hashset(H, "jfoeao");
   // has_hashset(H, "Harry Potter");
   // has_hashset(H, "sam fields");
    // char* new = "hello";
    put_hashset(H, "harry potter", 7319);
    put_hashset(H, "sam fields", 841);
    put_hashset(H, "harry potter", 144);

    // has_hashset(H, new);
    print_hashset(H);
}
