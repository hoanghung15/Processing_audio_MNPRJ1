#pragma once
void readCurrentFileWav(const char* filename);
void readFileToMath(const char* filename);
void setGrid();

//PROCESSING
void timeTransfer(int n);
void timeReverse();
void plusTwoSignal();
void subtractionSignal();
void multiplicationSignal();
void downSampling(int k);
void upSampling(int k);

//EFFECT
void fadeInEffect(double duration);
void fadeOutEffect(double duration);
void echoEffect(double delay, double decase);
void reverbEffect(double decay, double mix, double delayInMs, double wet, double reverberance);
void flangingEffect(double A, double r0, double f);
void VolumeUp_Down(double k);
void chorusEffect(double depth, double rate, double delaytime);

//FILTER
void LPFilter(int N, double fc);
void HPFilter(int N, double fc);
void BPFilter(int N, double fc1, double fc2);
void BSFilter(int N, double fc1, double fc2);
