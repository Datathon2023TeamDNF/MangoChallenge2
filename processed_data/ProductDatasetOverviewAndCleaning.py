# -*- coding: utf-8 -*-
"""DatabaseCleaningSaving.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1Z4Ti8Y_D_uhrEmf1VLQYbD-Bv2qofi0v

First, we install pandas
"""

!pip install pandas

"""Then we import it."""

import pandas as pd

"""We read all the data from the csv to a DataFrame in order to manipulate it properly."""

url = 'product_data.csv'
rawdata = pd.read_csv(url, index_col=0)

"""We visualize it's shape and head to see how is it."""

print(rawdata.shape)
print(rawdata.head)

"""In particular we want to see which colums we have."""

print(rawdata.columns)

"""From the dataset description:

1. cod_modelo_color - A unique identifier for a particular fashion product based on its model and color.
2. cod_color_code - A code indicating the specific color of the product.
3. des_color_specification_esp - Description of the color in Spanish.
4. des_agrup_color_eng - Grouped color description in English.
5. des_sex - Gender for which the product is intended (e.g., "Unisex", "Female", "Male").
6. des_age - Age group for which the product is intended (e.g., "Adult", "Child").
7. des_line - The line or collection to which the product belongs.
8. des_fabric - The fabric or material of the product.
9. des_product_category - Broad category of the product (e.g., "Bottoms", "Tops").
10. des_product_aggregated_family - An aggregated family description of the product (e.g., "Trousers & leggings").
11. des_product_family - Family description of the product (e.g., "Trousers", "Shirts").
12. des_product_type - Type of the product within its family (e.g., "Trousers", "Shirt").
13. des_filename - File path of the product image associated with the cod_modelo_color. Images can be found in the images folder. Image is always the frontal image of the product without a human model.

All the data is categorical, so we should be careful and preprocess it.
We will go column by column and try to reduce the number of unique values by removing outliers

First we'll process `cod_counts`.
"""

cod_counts = rawdata['cod_color_code'].value_counts()
print(cod_counts)

"""We see that there are a lot of unique values, but most of them only appear once, let's remove the rows with those rare values."""

MIN_COD_FREQ = 30
ndata1 = rawdata[rawdata['cod_color_code'].isin(cod_counts[cod_counts.gt(MIN_COD_FREQ)].index)]

print(ndata1['cod_color_code'].value_counts().shape)
print(rawdata.shape)
print(ndata1.shape)

"""We have removed about 600 rows, but now there are less colors."""

agrup_counts = ndata1['des_agrup_color_eng'].value_counts()
print(agrup_counts)

"""We can see `des_agrup_color_eng` does not need any work.

Now we are going to drop all rows except ones that have products for Female Adults from the line SHE, since that's what happens the majority of the times and since that allows us to remove the colums that contain that data.
"""

ndata2 = ndata1[(ndata1['des_sex'] == 'Female') & (ndata1['des_age'] == 'Adult') & (ndata1['des_line'] == 'SHE')]
print(ndata2.shape)
print(ndata2.head)

ndata3 = ndata2.drop(['des_sex', 'des_age', 'des_line'], axis=1)
print(ndata3.shape)
print(ndata3.head)

"""Now we can drop some of the uncommon `des_product_family` values, some of which may be more difficult to create combinations with, like fragances and swimwear. We will remove those as well af some other outliers."""

fam_counts = ndata3['des_product_family'].value_counts()
print(fam_counts)

MIN_FAM_FREQ = 45
ndata4 = ndata3[ndata3['des_product_family'].isin(fam_counts[fam_counts.gt(MIN_FAM_FREQ)].index)]

print(ndata4['des_product_family'].value_counts().shape)
print(ndata3.shape)
print(ndata4.shape)

"""Now we will drop some other columns that are either too specific or contain almost identical data to another column."""

todrop = ['des_color_specification_esp','des_product_aggregated_family','des_product_type']
ndata5 = ndata4.drop(todrop, axis=1)

defdata = ndata5

"""We can overview the remaining data. We have dropped less than 10% of the dataset and have 5 remaining columns."""

print(defdata.shape)
print(defdata.head)

for column in defdata:
  print(column, defdata[column].value_counts().shape)

"""As we can see above the number of unique values in each column and the number of columns has been reduced. This is very useful for AI models that can use One Hot Encoding more easily or for any other application."""

defdata.to_csv('clean_product_data_with_filenames.csv')

"""In the end we save our obtained data."""

defdatanofile = defdata.drop('des_filename', axis=1)
defdatanofile.to_csv('clean_product_data.csv')

"""We may also want a version without files for other applications."""