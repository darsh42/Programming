
import cv2 

"""LNN = tf.keras.Sequential([
    tf.keras.layers.Flatten(input_shape=(28, 28)),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Dense(26)
])

CNN = model = tf.keras.Sequential([
        # Convolutional layers
        tf.keras.layers.Conv2D(32, (5, 5), input_shape=(28, 28, 1), padding='same', activation='relu'),
        tf.keras.layers.Conv2D(32, (5, 5), input_shape=(28, 28, 1), padding='same', activation='relu'),
        tf.keras.layers.MaxPooling2D((2, 2)),
        tf.keras.layers.Conv2D(64, (3, 3), input_shape=(28, 28, 1), padding='same', activation='relu'),
        tf.keras.layers.Conv2D(64, (3, 3), input_shape=(28, 28, 1), padding='same', activation='relu'),
        tf.keras.layers.MaxPooling2D((2, 2)),
        # flattening the array
        tf.keras.layers.Flatten(),
        # applying the FC layers
        tf.keras.layers.Dense(64, activation='relu'),
        tf.keras.layers.Dense(26, activation='softmax')
    ])

CNN.summary()
LNN.summary()"""


img=cv2.imread(r'images\readin\sd19 (1).jpg')
imgCont=img.copy()
imgFinal=img.copy()


imggray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
th, imgBin = cv2.threshold(imggray, 80, 255, cv2.THRESH_OTSU)
imgcanny=cv2.Canny(imgBin,200,240)

cv2.imshow('canny',imgcanny)
cv2.waitKey(0)
cv2.destroyAllWindows()