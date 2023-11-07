#String json_data = "{\"Sesor_id\":\"3E24R\",\"Value\":" + (String)randNumber + "}";

import threading
import numpy as np
import serial, time, json, random
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque


# MAX NO. OF POINTS TO STORE
max_points=10

que2 = deque(maxlen = max_points)
que3 = deque(maxlen = max_points)
t = deque(maxlen = max_points)
dt = 0.1  # time interval

que2.append(0.)
que3.append(0.)
t.append(0.)

data_tx=np.zeros((8,8))
image = np.random.rand(10,10,10)

# setup figure
fig = plt.figure(figsize=(12,4))

ax1=fig.add_subplot(1,3,1)
ax2=fig.add_subplot(1,3,2)
ax3=fig.add_subplot(1,3,3)

#set up list of images for animation
ims=[]

hw_sensor = serial.Serial(port='COM4', baudrate=9600, timeout=10, write_timeout=10)


def decryption_heat_data(raw_data):
    raw_decrypted=[]
    for item in raw_data:
        raw_decrypted.append((item/10)+20)
    
    return raw_decrypted


def decryption_mq_data(raw_data):
    # Valor medido Alcohol
    alc_val=0
    mq_alc_list=raw_data[0:4]
    for alc_item in mq_alc_list:
        alc_val=(alc_val+alc_item)*100
    alc_val/=10000

    # Valor medido CO2
    co2_val=0
    mq_co2_list=raw_data[4:]
    for co2_item in mq_co2_list:
        co2_val=(co2_val+co2_item)*100
    co2_val/=10000

    return alc_val, co2_val


def tx_data_head(raw_string_j):
    package_num=raw_string_j['package_num']
    val_head=raw_string_j['val_head']
    
    global data_tx
    mq_alc=0
    mq_co2=0

    if (package_num<8):
        data_tx[package_num]=decryption_heat_data(val_head)
    elif (package_num==8):
        sensor_mq=val_head=raw_string_j['val_head']

        mq_alc,mq_co2=decryption_mq_data(sensor_mq)

        global que2
        que2.append(mq_alc)
        global que3
        que3.append(mq_co2)
        global t
        t.append(np.round(t[-1]+dt,2))

    print('Mariz del Mapa de Calor')
    print(data_tx)
    print()
    #print(data_tx.tolist())
    print(f'Sensor MQ-3 (Gases Inflamables): {mq_alc} -> Historial: {list(que2)}')
    print(f'Sensor MQ-135 (CO2): {mq_co2} -> Historial: {list(que3)}')
    print('\n\n')


def tx_data_proc(raw_string_j):
    if(raw_string_j["Sensor_id"]=='AMG8833'):
        tx_data_head(raw_string_j)


def read_arduino_tx():
    contador=0
    print('Hola 01')
    while True:
        hw_sensor.write('getValue'.encode('utf-8'))
        #time.sleep(0.1)
        time.sleep(1)
        contador+=1
        try:
            raw_string_b = hw_sensor.readline()
            raw_string_s = raw_string_b.decode('utf-8')
            print('Hola 02')
            #print(raw_string_s)
            if(raw_string_s.index("}")>=0 and raw_string_s.index("{")==0):
                raw_string_s = raw_string_s[0:raw_string_s.index("}")+1]
                raw_string_j = json.loads(raw_string_s)

                print('Hola 03')
                
                print(raw_string_j)
                print(raw_string_j["Sensor_id"])
                print(raw_string_j["Value"])

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
    im = ax1.imshow(data_tx, cmap = "jet", interpolation='bilinear', vmin = 20, vmax = 31)
    #im = ax1.imshow(data_tx, cmap = "jet", vmin = 20, vmax = 31)

    ax2.clear()
    ax3.clear()

    im2, = ax2.plot(t, que2, color='green')
    im3, = ax3.plot(t, que3, color='crimson')

    ax1.set_title("Mapa de calor")

    ax2.set_title("Pureza de aire")
    ax2.set_xlabel("Tiempo")
    ax2.set_ylabel("Grados")

    ax3.set_title("Gases inflamables")
    ax3.set_xlabel("Tiempo")
    ax3.set_ylabel("Grados")

    ims.append([im, im2, im3])


#read_arduino_tx()

# Iniciando un Hilo para la lectura de datos de Arduino (SERIAL PORT)
thread = threading.Thread(target=read_arduino_tx)
thread.start()

# Cargando la visualización gráfica del mapa de calor
animation = FuncAnimation(fig, func=animation_frame, frames=1, interval=2000)
plt.show()
