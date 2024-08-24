import tensorflow as tf
import os
import numpy as np
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
from keras import layers


def create_model(num_classes):
    model = tf.keras.Sequential([
        # Convolutional layers
        layers.Conv2D(64, (5, 5), input_shape=(28, 28, 1), padding='same', activation='relu'),
        layers.Conv2D(64, (5, 5), input_shape=(28, 28, 1), padding='same', activation='relu'),
        layers.MaxPooling2D((2, 2)),
        layers.Conv2D(128, (3, 3), input_shape=(28, 28, 1), padding='same', activation='relu'),
        layers.Conv2D(128, (3, 3), input_shape=(28, 28, 1), padding='same', activation='relu'),
        layers.MaxPooling2D((2, 2)),
        # flattening the array
        layers.Flatten(),
        # applying the FC layers
        layers.Dense(128, activation='relu'),
        layers.Dense(num_classes, activation='softmax')
    ])

    model.compile(optimizer='adam',
                  loss=tf.keras.losses.SparseCategoricalCrossentropy(),
                  metrics=['accuracy'])
    return model

def applyModel(imagePath):
    WEIGHT_CHECPOINT = r"D:\My folder\Programming\Python projects\School work\NEA\model\datasets\model_weights\cp.ckpt"
    class_names = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
                   'C','D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M','N',
                   'O', 'P','Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
    model = create_model(len(class_names))
    model.load_weights(WEIGHT_CHECPOINT)

    
    predictions = model.predict(imagePath)
    print(predictions)
    return class_names[np.argmax(predictions)]



