// To eliminate the problems with min/max for ESP32
#ifdef ESP32
#define min(a, b) (a) < (b) ? (a) : (b)
#define max(a, b) (a) > (b) ? (a) : (b)
#define abs(x) ((x) > 0 ? (x) : -(x))
#endif
#include <string.h>

// Uncomment the dataset of choice to use it when training on MCUs.
//#include"Aus_sign_lang.h"
#include"Multiclass_Digits.h"

// Function to evaluate the multi-class classifiers.
        class Evaluation_function {
        public:
            Evaluation_function() :
                    t_p(0), t_n(0), f_p(0), f_n(0) {
            }

            void truevsfalse(int actual, int predicted) {
                if (predicted > 0 && predicted == actual)
                    t_p++;
                else if (predicted > 0 && predicted != actual)
                    f_p++;
                else if (predicted <= 0 && predicted == actual)
                    t_n++;
                else
                    f_n++;   }

            float accuracy() {
                return (1.0f * t_p + t_n) / support();   }

            unsigned int support() {
                return t_p + t_n + f_p + f_n;    }

        protected:
            unsigned int t_p;  unsigned int t_n;  unsigned int f_p;  unsigned int f_n;    };

        template<unsigned int size>
        float dot(float x[size], float w[size]) {
            float sum = 0;
            for (unsigned int i = 0; i < size; i++)
                sum += x[i] * w[i];
            return sum;
        } 

        template<unsigned int num_features>
        class Base_classifier {
        public:
            virtual void fitModel(float x[num_features], int y) = 0;
            virtual int predict(float x[num_features]) = 0;
            virtual void set(const char *param, float value) {
            }

        protected:
            template<typename T>
            void setParam(const char *param_name, const char* target_name, T *param, T value) {
                if (strcmp(param_name, target_name) == 0)
                    *param = value;
            } }; 
            
        template<unsigned int num_features>
        class SGD : public Base_classifier<num_features> {
        public:
            SGD(float alpha = 0.001) :
                    model_alpha(alpha) {
                parameters.normalizeAlpha = false;
                parameters.momentum = 0;

                for (unsigned int i = 0; i < num_features + 1; i++)
                    model_weights[i] = weights_updates[i] = 0;   }

            void set(const char *param, float value) {
                this->setParam(param, "alpha", &model_alpha, value);
                this->setParam(param, "momentum", &parameters.momentum, value);
                this->setParam(param, "normalize", &parameters.normalizeAlpha, value > 0);  }

            void normalizeLearningRate() {
                parameters.normalizeAlpha = true;   }

            void momentum(float momentum) {
                parameters.momentum = momentum;    }

            void fitModel(float x[num_features], int y) {
                int predicted = predict(x);
                int error = predicted - y;
                float alpha = model_alpha;
                float updates[num_features + 1] = {0};

                if (error == 0)
                    return;

                if (parameters.normalizeAlpha)
                    alpha /= 1 + model_alpha * dot<num_features>(x, x);

                weights_updates[0] = (parameters.momentum * weights_updates[0]) - (alpha * error);
                model_weights[0] += weights_updates[0];

                for (unsigned int i = 0; i < num_features; i++) {
                    float momentum = parameters.momentum * weights_updates[i + 1];
                    float correction = alpha * x[i] * error;

                    weights_updates[i + 1] = momentum - correction;
                    model_weights[i + 1] += weights_updates[i + 1];   }   }

            int predict(float x[num_features]) {
                float xx[num_features + 1] = {1};

                for (unsigned int i = 0; i < num_features; i++)
                    xx[i + 1] = x[i];
                return (1.0f / (1 + exp(-dot<num_features+1>(x, model_weights)))) > 0.5 ? 1 : 0;   }

        protected:
            float model_alpha;
            float model_weights[num_features + 1];
            float weights_updates[num_features + 1];
            struct {
                bool normalizeAlpha;
                float momentum;
            } parameters;  }; 

        template<class Classifier, int numFeatures, int numClasses>
        class OneVsOne : Base_classifier<numFeatures> {
        public:

            OneVsOne() {
                for (int i = 0; i < numClassifiers(); i++)
                    classifiers_var[i] = new Classifier();  }

            
            void fitModel(float x[numFeatures], int y) {
                int k = 0;

                for (int i = 0; i < numClasses - 1; i++) {
                    for (int j = i + 1; j < numClasses; j++) {
                        if (i == y || j == y) {
                            classifiers_var[k]->fitModel(x, y == i ? 1 : 0);
                        }
                        k++;  }  }  }

            int predict(float x[numFeatures]) {
                int k = 0;
                int votes[numClasses] = {0};

                for (int i = 0; i < numClasses - 1; i++) {
                    for (int j = i + 1; j < numClasses; j++) {
                        int y_pred = classifiers_var[k]->predict(x);
                        votes[y_pred > 0 ? i : j] += 1;
                        k++;     } }

                int maxVotes = -1;
                int classIdx = -1;

                for (int i = 0; i < numClasses; i++) {
                    if (votes[i] > maxVotes) {
                        maxVotes = votes[i];
                        classIdx = i;   } }
                return classIdx;  }

            void set(const char *param, float value) {
                for (int i = 0; i < numClassifiers(); i++)
                    classifiers_var[i]->set(param, value);  }

        protected:
            Classifier *classifiers_var[numClasses * (numClasses - 1) / 2];
            int numClassifiers() {
                return numClasses * (numClasses - 1) / 2; } }; 

int temp, temp1 = 0;
void setup() 
{
    Serial.begin(115200);
    delay(5000);
}

void loop() 
{
    int trainSamples;
    int retrain_cycles;
    Evaluation_function eval;
    OneVsOne<SGD<FEATURES_DIM>, FEATURES_DIM, NUM_CLASSES> call_fn;

    call_fn.set("alpha", 1); // Set the configuration of the Opt-SGD bases binary classifiers. alpha and momentum refer to Opt-SGD
    call_fn.set("momentum", 0.7);
    call_fn.set("C", 0.1);
    Serial.println();   
    trainSamples = readSerialNumber("Enter a train set size ", TRAIN_SAMPLES - 2);
    retrain_cycles = readSerialNumber("Enter the times to cycly over the train set", 100);

    if (trainSamples == 0 || retrain_cycles == 0)
        return;
    
    Serial.print("Starting to train using the entered train set size");
    time_t start = millis();
    // Repeating the training a few times over the same dataset increases performance. Do not re-train too much the accuracy might drop
    for (uint16_t cycle = 0; cycle < retrain_cycles; cycle++)
        for (uint16_t i = 0; i < trainSamples; i++)
        call_fn.fitModel(X_train[i % TRAIN_SAMPLES], y_train[i % TRAIN_SAMPLES]);
    
    Serial.println();   
    Serial.print("It took ");
    temp1 = millis() - start;
    Serial.print(temp1);
    Serial.print("ms to train ");
    Serial.println();
    
    // Predict using onboard trained classifier
    start = millis();
    for (int i = 0; i < TEST_SAMPLES; i++) 
    {
        int y_true = y_test[i];
        int y_pred = call_fn.predict(X_test[i]);

        Serial.print("Predicted ");
        Serial.print(y_pred);
        Serial.print(" vs ");
        Serial.println(y_true);
        eval.truevsfalse(y_true, y_pred);
    }
    Serial.print("It took ");
    temp = millis() - start;
    Serial.print(temp);
    Serial.print("ms for infering using the full test set");
    Serial.println();
    Serial.print("Accuracy = ");
    Serial.print(eval.accuracy() * 100);
    Serial.print(" out of ");
    Serial.print(eval.support());
    Serial.println(" samples");
    delay(1000);
}

int readSerialNumber(String str_read, int max_allowed_size) {
    Serial.print(str_read);
    Serial.print(" (");
    Serial.print(max_allowed_size);
    Serial.print(" max) ");
    while (!Serial.available()) delay(1);
    int n = Serial.readStringUntil('\n').toInt();
    Serial.println(n);
    return max(0, min(n, max_allowed_size));
}
