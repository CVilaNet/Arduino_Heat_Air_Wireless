#String json_data = "{\"Sesor_id\":\"3E24R\",\"Value\":" + (String)randNumber + "}";

import serial, time, json
from threading import Thread

import matplotlib.pyplot as plt
import matplotlib.animation as animacion
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import numpy as np

tx_matrix=np.zeros((9,9), int)
hw_sensor = serial.Serial(port='COM6', baudrate=9600, timeout=1, write_timeout=1)
fig, ax = plt.subplots()

def plot_head_map():
    # Datos
    data = tx_matrix

    # Mapa de calor
    #fig, ax = plt.subplots()
    im = ax.imshow(data, cmap = "jet", vmin = 20, vmax = 30)

    # Agregar la leyenda
    cbar = ax.figure.colorbar(im, ax = ax)
    cbar.ax.set_ylabel("Heat Map", rotation = -90, va = "bottom")

    #plt.show()


def tx_data_head(raw_string_j):
    package_num=raw_string_j['package_num']
    val_head=raw_string_j['val_head']
    tx_matrix[package_num]=val_head
    print(tx_matrix)
    print()
    print(tx_matrix.tolist())
    print('\n\n')

    #if (package_num==7):
    #    plot_head_map()


def tx_data_proc(raw_string_j):
    if(raw_string_j["Sensor_id"]=='AMG8833'):
        tx_data_head(raw_string_j)


if __name__ == '__main__':
    #anim = animacion.FuncAnimation(fig=fig, func=plot_head_map, interval=100, frames=100)
    #plt.show()

    while True:
        hw_sensor.write('getValue'.encode('utf-8'))
        time.sleep(0.1)
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

