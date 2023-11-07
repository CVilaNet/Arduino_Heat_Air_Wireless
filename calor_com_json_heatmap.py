#String json_data = "{"Sensor_id":"AMG8833","Value":[4,23.25,22.00,22.25,23.00,24.25,23.75,23.50,25.25]}";

import threading
import numpy as np
import serial, time, json
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation


data_tx=np.zeros((8,8))
image = np.random.rand(10,10,10)

# setup figure
fig = plt.figure(figsize=(6,4))
ax1=fig.add_subplot(1,1,1)

#set up list of images for animation
ims=[]

hw_sensor = serial.Serial(port='COM4', baudrate=9600, timeout=10, write_timeout=10)

def tx_data_head(raw_string_j):
    val_head=raw_string_j['Value']
    
    global data_tx

    hp_section_array=np.array(val_head)

    x = int(hp_section_array[0])
    for y in range (len(hp_section_array)-1): 
        data_tx[x][y]=hp_section_array[y+1]
        
    print('Mariz del Mapa de Calor')
    print(data_tx)
    print()
    #print(data_tx.tolist())
    print('\n\n')


def tx_data_proc(raw_string_j):
    if(raw_string_j["Sensor_id"]=='AMG8833'):
        tx_data_head(raw_string_j)


def read_arduino_tx():
    contador=0
    while True:
        hw_sensor.write('getValue'.encode('utf-8'))
        time.sleep(0.1)
        contador+=1
        try:
            raw_string_b = hw_sensor.readline()
            raw_string_s = raw_string_b.decode('utf-8')
            if(raw_string_s.index("}")>=0 and raw_string_s.index("{")==0):
                raw_string_s = raw_string_s[0:raw_string_s.index("}")+1]
                raw_string_j = json.loads(raw_string_s)

                print(raw_string_j)
                #print(raw_string_j["Sensor_id"])
                #print(raw_string_j["Value"])

                tx_data_proc(raw_string_j)
            else:
                print("error/ no } found.")
        except Exception as e:
            print("Exception occurred, somthing wrong...")
            print(f'EXCEPTION: {str(e)} \n')
    hw_sensor.close()


def animation_frame(x):
    # Mapa de calor
    #im = ax1.imshow(data_tx, cmap = "jet", interpolation='bilinear')
    im = ax1.imshow(data_tx, cmap = "jet", interpolation='bilinear', vmin = 20, vmax = 29)
    #im = ax1.imshow(data_tx, cmap = "jet", vmin = 20, vmax = 29)

    ax1.set_title("Mapa de calor")

    ims.append([im])


# Iniciando un Hilo para la lectura de datos de Arduino (SERIAL PORT)
thread = threading.Thread(target=read_arduino_tx)
thread.start()

# Cargando la visualización gráfica del mapa de calor
animation = FuncAnimation(fig, func=animation_frame, frames=1, interval=1000)
plt.show()
