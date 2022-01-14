import cv2 as cv
import numpy as np
import os


from openvino.inference_engine import IENetwork, IECore
import argparse


parser = argparse.ArgumentParser(description='Run face-detection')
parser.add_argument('-m1', dest='model_1', default='person-vehicle-bike-detection-crossroad-1016', help='Path to the model1')
parser.add_argument('-m2', dest='model_2', default='person-reidentification-retail-0277', help='Path to the model2')
args = parser.parse_args()

ie = IECore()

# Setup networks

net_1 = ie.read_network(args.model_1 + '.xml', args.model_1 + '.bin')
net_2 = ie.read_network(args.model_2 + '.xml', args.model_2 + '.bin')

# Load network to device

exet_net_1 = ie.load_network(network = net_1, device_name = 'CPU')
exet_net_2 = ie.load_network(network = net_2, device_name = 'CPU')

input_name = next(iter(net_1.input_info))
net_1_width, net_1_height = 512, 512

rei_input_name = next(iter(net_2.input_info))
rei_widht, rei_height = 128, 256

cap = cv.VideoCapture(0)
ret, frame = cap.read()
cap_widht, cap_height = frame.shape[0], frame.shape[1]
cv.imshow('result', frame)
base = []
acc = 0.7
rei_acc = 200
rei_conf = 190
size_base = 30
n = 0
while(cap.isOpened()):
    #print(1)
    ret, frame = cap.read()
    if cv.waitKey(1) & 0xFF == ord('q') or ret == False:
        break
    resized = cv.resize(frame, (net_1_width, net_1_height), interpolation=cv.INTER_CUBIC)
    inp = resized.transpose(2,0,1)
    outs = exet_net_1.infer({input_name:inp})
    out = next(iter(outs.values()))
    for outt in out[0][0]:
        if outt[1] ==2.0:
            if outt[2] >= acc:
                #print(2)
                print(outt[2])
                x_min = int(outt[3]*cap_height)
                y_min = int(outt[4]*cap_widht)
                x_max = int(outt[5]*cap_height)
                y_max = int(outt[6]*cap_widht)
                cv.rectangle(frame, (x_min, y_min), (x_max, y_max), color=(0, 0, 255))
                reindefication = frame[y_min:y_max, x_min:x_max]
                rei_resized = cv.resize(reindefication, (rei_widht, rei_height), interpolation=cv.INTER_CUBIC)
                rei_inp = rei_resized.transpose(2,0,1)
                rei_outs = exet_net_2.infer({rei_input_name:rei_inp})
                rei_out = next(iter(rei_outs.values()))
                ind = -1
                k = 0
                min_inc = -1
                #print('n =', n)
                if n > 0:
                    for person in base:
                        #print(3)
                        i_min_inc = -1
                        for rei in person:
                            #print(4)
                            inc = 0
                            j = 0
                            while j < 256:
                                inc += abs(rei[j] - rei_out[0][j])
                                j += 1
                            if inc < i_min_inc or i_min_inc == -1:
                                i_min_inc = inc
                            #print(i_min_inc)
                        if (i_min_inc < min_inc or ind == -1) and i_min_inc < rei_acc:
                            min_inc = i_min_inc
                            ind = k
                        k += 1
                #print(min_inc, ind)
                if n == 0:
                    #print(5)
                    base.append(rei_out)
                    cv.putText(frame, str(n), (x_min, y_max), cv.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 3)
                    n += 1
                else:
                    if ind == -1:
                        #print(6)
                        base.append(rei_out)
                        cv.putText(frame, str(n), (x_min, y_max), cv.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 3)
                        #пишем индекс = n
                        n += 1
                    else:
                        #print(7)
                        cv.putText(frame, str(ind), (x_min, y_max), cv.FONT_HERSHEY_SIMPLEX, 4, (0, 0, 255), 3)
                        if len(rei_out[0]) < size_base:
                            if min_inc > rei_con:
                                base[ind].append(rei_out[0])
                            #пишем индекс = ind
            else:
                break
    cv.imshow('result', frame)

cap.release()
cv.destroyAllWindows()
