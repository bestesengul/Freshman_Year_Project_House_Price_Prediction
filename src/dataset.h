#ifndef _DATASET_H
#define _DATASET_H

typedef struct house {
 int id;
 int lotarea ;
 char street[15] ;
 int saleprice ;
 char neighborhood[15] ;
 int yearbuilt ;
 int overallqual ;
 int overallcond ;
 char kitchenqual[5] ;
 }House;

//count_house() fonksiyonu icin yazildi
typedef struct count{
  int row; //dosyadaki ev sayisini tutar 
  int column; //okunan dosyadki kolon sayisini tutar (sadece read_house_data() icin house_pice_test.csv doyasini okuyabilsin diye yazildi)
 }Count;

int hash_compute(int id, int birinciid);
Count count_house(char dosya_adi[30]);
int count_neighborhood(House* house, House** houses, int size);
void print_house(House* house);
House* get_house_byid(House** houses, int id, int sizeofhouses);
void swap(House* a, House* b);
void sort_houses(House**houses, int first,int last, char* criter_name); //sort yapar ve sirali listeyi dosya acip basar
void quicksort(House**houses, int first,int last, char* criter_name); //sadece sort yapar
void delay(double dly);
void print_houseList(House** houses, int size);
void mean_sale_prices(House ** houses, int size, char* criter_name);

void delete_table(House** table, int size);
void read_house_data(char* dosya_adi, House** houses, Count size);
House** get_neighborhoods(House* house,House** houses,int size);
House** create_table(int size);

#endif //_DATASET_H

