#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dataset.h"
#include "models.h"


// PROGRAMI HATA VE BASARI DURUMUNDA BITIRMEK ICIN DEGISKENLER
#define EXIT_ERROR_ARGUMENTS 1
#define EXIT_FILE_ERROR 1
#define EXIT_SUCCES 0

// WHILE DONGUSUNU SONLANDIRMAK/DEVAM ETMEK ICIN SABIT DEGERLER
#define NOT_STOP 1
#define STOP 0

// ANA MENUDE SECENEKLERI GORMEK/GORMEMEK ICIN DEGISKEN SABITI
#define PRINT_MENU 1

int main(int argc, char* argv[]){
	
	if(argc != 3){ // EGER ARGUMANLAR YANLIS GIRILIRSE HATA VERMEK ICIN
		printf("Arguman sayisi 2'den az veya fazla olamaz!\n");
		printf("Please try: <<  <argv[0]> <argv[1]> <argv[2]>  >>\n");
		printf("\targ[1] : *.csv file for data analysis\n\targ[2] : *.csv file for price prediction\n");
		exit(EXIT_ERROR_ARGUMENTS);
	}
	
	Count houseList_size = count_house(argv[1]);
	House* house_list[houseList_size.row];
	memset(house_list,0,sizeof(house_list));
	read_house_data(argv[1], house_list, houseList_size);
	if(house_list == NULL){ // ARGUMAN OLARAK GONDERILEN DATA DOSYASI BULUNAMAZSA VEYA DOSYA ACILAMAZSA HATA VERMEK ICIN
		printf("Dosya bulunamadi veya hafiza tahsis edilemedi!\n");
		exit(EXIT_FILE_ERROR);
	}
	House* temp[houseList_size.row]; //sıralama yapilan seceneklerde kullanilmasi icin tanimlandi.
    	memset(temp,0,sizeof(temp));
   	read_house_data(argv[1],temp,houseList_size);
	
	Count testList_size = count_house(argv[2]);
	House* test_list[testList_size.row];
	memset(test_list,0,sizeof(test_list));
	read_house_data(argv[2], test_list, testList_size);
	if(test_list == NULL){ // ARGUMAN OLARAK GONDERILEN TEST DOSYASI BULUNAMAZSA VEYA DOSYA ACILAMAZSA HATA VERMEK ICIN
		printf("Dosya bulunamadi veya hafiza tahsis edilemedi!\n");
		exit(EXIT_FILE_ERROR);
	}

	int control = NOT_STOP; // PROGRAMDAN CIKILMAMASI ICIN BELIRLENEN DEGER
	
	printf("\nEmlak Programina Hos Geldiniz!\n");
	int operation;
	
	// asagidaki deger, ana menunun ilk seferden sonra istege bagli olarak bastirilmasi icin tanimlandi.
	int print_menu = PRINT_MENU;
	
	do{	
		printf("__________________________________________________________________________________\n\n");
		if (print_menu==PRINT_MENU){
			printf("Yapmak istediginiz islemin numarasini asagidan secebilirsiniz:\n\n");
			printf("1 - Evleri listele\n2 - ID degeri ile ev bul\n3 - ID degeri verilen evin komsu evlerini bul\n4 - Kritere gore ortalama fiyatlari listele\n5 - En yuksek fiyata sahip ilk 10 evi goster\n6 - Evleri sirala & kaydet\n7 - Evler icin fiyat tahmini yap (model_by_similarity)\n8 - Evler icin fiyat tahmini (dogrusal iliski)\nProgramdan cikmak icin 0'a basiniz.\n");
		}
		else{
			printf("(Ana menu seceneklerini tekrar gormek icin 9, programdan cikmak icin 0 tuslayiniz.)\n");
		}
		
		printf("Yapmak istediginiz islem:\n");
		scanf("%d", &operation);
		if(operation==9){
			print_menu=1;
			continue;
		}print_menu++;
		
		printf("Yonlendiriliyorsunuz..\n");
		printf("__________________________________________________________________________________\n");
		delay(1);
		
		switch (operation){
			case STOP:  // CIKIS YAPMA
				control = STOP;
				printf("Cikis yapiliyor\n");
				delay(1);
				delete_table(house_list, houseList_size.row);
				delete_table(temp, houseList_size.row);
				delete_table(test_list,testList_size.row);
				break;
				
			case 1:  // EV LISTELEME
				printf("Evler hazirlaniyor..\n");
				delay(2);
				print_houseList(house_list, houseList_size.row);
				delay(2);
				printf("Ana menuye donuluyor..\n");
				delay(1);
				break;
				
			case 2:  // ID ILE EV BULMA
				printf("Aramak istediginiz evin ID numarasini giriniz:\n");
				int id_input;
				scanf("%d", &id_input);
				printf("Araniyor...\n");
				delay(1);
				House* house_searched = get_house_byid(house_list, id_input, houseList_size.row);
				if(house_searched==NULL){
					printf("%d ID nolu bir ev bulunamadi.\n",id_input);
				}
				else{
					printf("%d ID nolu ev:\n", id_input);
					print_house(house_searched);
				}
				delay(2);
				printf("Ana menuye donuluyor..\n");
				delay(1);
				//free aranan house
				break;
				
			case 3:  // ID ILE KOMSULAR BULMA
                		printf("Komsularini gormek istediginiz evin ID no'sunu giriniz:\n");
                		int id_toget_neighbor;
               			scanf("%d",&id_toget_neighbor);
                		printf("Komsular araniyor..\n");
                		delay(2);
                		if (get_house_byid(house_list, id_toget_neighbor, houseList_size.row) == NULL){
                   	 		printf("ID no verilen ev gecersiz!\n");
                		}
                		else{
                    			int count = count_neighborhood(get_house_byid(house_list, id_toget_neighbor, houseList_size.row), house_list, houseList_size.row);
                   			House** houses_neighbors = get_neighborhoods(get_house_byid(house_list, id_toget_neighbor, houseList_size.row),house_list,houseList_size.row);
                    			int i=0;
                   			while(i<count){
                        			print_house(houses_neighbors[i]);
                        			i++;
                    			}
                    			free(houses_neighbors);
                		}
                		delay(2);
                		printf("Ana menuye donuluyor..\n");
                		delay(1);
               			 break;
				
			case 4:  // KRITERE GORE ORTALAMA FIYAT YAZDIRMA
				printf("Kriterler:\n1 - Ev alani\n2 - Sokak adi\n3 - Mahalle adi\n4 - Imar yili\n5 - Genel niteligi\n6 - Genel durumu\n7 - Mutfak kalitesi\n\n");
				printf("Fiyat ortalamarini gormek istediginiz kriterin numarasini seciniz:\n");
				int criteria_mean;
				scanf("%d",&criteria_mean);
				delay(2);
				if(criteria_mean==1){ // lotarea
					printf("Ev alanlarina gore evlerin fiyat ortalamasi:\n");
					mean_sale_prices(temp, houseList_size.row,"lotarea");
				}
				else if(criteria_mean==2){ // street
					printf("Sokaklara gore evlerin fiyat ortalamasi:\n");
					mean_sale_prices(temp, houseList_size.row,"street");
				}
				else if(criteria_mean==3){ // neighborhood
					printf("Mahallelere gore evlerin fiyat ortalamasi:\n");
					mean_sale_prices(temp, houseList_size.row,"neighborhood");
				}
				else if(criteria_mean==4){ // yearbuilt
					printf("Imar yilina gore evlerin fiyat ortalamasi:\n");
					mean_sale_prices(temp, houseList_size.row,"yearbuilt");
				}
				else if(criteria_mean==5){ // overallqual
					printf("Genel niteliklere gore evlerin fiyat ortalamasi:\n");
					mean_sale_prices(temp, houseList_size.row,"overallqual");
				}
				else if(criteria_mean==6){ // overallcond  BUNDA BIR SIKINTI CIKIYOR! SONRA BAKMALI SIMDI USENDIM
					printf("Genel durumlara gore evlerin fiyat ortalamasi:\n");
					mean_sale_prices(temp, houseList_size.row,"overallcond");
				}
				else if(criteria_mean==7){ // kitchenqual
					printf("Mutfak kalitelerine gore evlerin fiyat ortalamasi:\n");
					mean_sale_prices(temp, houseList_size.row,"kitchenqual");
				}
				else{
					printf("Gecersiz kriter!\n");
					break;
					
				}
				
				// free stock
				delay(2);
				printf("Ana menuye donuluyor..\n");
				delay(1);
				break;
				
			case 5: ; // EN YUKSEK FIYATA SAHIP ILK 10EV 
				quicksort(temp, 0, houseList_size.row-1, "saleprice");
				int i,k=1;
				for(i=houseList_size.row-1; i>houseList_size.row-11; i--){
					printf("Ev [%2d]\n",k);
					print_house(temp[i]);
					k++;
				}
				delay(2);
				printf("Ana menuye donuluyor..\n");
				delay(1);
				break;
				
			case 6:  // KRITERE GORE EVLERI SIRALAMA & DOSYAYA KAYDETME
				printf("Kriterler:\n1 - Ev alani\n2 - Sokak adi\n3 - Mahalle adi\n4 - Imar senesi\n5 - Genel niteligi\n6 - Genel durumu\n7 - Mutfak niteligi\n8 - Fiyati\n");
				printf("Evleri siralamak istediginiz kriterin numarasini giriniz:\n");
				int criteria_sort;
				scanf("%d",&criteria_sort);
				delay(2);
				if(criteria_sort==1){ // lotarea
					sort_houses(temp, 0, houseList_size.row-1, "lotarea");
				}
				else if(criteria_sort==2){ // street
					sort_houses(temp, 0, houseList_size.row-1, "street");
				}
				else if(criteria_sort==3){ // neighborhood
					sort_houses(temp, 0, houseList_size.row-1, "neighborhood");
				}
				else if(criteria_sort==4){ // yearbuilt
					sort_houses(temp, 0, houseList_size.row-1, "yearbuilt");
				}
				else if(criteria_sort==5){ // overallqual
					sort_houses(temp, 0, houseList_size.row-1, "overallqual");
				}
				else if(criteria_sort==6){ // overallcond
					sort_houses(temp, 0, houseList_size.row-1, "overallcond");
				}
				else if(criteria_sort==7){ // kitchenqual
					sort_houses(temp, 0, houseList_size.row-1, "kitchenqual");
				}
				else if(criteria_sort==8){ // saleprice
					sort_houses(temp, 0, houseList_size.row-1, "saleprice");
				}
				else{
					printf("Yanlis tuslama!\n");
				}
				printf("Liste siralandi! Sirali listeyi dosyalarinizda bulabilirsiniz.\n");
				delay(2);
				printf("Ana menuye donuluyor..\n");
				delay(1);
				break;
			
			case 7: //MODEL_BY_SIMILARITY FONKSIYONU ILE HOUSE_PRICE_TEST.CSV DOSYASINDAKI EVLERIN FIYATLARINI TAHMIN ETME
				printf("Fiyat tahmini icin veriler isleniyor...\n");
				delay(1);
               		FILE* f=fopen("prices_by_similarity.txt","w");
                		if(f == NULL){
                    			printf("Error!");
                    			exit(1);
                		}else{
                    			int x;
                    			for (x=0; x<testList_size.row; x++){
                    				fprintf(f,"ID: %i: Fiyat: %d\n",test_list[x]->id,model_by_similarity(temp,houseList_size.row,test_list[x]));
                			}
                			printf("prices_by_similarity.txt dosyasi olusturuldu!\n");
                			fclose(f);
                		}
                		delay(2);
				printf("Ana menuye donuluyor..\n");
				delay(1);
				break;
				
			case 8: //MAKE_PREDICTION FONKSIYONU ILE HOUSE_PRICE_TEST.CSV DOSYASINDAKI EVLERIN FIYATLARINI TAHMIN ETME
				printf("Fiyat tahmini icin veriler isleniyor...\n");
				delay(1);
				int Dpitch=2, Ppitch=1;
				int Drow = houseList_size.row;
				int Prow = houseList_size.row;
				float *data = (float *)calloc(Drow*Dpitch,sizeof(int));
				float *prices = (float *)calloc(Prow*Ppitch,sizeof(int));
				create_data_matrice(house_list, data, Dpitch, Drow, prices, Ppitch, Prow);				
				float *coefficients = calculate_parameter(data, Dpitch, Drow, prices, Ppitch, Prow);
				int Cpitch=1, Crow=2;
				if(coefficients==0){
					printf("Hesaplamalar esnasinda bir yanlislik olustu!\n");
					delay(2);
					printf("Ana menuye donuluyor..\n");
					delay(1);
					break;
				}
				make_prediction(test_list, testList_size.row, coefficients, Cpitch, Crow);
				delay(2);
				free(data);
				free(prices);
				free(coefficients);
				printf("Ana menuye donuluyor..\n");
				delay(1);
				break;
				
			default:
				printf("Yanlis tuslama yaptiniz! Tekrar deneyin.\n");
				break;
				
		}
	}while(control == NOT_STOP);
	printf("Programi kullandiginiz icin tesekkurler!\n");
	return 0;
}

