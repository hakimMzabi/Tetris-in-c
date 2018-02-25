#ifndef MENU_H
#define MENU_H

#include <stdio.h>

struct action_s
{
   char * desc_a;
   void(*f)(const char* s);
  
}; 
typedef struct action_s action;


struct item_s
{
  enum { ACT, SM, NI } it_flag;
  union {
    struct menu_s* sous_menu ;
    action action ;
  } it_content;
};
typedef struct item_s item;

struct menu_s
{
  struct menu_s* parent;
  char * desc;
  item items[5];
};

typedef struct menu_s menu;


//********menu*********//
menu *createMenu(const char* text);
void addMenuAction(menu* m, const char* text, void(*f)()); 
void addSubMenu(menu* m, menu* sm);
void deleteMenu(menu* m);
void launchMenu(menu* m);
void quit();
void f();
void f2();
void f3();
void f4();



#endif
