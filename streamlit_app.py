import streamlit as st
from streamlit_image_select import image_select
import pandas as pd
from PIL import Image
import subprocess

"""
# Outfit generator demo
Select an image you want to generate a new outfit from.
"""


def reroll():
    nrawdf = pd.read_csv('processed_data/clean_product_data_with_filenames.csv')
    nrawdf = nrawdf[['cod_modelo_color', 'des_filename']]
    nprdf = nrawdf.sample(n=10)
    st.session_state['rawdf'] = nrawdf
    st.session_state['selection'] = nprdf


if 'selection' not in st.session_state:
    reroll()

rawdf = st.session_state['rawdf']
prdf = st.session_state['selection']

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

if st.button('Get different products'):
    reroll()
    st.rerun()
if st.button('Generate fresh outfit'):
    fileq = open("request.txt", 'w')
    fileq.write(prdf.iloc[chosen_idx]['cod_modelo_color'])
    fileq.close()
    #subprocess.run(["gen"])
    filea = open("answer.txt", 'r')
    lines = filea.readlines()
    filea.close()
    imgs = []
    for line in lines:
        line = line.rstrip()
        my_row = rawdf.loc[rawdf["cod_modelo_color"] == line]
        my_img = Image.open(my_row.iloc[0]['des_filename'])
        imgs.append(my_img)
    st.image(imgs, width=225)

