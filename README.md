# Freshman Year Project - House Price Prediction

  This project was written in C by @abalioglu, @skoleli and @bestesengul.


## Folders

### `data` folder 

  It contains houses' data in 2 separate `*.csv` files: `house_price_data.csv` and `house_price_test.csv`. A house is defined with 9 different attributes:
  
1. ID: It is the unique identification number that each house has. IDs of the houses in `house_price_data.csv` are between 101 and 1460; ones in the `house_price_test.csv` 
are between 1 and 100.
2. Lot Area 
3. Sale Price
4. Street
5. Overall Qual: It is the column that keeps the general quality of each house out of 10.
6. Overall Cond: It keeps the score out of 9 for the general condition of each house.
7. Kitchen Qual: It is the column that holds the kitchen quality of the houses. Rating; Ex
(Excellent), Gd (Good), TA (Typical/Average) and Fa(Fair) values.
8. Year Built
9. Neighborhood

***Note***: *In `house_price_data.csv`, the `Sale Price` attribute exists for making sale price prediction of the houses in the `house_price_test.csv` file.*


### `src` folder
  
  It contains `dataset.c` and `dataset.h` files which are mainly used for the applications made on the dataset. `models.c` and `models.h` keep two different methods for the house sale price prediction.
  
  First method is "Finding the nearest house based on properties". Firts, the price distributions of houses according to districts, years and quality can be obteined from datasets with the help of the functions in the `dataset.c`. Then, the sale price prediction of the houses in the `house_price_test.csv` file can be made by searching for the most similar houses to them in `house_price_data.csv` file. After finding the similar houses in `house_price_data.csv` file, the sale price can be predicted.
  
  Second method is "Linear Correlation" between lot area and sale price. The ***Y = f(x) + ϵ*** formula is used but the ***ϵ*** (the noise in the data) is not taken into account.
  
  ![image](https://user-images.githubusercontent.com/83185578/124277557-d5bf9780-db4d-11eb-833c-a59aca9aa397.png =1086x470)
  
