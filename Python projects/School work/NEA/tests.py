import unittest
import numpy as np
from model.datasets.data_manipulation import dataset
from model.train import parser
from model.model import create_model


class datasetTest(unittest.TestCase):

    #tests the dataset function
    def test_dataset_split(self):
        train_images, test_images, train_labels, test_labels = dataset()
        self.assertEqual(train_images.shape, (331838, 28, 28, 1), 'Should be (331838, 28, 28, 1)')
        self.assertEqual(train_labels.shape, (331838,), 'Should be (331838,)')
        self.assertEqual(test_images.shape, (110613, 28, 28, 1), 'Should be (110613, 28, 28, 1)')
        self.assertEqual(test_labels.shape, (110613,), 'Should be (110613,)')

class ParserTest(unittest.TestCase):
    #Parser tests
    def setup(self):
        self.parser = parser()
    
    def test_parser(self):        
        MODEL_TRAIN, WEIGHT_LOAD, EVALUATE = self.parser.parse_args(['-l', '-e', '-t'])
        self.assertTrue(MODEL_TRAIN)
        self.assertTrue(EVALUATE)
        self.assertTrue(WEIGHT_LOAD)

class testLetterRecog(unittest.TestCase):
    class_names = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
    
    model = create_model(len(class_names))
    model.load_weights(r'model\datasets\model_weights\cp.ckpt')
    predictions = model.predict()

if __name__ == '__main__':
    unittest.main()