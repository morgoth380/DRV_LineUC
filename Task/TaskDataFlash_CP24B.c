#if defined (_BOARD_CP24B_V1_0) || defined (_BOARD_CP24B_V1_1)

#include "TaskDataFlash.h"
#include "Global_include.h"
#include "GlobalVar.h"

u16 CodChangeUst;
u16 MasForUst[16] = {0};

xQueueHandle xQueueFlash;
// Семафор для работы с Flash. Чтения и запись данных.
osSemaphoreId_t FlashSemRdWr;            
const osSemaphoreAttr_t at45SemRdWr_attributes = {
  .name = "FlashSemRdWr"
};
/*============================================================================*/
//                       Задача SPI
// Задача пробуждается только по запросу приходящему в MBX. Для синхронизации
// задачи используются запросы к памяти FLASH по необходимости.
/*============================================================================*/
void FuncDataFlash(void *argument){
  type_FlashMessage *msg;  // Указатель на структуру с запросом к процессу
  setSpiForAt45();        // Настроить порт SPI для АТ45
  
  // семафор для исключения обращения к Flash во время считыванния/записи данных из самой Flash
  FlashSemRdWr = osSemaphoreNew(1, 1, &at45SemRdWr_attributes);                  // создаем бинарный семафор для чтения/записи параметров Flash
  osSemaphoreAcquire(FlashSemRdWr, portMAX_DELAY);
  xQueueFlash = xQueueCreate(10, sizeof(type_FlashMessage));                      // создаем очередь на 5 запросов к АТ45

  while(1)
  {
    xQueueReceive(xQueueFlash, &msg, portMAX_DELAY);                             // Ждем запроса к Flash
  
    switch(*msg->opCode)
    {
        case RD:
            spflRd(msg->dest, msg->srs, msg->size);                             // Файловое чтения по UART
        break;
        case WR:           
            spflWrSektor(msg->dest, msg->srs, msg->size);                           // Файловая запись по UART
        break; 
/*==============================================================================
Изменение уставок
================================================================================*/
        case RDUST:                                                             
           RdUst(msg);                                                          // Чтения уставок с FRAM (AT45->FRAM)
        break;
        case WRUST:                                                             
           WrUst(msg);                                                          // Запись уставок в FRAM (FRAM->AT45)
        break;
//************************************************
//Сброс уставок в заводские значения
//************************************************
          case RDONEUST:                                       //Читаем из AT45 уставки одной групы и копируем её в FRAM
             OneGrupAT45toFRAM(msg);                           //сбрасываем группу уставок
             break;

    }
      if(msg->ptrSem != NULL)                                                   // Если кто-то ожидает, то послать семафор
        osSemaphoreRelease(FlashSemRdWr);
  }
}


/*-----------------------------------------------------------------------------------------
Чтение уставок FRAM->AT45
-----------------------------------------------------------------------------------------*/
void WrUst(type_FlashMessage *msg)
{
    u16 i;
    u16 crc = 0xffff;
    u16 Size, SizeEnd;
    file_microprog_type *pntMicro;   // разметка файла во flash памяти
    HeadUst_type        HeadUst;     // Заголовок файла уставок 
    MicroProg_type      *pntM;
    void                *pntDst, *pntSrc;

    
    // 1) Инициализация указателей 
    pntM = (MicroProg_type *)MicroProgTabl;             // Указатель на данные микропрограммы (количество уставок и сигнатуру)
    pntMicro = (file_microprog_type *)(msg->dest);      // На файл уставок во Flash
    pntDst = &pntMicro->UstMicro.MicroProg;             // Указатель на тело файла уставок во flash
    
    // 2) Очистить копию заголовка файла уставок в ОЗУ
    memset(&HeadUst, 0xff, sizeof(HeadUst_type));
    
    // 3) Заполнить заголовок файла уставок (кроме сигнатуры и crc)
    HeadUst.Crc = 0xffff;                            // crc
    HeadUst.MicroSignatura = 0xffff;                 // Сигнатура МП
    HeadUst.SzUst = msg->size = pntM->kolvUst;       // Количество уставок
    
    // 4) Проверим количество уставок
    if(HeadUst.SzUst == 0){
       CodChangeUst = 3;      // Ошибка, количество уставок не может равнятся нулю
       
       return;                // выходим!!!        
    }

    // 5) Записать заголовок файла уставок во Flash (с заведомо не корректными сигнатурой и crc - их востановим в последнюю очередь после записи всего файла) 
    spflWrSektor(&pntMicro->UstMicro.HeadUst, &HeadUst, sizeof(HeadUst_type));   // убиваем сигнатуру AT45

    // 6) Востановить в заголовке корректную сигнатуру
    HeadUst.MicroSignatura = pntM->SignatCoreMicro;                   // Сигнатура МП
    
    // 7) Посчитаем crc на заголовок (без crc - естественно)
    Size = (sizeof(HeadUst) - sizeof(HeadUst.Crc)) / 2;           // Розмер в словах
    pntSrc = (void*)((u32)(&HeadUst) + sizeof(HeadUst.Crc));  
    crc = CrcPortion(pntSrc, Size, crc);    
    
    // 8) Запись FRAM->АТ45 + подсчет контрольной суммы
    SizeEnd = msg->size << 1;   // Количество данных которые нужно записать
    i = 0;                     
    
    do{
       // 9) Проверим остаток для записи
       if(SizeEnd > 16){
          Size = 16;        //в словах                          
       }
       else{
          Size = SizeEnd;    // Данных для записи меньше чем размер страници - дописываем только остаток
       }         
       
       // 10) Обновить размер данных который необходимо еще записать
       SizeEnd -= Size;

       // 11) Читаем кусок данных из FRAM
       framRd(&nv.MicroProg[i], &MasForUst[0], Size*2);
       
       // 12) Посчитать КС на кусок вычитанных данных
       crc = CrcPortion(&MasForUst[0], Size, crc);

       // 13) Инкримент указателя на файл уставок во flash
       i += Size;
       *msg->opCode = ERRat45notErr;
       
       // 14) Запись вычитанного куска в Flash
       if( !(spflWrSektor(pntDst, &MasForUst[0], Size*2))){
           *msg->opCode  = NOTerr;                               //операция успешна
       }
       else{                                                     //ошибка записи
           *msg->opCode |= ERRat45sign; 
           CodChangeUst = 3;
           return;
       }
       
       // 15) Инкримент указателя на файл во Flash
       pntDst = (u16*)((u32)pntDst + (Size<<1));                //адрес двигаем в количестве перед. слов
    
    }while(SizeEnd != 0); 
    
    // 16) Востановить сигнатуру и crc
    HeadUst.Crc = crc;                                // crc
    HeadUst.MicroSignatura = pntM->SignatCoreMicro;   // Сигнатура МП
    
    spflWrSt((u32)(&pntMicro->UstMicro.HeadUst.Crc), (u32)(&HeadUst.Crc), sizeof(HeadUst.Crc));
    spflWrSt((u32)(&pntMicro->UstMicro.HeadUst.MicroSignatura), (u32)(&HeadUst.MicroSignatura), sizeof(HeadUst.MicroSignatura));
          
    CodChangeUst = 2;
}

/*-----------------------------------------------------------------------------------------
Чтение уставок AT45->FRAM
-----------------------------------------------------------------------------------------*/
void RdUst(type_FlashMessage *msg)
{
    u32 i;
    u16 size, Size;
    file_microprog_type *pntMicro;
    void *pntSrs;
    u16 crc = 0xFFFF;
    u16 crc1 = 0xffff;

    MicroProg_type *pntM;

    memset(MasForUst, 0, sizeof(MasForUst));
    
    pntM = (MicroProg_type *)MicroProgTabl;
    pntMicro = (file_microprog_type *)msg->srs;

/*----------------------------------------------------------
                  Проверка сигнатуры АТ45
----------------------------------------------------------*/
    spflRd(&MasForUst[0], &pntMicro->UstMicro.HeadUst.MicroSignatura, 4);//читаем сигнатуру AT45 и количество уставок размером 4байта
    
    if(MasForUst[1] != pntM->kolvUst){
        CodChangeUst = 3;
        return;
    }
    
    msg->size = MasForUst[1];
    crc = CrcPortion(&MasForUst[0], 2, crc);    //КС на сигнатуру и кол-во уставок

    if (MasForUst[0] !=  pntM->SignatCoreMicro){
        CodChangeUst = 3;
        return;
    }
    if(msg->size > 0){
/*----------------------------------------------------------
      Подсчет контрольной суммы микропрограммы в АТ45
----------------------------------------------------------*/
        Size = (sizeof(pntMicro->UstMicro.HeadUst.rsrvd)>>1);
        pntSrs = &pntMicro->UstMicro.HeadUst.rsrvd;
        i = 0;
        do{
            if(Size > 16){
                size = 16;
                Size -= 16;
            }
            else{
                size = Size;
                Size = 0;
            }

            spflRd(&MasForUst[0], pntSrs, size*2);
            crc = CrcPortion(&MasForUst[0], size, crc);
            i += size;                                //Для вычисления смещения в FRAM
            pntSrs = (u16*)((u32)pntSrs + (size<<1));
        }while( Size !=0 );

        Size = (msg->size << 1);
        pntSrs = &pntMicro->UstMicro.MicroProg;
        i = 0;
        do{
            osDelay(1);
            if(Size > 16){
                size = 16;
                Size -= 16;
            }
            else{
                size = Size;
                Size = 0;
            }
        
            spflRd(&MasForUst[0], pntSrs, size*2);
            crc = CrcPortion(&MasForUst[0], size, crc);
            i += size;                                //Для вычисления смещения в FRAM
            pntSrs = (u16*)((u32)pntSrs + (size<<1));
        }while( Size !=0 );
        
        Size = msg->size;       //Для проверки КС fram
        pntSrs = msg->srs;
        
        spflRd(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
        if (crc != MasForUst[0]){                     //Проверка КС ат45
            CodChangeUst = 3;
            return;
        }
/*----------------------------------------------------------
                    Порча сигнатуры FRAM
----------------------------------------------------------*/
        MasForUst[0] = pntM->SignatCoreMicro - 1;
        framWr(&nv.MemSignatura, &MasForUst[0], sizeof(u16));   //Перетираем сигнатуру, если что-то пойдет не так
/*----------------------------------------------------------
                    Запись AT45->FRAM
----------------------------------------------------------*/
        i = 0;
        Size = msg->size << 1;
        pntSrs = &pntMicro->UstMicro.MicroProg;
        crc = 0xffff;
        do{
            if(Size > 16){
                size = 16;
                Size -= 16;
            }
            else{
                size =Size;
                Size = 0;
            }

            spflRd(&MasForUst[0],pntSrs, size*2);

            framWr(&nv.MicroProg[i], &MasForUst[0], size*2);
            crc = CrcPortion(&MasForUst[0], size, crc);
            i += size;                                //Для вычисления смещения в FRAM
            pntSrs = (u16*)((u32)pntSrs + (size<<1));
        }while( Size !=0 );
        
/*----------------------------------------------------------
             Подсчет контрольной суммы FRAM
----------------------------------------------------------*/
        i = 0;
        Size = msg->size << 1;
        crc1 = 0xffff;
        do{
            if(Size > 16){
                size = 16;
                Size -= 16;
            }
            else{
                size = Size;
                Size = 0;
            }
            framRd(&nv.MicroProg[i],&MasForUst[0], size*2);
            crc1 = CrcPortion(&MasForUst[0], size, crc1);
            i += size;                                //Для вычисления смещения в FRAM
        }while( Size !=0 );
        
        spflRd(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
        if (crc1 != crc){                     //Проверка КС ат45 и FRAM
            CodChangeUst = 3;            
        }
/*----------------------------------------------------------
                Восстановление сигнатуры
----------------------------------------------------------*/
        else{
            MasForUst[0] =  pntM->SignatCoreMicro;
            framWr(&nv.MemSignatura, &MasForUst[0], sizeof(u16));//Восстанавливаем сигнатуру
            CodChangeUst = 1;                                    //провереятся в FACSET
        }
    }
    else {
        CodChangeUst = 3;
    }
}

//**********************************************************
//Сброс уставок в заводские значения
//**********************************************************
void OneGrupAT45toFRAM(type_FlashMessage *msg)
{
	u32 i;
	u16 size, Size, Siz;
	file_microprog_type *pntMicro;
	void *pntSrs;
	u16 crc = 0xFFFF;
	s32 Grup = 0;
	s32 Param = 0;
	u32 adr = 0;
	MicroProg_type *pntM;
	wm_type Wmem, *Wm;
	u32 FlagsPrm = 0;

	Wm = &Wmem;
	Grup = msg->size;  //сохраняем номер группы

	memset(MasForUst, 0, sizeof(MasForUst));
	pntM = (MicroProg_type *)MicroProgTabl;
	pntMicro = &spfl.fileMicroProg1;         //указатель на начало файла с настройками по умолчанию
/*----------------------------------------------------------
				  Проверка сигнатуры АТ45
----------------------------------------------------------*/
	spflRd(&MasForUst[0], &pntMicro->UstMicro.HeadUst.MicroSignatura, 6);//читаем сигнатуру AT45 и количество уставок размером 4байта
	Siz = MasForUst[1];
	crc = CrcPortion(&MasForUst[0], 2, crc);    //КС на сигнатуру и кол-во уставок

	if (MasForUst[0] !=  pntM->SignatCoreMicro){
		CodChangeUst = 3;
		return;
	}

/*----------------------------------------------------------
	  Подсчет контрольной суммы микропрограммы в АТ45
----------------------------------------------------------*/
	Size = (sizeof(pntMicro->UstMicro.HeadUst.rsrvd)>>1);
	pntSrs = &pntMicro->UstMicro.HeadUst.rsrvd;
	i = 0;
	do{
		 if(Size > 16)
		 {
			size = 16;
			Size -= 16;
		 }
		else
		{
			size = Size;
			Size = 0;
		}

		spflRd(&MasForUst[0], pntSrs, size*2);
		crc = CrcPortion(&MasForUst[0], size, crc);
		i += size;                                //Для вычисления смещения в FRAM
		pntSrs = (u16*)((u32)pntSrs + (size<<1));
	 }
	 while( Size !=0 );

	 Size = (Siz << 1);
	 pntSrs = &pntMicro->UstMicro.MicroProg;
	 i = 0;
	 do{
		//Task_sleep(1);
                osDelay(1);
		if(Size > 16){
			size = 16;
			Size -= 16;
		}
		else{
			size = Size;
			Size = 0;
		}

		spflRd(&MasForUst[0], pntSrs, size*2);
		crc = CrcPortion(&MasForUst[0], size, crc);
		i += size;                                //Для вычисления смещения в FRAM
		pntSrs = (u16*)((u32)pntSrs + (size<<1));
	 }while( Size !=0 );

	 Size = Siz;       //Для проверки КС fram
	 pntSrs = msg->srs;

	 spflRd(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
	 if (crc != MasForUst[0]){                     //Проверка КС ат45
		CodChangeUst = 3;
		return;
	}

/*----------------------------------------------------------
					Порча сигнатуры FRAM
----------------------------------------------------------*/
	MasForUst[0] = pntM->SignatCoreMicro - 1;
	framWr(&nv.MemSignatura, &MasForUst[0], sizeof(u16));   //Перетираем сигнатуру, если что-то пойдет не так
/*----------------------------------------------------------
					Запись AT45->FRAM
----------------------------------------------------------*/

	size = (sizeof(s32))/2;

	for(Param = 0; Param < MAXPARAMGRUP; Param++) //Param = 0...99
	{
		Wm->AdrPrm = (Grup << 7) | Param; //Вычисляем адрес каждого параметра для переданного данной функции номера группы

		FlagsPrm = GetFlagsPrm(Wm);                                        //проверяем существует ли такая уставка.
		if( (FlagsPrm == Ok_err) || (FlagsPrm == NotInCurFunctional_err) ) //NotInCurFunctional_err - это маскирует уставку в зависимости от функционала
		{
			adr = (u32)((Wm->AdrPrm & (u16)(~LINEadr))*4); //вычисляем смещение относительно начала файла по линейному адресу (получаем адрес параметра, выраженный в байтах?)

			pntSrs = &pntMicro->UstMicro.MicroProg; //определяем адрес начала файла
			pntSrs = (u16*)((u32)pntSrs + adr);     //это адрес нужной нам уставки во флеш памяти.

		//	framRd(&MasForUst[0], &nv.MicroProg[adr/2], size);  //для проверки
			spflRd(&MasForUst[0],pntSrs, size*2);                   //читаем уставку с флеш памяти
			framWr(&nv.MicroProg[adr/2], &MasForUst[0], size*2);    //пишем уставку в фрам
		}
		else
		{
			MasForUst[0] =  pntM->SignatCoreMicro;
			framWr(&nv.MemSignatura, &MasForUst[0], sizeof(u16));  //Восстанавливаем сигнатуру!!!

			CodChangeUst = 1;  //OK!!!
			 break;
		}

	 }
}
#endif