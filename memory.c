#include "stdafx.h"
#include "mmemory.h"

#include <stdlib.h>
#include "memory.h"


/*
struct block {
	struct block*	pNext;	// следующий свободный или
	// занятый блок
	unsigned int	szBlock;	// размер блока
};

struct page {
	struct block*	pFirstFree;		// первый свободный блок
	struct block*	pFirstUse;		// первый занятый блок
	unsigned int	maxSizeFreeBlock;	// макс. размер сводного блока
};

struct pageInfo {
	unsigned long	offsetPage;		// смещение страницы от начала
	// памяти или файла
	char isUse;						// флаг, описывающий находится ли
	// страница в памяти или на диске
};*/




/**
@func	_malloc
@brief	Выделяет блок памяти определенного размера

@param	[out] ptr		адресс блока
@param	[in]  szBlock	размер блока

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	нехватка памяти
@retval	1	неизвестная ошибка
**/
//int _malloc(VA* ptr, size_t szBlock);

/**
@func	_free
@brief	Удаление блока памяти

@param	[in] ptr		адресс блока

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	1	неизвестная ошибка
**/
//int _free(VA ptr);

/**
@func	_read
@brief	Чтение информации из блока памяти

@param	[in] ptr		адресс блока
@param	[in] pBuffer	адресс буфера куда копируется инфомация
@param	[in] szBuffer	размер буфера

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	доступ за пределы блока
@retval	1	неизвестная ошибка
**/
//int _read(VA ptr, void* pBuffer, size_t szBuffer);

/**
@func	_write
@brief	Запись информации в блок памяти

@param	[in] ptr		адресс блока
@param	[in] pBuffer	адресс буфера куда копируется инфомация
@param	[in] szBuffer	размер буфера

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	-2	доступ за пределы блока
@retval	1	неизвестная ошибка
**/
//int _write(VA ptr, void* pBuffer, size_t szBuffer);

/**
@func	_init
@brief	Инициализация модели менеджера памяти

@param	[in] n		количество страниц
@param	[in] szPage	размер страницы

В варианте 1 и 2 общий объем памяти расчитывается как n*szPage

@return	код ошибки
@retval	0	успешное выполнение
@retval	-1	неверные параметры
@retval	1	неизвестная ошибка
**/
int _init(int n, int szPage);

void page_init(int n, int pageSize);

void block_init();

struct block *get_block_by_va(VA ptr, int size);


struct pageDescription {
    struct page *page;
    struct pageInfo *pageInfo;
};


struct memory {
    struct page *pages;
    int pageNumber;
    int freeMemorySize;
};


struct hardDrive {
    struct page *pages;
    int capacity;
};


struct pageTable {
    VA addr;
    struct pageDescription *descriptors;
    int pageNumber;
    struct pageTable *nextTable;
};


struct pageTable *firstTable = NULL;
struct memory *memory;
struct hardDrive *hdd;
const int OUT_OF_MEMORY_ERROR;
unsigned const int PAGE_SIZE = 2;
unsigned const int BLOCK_SIZE = 2;
unsigned const int PAGES_ON_HDD_NUMBER = 20;
unsigned const int BIT_CAPACITY = 16;
//struct page *memory;
//int freeMemorySize;
//int pageSize;
//int pageNumber;


void toDecimalNumber(VA addr, int * p, int * i){
    i = 0;
    int pageNumber =0;

    for (int rank = BIT_CAPACITY/2; rank >= 0; rank--){
        if(addr[rank]=='1'){
           // pageNumber+=pow(2,rank);
        }
    }
    p = pageNumber;
/*
    for (int rank = BIT_CAPACITY/2; rank >= BIT_CAPACITY/2; rank--){
        if(addr[])
    }
*/

}





int getIndexNextFreeMemoryPage(int pageNumber) {
    for (int index = pageNumber; index < memory->pageNumber; index++) {
        if (memory->pages[index].pFirstUse == NULL) {
            return index;
        }
    }
    return -1;
}


int getIndexNextFreeHDDPage(int pageNumber) {
    for (int index = pageNumber; index < hdd->capacity; index++) {
        if (memory->pages[index].pFirstUse == NULL) {
            return index;
        }
    }
    return -1;
}

int _init(int n, int szPage) {
    if (szPage <= 0 || n <= 0) {
        return -1;
    }

    memory = malloc(sizeof(memory));
    memory->freeMemorySize = n * szPage;
    memory->pageNumber = n;
    memory->pages = calloc(n, sizeof(struct page));
    hdd = malloc(sizeof(struct hardDrive));
    hdd->pages = calloc(PAGES_ON_HDD_NUMBER, sizeof(struct page));
    hdd->capacity = PAGES_ON_HDD_NUMBER;
    struct block *prevBlock = NULL;
    for (int pageIndex = 0; pageIndex < memory->pageNumber; pageIndex++) {
        struct page page;
        for (int blockIndex = 0; blockIndex < PAGE_SIZE; blockIndex++) {
            struct block *block = malloc(sizeof(struct block));
            if (blockIndex == 0) {
                page.pFirstFree = block;
            }
            block->szBlock = BLOCK_SIZE;
            if (prevBlock != NULL) {
                prevBlock->pNext = block;
            }
            prevBlock = block;
        }
        page.maxSizeFreeBlock = page.pFirstFree->szBlock;
        page.pFirstUse = NULL;
    }
    VA *p;
    _malloc(p, 3);
}


int _malloc(VA *ptr, size_t szBlock) {
    if (szBlock <= 0 || !ptr)
        return -1;
    if (szBlock > memory->freeMemorySize)
        return -2;

    struct pageTable *table = malloc(sizeof(struct pageTable));


    if (firstTable == NULL) {
        firstTable = table;
    } else {
        struct pageTable *prevTable = firstTable;
        while (prevTable->nextTable != NULL) {
            prevTable = prevTable->nextTable;
        }
        prevTable->nextTable = table;
    }

    int pageSize = PAGE_SIZE * BLOCK_SIZE;

    if (szBlock % pageSize == 0) {
        table->pageNumber = szBlock / pageSize;
    } else {
        table->pageNumber = szBlock / pageSize + 1;
    }


    table->descriptors = calloc(table->pageNumber, sizeof(struct pageDescription));
    int nextPageInMemoryNumber = 0;
    int nextPageInHDDNumber = 0;
/*    for (int descriptorIndex = 0; descriptorIndex < table->pageNumber; descriptorIndex++) {
        nextPageInMemoryNumber = getIndexNextFreeMemoryPage(nextPageInMemoryNumber);
        if (nextPageInMemoryNumber == -1) {
            nextPageInHDDNumber = getIndexNextFreeHDDPage(nextPageInHDDNumber);
            if (nextPageInHDDNumber == -1) {
                return OUT_OF_MEMORY_ERROR;
            } else{
                table->
            }
        }
        struct pageDescription description = table->descriptors[descriptorIndex];
        description.page = &memory->pages[nextPageInMemoryNumber];
        description.pageInfo = malloc(sizeof(struct pageInfo));

    }*/


}


