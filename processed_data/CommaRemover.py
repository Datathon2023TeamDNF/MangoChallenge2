import pandas as pd
filenames = ['clean_product_data', 'clean_product_data_with_filenames']
for file in filenames:
    df = pd.read_csv(file+'.csv')
    df.to_csv(file + '_newsep.txt', sep=';')
