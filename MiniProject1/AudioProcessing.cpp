#pragma once
#include<bits/stdc++.h>
#include "gnuplot-iostream.h"
#include "AudioProcessing.h"
using namespace std;

Gnuplot gp("\"C:\\Program Files\\gnuplot\\bin\\gnuplot.exe\"");

vector<pair<int, double>>pointsToMath; //default_Wav_File_From_fntest.wav
vector<pair<int, double>> currentPoints;

vector<pair<int, double>> currentPointsAfterFormat;
vector<pair<int, double>> pointsToMathAfterFormat;

vector<pair<int, double>> tempPoints;
vector<pair<int, double>> tempPointsOfVec1;
vector<pair<int, double>> tempPointsOfVec2;
vector<pair<int, double>> tempPointsEcho;
vector < double > tempPointsReverb;
vector<int16_t>tempWrite;
vector<double>h;
vector<double>s;

uint32_t sRate;
uint16_t bitsPSmp;
uint16_t numC;

double pi = 3.14159;

struct WavHeader {
	char chunkID[4];
	uint32_t chunkSize;
	char format[4];
	char subchunk1ID[4];
	uint32_t subchunk1Size;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	char subchunk2ID[4];
	uint32_t subchunk2Size;
};

bool compare(const pair<int, double>& a, const pair<int, double>& b) {
	return a.first < b.first;
}

int coordinateOrigin(const vector<pair<int, double>>& vec) {
	int pos = 0;
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i].first == 0) {
			pos = i;
		}
	}
	return pos;
}

void setGrid() {
	gp << "set xlabel 'index X' font ',14 bold'\n";
	gp << "set ylabel 'value'" << endl;
	gp << "set grid" << endl;
}

void formatVector(vector<pair<int, double>> vector1, vector<pair<int, double>> vector2) {

	tempPointsOfVec1.clear();
	tempPointsOfVec2.clear();
	tempPoints.clear();

	int posOfVct1 = coordinateOrigin(vector1); //default_wav_file
	int posOfVct2 = coordinateOrigin(vector2);
	int coutOfNumberVct1 = vector1.size() - posOfVct1 - 1;
	int coutOfNumberVct2 = vector2.size() - posOfVct2 - 1;

	//compare_after_pos(0)
	if (posOfVct1 < posOfVct2) {
		for (int i = vector2[0].first; i < vector1[0].first; i++) {
			tempPointsOfVec1.push_back(make_pair(i, 0));
		}
		for (int i = 0; i < vector1.size(); i++) {
			tempPointsOfVec1.push_back(make_pair(vector1[i].first, vector1[i].second));
		}
		vector1.clear();
		vector1 = tempPointsOfVec1;

	}
	else if (posOfVct1 > posOfVct2) {
		for (int i = vector1[0].first; i < vector2[0].first; i++) {
			tempPointsOfVec2.push_back(make_pair(i, 0));
		}
		for (int i = 0; i < vector2.size(); i++) {
			tempPointsOfVec2.push_back(make_pair(vector1[i].first, vector2[i].second));
		}
		vector2.clear();
		vector2 = tempPointsOfVec2;
	}

	//compare_befor_pos(0)
	if (coutOfNumberVct1 < coutOfNumberVct2) {
		tempPointsOfVec1.clear();
		for (int i = 0; i < vector1.size(); i++) {
			tempPointsOfVec1.push_back(make_pair(vector1[i].first, vector1[i].second));
		}

		for (int i = tempPointsOfVec1[tempPointsOfVec1.size() - 1].first + 1; i <= vector2[vector2.size() - 1].first; i++) {
			tempPointsOfVec1.push_back(make_pair(i, 0));
		}
		vector1.clear();
		vector1 = tempPointsOfVec1;
	}
	else if (coutOfNumberVct1 > coutOfNumberVct2) {
		tempPointsOfVec2.clear();

		for (int i = 0; i < vector2.size(); i++) {
			tempPointsOfVec2.push_back(make_pair(vector2[i].first, vector2[i].second));
		}

		for (int i = tempPointsOfVec2[tempPointsOfVec2.size() - 1].first + 1; i <= vector1[vector1.size() - 1].first; i++) {
			tempPointsOfVec2.push_back(make_pair(i, 0));
		}
		vector2.clear();
		vector2 = tempPointsOfVec2;
	}
	currentPointsAfterFormat.clear();
	tempPoints = vector2;
	currentPointsAfterFormat = tempPoints;
	tempPoints.clear();

	pointsToMathAfterFormat.clear();
	tempPoints = vector1;
	pointsToMathAfterFormat = tempPoints;
	tempPoints.clear();

	cout << "FORMAT DONE";
}

//READ AND WRITE FILE
void readCurrentFileWav(const char* filename) {
	tempPoints.clear();
	std::ifstream file(filename, std::ios::binary);
	WavHeader header;
	if (!file) {
		std::cerr << "Error: Couldn't open file: " << filename << std::endl;
		return;
	}

	file.read(reinterpret_cast<char*>(&header), sizeof(header));

	sRate = header.sampleRate;
	bitsPSmp = header.bitsPerSample;
	numC = header.numChannels;

	cout << "CurrentFile:";

	std::cout << "sampleRate    : " << header.sampleRate << std::endl;

	cout << sRate << " " << bitsPSmp << " " << numC << endl;

	if (header.bitsPerSample != 16) {
		std::cerr << "Error: Only 16-bit audio is supported." << std::endl;
		return;
	}

	uint32_t numSamples = header.subchunk2Size / (header.bitsPerSample / 8);

	int16_t sample;
	int i = 0;
	while (file.read(reinterpret_cast<char*>(&sample), sizeof(int16_t))) {
		//currentPoints.push_back(make_pair(i, sample));
		tempPoints.push_back(make_pair(i, sample));
		i++;
	}


	for (int i = 44; i < tempPoints.size(); i++) {
		currentPoints.push_back(make_pair(i - 44, tempPoints[i].second));
		s.push_back(tempPoints[i].second);
	}
	cout << endl;
}

void readFileToMath(const char* filename) {
	tempPoints.clear();
	std::ifstream file(filename, std::ios::binary);
	WavHeader header;
	if (!file) {
		std::cerr << "Error: Couldn't open file: " << filename << std::endl;
		return;
	}

	file.read(reinterpret_cast<char*>(&header), sizeof(header));

	sRate = header.sampleRate;
	bitsPSmp = header.bitsPerSample;
	numC = header.numChannels;

	cout << "FileToMath: ";
	std::cout << "sampleRate    : " << header.sampleRate << std::endl;

	cout << sRate << " " << bitsPSmp << " " << numC << endl;

	if (header.bitsPerSample != 16) {
		std::cerr << "Error: Only 16-bit audio is supported." << std::endl;
		return;
	}

	uint32_t numSamples = header.subchunk2Size / (header.bitsPerSample / 8);

	int16_t sample;
	int i = 0;
	while (file.read(reinterpret_cast<char*>(&sample), sizeof(int16_t))) {
		//currentPoints.push_back(make_pair(i, sample));
		tempPoints.push_back(make_pair(i, sample));
		i++;
	}
	for (int i = 44; i < tempPoints.size(); i++) {
		pointsToMath.push_back(make_pair(i - 44, tempPoints[i].second));

	}
	cout << endl;
}

void updateVector(vector<pair<int, double>>a) {
	cout << "test" << endl;
	tempWrite.clear();

	cout << coordinateOrigin(a) << endl;
	for (int i = coordinateOrigin(a); i < a.size(); i++) {
		tempWrite.push_back((int)a[i].second);
	}
}

void writeWavFile(const char* filename, const std::vector<int16_t>& samples, uint32_t sampleRate, uint16_t numChannels, uint16_t bitsPerSample) {
	// Mở file WAV để ghi
	std::ofstream file(filename, std::ios::binary | std::ios::trunc);
	if (!file) {
		std::cerr << "Không thể mở file WAV để ghi." << std::endl;
		return;
	}
	// Tạo header cho file WAV mới
	WavHeader header;
	strcpy(header.chunkID, "RIFF");
	header.chunkSize = static_cast<uint32_t>(samples.size() * sizeof(int16_t) + sizeof(WavHeader) - 8);
	strcpy(header.format, "WAVE");
	strcpy(header.subchunk1ID, "fmt ");
	header.subchunk1Size = 16;
	header.audioFormat = 1; // PCM
	header.numChannels = numChannels;
	header.sampleRate = sampleRate;
	header.bitsPerSample = bitsPerSample;
	header.byteRate = sampleRate * numChannels * bitsPerSample / 8;
	header.blockAlign = numChannels * bitsPerSample / 8;
	strcpy(header.subchunk2ID, "data");
	header.subchunk2Size = static_cast<uint32_t>(samples.size() * sizeof(int16_t));

	// Ghi header vào file
	file.write(reinterpret_cast<char*>(&header), sizeof(WavHeader));

	// Ghi dữ liệu âm thanh vào file
	file.write(reinterpret_cast<const char*>(samples.data()), samples.size() * sizeof(int16_t));

	// Đóng file
	file.close();

	std::cout << "Đã ghi ra file WAV mới: " << filename << std::endl;
}

//PERFORM TRANSFORMATIONS
void timeTransfer(int n) {
	cout << "____________________________Test TIMETRANSFER_________________" << endl;
	tempPoints.clear();
	int pos;
	int countOfNumber;

	if (n <= 0) {
		n = abs(n);
		pos = coordinateOrigin(currentPoints);
		countOfNumber = currentPoints.size() - pos - 1;

		if (n <= countOfNumber) {
			for (int i = 0; i < currentPoints.size(); i++) {
				tempPoints.push_back(make_pair(currentPoints[i].first - n, currentPoints[i].second));
			}

			currentPoints.clear();
			currentPoints = tempPoints;

			//for (int i = 0; i < tempPoints.size(); i++) {
			//	cout << " " << tempPoints[i].first << " " << tempPoints[i].second;
			//}
			cout << "doneeee" << endl;

			gp << "plot '-' with lines" << endl;
			gp.send(tempPoints);
			cin.get();
		}
		else if (n > countOfNumber) {
			cout << "out of Transfer -" << endl;
			//test_current_now : Current having values, temp no.
			//for (int i = 0; i < currentPoints.size(); i++) {
			//	cout << " " << currentPoints[i].first << " " << currentPoints[i].second;
			//}

			//push_value_in_temp;
			for (int i = 0; i < currentPoints.size(); i++) {
				tempPoints.push_back(make_pair(currentPoints[i].first - n, currentPoints[i].second));
			}
			currentPoints.clear();
			cout << "Print value of tempPoint" << endl;
			//for (int i = 0; i < tempPoints.size(); i++) {
			//	cout << " " << tempPoints[i].first << " " << tempPoints[i].second;
			//}
			//cout << endl;
			for (int i = tempPoints[tempPoints.size() - 1].first; i <= 0; i++) {
				tempPoints.push_back(make_pair(i, 0));
			}
			//test 
			//cout << "Templ after push_back" << endl;
			//for (int i = 0; i < tempPoints.size(); i++) {
			//	cout << " " << tempPoints[i].first << " " << tempPoints[i].second;
			//}
			cout << endl;
			currentPoints = tempPoints;
			//draw
			gp << "plot '-' with  lines" << endl;
			gp.send(tempPoints);
			cin.get();
		}
	}
	else if (n > 0) {
		pos = coordinateOrigin(currentPoints);
		countOfNumber = pos;
		//test


		if (n <= pos) {
			for (int i = 0; i < currentPoints.size(); i++) {
				tempPoints.push_back(make_pair(currentPoints[i].first + n, currentPoints[i].second));
			}
			currentPoints.clear();
			currentPoints = tempPoints;

			//test
			//draw
			gp << "plot '-' with  lines" << endl;
			gp.send(tempPoints);
			cin.get();
		}
		else if (n > pos) {
			//test


			for (int i = 0; i < currentPoints.size(); i++) {
				tempPoints.push_back(make_pair(currentPoints[i].first + n, currentPoints[i].second));
			}
			currentPoints.clear();
			currentPoints = tempPoints;
			tempPoints.clear();

			for (int i = 0; i < currentPoints[0].first; i++) {
				tempPoints.push_back(make_pair(i, 0));
			}
			//test



			for (int i = 0; i < currentPoints.size(); i++) {
				tempPoints.push_back(make_pair(currentPoints[i].first, currentPoints[i].second));
			}
			cout << "Test:" << tempPoints.size() << endl;

			currentPoints.clear();
			currentPoints = tempPoints;

			gp << "plot '-' with  lines" << endl;
			gp.send(tempPoints);
			cin.get();

		}

	}

	updateVector(tempPoints);
	writeWavFile("timeTransferN.wav", tempWrite, sRate, numC, bitsPSmp);
}

void timeReverse() {
	tempPoints.clear();
	for (int i = 0; i < currentPoints.size(); i++) {
		tempPoints.push_back(make_pair(-currentPoints[i].first, currentPoints[i].second));
	}
	sort(tempPoints.begin(), tempPoints.end(), compare);
	currentPoints.clear();
	currentPoints = tempPoints;

	updateVector(tempPoints);
	writeWavFile("timeReverse.wav", tempWrite, sRate, numC, bitsPSmp);

	gp << "plot '-' with lines" << endl;
	gp.send(tempPoints);
	cin.get();

}

void plusTwoSignal() {
	tempPoints.clear();
	formatVector(pointsToMath, currentPoints);
	for (int i = 0; i < pointsToMathAfterFormat.size(); i++) {
		tempPoints.push_back(make_pair(pointsToMathAfterFormat[i].first, currentPointsAfterFormat[i].second + pointsToMathAfterFormat[i].second));
	}

	updateVector(tempPoints);
	writeWavFile("plusTwoSignal.wav", tempWrite, sRate, numC, bitsPSmp);

	gp << "plot '-'  with lines" << endl;
	gp.send(tempPoints);
	cin.get();
}

void subtractionSignal() {
	tempPoints.clear();
	formatVector(pointsToMath, currentPoints);
	for (int i = 0; i < pointsToMathAfterFormat.size(); i++) {
		tempPoints.push_back(make_pair(pointsToMathAfterFormat[i].first, currentPointsAfterFormat[i].second - pointsToMathAfterFormat[i].second));
	}

	updateVector(tempPoints);
	writeWavFile("subtractionSignal.wav", tempWrite, sRate, numC, bitsPSmp);

	gp << "plot '-'  with lines" << endl;
	gp.send(tempPoints);
	cin.get();
}

void multiplicationSignal() {
	tempPoints.clear();
	formatVector(pointsToMath, currentPoints);
	for (int i = 0; i < pointsToMathAfterFormat.size(); i++) {
		tempPoints.push_back(make_pair(pointsToMathAfterFormat[i].first, currentPointsAfterFormat[i].second * pointsToMathAfterFormat[i].second));
	}

	updateVector(tempPoints);
	writeWavFile("multiplicationSignal.wav", tempWrite, sRate, numC, bitsPSmp);

	gp << "plot '-'  with lines" << endl;
	gp.send(tempPoints);
	cin.get();
}

void downSampling(int k) {
	tempPoints.clear();

	int pos = coordinateOrigin(currentPoints);
	int afterCoordinate = pos / k;
	int beforCoordinate = (currentPoints.size() - 1 - pos) / k;

	if (afterCoordinate > 0) {
		for (int i = -afterCoordinate; i < 0; i++) {
			tempPoints.push_back(make_pair(i, currentPoints[pos - (abs(i) * k)].second));
		}
	}
	else {
		cout << "Not Enough Value After Coordinate to Push" << endl;
	}
	tempPoints.push_back(make_pair(0, currentPoints[pos].second));
	cout << beforCoordinate << endl;
	if (beforCoordinate > 0) {
		for (int i = 1; i <= beforCoordinate; i++) {
			tempPoints.push_back(make_pair(i, currentPoints[pos + i * k].second));
		}
	}
	else {
		cout << "Not Enough Value befor Coordinate to Push" << endl;
	}

	currentPoints.clear();
	currentPoints = tempPoints;

	updateVector(tempPoints);
	writeWavFile("downSampling.wav", tempWrite, sRate / k, numC, bitsPSmp);

	gp << "plot '-' with lines" << endl;
	gp.send(currentPoints);
	cin.get();
}

void upSampling(int k) {
	tempPoints.clear();

	int pos = coordinateOrigin(currentPoints);
	int coutOfNumber = currentPoints.size() - pos - 1;

	if (pos > 0) {
		for (int i = 0; i < pos; i++) {
			tempPoints.push_back(make_pair(currentPoints[i].first * k, currentPoints[i].second));
			for (int j = 1; j < k; j++) {
				tempPoints.push_back(make_pair(currentPoints[i].first * k + j, 0));
			}

		}
		tempPoints.push_back(make_pair(0, currentPoints[pos].second));
		for (int i = 1; i <= coutOfNumber; i++) {
			for (int j = 1; j < k; j++) {
				tempPoints.push_back(make_pair(tempPoints[tempPoints.size() - 1].first + 1, 0));
			}
			tempPoints.push_back(make_pair(i * k, currentPoints[pos + i].second));
		}
		//test


		currentPoints.clear();
		currentPoints = tempPoints;


		gp << "plot '-' with lines" << endl;
		gp.send(currentPoints);
		cin.get();

	}
	else {
		tempPoints.push_back(make_pair(0, currentPoints[pos].second));
		for (int i = 1; i <= coutOfNumber; i++) {
			for (int j = 1; j < k; j++) {
				tempPoints.push_back(make_pair(tempPoints[tempPoints.size() - 1].first + 1, 0));
			}
			tempPoints.push_back(make_pair(i * k, currentPoints[pos + i].second));
		}
		//test


		gp << "plot '-' with lines" << endl;
		gp.send(tempPoints);
		cin.get();
	}

	updateVector(tempPoints);
	writeWavFile("upSampling.wav", tempWrite, sRate * k, numC, bitsPSmp);
}

//EFFECT FOR WAV FILE
void fadeInEffect(double duration) {
	tempPoints.clear();
	tempPoints = currentPoints;

	double fadeSamples = duration * sRate * numC;
	double fadeIncrement = 1.0 / fadeSamples;

	for (size_t i = 0; i < fadeSamples && i < currentPoints.size(); ++i) {
		double fade = fadeIncrement * i;
		tempPoints[i].second *= fade;
	}

	updateVector(tempPoints);
	writeWavFile("fadeInEffect.wav", tempWrite, sRate, numC, bitsPSmp);

}

void fadeOutEffect(double duration) {
	tempPoints.clear();
	double fadeSamples = duration * numC * 44100;
	double fadeIncrement = 1.0 / fadeSamples;

	for (size_t i = 0; i < fadeSamples && i < currentPoints.size(); ++i) {
		double fade = 1 - fadeIncrement * i;
		currentPoints[i].second *= fade;
	}
	tempPoints = currentPoints;

	updateVector(tempPoints);
	writeWavFile("fadeOutEffect.wav", tempWrite, sRate, numC, bitsPSmp);

}

void echoEffect(double delay, double decase) {
	tempPointsEcho.clear();
	tempPoints.clear();
	int position = static_cast<int>(delay * sRate) * 2;
	cout << position << endl;

	for (int i = 0; i < position; i++) {
		tempPointsEcho.push_back(make_pair(i, 0));
	}
	for (int i = position; i < currentPoints.size(); i++) {
		tempPointsEcho.push_back(make_pair(i, static_cast<int>(currentPoints[i - position].second * decase)));
	}

	if (tempPointsEcho.size() == currentPoints.size()) {
		cout << "true" << endl;
	}

	cout << tempPointsEcho[position + 20].second << " " << currentPoints[20].second << endl;
	for (int i = 0; i < currentPoints.size(); i++) {
		tempPoints.push_back(make_pair(i, static_cast<int>(currentPoints[i].second + tempPointsEcho[i].second)));
	}

	updateVector(tempPoints);
	writeWavFile("echoEffect.wav", tempWrite, sRate, numC, bitsPSmp);

}

void reverbEffect(double decay, double mix, double delayInMs, double wet, double reverberance) {
	tempPoints.clear();
	tempPoints = currentPoints;

	int delayInSamples = static_cast<int>(delayInMs * sRate / 1000.0);
	std::vector<std::pair<double, double>> reverbBuffer(delayInSamples, { 0.0, 0.0 });

	for (int i = 0; i < currentPoints.size(); ++i) {
		tempPoints[i].first = (1.0 - wet) * tempPoints[i].first + wet * (tempPoints[i].first + mix * reverbBuffer[i % delayInSamples].first);
		tempPoints[i].second = (1.0 - wet) * tempPoints[i].second + wet * (tempPoints[i].second + mix * reverbBuffer[i % delayInSamples].second);

		reverbBuffer[i % delayInSamples].first = (1.0 - reverberance) * tempPoints[i].first * decay + reverberance * reverbBuffer[i % delayInSamples].first;
		reverbBuffer[i % delayInSamples].second = (1.0 - reverberance) * tempPoints[i].second * decay + reverberance * reverbBuffer[i % delayInSamples].second;
	}

	updateVector(tempPoints);
	writeWavFile("reverbEffect.wav", tempWrite, sRate, numC, bitsPSmp);

}

void flangingEffect(double A, double r0, double f) {
	tempPoints.clear();
	tempPoints = currentPoints;

	for (int n = 0; n < tempPoints.size(); ++n) {
		double delayedSampleIndex = n - (r0 / 2) * (1 - sin(2 * pi * f * n / sRate));
		int delayedSampleIndex_floor = (int)floor(delayedSampleIndex);
		int delayedSampleIndex_ceil = (int)ceil(delayedSampleIndex);

		double delayedSample = 0.0;
		if (delayedSampleIndex_floor >= 0 && delayedSampleIndex_ceil < tempPoints.size()) {
			double fraction = delayedSampleIndex - delayedSampleIndex_floor;
			delayedSample = tempPoints[delayedSampleIndex_floor].second * (1 - fraction) + tempPoints[delayedSampleIndex_ceil].second * fraction;
		}
		tempPoints[n].second += A * delayedSample;
	}

	updateVector(tempPoints);
	writeWavFile("flangingEffect.wav", tempWrite, sRate, numC, bitsPSmp);

}

void VolumeUp_Down(double k) {
	tempPoints.clear();
	tempPoints = currentPoints;
	for (int i = 0; i < tempPoints.size(); i++) {
		tempPoints[i].second *= k;
	}
	updateVector(tempPoints);
	writeWavFile("VolumeUp_Down.wav", tempWrite, sRate, numC, bitsPSmp);
}

float chorusFunction(double sample, double depth, double rate, double delayTime, uint32_t sampleRate, double phase) {
	// Tính toán pha
	float mod = 1 + depth * sin(2 * pi * rate * phase / sampleRate);

	// Tính toán vị trí trễ
	int delayIndex = static_cast<int>(delayTime * sampleRate * mod);

	// Trả về giá trị âm thanh đã biến đổi
	return sample + depth * 0.5f * (sample + sample) * sin(2 * pi * delayIndex / sampleRate);
}

void applyChorus( double depth, double rate, double delayTime, double sampleRate) {
	tempPoints.clear();
	tempPoints = currentPoints;

	const int bufferSize = tempPoints.size();
	float phase = 0.0f;

	for (int i = 0; i < bufferSize; ++i) {
		tempPoints[i].second = chorusFunction(tempPoints[i].second, depth, rate, delayTime, sampleRate, phase);
		phase += 1.0f;
		if (phase > sampleRate) {
			phase -= sampleRate;
		}
	}
	updateVector(tempPoints);
	writeWavFile("ChorusEffect.wav", tempWrite, sRate, numC, bitsPSmp);
}
void chorusEffect(double depth, double rate, double delaytime) {
	applyChorus(depth, rate, delaytime, sRate);
}

//FILTER
vector < double > crossDiagonalMultiplication(const vector < double >& v1, const vector<double>& v2) {
	int rows = v1.size();
	int cols = v2.size();

	// Tạo vector kết quả với kích thước là rows + cols - 1
	int resultSize = rows + cols - 1;
	vector < double > result(resultSize, 0);

	// Nhân bảng và cộng kết quả theo đường chéo
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			result[i + j] += v1[i] * v2[j];
		}
	}
	return result;
}

void LPFilter(int N, double fc) {
	h.clear();
	double pos = ((double)N - 1) / 2;
	double wc = (double)2 * pi * fc / (sRate);


	for (int i = 0; i < N; i++) {
		if ((double)i == pos) {
			h.push_back(wc / pi);
		}
		else {
			double tmp = (wc / pi * sin(wc * ((double)i - ((double)N - 1) / 2)) / (wc * ((double)i - ((double)N - 1) / 2)));
			h.push_back(tmp);
		}
	}
	vector <double> result = crossDiagonalMultiplication(h, s);
	tempWrite.clear();

	for (int i = 0; i < h.size(); i++) {
		cout << h[i] << " ";
	}
	cout << "|| ";
	for (int i = 0; i < result.size(); i++) {
		tempWrite.push_back((int)result[i]);
		//cout << result[i] << " ";
	}

	writeWavFile("LPFilter.wav", tempWrite, sRate, numC, bitsPSmp);
	cout << endl;
}

void HPFilter(int N, double fc) {
	h.clear();
	double pos = ((double)N - 1) / 2;
	double wc = (double)2 * pi * fc / ( sRate);

	for (int i = 0; i < N; i++) {
		if ((double)i == pos) {
			h.push_back(1 - wc / pi);
		}
		else {
			double tmp = (-1) * (wc / pi * sin(wc * ((double)i - ((double)N - 1) / 2)) / (wc * ((double)i - ((double)N - 1) / 2)));
			h.push_back(tmp);
		}
	}
	vector <double> result = crossDiagonalMultiplication(h, s);
	tempWrite.clear();

	for (int i = 0; i < h.size(); i++) {
		cout << h[i] << " ";
	}
	cout << "|| ";
	for (int i = 0; i < result.size(); i++) {
		tempWrite.push_back((int)result[i]);
		//cout << result[i] << " ";
	}
	//testing(result);
	writeWavFile("HPFilter.wav", tempWrite, sRate, numC, bitsPSmp);
	cout << endl;
}

void BPFilter(int N, double fc1, double fc2) {
	h.clear();
	double pos = ((double)N - 1) / 2;
	double wc1 = (double)2 * pi * fc1 / ( sRate);
	double wc2 = (double)2 * pi * fc2 / ( sRate);

	for (int i = 0; i < N; i++) {
		if ((double)i == pos) {
			h.push_back(wc2 / pi - wc1 / pi);
		}
		else {
			double tmp = (wc2 / pi * sin(wc2 * ((double)i - ((double)N - 1) / 2)) / (wc2 * ((double)i - ((double)N - 1) / 2))) - (wc1 / pi * sin(wc1 * ((double)i - ((double)N - 1) / 2)) / (wc1 * ((double)i - ((double)N - 1) / 2)));
			h.push_back(tmp);
		}
	}
	vector <double> result = crossDiagonalMultiplication(h, s);
	tempWrite.clear();

	for (int i = 0; i < h.size(); i++) {
		cout << h[i] << " ";
	}
	cout << "||";
	for (int i = 0; i < result.size(); i++) {
		tempWrite.push_back((int)result[i]);
		//cout << result[i] << " ";
	}
	writeWavFile("BPFilter.wav", tempWrite, sRate, numC, bitsPSmp);
	cout << endl;
}

void BSFilter(int N, double fc1, double fc2) {
	h.clear();
	double pos = ((double)N - 1) / 2;
	double wc1 = (double)2 * pi * fc1 / ( sRate);
	double wc2 = (double)2 * pi * fc2 / ( sRate);

	for (int i = 0; i < N; i++) {
		if ((double)i == pos) {
			h.push_back(1.0 - (wc2 / pi - wc1 / pi));
		}
		else {
			double tmp = (-1.0) * ((wc2 / pi * sin(wc2 * ((double)i - ((double)N - 1) / 2)) / (wc2 * ((double)i - ((double)N - 1) / 2))) - (wc1 / pi * sin(wc1 * ((double)i - ((double)N - 1) / 2)) / (wc1 * ((double)i - ((double)N - 1) / 2))));
			h.push_back(tmp);
		}
	}
	vector <double> result = crossDiagonalMultiplication(h, s);
	tempWrite.clear();

	for (int i = 0; i < h.size(); i++) {
		cout << h[i] << " ";
	}
	cout << "||";
	for (int i = 0; i < result.size(); i++) {
		tempWrite.push_back((int)result[i]);
		//cout << result[i] << " ";
	}
	writeWavFile("BSFilter.wav", tempWrite, sRate, numC, bitsPSmp);
	cout << endl;
}