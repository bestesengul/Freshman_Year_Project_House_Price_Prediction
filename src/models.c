#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include"models.h"
#include"dataset.h"
#define AREA 1500
#define YEAR 10
/*
 * neighbors: ayni komsulukta olanlar
 * nearest: neighbors'dan belirlenen lotarea araligina girenler
 * recent: nearest'tan belirlenen yil araligina girenler
 * most_similar: nearest'tan
*/
int model_by_similarity(House**houses,int size_houses, House* new_house){
    int i,sum=0,new_saleprice=0;
    House** neighbors=get_neighborhoods(new_house,houses,size_houses);
    int size_neighbors=count_neighborhood(new_house,houses,size_houses); //neighbors tablomuzun size'i
    char* criter1="lotarea";
    quicksort(neighbors,0,size_neighbors-1,criter1); //lotarea'ya gore siralandi

    int l, size_nearest=0;
    for(l=0;l<size_neighbors;l++){ //belirlenen lotarea araligindaki ev sayisi
        if(new_house->lotarea-AREA < neighbors[l]->lotarea
          && neighbors[l]->lotarea < new_house->lotarea+AREA)
            size_nearest++;
    }//printf("nearest %i\n",size_nearest);

    if(size_nearest!=0){ //belirlenen lotarea araliginda ev varsa devam
        int j=0,k=0;
        House** nearest=create_table(size_nearest); //belirlenen lotarea araligindaki ev sayisi kadar buyuk yeni tablo
        while(k<size_neighbors&&j<size_nearest){
            if(new_house->lotarea-AREA < neighbors[k]->lotarea
              && neighbors[k]->lotarea < new_house->lotarea+AREA){
                nearest[j]=neighbors[k];
                k++;j++;
            }else
                k++;
        }
        char* criter2="yearbuilt";
        quicksort(nearest,0,size_nearest-1,criter2); //lotareaya gore elemeden gecen evleri imar yilina gore sirala

        int a,size_recent=0;
        for(a=0;a<size_nearest;a++){ //belirlenen yil araliginda kac ev var say
            if(new_house->yearbuilt-YEAR < nearest[a]->yearbuilt
              && nearest[a]->yearbuilt < new_house->yearbuilt+YEAR)
                size_recent++;
        }//printf("most recent %i\n",size_recent);

        if(size_recent!=0){  //belirlenen yil araliginda ev varsa devam
            int b=0,c=0;
            House** recent=create_table(size_recent); //belirlenen lotarea ve yil araligina dusenler icin yeni tablo
            while(b<size_nearest&&c<size_recent){
                if(new_house->yearbuilt-YEAR < nearest[b]->yearbuilt
                    && nearest[b]->yearbuilt < new_house->yearbuilt+YEAR){
                    recent[c]=nearest[b];
                    b++;c++;
                }else
                    b++;
            }
            if(size_recent>5){ //belirlenen yil araligina girenler besten fazla ise devam
                int d,size_most_similar=0;
                int moyen=0, new_moyen=0;
                for(d=0;d<size_recent;d++){ //overallqual, overallcond ve kitchenqual icin belirlenen kosullara uyanlarin sayisi
                    moyen=(recent[d]->overallcond+recent[d]->overallqual)/2;
                    new_moyen=(new_house->overallcond+new_house->overallqual)/2;
                    if(moyen==new_moyen && strcmp(new_house->kitchenqual,recent[d]->kitchenqual)==0)
                        size_most_similar++;
                }//printf("most similar %i\n",size_most_similar);

                if(size_most_similar!=0){ //overallqual, overallcond ve kitchenqual icin belirlenen kosullara uyanlar varsa devam
                    int e=0,f=0;
                    House**most_similar=create_table(size_most_similar); //overallqual, overallcond ve kitchenqual icin belirlenen
                    // kosullara uyanlar icin yeni tablo (sona kalanlar)
                    while(e<size_recent&&f<size_most_similar){
                        moyen=(recent[e]->overallcond+recent[e]->overallqual)/2;
                        if(moyen==new_moyen && strcmp(new_house->kitchenqual,recent[e]->kitchenqual)==0){
                            most_similar[f]=recent[e];
                            e++;f++;
                        }else
                            e++;
                    }
                    for(i=0;i<size_most_similar;i++){
                        sum+=most_similar[i]->saleprice;
                    }new_saleprice=sum/size_most_similar; //sona kalanlarin fiyat ortalamasi
                    free(most_similar);
                }else{
                    for(i=0;i<size_recent;i++){
                        sum+=recent[i]->saleprice; //size_most_similar=0 ise
                    }new_saleprice=sum/size_recent;
                }
            }else{
                for(i=0;i<size_recent;i++){
                    sum+=recent[i]->saleprice; //size_recent=<5 ise
                }new_saleprice=sum/size_recent;
            }free(recent);
        }else{
            for(i=0;i<size_nearest;i++){
                sum+=nearest[i]->saleprice; //size_recent=0 ise
            }new_saleprice=sum/size_nearest;
        }free(nearest);

    }else{
        for(i=0;i<size_neighbors;i++){
            sum+=neighbors[i]->saleprice; //size_nearest=0 ise
        }new_saleprice=sum/size_neighbors;
    }free(neighbors);

return new_saleprice;
}


/*
 * Dogrusal Iliski - Yontem 2
 *
 * Parametre olarak verilen evlerin alan ve fiyat
 * bilgilerini iki adet tek boyutlu dizi olarak geri dondurur.
 * 		Y 	 = 		X 		  * W
 * Ev fiyati = Ev Alan Olcusu x W
 *
 * Matrislerin boyutlari:
 * 				pitch (bir satirda bulunacak eleman sayisi = sutun)
 * Y: N x 1 -> 		1
 * X: N x 2 -> 		2 (carpim kolayligi icin ilk sutun degerlerinin hepsi 1)
 * W: 2 x 2 -> 		2
 *
 */

void create_data_matrice(House** houses, float* X, int Xpitch, int Xrow, 
                                         float* Y, int Ypitch, int Yrow) {
	int i = 0, j = 0;
	for (i = 0; i < Xrow; i++){
		for (j = 0; j < Xpitch; j++){
			if(j == 0)
				X[i*Xpitch + j] = 1;
			else
				X[i*Xpitch + j] = houses[i]->lotarea;
		}
	}
	for (i = 0; i < Yrow; i++){
		for (j = 0; j < Ypitch; j++)
			Y[i*Ypitch + j] = houses[i]->saleprice;
	}	
}

/*
 * Parametre olarak verilen matrice'i ekrana basar.
 * --- Parametreler ---
 * matricePitch: sutun sayisi
 * row: satir sayisi
 * matrice: Ekrana bastirilmak istenen matris
 */

void print_data_matrice(float* matrice, int matricePitch, int row){
	int i = 0, j = 0;
	printf("\n*** Matris baslangic ***\n\n");
	for (i = 0; i < row; ++i) {
		printf("%4d )  ", i+1);
    	for (j = 0; j < matricePitch; ++j)
      		printf("%6.2f ", matrice[i*matricePitch + j]);
    printf("\n");
    }
	printf("\n***** Matris bitis *****\n");
}



/* 
 * Parametre olarak verilen matrisin (A matrisinin) devrigi
 * yeni bir matrisde (S matrisinde) saklar.
 * Verilen her matris icin pitch degeri vardir (Apitch).
 * Fonksiyon sonunda transpoze sonucu oluşan S matrisinin
 * sutun ve satir degerleri guncellenir 
 * (Guncellenebilmeleri icin Srow ve Spitch adresi isteniyor).
 * --- Parametreler ---
 * int *A ve *S 		-> matrisler
 * int Apitch ve *Spitch -> matrislerin sutun degerleri
 * int Arow ve *Srow 	-> matrislerin satir degerleri
 */

void get_transpose(float* A, int Apitch,  int Arow,
           		   float* S, int* Spitch, int* Srow) {
	int i = 0, j = 0;
  	for (i = 0; i < Arow; i++){
  		for (j= 0; j < Apitch; ++j)
  			S[j*Arow+i] = A[i*Apitch+j];
  	}
  	*Spitch = Arow;
  	*Srow = Apitch;
}


/* Parametre olarak verilen iki matris
 * carpimini geri dondurur.
 * Klasik Matris carpimi algoritmasi: S = A * B
*/

void get_multiplication_normal(float* A,int Apitch,  int Arow,
             				   float* B,int Bpitch,  int Brow,
             				   float* S,int* Spitch, int* Srow) {

	if(Brow != Apitch){
		printf("\nNormal carpma islemi icin gecerli olmayan matrisler kullanilmak isteniyor:\n\t>>> Ilk matrisin sutun sayisi ikinci matrisin satir sayisina esit degildir!\n");
		return;
	}
  	float toplam = 0;
	int i = 0, j = 0, k = 0;
  	*Spitch = Bpitch;
  	*Srow = Arow;
  	for (i = 0; i < Arow; i++){
    	for (j = 0; j < Bpitch; j++){
      		for (k = 0; k < Apitch; k++)
        	// (A -> i. satir elemani)*(B -> k. sutun elemani)
        	toplam += A[i*Apitch + k] * B[k*Bpitch + j];
      	S[i*(*Spitch) + j] = toplam;
      	toplam = 0;
    	}
  	}
}

/*
 * -> Parametre olarak verilen A(m,n) matrisinin i satiri 
 * ve j kolonu olmayan bir minorunu A_{i,j} olarak adlandirdigimiz durumda
 * det(A) = + A_{i,1}*det(A_{i,1}) - A_{i,2}*det(A_{i,2}) + .. - .. + [(-1)^n] * A_{m,n} * det(A_{m,n})
 * formulunu kullanarak A matrisinin determinantini hesaplar ve dondurur.
 */

float getDeterminant(float* A, int Apitch, int Arow){

	// temel durum icin donus degeri ayarlandi
	if(Apitch*Arow==1){return A[0];} 
	float sign=1, det=0;
	
	// matrisin minorlerinin hesabi icin b matrisi ayarlandi
	int bN = Apitch-1;
	float b[bN*bN];
	
	int i,j,mult;
	int row,col;
	// minoru alinan matrisin katsayisi tutuldu.
	for(mult=0; mult<Arow; mult++){ 
		i=0; j=0;
		// row ve col degerleriyle A matrisi uzerinde geziniyoruz
		for(row=0; row<Arow; row++){  
			for(col=0; col<Apitch; col++){
				b[row*bN+col]=0;
				// A matrisinde [row] satirinda ve [col] sutununda olmayanlar
				// b matrisinde yerlerine atandi. 
				// (b matrisi A matrisinin minorlerinin kaydedilebilmesi icin olusturulmustu)
				if(row!=0 && col!=mult){
					b[i*bN+j] = A[row*Apitch+col];
					
					// b matrisinde satir dolmus mu dolmamis mi diye kontrol eder
					// dolmamissa yalnizca sutun degerini(j) arttirir.  
					if(j<Apitch-2) {
						j++;
					}
					// b matrisinde satir dolduysa sutun degerini(j) sifirlar
					// ve satir degerini(i) bir arttirir.
					else{
						j=0; i++;
					}
				}
			}
		}
		// determinant degerini rekursif olarak cagirdigi fonksiyonlarla hesapladi
		det = det + sign*(A[mult]*getDeterminant(b, bN, bN));
		sign = -1 * sign;
	}
	return det;
}

/*
 * -> Parametre olarak verilen matrisin 
 * tersi alinamiyorsa 0, alinabiliyorsa 1 gonderir.
 */

int is_invertible(float* A, int Apitch, int Arow){
	// tersinin alinabilmesi icin kare matris olmali
	// o yuzden satir ve sutun degerlerinin esitligini kontrol eder
	if(Apitch!=Arow){ return 0; }
	
	// tersinin alinabilmesi icin determinant 0'dan farkli olmali
	// bu sebeple A matrisinin determinant degerini kontrol eder
	if (getDeterminant(A, Apitch, Arow)==0){ return 0; }
	
	// butun testlerden gectiyse fonksiyon basari degeri gonderir.
	return 1;
}


/*
 * -> A matrisinin kofaktor(es carpan) matrisini hesaplayip 
 * C matrisini bununla doldurur.
 */

void getCofactor(float* A, int Apitch, int Arow,
                 float* C, int *Cpitch, int *Crow){  
	*Cpitch=Apitch; *Crow=Arow;
	//// A matrisinin minorlerinin hesabi icin b matrisi ayarlandi
	int bN = Arow-1;
	float b[bN*bN];
	
	int i,j,m,n;
	int row,col;
	// C matrisi uzerinde gezinmek icin i ve j degerleri ayarlandi
	for(i=0; i<Arow; i++){
		for(j=0; j<Apitch; j++){
			m=0; n=0; 
			// A matrisi uzerinde gezinmek icin row ve col degerleri ayarlandi
			for(row=0; row<Arow; row++){
				for(col=0; col<Apitch; col++){
					b[row*bN+col] = 0;
					// C matrisinde [i] satirinda ve [j] sutununda olmayanlar
					// b matrisinde yerlerine atandi. 
					// (b matrisi A matrisinin minorlerinin kaydedilebilmesi icin olusturulmustu)
					if(row!=i && col!=j){ 
						b[m*bN+n] = A[row*Apitch+col];
						// b matrisinde satir dolmus mu dolmamis mi diye kontrol eder
						// dolmamissa yalnizca sutun degerini(n) arttirir. 
						if(n<Arow-2) {
							n++;
						}
						// b matrisinde satir dolduysa sutun degerini(n) sifirlar
						// ve satir degerini(m) bir arttirir.
						else {
							n=0; m++;
						} 
					}
				}
			}
			// A matrisinin A(row,col) elemaninin kofaktorunu hesaplayip C matrisindeki yerine yerlestirir.
			if((i+j)%2==0){
				*(C + i*(Apitch)+j) = getDeterminant(b, bN, bN);
			}
			else{
				*(C + i*(Apitch)+j) = -getDeterminant(b, bN, bN);
			}
		}
	}
}


/*
 * -> Parametre olarak verilen A matrisinin ek matrisini(adjoint) 
 * Adjoint(A) = Transpose(Cofactor(A))
 * islemiyle hesaplayip S matrisine gonderir.
 */

void adjoint(float* A, int Apitch, int Arow,
             float* S, int Spitch, int Srow){
	
	// A'nin kofaktor matrisi icin yer ayrildi
	int Cpitch=Apitch, Crow=Arow;
	float C[Cpitch*Crow];
	// A'nin kofaktor matrisi hesaplanip C matrisine gonderildi
	getCofactor(A, Apitch, Arow, C, &Cpitch, &Crow);
	// elde edilen kofaktor matrisin transpozu alinip 
	// ek matris(adjoint) elde edildi
	get_transpose(C, Cpitch, Crow, S, &Spitch, &Srow);
}

/* 
 * "inverse(A) = adjoint(A)/determinant(A)" 
 * formulunu kullanarak A matrisinin tersini
 * hesaplayip I matrisine gonderir.
 * A matrisi ters dondurulemez ise 0,
 * dondurulebilir ise 1 dondurur.
 */

int get_inverse(float *A, int Apitch, int Arow,
                float *I, int *Ipitch, int *Irow){
	// A matrisinin ters cevrilebilir olup olmadigi kontrol edildi
	if(is_invertible(A, Apitch, Arow) == 0){
		// ters cevrilemez ise 0 dondurdu
		return 0; 
	}
	*Ipitch = Apitch;
	*Irow = Arow;
	// A'nin ek matrisini bulmak icin hesaplama yapildi
	int Spitch=Apitch,Srow=Arow;
	float S[Spitch*Srow];
	adjoint(A, Apitch, Arow, S, Spitch, Srow);
	// determinanti bir degerde tuttu
	float det = getDeterminant(A, Apitch, Arow);
	// I matrisine A matrisinin tersi sirayla yerlestirdi
	int row,col;
	for(row=0; row<Arow; row++){
		for(col=0; col<Apitch; col++){
			I[row*Spitch+col] = S[row*Spitch+col]/det;
		}
	}
	// butun islemler bittikten sonra 1 dondurdu
	return 1;
}


// [(X^T * X)^-1] * X^T * Y = W formulune gore 
// W matrisinin hesabini yapar
// W matrisinin icerigini gosteren bir pointer dondurur.

float* calculate_parameter(float* X, int Xpitch, int Xrow,
                         float* Y, int Ypitch, int Yrow){
                         
        // X^T hesaplanir:
	int XTpitch = Xrow, XTrow=Xpitch;
	float XT[XTpitch*XTrow];
	get_transpose(X, Xpitch,  Xrow, XT, &XTpitch, &XTrow);
	
	// X^T * X hesaplanip M1 matrisinde tutulur:
	int M1pitch = Xpitch, M1row = XTrow;
	float M1[M1pitch*M1row];
	get_multiplication_normal(XT, XTpitch, XTrow, X, Xpitch, Xrow, M1, &M1pitch, &M1row);
	
	// M1 matrisinin tersi alinip I matrisinde tutulur:
	// M1 matrisi tersi alinamaz ozellikteyse 0 dondurur.
	int Ipitch = M1pitch, Irow = M1row;
	float I[Ipitch*Irow];
	int is_succesful = get_inverse(M1, M1pitch, M1row, I, &Ipitch, &Irow);
	if(is_succesful == 0){ 
		printf("Tersi alinamaz!\n"); 
		return 0; 
	}
	
	// X^T * Y hesaplanir ve M2 matrisinde tutulur:
	int M2pitch = Ypitch, M2row = XTrow; 
	float M2[M2pitch*M2row];
	get_multiplication_normal(XT, XTpitch, XTrow, Y, Ypitch, Yrow, M2, &M2pitch, &M2row);
	
	// son olarak I * M2 matrisi hesaplanir, islem sonucu W matrisine atanir:
	int Wpitch = M2pitch, Wrow = Irow;
	float *W = NULL;
	W = (float*)calloc(sizeof(float), Wpitch*Wrow);
	if (W==NULL){return 0;}
	get_multiplication_normal(I, Ipitch, Irow, M2, M2pitch, M2row, W, &Wpitch, &Wrow);
	// islemler sonucu elde edilen W matrisi geri dondurulur:
	return W;
}

/*
 * -> Parametre olarak verilen ve icinde tuttugu evlerin 
 * fiyati bilinmeyen test_list tablosunu alir.
 * -> Yine parametre olarak verilen coefficients matrisi ile
 * test_list tablosundaki evlerin tahmini fiyatini
 * "prices_by_linear_correlation.txt" isimli bir dosyaya yazdirir.
 */
void make_prediction(House** test_list, int listSize, float* coefficients, int Cpitch, int Crow){
	// alan ve fiyat bilgisini tutan X ve Y matrisleri icin yer ayrildi
	int Xpitch = 2, Ypitch = 1;
	int Xrow = listSize, Yrow = listSize;
	float *X = (float*)calloc(Xpitch*Xrow, sizeof(int));
	float *Y = (float*)calloc(Ypitch*Yrow, sizeof(int));
	// test_list'teki bilgilerle X ve Y matrisi dolduruldu.
	create_data_matrice(test_list, X, Xpitch, Xrow, Y, Ypitch, Yrow);
	// alan bilgilerini tutan X matrisi ve coefficients matrisi carpilarak
	// tahmini fiyat bilgilerini tutan Y matrisi olusturulur.
	get_multiplication_normal(X, Xpitch, Xrow, coefficients, Cpitch,  Crow, Y, &Ypitch, &Yrow);
	
	// "prices_by_linear_correlation.txt" dosyasi acilarak ID ve 
	// ID'ye ait fiyat tahminleri yazilir.
	FILE* f=fopen("prices_by_linear_correlation.txt","w");
	if(f == NULL){
		printf("Error!");
		exit(1);
	}else{
		int i;
		for (i=0; i<listSize; i++){
			fprintf(f,"ID: %5i\t Fiyat: %15.2f\n", test_list[i]->id, Y[i*Ypitch]);
		}
		printf("prices_by_linear_correlation.txt dosyasi olusturuldu!\n");
		fclose(f);
	}
	// X ve Y matrislerine ayrilan yerler iade edilir.
	free(X);
	free(Y);
}

