# Multi-class ML Classifier Training and Real-time Inference on Arduino MCUs
We provide *ML-MCU* (Machine Learning - Microcontroller Unit), a framework with novel *Opt-SGD* and *Opt-OVO* algorithms to enable binary and multi-class ML classifier training directly on Arduino MCU boards. *ML-MCU* can be used to enable billions of MCU-based IoT edge devices to self learn/train (offline) after their deployment, using live data from a wide range of IoT use-cases.

**Optimised-Stochastic Gradient Descent (Opt-SGD):** This is the binary classifier training algorithm of *ML-MCU* that enables incremental binary classifier training on MCUs. *Opt-SGD* combines benefits from both Gradient Descent (GD) and Stochastic Gradient Descent (SGD). Thus, it inherits the stability of GD while retaining the work-efficiency of SGD. 

**Optimized One-Versus-One (Opt-OVO):** At the time of designing *Opt-OVO* (Nov 2020), it is the first novel algorithm to enable multi-class classifiers training on MCUs. *Opt-OVO* archives reduced computation by identifying and removing base classifiers that lack significant contributions to the overall multi-class classification result.

## Datasets, MCU boards for Training and Inference on MCUs

### Datasets

We converted all the listed datasets into MCU executable *.h* files and placed them inside the algorithm folders. The datasets 1, 2, 3 and 4 are used for training binary classifiers on MCUs using *Opt-SGD*. The datasets 5 and 6 are used for training multi-class classifiers on MCUs using *Opt-OVO*. The users have to uncomment their dataset of choice (header file at the beginning of the main algorithm program) to use it when training on MCUs.

1. [Iris Flowers](https://archive.ics.uci.edu/ml/datasets/iris "Google's Homepage"): Using *Opt-SGD*, we train a binary classifier to distinguish Iris Setosa from other flowers based on the input features.
2. [Heart Disease](https://archive.ics.uci.edu/ml/datasets/heart+Disease): After training, based on the input features, the *Opt-SGD* trained classifier should be able to identify the presence of heart disease in the patient.
3. [Breast Cancer](https://www.kaggle.com/uciml/breast-cancer-wisconsin-data): Here we train a binary classifier that can find the class names (malignant or benign) based on the input features.
4. [MNIST Handwritten Digits](http://yann.lecun.com/exdb/mnist/): Here, we extracted data fields for digit 6, with positive and negative samples. Then using *Opt-SGD*, we trained a binary classifier on MCUs, that distinguishes digit 6 from other digits, based on the input features. 

5. [MNIST Handwritten Digits](http://yann.lecun.com/exdb/mnist/) in a multi-class setting: Contains 10 classes (digits 0 to 9). The onboard *Opt-OVO* trained multi-class classifier should distinguish digits, based on the input features.
6. [Australian Sign Language signs](https://archive.ics.uci.edu/ml/datasets/Australian+Sign+Language+signs+(High+Quality)): Here, the class count is 50 since it contains data of 50 Auslan signs varying from *alive* to *more*. Then using *Opt-OVO*, we trained a classifier on MCUs, that distinguish Auslan signs based on the input features.

**Tip:** When first time using the *Opt-OVO* and *Opt-SGD* code for training ML models on MCUs, we recommend using better resource boards like ESP32 and Adafruit Feather nrf52, then move on to using the tiny ones like Arduino Nano, Uno, etc.
