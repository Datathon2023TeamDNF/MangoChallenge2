import streamlit as st
from streamlit_image_select import image_select
import pandas as pd
from PIL import Image

"""
# Outfit generator demo
Select an image you want to generate a new outfit from.
"""

if 'selection' not in st.session_state:
    nprdf = pd.read_csv('processed_data/clean_product_data_with_filenames.csv')
    nprdf = nprdf[['cod_modelo_color', 'des_filename']]
    nprdf = nprdf.sample(n=10)
    st.session_state['selection'] = nprdf

prdf = st.session_state['selection']

#prdf

sqrimgs = []

for imgdir in prdf['des_filename']:
    img = Image.open(imgdir)
    x1, y1, x2, y2 = -(334-239)//2, 0, 334-(334-239)//2, 334  # cropping coordinates
    newimg = Image.new('RGB', (x2 - x1, y2 - y1), (14, 17, 23))
    newimg.paste(img, (-x1, -y1))
    sqrimgs.append(newimg)

chosen_idx = image_select(
    label="",
    images=sqrimgs,
    return_value="index",
    use_container_width=False
)

if st.button('Generate fresh outfit'):
    chosen_idx
