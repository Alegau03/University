"""
Multilayer perceptron in pytorch
"""

from torch.utils.data import DataLoader, Dataset
from torchvision import datasets
from torchvision.transforms import ToTensor
from torch import nn
import torchmetrics


#import the data

training_data=datasets.MNIST(
    root="data",
    train=True,
    download=True,
    transform=ToTensor()
)
test_data=datasets.MNIST(
    root="data",
    train=False,
    download=True,
    transform=ToTensor()
)
#create the model
device = ("mps" if torch.cuda.is_available() else "cpu")
