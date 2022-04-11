#if defined (_BOARD_CP24_V2_0) || defined (_BOARD_CP24_V2_1)

#include "TaskDataFlash.h"
#include "Global_include.h"
#include "GlobalVar.h"

u16 CodChangeUst;
u16 MasForUst[16] = {0};
//extern u16     MODBUS_FILE_RDWR;
//extern u16     MODBUS_PWM_ON;

void setSpiForFlash (CP24version_type versionCP24);
void FuncDataFlash_AT45 ();
void FuncDataFlash_W25 ();

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
  //type_FlashMessage *msg;  // Указатель на структуру с запросом к процессу

  // Запускаем канал обмена по SPI в зависимости от типа блока СР24
//#warning Заблокирована память Flash
  setSpiForFlash (GlobalM4.versionCP24);
     
  // семафор для исключения обращения к Flash во время считыванния/записи данных из самой Flash
  FlashSemRdWr = osSemaphoreNew(1, 1, &at45SemRdWr_attributes);                  // создаем бинарный семафор для чтения/записи параметров Flash
  //osSemaphoreAcquire(FlashSemRdWr, portMAX_DELAY);                             // Предзахват уже не нужен!!!
  xQueueFlash = xQueueCreate(10, sizeof(type_FlashMessage));                      // создаем очередь на 5 запросов к АТ45
  GlobalM4.BLOCK_DMA.all = 0;
  
  if (GlobalM4.versionCP24 == CP24_v21) {
    FuncDataFlash_AT45();
  } else {
    FuncDataFlash_W25();
  }
}

/*-----------------------------------------------------------------------------------------
// Подпрограмма задачи при работе с памятью АТ45
-----------------------------------------------------------------------------------------*/
void FuncDataFlash_AT45(void){
  type_FlashMessage *msg;  // Указатель на структуру с запросом к процессу  
  
  while(1)
  {    
    GlobalM4.BLOCK_DMA.all = 0;
    xQueueReceive(xQueueFlash, &msg, portMAX_DELAY);                             // Ждем запроса к Flash
//    MODBUS_FILE_RDWR = 6; // Заводим счетчик блокировки работы - 10 такто микропрограммы
    GlobalM4.BLOCK_DMA.bits.FlashDMA_Req = 1; // От флэши есть запрос на DMA
    
    switch(*msg->opCode)
    {
        case RD:
          while (GlobalM4.BLOCK_DMA.all & 0x03 ){
            osDelay(1);   // Пока GlobalM4.BLOCK_DMA = 1, ждем отпускания процесса
          }  
          msg->codeFail = spflRdAT45(msg->dest, msg->srs, msg->size);                             // Файловое чтения по UART
        break;
        
        case WR:
          while (GlobalM4.BLOCK_DMA.all & 0x03){
            osDelay(1); 
          }  
          msg->codeFail = spflWrPgAT45(msg->dest, msg->srs, msg->size);                           // Файловая запись по UART
        break; 
/*==============================================================================
Изменение уставок
================================================================================*/
        case RDUST:
           while (GlobalM4.BLOCK_DMA.all & 0x03){
            osDelay(1);   // Пока GlobalM4.BLOCK_DMA = 1, ждем отпускания процесса
           }
           RdUstAT45(msg);                                                          // Чтения уставок с FRAM (AT45->FRAM)
        break;
        case WRUST:  
           while (GlobalM4.BLOCK_DMA.all & 0x03){
            osDelay(1);   // Пока GlobalM4.BLOCK_DMA = 1, ждем отпускания процесса
           }
           WrUstAT45(msg);                                                          // Запись уставок в FRAM (FRAM->AT45)
        break;
//************************************************
//Сброс уставок в заводские значения
//************************************************
          case RDONEUST:                                                        //Читаем из AT45 уставки одной групы и копируем её в FRAM
            while (GlobalM4.BLOCK_DMA.all & 0x3){
              osDelay(1);   // Пока GlobalM4.BLOCK_DMA = 1, ждем отпускания процесса
            }            

            OneGrupAT45toFRAM(msg);                                             //сбрасываем группу уставок
          break;
    }
      if(msg->ptrSem != NULL)                                                   // Если кто-то ожидает, то послать семафор
        osSemaphoreRelease(FlashSemRdWr); 
               
    GlobalM4.BLOCK_DMA.all = 0x00;
  }  
}

/*-----------------------------------------------------------------------------------------
// Подпрограмма задачи при работе с памятью W25
-----------------------------------------------------------------------------------------*/
void FuncDataFlash_W25(void){
  type_FlashMessage *msg;  // Указатель на структуру с запросом к процессу  
  
  while(1)
  {
    xQueueReceive(xQueueFlash, &msg, portMAX_DELAY);                             // Ждем запроса к Flash
  
    switch(*msg->opCode)
    {
        case RD:
            spflRdW25(msg->dest, msg->srs, msg->size);                             // Файловое чтения по UART
        break;
        case WR:           
            spflWrSektorW25(msg->dest, msg->srs, msg->size);                           // Файловая запись по UART
        break; 
/*==============================================================================
Изменение уставок
================================================================================*/
        case RDUST:                                                             
           RdUstW25(msg);                                                          // Чтения уставок с FRAM (AT45->FRAM)
        break;
        case WRUST:                                                             
           WrUstW25(msg);                                                          // Запись уставок в FRAM (FRAM->AT45)
        break;
//************************************************
//Сброс уставок в заводские значения
//************************************************
          case RDONEUST:                                       //Читаем из AT45 уставки одной групы и копируем её в FRAM
             OneGrupW25toFRAM(msg);                           //сбрасываем группу уставок
             break;

    }
      if(msg->ptrSem != NULL)                                                   // Если кто-то ожидает, то послать семафор
        osSemaphoreRelease(FlashSemRdWr);
  }
}

/*-----------------------------------------------------------------------------------------
Определяем какой тип памяти будем запускать AT45 vs W25Q16
-----------------------------------------------------------------------------------------*/
void setSpiForFlash (CP24version_type versionCP24) {
  
  switch (versionCP24) {
    case CP24_v21: 
      setSpiForAt45();        // Если блок версии 2.1 Настроить порт SPI для АТ45
      break;
      
    case CP24_v22:
    case CP24_v23:
    case CP24_v24:      
      setSpiForW25();        // Если блок версии 2.2 и выше Настроить порт SPI для W25
      break;           
  }   
}

/*-----------------------------------------------------------------------------------------
Чтение уставок FRAM->AT45
-----------------------------------------------------------------------------------------*/
void WrUstAT45(type_FlashMessage *msg)
{
    u16 i;
    u16 crc = 0xffff;
    u16 AdrFromBeginPage;
    u16 size, Size;
    u16 Err;
    file_microprog_type *pntMicro;
    MicroProg_type *pntM;
    void *pntDst, *pntSrc;

    memset(MasForUst, 0, sizeof(MasForUst));

    pntM = (MicroProg_type *)MicroProgTabl;

    msg->size = ((MicroProg_type *)MicroProgTabl)->kolvUst;

    msg->size = pntM->kolvUst;

    pntMicro = (file_microprog_type *)(msg->dest);

    i = 0;
    if(msg->size > 0){
/*----------------------------------------------------------
                  Порча сигнатуры AT45
----------------------------------------------------------*/
        MasForUst[0] = pntM->SignatCoreMicro;                                          // swapU16(pntM->SignatCoreMicro);
        crc = CrcPortion(&MasForUst[0], sizeof(MasForUst[0])>>1, crc);                 // КС на сигнатуру и кол-во уставок
        MasForUst[1] =pntM->kolvUst;                                                   // swapU16(pntM->kolvUst);
        crc = CrcPortion(&MasForUst[1], sizeof(MasForUst[1])>>1, crc);
        MasForUst[0]--;                                                                // Бьем сигнатуру
        spflWrPgAT45(&pntMicro->UstMicro.HeadUst.MicroSignatura, &MasForUst[0], sizeof(u16));   // убиваем сигнатуру AT45
        spflRdAT45(&MasForUst[0], &pntMicro->UstMicro.HeadUst.MicroSignatura, sizeof(u16));
/*----------------------------------------------------------
      Подсчет КС на заголовок файла
----------------------------------------------------------*/
        pntSrc = &pntMicro->UstMicro.HeadUst.rsrvd;
        Size = sizeof(pntMicro->UstMicro.HeadUst.rsrvd)>>1;
        do{
            if(Size > 16){
                size = 16;
                Size -= 16;
            }
            else{
                size = Size;
                Size = 0;
            }
            spflRdAT45(&MasForUst[0], pntSrc, size*2);
            crc = CrcPortion(&MasForUst[0], size, crc);
            pntSrc = (u16*)((u32)pntSrc + (size<<1));
        }while (Size != 0);
/*----------------------------------------------------------
      Запись FRAM->АТ45 и подсчет контрольной суммы
----------------------------------------------------------*/
        pntDst = &pntMicro->UstMicro.MicroProg;
        Size = msg->size << 1;
        do{
            
            if ((u32)pntDst < 528)
                AdrFromBeginPage = (u32)msg->dest;
            else
                AdrFromBeginPage = ((u32)pntDst  % 528);       //Вычисление адреса относительно начала страницы
            
            if(Size > 16)
                size = 16;//в словах                          
            else
                size = Size;
            
            if ((AdrFromBeginPage + (size << 1)) > 528)                    //в байтах
                size = (528 - AdrFromBeginPage) >> 1;

            Size -= size;

            framRd(&nv.MicroProg[i],&MasForUst[0], size*2);
            crc = CrcPortion(&MasForUst[0], size, crc);

            i += size;
            *msg->opCode = ERRat45notErr;
                         
            if( !(spflWrPgAT45(pntDst, &MasForUst[0], size*2))){
                *msg->opCode  = NOTerr;                               //операция успешна
                Err = 2;                 //CodChangeUst = 2;          //провереятся в FACSET
            }
            else{                                                     //ошибка записи
                *msg->opCode |= ERRat45sign; 
                CodChangeUst = 3;
                return;
            }
            pntDst = (u16*)((u32)pntDst + (size<<1));                //адрес двигаем в количестве перед. слов, драйвер сам корректирует
        }while( Size !=0);
/*----------------------------------------------------------
      Восстановление сигнатуры и запись контрольной суммы
----------------------------------------------------------*/
        if (Err == 2){
            MasForUst[0] =pntM->SignatCoreMicro;            
            spflWrPgAT45(&pntMicro->UstMicro.HeadUst.MicroSignatura, &MasForUst[0], 2);

            MasForUst[0] = crc;
            spflWrPgAT45(&pntMicro->UstMicro.HeadUst.Crc, &MasForUst[0], 2);

            MasForUst[0] = pntM->kolvUst;
            spflWrPgAT45(&pntMicro->UstMicro.HeadUst.SzUst, &MasForUst[0], 2);
          
            CodChangeUst = 2;
        }
        else 
          CodChangeUst = 3;
    }
    else {
        CodChangeUst = 3;
    }
}
/*-----------------------------------------------------------------------------------------
Чтение уставок AT45->FRAM
-----------------------------------------------------------------------------------------*/
void RdUstAT45(type_FlashMessage *msg)
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
    
    size = osSemaphoreGetCount (FlashSemRdWr); 
/*----------------------------------------------------------
                  Проверка сигнатуры АТ45
----------------------------------------------------------*/
    spflRdAT45(&MasForUst[0], &pntMicro->UstMicro.HeadUst.MicroSignatura, 4);//читаем сигнатуру AT45 и количество уставок размером 4байта
    
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

            spflRdAT45(&MasForUst[0], pntSrs, size*2);
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
        
            spflRdAT45(&MasForUst[0], pntSrs, size*2);
            crc = CrcPortion(&MasForUst[0], size, crc);
            i += size;                                //Для вычисления смещения в FRAM
            pntSrs = (u16*)((u32)pntSrs + (size<<1));
        }while( Size !=0 );
        
        Size = msg->size;       //Для проверки КС fram
        pntSrs = msg->srs;
        
        spflRdAT45(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
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

            spflRdAT45(&MasForUst[0],pntSrs, size*2);

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
        
        spflRdAT45(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
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
	spflRdAT45(&MasForUst[0], &pntMicro->UstMicro.HeadUst.MicroSignatura, 6);//читаем сигнатуру AT45 и количество уставок размером 4байта
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

		spflRdAT45(&MasForUst[0], pntSrs, size*2);
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

		spflRdAT45(&MasForUst[0], pntSrs, size*2);
		crc = CrcPortion(&MasForUst[0], size, crc);
		i += size;                                //Для вычисления смещения в FRAM
		pntSrs = (u16*)((u32)pntSrs + (size<<1));
	 }while( Size !=0 );

	 Size = Siz;       //Для проверки КС fram
	 pntSrs = msg->srs;

	 spflRdAT45(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
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
			adr = (u32)((Wm->AdrPrm & ~LINEadr)*4); //вычисляем смещение относительно начала файла по линейному адресу (получаем адрес параметра, выраженный в байтах?)

			pntSrs = &pntMicro->UstMicro.MicroProg; //определяем адрес начала файла
			pntSrs = (u16*)((u32)pntSrs + adr);     //это адрес нужной нам уставки во флеш памяти.

		//	framRd(&MasForUst[0], &nv.MicroProg[adr/2], size);  //для проверки
			spflRdAT45(&MasForUst[0],pntSrs, size*2);                   //читаем уставку с флеш памяти
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

/*-----------------------------------------------------------------------------------------
Чтение уставок FRAM->W25
-----------------------------------------------------------------------------------------*/
void WrUstW25(type_FlashMessage *msg)
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
    spflWrSektorW25(&pntMicro->UstMicro.HeadUst, &HeadUst, sizeof(HeadUst_type));   // убиваем сигнатуру AT45

    // 6) Востановить в заголовке корректную сигнатуру
    HeadUst.MicroSignatura = pntM->SignatCoreMicro;                   // Сигнатура МП
    
    // 7) Посчитаем crc на заголовок (без crc - естественно)
    Size = (sizeof(HeadUst) - sizeof(HeadUst.Crc)) / 2;           // Розмер в словах
    pntSrc = (void*)((u32)(&HeadUst) + sizeof(HeadUst.Crc));  
    crc = CrcPortion(pntSrc, Size, crc);    
    
    // 8) Запись FRAM->W25 + подсчет контрольной суммы
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
       if( !(spflWrSektorW25(pntDst, &MasForUst[0], Size*2))){
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
    
    spflWrStW25((u32)(&pntMicro->UstMicro.HeadUst.Crc), (u32)(&HeadUst.Crc), sizeof(HeadUst.Crc));
    spflWrStW25((u32)(&pntMicro->UstMicro.HeadUst.MicroSignatura), (u32)(&HeadUst.MicroSignatura), sizeof(HeadUst.MicroSignatura));
          
    CodChangeUst = 2;
}

/*-----------------------------------------------------------------------------------------
Чтение уставок W25->FRAM
-----------------------------------------------------------------------------------------*/
void RdUstW25(type_FlashMessage *msg)
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
    spflRdW25(&MasForUst[0], &pntMicro->UstMicro.HeadUst.MicroSignatura, 4);//читаем сигнатуру AT45 и количество уставок размером 4байта
    
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

            spflRdW25(&MasForUst[0], pntSrs, size*2);
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
        
            spflRdW25(&MasForUst[0], pntSrs, size*2);
            crc = CrcPortion(&MasForUst[0], size, crc);
            i += size;                                //Для вычисления смещения в FRAM
            pntSrs = (u16*)((u32)pntSrs + (size<<1));
        }while( Size !=0 );
        
        Size = msg->size;       //Для проверки КС fram
        pntSrs = msg->srs;
        
        spflRdW25(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
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

            spflRdW25(&MasForUst[0],pntSrs, size*2);

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
        
        spflRdW25(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
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
void OneGrupW25toFRAM(type_FlashMessage *msg)
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
	spflRdW25(&MasForUst[0], &pntMicro->UstMicro.HeadUst.MicroSignatura, 6);//читаем сигнатуру AT45 и количество уставок размером 4байта
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

		spflRdW25(&MasForUst[0], pntSrs, size*2);
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

		spflRdW25(&MasForUst[0], pntSrs, size*2);
		crc = CrcPortion(&MasForUst[0], size, crc);
		i += size;                                //Для вычисления смещения в FRAM
		pntSrs = (u16*)((u32)pntSrs + (size<<1));
	 }while( Size !=0 );

	 Size = Siz;       //Для проверки КС fram
	 pntSrs = msg->srs;

	 spflRdW25(&MasForUst[0], &pntMicro->UstMicro.HeadUst.Crc, size*2);
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
			spflRdW25(&MasForUst[0],pntSrs, size*2);                   //читаем уставку с флеш памяти
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