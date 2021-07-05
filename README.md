# Multi-class ML Classifier Training and Real-time Inference on Arduino MCUs
We provide *ML-MCU* (Machine Learning - Microcontroller Unit), a framework with novel *Opt-SGD* and *Opt-OVO* algorithms to enable binary and multi-class ML classifier training directly on Arduino MCU boards. *ML-MCU* can be used to enable billions of MCU-based IoT edge devices to self learn/train (offline) after their deployment, using live data from a wide range of IoT use-cases.

**Optimised-Stochastic Gradient Descent (Opt-SGD):** This is the binary classifier training algorithm of *ML-MCU* that enables incremental binary classifier training on MCUs. *Opt-SGD* combines benefits from both Gradient Descent (GD) and Stochastic Gradient Descent (SGD). Thus, it inherits the stability of GD while retaining the work-efficiency of SGD. 

**Optimized One-Versus-One (Opt-OVO):** At the time of designing *Opt-OVO* (Nov 2020), it is the first novel algorithm to enable multi-class classifiers training on MCUs. *Opt-OVO* archives reduced computation by identifying and removing base classifiers that lack significant contributions to the overall multi-class classification result.

**Exciting finding** The *Opt-OVO* algorithm trained a multi-class classifier using a dataset of size 1250 and class count 50 on a $3 ESP32 board. It also performed unit inference on ESP32 for the same 50 class data sample in super real-time of 6.2 ms.


## Datasets, MCU boards for Training and Inference on MCUs

### Datasets

We converted all the listed datasets into MCU executable *.h* files and placed them inside the algorithm folders. The datasets 1, 2, 3 and 4 are used for training binary classifiers on MCUs using *Opt-SGD*. The datasets 5 and 6 are used for training multi-class classifiers on MCUs using *Opt-OVO*. The users have to uncomment their dataset of choice (header file at the beginning of the main algorithm program) to use it when training and inference on MCUs.

1. [Iris Flowers](https://archive.ics.uci.edu/ml/datasets/iris "Google's Homepage") (4 features, 3 classes, 150 samples): Using *Opt-SGD*, we train a binary classifier to distinguish Iris Setosa from other flowers based on the input features.
2. [Heart Disease](https://archive.ics.uci.edu/ml/datasets/heart+Disease) (13 features, 2 classes, 303 samples): After training, based on the input features, the *Opt-SGD* trained classifier should be able to identify the presence of heart disease in the patient.
3. [Breast Cancer](https://www.kaggle.com/uciml/breast-cancer-wisconsin-data) (30 features, 2 classes, 569 samples): Here we train a binary classifier that can find the class names (malignant or benign) based on the input features.
4. [MNIST Handwritten Digits](http://yann.lecun.com/exdb/mnist/) (64 features, 10 classes, 1797 samples): Here, we extracted data fields for digit 6, with positive and negative samples. Then using *Opt-SGD*, we trained a binary classifier on MCUs, that distinguishes digit 6 from other digits, based on the input features. 

5. MNIST Handwritten Digits: This time in a multi-class setting that includes all 10 classes (digits 0 to 9), the onboard *Opt-OVO* trained multi-class classifier should distinguish digits, based on the input features.
6. [Australian Sign Language signs](https://archive.ics.uci.edu/ml/datasets/Australian+Sign+Language+signs+(High+Quality)) (22 features, 95 classes, 6650 samples): Here, the class count is 50 since we extracted the files that contains data of 50 Auslan signs varying from *alive* to *more*. Then using *Opt-OVO*, we trained a classifier on MCUs, that distinguish Auslan signs based on the input features.

### MCU Boards

Using Arduino IDE, as required, we upload the *Opt-OVO* and *Opt-SGD* algorithm along with the selected/uncommented dataset on the following popular boards. After successful upload, we trained various ML classifier models (both binary and multi-class) on MCUs, performed the onboard model evaluation and inference performance evaluation of the thus trained MCU models.

1. MCU1 [nRF52840 Adafruit Feather](https://www.adafruit.com/product/4062): ARM Cortex-M4 @64MHz, 1MB Flash, 256KB SRAM.
2. MCU2 [STM32f103c8 Blue Pill](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html): ARM Cortex-M0 @72MHz, 128KB Flash, 20KB SRAM.
3. MCU3 [Generic ESP32](https://www.espressif.com/en/products/devkits): Xtensa LX6 @240MHz, 4MB Flash, 520KB SRAM.
4. MCU4 [ATSAMD21G18 Adafruit METRO](https://www.adafruit.com/product/3505): ARM Cortex-M0+ @48 MHz, 256kB Flash, 32KB SRAM. 

## ML-MCU Performance Evaluation

### Opt-SGD

We uploaded the *Opt-SGD* algorithm's C++ implementation on all boards. We then power on each board, connect them to a PC via the serial port to feed the training data, receive training time and classification accuracy from MCUs. The first 70% of data was used for training, the remaining 30% data for evaluation. When we instruct the board to train, *Opt-SGD* iteratively loads the data chunks and trains. Next, we load the test set, infer using the trained models to evaluate the MCU trained classifiers. 

![alt text](https://github.com/bharathsudharsan/ML-MCU/blob/master/opt-sgd_results.PNG)

The following analysis is made from the above table:

1. Even for the high 64-dimensional Digits dataset, Opt-SGD achieves real-time unit inference, within a ms, even on the slowest MCU 4. 
2. Although the training time on MCUs is higher than CPUs, Opt-SGD trained models produce classification accuracies close to those of Python scikit-learn trained models.
3. The run-time variables generated during training need to be stored within the limited SRAM of MCUs. The boards we chose have only 20 kB to a max of 520 kB of SRAM. In scenarios with large volume and high dimensional data, the upper bound is imposed by MCUs memory constraints restricting model training. Since *Opt-SGD* was designed to be capable of incrementally training a model, even on small MCUs, with only KBs of memory, it can incrementally load n-samples of high-dimensional data (range from a few MB to hundreds of MBs), then perform the required model training. Hence, even on the lowest-spec MCU4, we were able to load both the Breast Cancer, Digits datasets incrementally and train in 13.8 ms and 14.1 ms respectively.

### Opt-OVO

We follow the same procedure explained earlier, starting from uploading the algorithm until loading the test set and evaluating the trained classifiers. Here, the difference is, we use the *Opt-OVO* algorithm, then use the multi-class data instead of binary. We illustrate the obtained training results in the below graph, which we use to analyze how the training time and accuracy vary w.r.t to the train set size. 

![alt text](https://github.com/bharathsudharsan/ML-MCU/blob/master/multiclass_training_results.png)


![alt text](https://github.com/bharathsudharsan/ML-MCU/blob/master/multiclass_inference_results.png)

1. Real-time unit inference in 11.8 ms even on the slowest MCU4.

2. The fastest MCU3 was able to infer for a 50 class input in 6.2 ms.

Overall, the Opt-OVO trained classifiers perform onboard unit inference for multi-class data in super real-time, within a second, across various MCUs. Complete evaluation and results analysis available in ML-MCU the paper.

**Tip:** When first time using the *Opt-OVO* and *Opt-SGD* code for training ML models on MCUs, we recommend using better resource boards like ESP32 and Adafruit Feather nrf52, then move on to using the tiny ones like Arduino Nano, Uno, etc.
