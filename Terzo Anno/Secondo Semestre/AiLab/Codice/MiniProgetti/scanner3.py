import gradio as gr
import numpy as np
import cv2

src_points=[]
def fixImg(img):
    dst_points= np.float32([
        [0,0],
        [0,600],
        [800,600],
        [800,0]
            ])
    scr_float=np.float32(src_points)
    H = cv2.getPerspectiveTransform(scr_float,dst_points)
    new_img=cv2.warpPerspective(img,H,(600,800))
    return new_img
def onSelect(value,evt: gr.EventData):
    if len(src_points)<4:
        src_points.append(evt._data['index'])
    return len(src_points)
    
def cancella(img):
    src_points.clear()
    inp.clear()
    out.clear()
    return 
with gr.Blocks() as demo:
    gr.Markdown("# Document Scanner")
    gr.Markdown("Click on the four corners of the document to scan it in counter clockways")
    coord_n= gr.Textbox(label="Coordinates")
    with gr.Row():
        inp= gr.Image(label="Input")
        #evento per i click sugli angoli
        inp.select(fn = onSelect,inputs=inp,outputs=coord_n)
        
        out= gr.Image(label="Output")
    with gr.Row():
        btn= gr.Button("Fix!",variant='primary',size='lg')
        btn.click(fn=fixImg, inputs=inp, outputs=out)      
        
    with gr.Row():
        btnCancella= gr.Button("Cancel",variant='danger',size='lg')
        btnCancella.click(fn=cancella, inputs=inp, outputs=out)
demo.launch()