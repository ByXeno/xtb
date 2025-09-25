
#ifndef XTB_DA_H_
#define XTB_DA_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef XTB_DA_DEF_SIZE
#define XTB_DA_DEF_SIZE 16
#endif // XTB_DA_DEF_SIZE

#if XTB_DA_DEF_SIZE == 0
#error "XTB_DA_DEF_SIZE can not be 0"
#endif

#ifndef XTB_DA_MUL
#define XTB_DA_MUL 2
#endif // XTB_DA_MUL

#if XTB_DA_MUL < 2
#error "XTB_DA_DEF_SIZE can not be less than 2"
#endif

#define __error__(msg) \
    {printf("%s:%d:%s\n",__FILE__,__LINE__,msg);exit(1);}

#define Panic_Nullptr()     {__error__("Got null ptr");}
#define Panic_OutOfMemory() {__error__("Out of memory");}
#define Panic_OutOfBounds() {__error__("Out of bound");}
#define Panic_ElSizeZero() {__error__("Element size can not be 0");}

typedef struct {
    void* data;
    uint32_t count;
    uint32_t capacity;
    const uint32_t el_size;
} da_list_t;

da_list_t da_create(uint32_t el_size);
da_list_t da_clone(da_list_t list);
void da_append(da_list_t* list,void* element); 
void* da_get_element(da_list_t* list,uint32_t index); 
void* da_get_first(da_list_t* list); 
void* da_get_last(da_list_t* list); 
void da_free(da_list_t* list); 
void da_realloc(da_list_t* list);
void da_clear(da_list_t* list);
void da_pop(da_list_t* list);

#ifdef XTB_DA_IMPEMENTATION

void da_pop
(da_list_t* list)
{
    if (!list || !list->data) { Panic_Nullptr(); }
    if (list->count == 0) { Panic_OutOfBounds(); }
    list->count--;
}

da_list_t da_clone
(da_list_t list)
{
    da_list_t tmp = list;
    tmp.data = calloc(list.capacity,list.el_size);
    memcpy(tmp.data,list.data,list.el_size * list.count);
    return tmp;
}

da_list_t da_create
(uint32_t el_size)
{
    if(el_size == 0) {Panic_ElSizeZero();}
    return (da_list_t){
	.el_size = el_size,
	.capacity = XTB_DA_DEF_SIZE,
	.count = 0,
	.data = calloc(XTB_DA_DEF_SIZE,el_size),
    };
}

void da_clear
(da_list_t* list)
{
    if(!list) {Panic_Nullptr();}
    if(!list->data) { Panic_Nullptr(); }
    memset(list->data,0,list->el_size * list->count);
    list->count = 0;
}

void da_free
(da_list_t* list)
{
    if(!list) {Panic_Nullptr();}
    if(!list->data) { Panic_Nullptr(); }
    free(list->data);
    list->data = 0;
    list->count = 0;
    list->capacity = 0;
}

void da_append
(da_list_t* list,void* element)
{
    if(!list) {Panic_Nullptr();}
    if(!list->data) { Panic_Nullptr(); }
    if(!element) {Panic_Nullptr();}
    if(list->count >= list->capacity)
    {da_realloc(list);}
    uint32_t off = list->el_size*list->count;
    memcpy((uint8_t*)list->data + off, element, list->el_size);
    list->count++;
}

void* da_get_first
(da_list_t* list)
{
    return da_get_element(list,0);
}

void* da_get_last
(da_list_t* list)
{
    return da_get_element(list,list->count-1);
}

void* da_get_element
(da_list_t* list,uint32_t index)
{
    if(!list) {Panic_Nullptr();}
    if(!list->data) { Panic_Nullptr(); }
    if(index >= list->count) {Panic_OutOfBounds();}
    return (uint8_t*)list->data + index * list->el_size;
}

void da_realloc
(da_list_t* list)
{
    if(!list) { Panic_Nullptr(); }
    if(!list->data) { Panic_Nullptr(); }
    list->capacity *= XTB_DA_MUL;
    list->data = realloc(list->data, list->capacity * list->el_size);
    if(!list->data) { Panic_OutOfMemory(); }
    memset(list->data + list->count, 0,
           (list->capacity - list->count) * sizeof(*list->data));
}

#endif // XTB_DA_IMPEMENTATION

#endif // XTB_DA_H_
