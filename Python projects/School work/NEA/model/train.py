from datasets.data_manipulation import dataset
from model import create_model

import tensorflow as tf
tf.get_logger().setLevel('ERROR')
import argparse


def parser():
    # making the parser
    my_parser = argparse.ArgumentParser(
        prog="Train", usage="%(prog)s [options] ",
        description="Training a machine learning model for alfa-numeric characters")
    
    # adding arguments 
    my_parser.add_argument(
        '-T', '--train', action='store_true', help='train the model')
    my_parser.add_argument(
        '-L', '--load_weights',action='store_true', default=True ,help='load weights from checkpoint')
    my_parser.add_argument(
        '-Ev', '--evaluate', action='store_true', help='show evaluation')
    my_parser.add_argument(
        '-e','--epochs', type=int, help='number of epochs the model is trained for')


    # Executing the parsed args
    args = my_parser.parse_args()
    return args


def main(args):

    #stores the weights for a trained model
    CHECKPOINT_PATH = "model_weights/cp.ckpt"
    cp_callback = tf.keras.callbacks.ModelCheckpoint(filepath=CHECKPOINT_PATH,
                                                     save_weights_only=True,
                                                     verbose=1)

    # images formatted from the dataset function in the datasets folder
    train_images, test_images, train_labels, test_labels = dataset()
    class_names = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
                   'C','D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M','N',
                   'O', 'P','Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
    
    #model is created form the model file 
    model = create_model(len(class_names))

    
    if args.train:
        if args.load_weights:
            model.load_weights(CHECKPOINT_PATH)

        model.fit(train_images,train_labels,
                batch_size=1000,
                epochs=args.epochs,
                validation_data=(test_images,test_labels),
                callbacks=cp_callback)
                
    if args.evaluate:
        if args.load_weights:
            model.load_weights(CHECKPOINT_PATH)

        test_loss, test_acc = model.evaluate(test_images, test_labels)
        print(f"Test loss : {test_loss}", f"Test Accuracy : {test_acc}")
    



if __name__ == '__main__':
    main(parser())
