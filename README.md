# ML-MCU: Machine Learning - Microcontroller
**Overview:** We provide ML-MCU, a framework with our novel Opt-SGD and Opt-OVO algorithms, to enable both binary and multi-class ML classifier training directly on MCUs. Thus, ML-MCU enables billions of MCU-based IoT edge devices to self learn/train (offline) after their deployment, using live data from a wide range of IoT use-cases.

**Repo Structure:**

**Optimised-Stochastic Gradient Descent (Opt-SGD):** This is the binary classifier training algorithm of our framework that enables incremental binary classifier training on MCUs. Opt-SGD combines benefits from both Gradient Descent (GD) and Stochastic Gradient Descent (SGD) thus, inheriting the stability of GD while retaining the work-efficiency of SGD. 

**Optimized One-Versus-One (Opt-OVO):** To the best of our knowledge, this is the first novel algorithm to enable multi-class classifiers training on MCUs. Our Opt-OVO archives reduced computation by identifying and removing base classifiers that lack significant contributions to the overall multi-class classification result.

**Datasets for onboard training:** We converted all the listed datasets into MCU executable files and placed them inside the algorithm folders. The users have to uncomment their dataset of choice (header file at the beginning of the main algorithm program) to use it when training on MCUs.

1. [Iris Flowers](https://archive.ics.uci.edu/ml/datasets/iris "Google's Homepage"):
2. [Heart Disease](https://archive.ics.uci.edu/ml/datasets/heart+Disease):
3. [Breast Cancer](https://www.kaggle.com/uciml/breast-cancer-wisconsin-data):
4. [MNIST Handwritten Digits](http://yann.lecun.com/exdb/mnist/):
5. [MNIST Handwritten Digits](http://yann.lecun.com/exdb/mnist/) in multi-class setting:
6. [Australian Sign Language signs](https://archive.ics.uci.edu/ml/datasets/Australian+Sign+Language+signs+(High+Quality)):

**Tip:** Before using the implementation of our algorithms for onboard training, we recommend users to use better resource boards like ESP32 and Adafruit Feather nrf52, then move on to using the tiny ones like Arduino Nano, Uno, etc.
