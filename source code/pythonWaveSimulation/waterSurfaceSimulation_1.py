import numpy as np
import time
import cv2

if __name__ == "__main__":
    simSize = (128, 128)
    speed = 0.2
    attenuation = 0.999
    saveImages = True

    values = np.zeros(shape=(simSize[0], simSize[1]))
    diff = np.zeros(shape=(simSize[0], simSize[1]))
    acc = np.zeros(shape=(simSize[0], simSize[1]))

    diff[92:94,92:94] = 1
    diff[64,58] = 1
    diff[10,40] = 1

    
    diff = diff + acc
    values = values + diff

    prevTime = time.time()
    stepCnt = 0
    while True:
        ## update datas
        # valuesPad = np.pad(values, ((1,1),(1,1)), 'constant', constant_values=((0,0),(0,0)))
        valuesPad = np.pad(values, ((1,1),(1,1)), 'edge')
        for row in range(simSize[0]):
            for col in range(simSize[1]):
                acc[row, col] = 0
                acc[row, col] += (valuesPad[row+2, col+1] - valuesPad[row+1, col+1]) - (valuesPad[row+1, col+1] - valuesPad[row+0, col+1])
                acc[row, col] += (valuesPad[row+1, col+2] - valuesPad[row+1, col+1]) - (valuesPad[row+1, col+1] - valuesPad[row+1, col+0])
                acc[row, col] += -values[row, col]*0.01
        acc = acc * speed * 0.9 # second order
        diff = diff * attenuation + acc # first order
        values = values + diff*0.9

        ## show result
        frame = np.sqrt(values[:,:]**2 + values[:,:]**2)
        maxVal, minVal = frame.max(), frame.min()
        frame = np.clip( (frame+1) * 128 , 0, 255).astype(np.uint8)
        if(saveImages):
            cv2.imwrite(f"images/wave_{stepCnt:03d}.png", frame)
        frame = cv2.resize(frame, (512, 512), interpolation=cv2.INTER_NEAREST)
        cv2.imshow("t", frame)
        cv2.waitKey(1)

        ##
        stepCnt = stepCnt + 1
        
        ## count frame rate
        currTime = time.time()
        deltaTime = currTime - prevTime
        prevTime = currTime
        print(f"\r FPS:{1/deltaTime:7.2f}  || max:{maxVal:15.10f} || min:{maxVal:15.10f}", end="")