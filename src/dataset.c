#include<stdio.h>
#include<stdlib.h>
#include"dataset.h"
#include <string.h>
#include <time.h>
#define DATACOL 9 //read_house_data() icin, house_price_data dosyasindaki kolon sayisi. 

/*
 * ID'si veilen evin anahtar degerini hesaplar.
 * birinciid:okunan dosyadaki ilk evin id'si
 */
int hash_compute(int id, int birinciid){
	return id-birinciid;
}

/*
* Okunan dosyada kac ev oldugunu Count structinin icindeki row ile dondurur.
*   -> read_house_data fonksiyonu tarafindan olusturulan hash table'in uygun boyutta olmasi icin onemli.
* Count structinin icindeki column ile ise okunan dosyadaki kolon sayisini dondur.
*   -> read_house_data() fonkisyonunun projedeki her iki dosyayi da okuyabilmesini saglar.
*/
Count count_house(char* dosya_adi){
	FILE* fp=fopen(dosya_adi, "r");
	char c;
	int row=0, column=0;
	if (!fp)
		printf("Dosyayi acamadik!\n");
	else{
		for (c = getc(fp); c != EOF; c = getc(fp)){
			if (c == '\n') //Linuxta CSV dosyalarinda satirlar '\n' karakteriyle biter
				row++;
			if(row==0 && c==',')
				column++;
		}
		fclose(fp);
	}
	Count count;
	count.row=row-1; //-1 yaptik cunku bir row tamamen basliklar
	count.column=column;
	return count;
}

/*
 * Verilen dosyayi okur icindeki ev bilgilerini House**houses tablosuna yerlestirir.
 * Count size: count_house() fonksiyonuna okunmasi istenen dosya ismi parametre olarak 
 * verildiginde dondurulen deger.
 * size.row: dosyadaki ev sayisi 
 * size.column: dosyadaki kolon sayisi (saleprice kolonu var mi?)
 */
void read_house_data(char* dosya_adi, House** houses, Count size){
	FILE* fp=fopen(dosya_adi, "r");
	if (!fp)
		printf("Dosya acilamadi!\n");
	else{
		char buffer[60000];
		int row=0;
		int column=0;
		int birinciid;
		while(fgets(buffer,60000,fp)){
			column=0;
			row++;
			if (row==1)  //basliklarin yazdigi ilk row'u atlamasi icin
				continue;
			char* value = strtok(buffer, ","); //Veriyi boluyoruz
			House* ev=malloc(sizeof(House));
			if(ev){
				while (value){
					if (column == 1){
						ev->id=atoi(value);//"value" bir karakter dizisidir ancak ID degeri bir
						                   //integerdir, atoi() fonksiyonuyla sayi olarak alabiliriz
						if(row==2){
							birinciid=ev->id; //hash_compute()'da kullanilmasi icin
						}
					}
					if(size.column == DATACOL){ //data dosyasindaki column sayisi
 						if (column == 2){
							ev->saleprice=atoi(value);
						}if (column == 3){
							ev->lotarea=atoi(value);
						}if (column == 4){
							strcpy(ev->street,value);
						}if (column == 5){
							ev->overallqual=atoi(value);
						}if (column == 6){
							ev->overallcond=atoi(value);
						}if (column == 7){
							strcpy(ev->kitchenqual,value);
						} if (column == 8){
							ev->yearbuilt=atoi(value);
						} if (column == 9){
							strcpy(ev->neighborhood,value);
						}
					}else{
						ev->saleprice=0; //test dosyasinda saleprice degeri bulunmadigi icin 0 atiyoruz
						if (column == 2){
							ev->lotarea=atoi(value);
						}if (column == 3){
							strcpy(ev->street,value);
						}if (column == 4){
							ev->overallqual=atoi(value);
						}if (column == 5){
							ev->overallcond=atoi(value);
						}if (column == 6){
							strcpy(ev->kitchenqual,value);
						} if (column == 7){
							ev->yearbuilt=atoi(value);
						} if (column == 8){
							strcpy(ev->neighborhood,value);
						}
					}
					value = strtok(NULL, ",");
					column++; //bir sonraki kolona gec
				}
				int index=hash_compute(ev->id,birinciid);
				houses[index]=ev; //tabloya yerlestiriyoruz

			}
			else
				printf("Hafiza acilamadi!\n"); //mallocla ev pointeri icin yer acilamadi
		}
		fclose(fp);
	}
}

/*
 * Size degeri kadar ev barindiracak bir hash tablosu olusturur, dondurur. 
 */
House** create_table(int size){
    House** table = malloc(size*sizeof(House*)); //verilen ev sayisini tasiyacak kadar yer acti
    int i;
    for (i=0; i<size; i++)
        table[i] = NULL; //acilan yeri bosaltti
    return table;
}

/*
 * Parametre olarak verilen evle ayni komsulukta olan kac tane ev oldugunu bulur.
 * get_neighborhoods fonskiyonunu kolaylastirmak icin yazilmistir.
 * size: houses'da bulunan ev sayisi
 */
int count_neighborhood(House* house, House** houses, int size){
	int i=0, count=0;
	for(;i<size;i++){
		if(strcmp(houses[i]->neighborhood,house->neighborhood)==0) //tabloyu tarar
		count++;
	}return count;
}

/*
 * house ile ayni komsulukta bulunan houses tablosundaki evleri dondurur.
 * Bunu, bu evleri yeni bir hash tablosuna ekleyerek gerceklestirir.
 * size: houses'da bulunan ev sayisi
 */
House** get_neighborhoods(House* house,House** houses,int size){
    if(house==NULL) {return NULL;}
    int i=0,j=0;
    int count=count_neighborhood(house,houses,size); //yeni olusturulacak hash tablosunun boyutunu belirlemek icin
    House**neighbors=create_table(count); 
    while(i<size&&j<count){
        if(strcmp(houses[i]->neighborhood,house->neighborhood)==0){
            neighbors[j]=houses[i]; //ayni komsuluktaysa yeni tabloya ekle
         i++;j++; //iki tabloyu da ilerlet
        }else //ayni komsulukta degilse bir sonraki houses elemanina gec
        i++;
    }
 return neighbors;
}

/* 
 * Parametre olarak verilen tek bir ev degerinin tum ozelliklerini ekrana bastirir.
 * Neighborhood okunurken sondakş '\n' karakteri de okundugu icin bu fonskiyonda 
 * '\n' ekstra konulmamistir
 */
void print_house(House* house){
	if(house==NULL){return;}
	printf("  ID: %d\n  Lot Area: %d\n", house->id, house->lotarea);
	printf("  Street: %s\n  Neighborhood :%s  Sale Price: %d\n", house->street,house->neighborhood, house->saleprice);
	printf("  Built Year: %d\n  Overall Quality: %d\n", house->yearbuilt, house->overallqual);
	printf("  Overall Condition: %d\n  Kitchen Quality: %s\n\n", house->overallcond, house->kitchenqual);
}

// Parametre olarak verilen ev listesindeki her bir evin ozelliklerini bastirir.
void print_houseList(House** houses, int size){
	int i;
	for(i=0; i<size; i++){
		printf("House [%d]:\n",i);
		print_house(houses[i]);
	}
}

//id degeri verilen bir evin bilgilerini geri donduren fonksiyon.
House* get_house_byid(House** houses, int id, int sizeofhouses){
	int hash_val = hash_compute(id, houses[0]->id);
	if (hash_val<0 || hash_val>=sizeofhouses){
		return NULL;
	}
	return houses[hash_val];
}
/*
 * Parametre olarak verilen evlerin yerlerini birbirleriyle degistirir.
 */
void swap(House* a, House* b){
    House t = *a;
    *a = *b;
    *b = t;
}

/*
 * Parametre olarak verilen hash tablosundaki evleri 
 * istenen kritere gore Quick Sort yontemiyle siralar.
 */
void quicksort(House**houses,int first,int last, char* criter_name){
 int i, j, pivot;
  if(strcmp(criter_name,"id")==0){
       printf("Zaten sirali\n");
    }
 else if(first<last && strcmp(criter_name,"id")!=0){ //first=last ise tek bir eleman var ve siralama yapmaya gerek yok demektir.
   pivot=first; //ilk elemanin indeksini pivot olarak atadik
   i=first;
   j=last;
   while(i<j){
    if(strcmp(criter_name,"lotarea")==0){
      while(houses[i]->lotarea<=houses[pivot]->lotarea&&i<last) //pivot'tan daha buyuk bir eleman bulana veya son elemana
          i++;                                     // ulasana kadar i'nin degerini arttiriyoruz (saga hareket ediyoruz)
      while(houses[j]->lotarea>houses[pivot]->lotarea) //pivottan kucuk/esit eleman bulana kadar
          j--;                                    //j'nin degerini azaltiyoruz (sola hareket ediyoruz)
    }if(strcmp(criter_name,"saleprice")==0){
      while(houses[i]->saleprice<=houses[pivot]->saleprice&&i<last)
          i++;
      while(houses[j]->saleprice>houses[pivot]->saleprice)
          j--;
    }if(strcmp(criter_name,"yearbuilt")==0){
      while(houses[i]->yearbuilt<=houses[pivot]->yearbuilt&&i<last)
          i++;
      while(houses[j]->yearbuilt>houses[pivot]->yearbuilt)
          j--;
    }if(strcmp(criter_name,"overallqual")==0){
      while(houses[i]->overallqual<=houses[pivot]->overallqual&&i<last)
          i++;
      while(houses[j]->overallqual>houses[pivot]->overallqual)
          j--;
    }if(strcmp(criter_name,"overallcond")==0){
      while(houses[i]->overallcond<=houses[pivot]->overallcond&&i<last)
          i++;
      while(houses[j]->overallcond>houses[pivot]->overallcond)
          j--;
    }if(strcmp(criter_name,"street")==0){ //alfabetik sıralama ayni mantik
       while (strcmp(houses[i]->street,houses[pivot]->street)<= 0 && i<last)
          i++;
       while (strcmp(houses[j]->street,houses[pivot]->street)>= 0 && j>first)
          j--;
    }if(strcmp(criter_name,"neighborhood")==0){
       while (strcmp(houses[i]->neighborhood,houses[pivot]->neighborhood)<= 0 && i<last)
          i++;
       while (strcmp(houses[j]->neighborhood,houses[pivot]->neighborhood)>= 0 && j>first)
          j--;
    }if(strcmp(criter_name,"kitchenqual")==0){
       while (strcmp(houses[i]->kitchenqual,houses[pivot]->kitchenqual)<= 0 && i<last)
          i++;
       while (strcmp(houses[j]->kitchenqual,houses[pivot]->kitchenqual)>= 0 && j>first)
          j--;
    }
      if(i<j){ //i<j olan son anda j'nin yeri, alt dizilerimizi ayrilma noktasi
        //i pivottan buyuk eleman buldu, j pivottan kucuk/esit eleman buldu
        //bunların yerlerini degistirerek buyuk elemanın sona daha yakin, kucuk elemanin
        //ise basa daha yakin olmasini sagliyoruz
       swap(houses[i],houses[j]);
      }
  }
 swap(houses[pivot],houses[j]);//i<j olan son anda j'nin yeri, alt dizilerimizin ayrilma noktasidir
     // o anda j'de bulunan eleman pivotla yer degistirir.
     // j+1 ilk pivot degerinden buyuk olan elemanlari bulunduran dizinin ilk indeksi, 
     // j-1 ise kucuk olanlarin son indeksine esittir.
 quicksort(houses,first,j-1,criter_name);
 quicksort(houses,j+1,last,criter_name);
 }
}


/*
 * Parametre olarak verilen ev tablosunu verilen kritere gore siralayip
 * bir *.csv dosyasina bu siralamayi kaydeder
 */
void sort_houses(House**houses,int first,int last, char* criter_name){
	char criter_file[25];
	strncpy(criter_file,criter_name,25);
	strncat(criter_file,".csv\0",5);
	FILE* fptr=fopen(criter_file, "w");
	int i;
	if(fptr == NULL){
		printf("Error!");
		exit(1);
	}
	
	else{
		quicksort(houses,first,last,criter_name);
		fprintf(fptr,",Id,SalePrice,LotArea,Street,OverallQual,OverallCond,KitchenQual,YearBuilt,Neighborhood\n");
		for(i=0;i<last;i++)
			fprintf(fptr,"%i,%i,%i,%i,%s,%i,%i,%s,%i,%s",i,houses[i]->id,houses[i]->saleprice,houses[i]->lotarea,houses[i]->street,houses[i]->overallqual,houses[i]->overallcond,houses[i]->kitchenqual,houses[i]->yearbuilt,houses[i]->neighborhood);
	fclose(fptr);
	}
}

/*
 * Parametre olarak criter_name'e gore yine parametre olarak verilen
 * houses hash tablosundaki Houses Struct elemaninin cesitlerinin her
 * biri icin ortalama satis degeri hesaplar ve erkana basar.
 */

void mean_sale_prices(House ** houses, int size, char* criter_name){
     
    int i = 0, j = 0, capacity = 1; // capacity = kriter cesit sayisi
   quicksort(houses, 0, size-1, criter_name);

   // id tek essiz Houses Struct elemanidir. 
    if(!strcmp(criter_name,"id"))
      return;

    // Verilen Houses Struct elemanindan kac cesit oldugu capacity degiskenine atanir.
    else if(!strcmp(criter_name,"lotarea")){
      for(i = 0; i < size-1; i++){
        if(houses[i]->lotarea != houses[i+1]->lotarea)
          capacity++;
      }
    }
    else if(!strcmp(criter_name,"street")){
      for(i = 0; i < size-1; i++){
        if(strcmp(houses[i]->street,houses[i+1]->street))
          capacity++;
      }
    }
    else if(!strcmp(criter_name,"neighborhood")){
      for(i = 0; i < size-1; i++){
        if(strcmp(houses[i]->neighborhood, houses[i+1]->neighborhood))
          capacity++;
      }
    }
    else if(!strcmp(criter_name,"yearbuilt")){
      for(i = 0; i < size-1; i++){
        if(houses[i]->yearbuilt != houses[i+1]->yearbuilt)
          capacity++;
      }
    }
    else if(!strcmp(criter_name,"overallqual")){
      for(i = 0; i < size-1; i++){
        if(houses[i]->overallqual != houses[i+1]->overallqual)
          capacity++;
      }
    }
    else if(!strcmp(criter_name,"overallcond")){
      for(i = 0; i < size-1; i++){
        if(houses[i]->overallcond != houses[i+1]->overallcond)
          capacity++;
      }
    }
    else if(!strcmp(criter_name,"kitchenqual")){
      for(i = 0; i < size-1; i++){
        if(strcmp(houses[i]->kitchenqual, houses[i+1]->kitchenqual))
          capacity++;
      }
    }

    float* mean = (float*) malloc(sizeof(float)*capacity); // ortalama tutan tablo
    float *sum = (float*) calloc(capacity, sizeof(float)); // toplam degerini (pay) tutan tablo
    float *miktar = (float*) calloc(capacity, sizeof(float)); // her bir cesidin adet sayisini (payda) tutan tablo
    j = 0;
    // for dongusu i= 1'den basladigi icin i = 0 icin ilk degerler atanir.
    sum[0] = houses[0]->saleprice;
    miktar[0] = 1;
    printf("capacity: %d\n",capacity);

    if(!strcmp(criter_name,"lotarea")){
       for(i = 1; i < size; i++){
          if(houses[i]->lotarea != houses[i-1]->lotarea){
            mean[j] = sum[j]/miktar[j];
            printf("(%5d ) Mean = %20.3f\tCriteria type: %10d\n",j+1,mean[j], houses[i-1]->lotarea);
            j++;
          }
        // En sonda eger farkli tipte bir verilen kriter tipi varsa
          if((i == size-1 && j == capacity-1) && houses[i]->lotarea != houses[i-1]->lotarea){
            miktar[j] = 1;
            sum[j] = houses[size-1]->saleprice;
            mean[j] = sum[j]/miktar[j];
            printf("(%5d ) Mean = %20.3f\tCriteria type: %10d\n",j+1,mean[j], houses[i]->lotarea);
          }
        sum[j] += houses[i]->saleprice;
        miktar[j]++;
      }
    }
    else if(!strcmp(criter_name,"street")){
       for(i = 1; i < size; i++){
        if(strcmp(houses[i]->street,houses[i-1]->street) || (j == capacity-1 && i == size-1)){
          if((j == capacity-1 && i == size-1) == 1){
            miktar[j]++;
            sum[j] += houses[size-1]->saleprice;
          }
          mean[j] = sum[j]/miktar[j];
          printf("(%5d ) Mean = %20.3f\tCriteria type: %15s\n",j+1,mean[j], houses[i-1]->street);
          j++;
        }
        if(j>=capacity) {break;}
        sum[j] += houses[i]->saleprice;
        miktar[j]++;
      }
    }
    else if(!strcmp(criter_name,"neighborhood")){ 
      for(i = 1; i < size; i++){
        if(strcmp(houses[i]->neighborhood,houses[i-1]->neighborhood) || (j == capacity-1 && i == size-1)){
          if((j == capacity-1 && i == size-1) == 1){
            miktar[j]++;
            sum[j] += houses[size-1]->saleprice;
          }
          mean[j] = sum[j]/miktar[j];
          printf("(%5d ) Mean = %20.3f\tCriteria type: %15s\n",j+1,mean[j], houses[i-1]->neighborhood);
          j++;
        }
        if(j>=capacity) {break;}
        sum[j] += houses[i]->saleprice;
        miktar[j]++;
      }
    }
    else if(!strcmp(criter_name,"overallqual")){
      for(i = 1; i < size; i++){
        if(houses[i]->overallqual != houses[i-1]->overallqual || (j == capacity-1 && i == size-1)){
          if((j == capacity-1 && i == size-1) == 1){
            miktar[j]++;
            sum[j] += houses[size-1]->saleprice;
          }
          mean[j] = sum[j]/miktar[j];
          printf("(%5d ) Mean = %20.3f\tCriteria type: %2d\n",j+1,mean[j], houses[i-1]->overallqual);
          j++;
        }
        if(j>=capacity) {break;}
        sum[j] += houses[i]->saleprice;
        miktar[j]++;
      }
    }
    else if(!strcmp(criter_name,"yearbuilt")){ 
      for(i = 1; i < size; i++){
        if(houses[i]->yearbuilt != houses[i-1]->yearbuilt){
          mean[j] = sum[j]/miktar[j];
          printf("(%5d ) Mean = %20.3f\tCriteria type: %d\n",j+1,mean[j], houses[i-1]->yearbuilt);
          j++;
        }
        // En sonda eger farkli tipte bir verilen kriter tipi varsa
        if((i == size-1 && j == capacity-1) && houses[i]->yearbuilt != houses[i-1]->yearbuilt){
            miktar[j] = 1;
            sum[j] = houses[size-1]->saleprice;
            mean[j] = sum[j]/miktar[j];
            printf("(%5d ) Mean = %20.3f\tCriteria type: %d\n",j+1,mean[j], houses[i]->yearbuilt);
          }
        sum[j] += houses[i]->saleprice;
        miktar[j]++;      
      }
    }

    else if(!strcmp(criter_name,"overallcond")){ 
      for(i = 1; i < size; i++){
        if(houses[i]->overallcond != houses[i-1]->overallcond || (j == capacity-1 && i == size-1)){
          if((j == capacity-1 && i == size-1) == 1){
            miktar[j]++;
            sum[j] += houses[size-1]->saleprice;
          }
          mean[j] = sum[j]/miktar[j];
          printf("(%5d ) Mean = %20.3f\tCriteria type: %d\n",j+1,mean[j], houses[i-1]->overallcond);
          j++;
        }
        if(j>=capacity) {break;}
        sum[j] += houses[i]->saleprice;
        miktar[j]++;
      }
    }
    else if(!strcmp(criter_name,"kitchenqual")){ 
      for(i = 1; i < size; i++){
        if(strcmp(houses[i]->kitchenqual,houses[i-1]->kitchenqual) || (j == capacity-1 && i == size-1)){
          if((j == capacity-1 && i == size-1) == 1){
            miktar[j]++;
            sum[j] += houses[size-1]->saleprice;
          }
          mean[j] = sum[j]/miktar[j];
          printf("(%5d ) Mean = %20.3f\tCriteria type: %s\n",j+1,mean[j], houses[i-1]->kitchenqual);
          j++;
        }
        if(j>=capacity) {break;}
        sum[j] += houses[i]->saleprice;
        miktar[j]++;
      }
    }
    printf(" *mean_sale_prices() %20s adina basarili*\n",criter_name);
    free(mean);
    free(sum);
    free(miktar);
}

// Parametre olarak verilen House hash tablosunun ozelliklerini sifirlar
// ve malloc ile daha onceden ayrilan bellegi siler.
void delete_table(House** table, int size){
	int i;
	for(i=size-1; i>=0; i--){
		strcpy(table[i]->street, "$$$$");
		strcpy(table[i]->neighborhood, "$$$$\n");
		strcpy(table[i]->kitchenqual, "$$");
		table[i]->lotarea = 0;
		table[i]->yearbuilt = 0;
		table[i]->overallqual = 0;
		table[i]->overallcond = 0;
		table[i]->saleprice = 0;
		free(table[i]);
	}
}


/*
 * --Main'in calisirken guzel formatta calismasi icin yazildi--
 *
 *  Parametre olarak verilen saniye kadar sure bekletir.
 */
void delay(double dly){
	/* save start time */
	const time_t start = time(NULL);
	time_t current;
	do{
		/* simdiki zamani al */
		time(&current);
		/* gerekli sure alindiginda donguyu kir */
	}while(difftime(current, start) < dly);
}

