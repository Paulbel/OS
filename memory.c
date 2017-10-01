#include "stdafx.h"
#include "mmemory.h"

#include <stdlib.h>

int _init(int n, int szPage);

void page_init(int n, int pageSize);

void block_init();

struct block *get_block_by_va(VA ptr, int size);


struct pageDescription {
    struct page *page;
    struct pageInfo pageInfo;
    int offset;
};


struct memory {
    struct page *pages;
    int pageNumber;
    int pageSize;
    int *isUsed;//1 if used, 0 if not
    int freeMemorySize;
};

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

struct memory *memory;
//struct page *memory;
//int freeMemorySize;
//int pageSize;
//int pageNumber;

int getFreeMemoryIndex() {
    for (int adress = 0; adress < memory->pageNumber; adress++) {
        if (memory->isUsed[adress] == 1) {
            return adress;//adress of the first free page in physical memory
        }
    }
    return 0;//if there are no free pages in physical memory
}

int pageIsFreeByAdress(int adress) {
    if (memory->isUsed[adress] == 0) {
        return 1;//if page in this adress is free
    }
    return 0;//if it isnt free
}


int _init(int n, int szPage) {
    if (szPage <= 0 || n <= 0) {
        return -1;
    }

    memory = malloc(sizeof(memory));
    memory->pages = calloc(n, sizeof(struct page));
    memory->pageNumber = n;
    memory->pageSize = szPage;
    memory->freeMemorySize = n*szPage;
    for (int pageIndex = 0; pageIndex < memory->pageNumber; ++pageIndex) {
        struct page page;
        page.pFirstFree = malloc(sizeof(struct block));
        page.pFirstUse = malloc(sizeof(struct block));
        page.pFirstFree->pNext = NULL;
        page.pFirstFree->szBlock = szPage;
        page.pFirstUse->szBlock = 0;
        page.maxSizeFreeBlock = szPage;
        memory->pages[pageIndex] = page;
    }
    memory->isUsed = calloc(n, sizeof(int));
    for (int index = 0; index < n; index++) {
        memory->isUsed[index] = 0;
    }


    VA *p;
    _malloc(p, 3);
}


int _malloc(VA *ptr, size_t szBlock) {
    if (szBlock <= 0 || !ptr)
        return -1;
    if (szBlock > memory->freeMemorySize)
        return -2;

    memory->freeMemorySize -= szBlock;

    int sizeLeft = szBlock;


    int pageNumber = sizeLeft / memory->pageSize;
    if ((sizeLeft % memory->pageSize) > 0) {
        pageNumber++;
    }
    //struct pageDescription *pageDescription = calloc(pageNumber, sizeof(struct pageDescription));


    struct pageDescription **pageTable =calloc(pageNumber, sizeof(struct pageDescription));

////
    for (int number = 0; number < pageNumber; number++) {
        int freePageNumber = getFreeMemoryIndex();
        struct pageDescription *pageDescription = malloc(sizeof(struct pageDescription));

        pageTable[number] = pageDescription;


        pageDescription = malloc(sizeof(struct pageDescription));

        pageDescription->page = &(memory->pages[freePageNumber]);

        memory->isUsed[freePageNumber] = 1;

        //pageDescription[number] = malloc(sizeof(struct pageDescription));
    }


    for (int pageIndex = 0; pageIndex < pageNumber; ++pageIndex) {
        struct page *page = pageTable[pageIndex]->page;


        if (page->pFirstUse->szBlock != 0) {
            page->pFirstUse->szBlock = 0;
            page->pFirstFree->szBlock = memory->pageSize;
        }

        if (sizeLeft >= memory->pageSize) {
            sizeLeft -= memory->pageSize;
        } else {
            page->pFirstUse->szBlock = sizeLeft;
            page->pFirstFree->szBlock = memory->pageSize - sizeLeft;
        }
    }

    // while (sizeLeft >= 0) {
    // pageDescription[]
    //sizeLeft -= memory->pageSize;
    //struct pageDescription *pageDescription = malloc(sizeof(struct pageDescription));
    //pageDescription->page.maxSizeFreeBlock = 3;

    //pageDescription = (struct pageDescription *) calloc(n, (sizeof(struct pageDescription)));
    //pageData = (struct data *) calloc(n, sizeof(struct data));
/*        for (int i = 0; i < n; i++) {
            struct pageDescription *pageD = (struct pageDescription *) malloc(sizeof(struct pageDescription));
            pageDes[i] = pageD;
            pageD->page.maxSizeFreeBlock = NULL;
            pageD->page.pFirstFree = first_block;
            pageD->page.pFirstUse = NULL;
            pageD->pageI.isUse = '0';
            pageD->pageI.offsetPage = create_offset(i * SIZE_OF_PAGE);
            pageDes[i] = malloc(sizeof(struct pageDescription));


            struct data *pageData_ = malloc(sizeof(struct data));;
            pageData_->numberOfPage = i;
            pageData_->offset = create_offset(i * SIZE_OF_PAGE);
            pageData_->someData = malloc(sizeof(SIZE_OF_PAGE));
            pageData_->someData[SIZE_OF_PAGE] = '\0';
            pageData[i] = pageData_;
        }*/
    // }


    return 0;
}


