#include<bits/stdc++.h>
#include "gnuplot-iostream.h"
#include "AudioProcessing.h"
using namespace std;

const char* filename = "E:\\Code\\noise.wav";
const char* filenameToMath = "E:\\Code\\fntest1.wav";
int N;
double fc,fc1,fc2;

int main() {
	setGrid();
	readCurrentFileWav(filename);
	readFileToMath(filenameToMath);
	bool check = true;
	int statement;

	while (check) {
		cout << "-------SELECTION----------" << endl;
		cout << "1. TimeTransfer with n" << endl;
		cout << "2. TimeReverse" << endl;
		cout << "3. Plus 2 Signal" << endl;
		cout << "4. Subtraction 2 Signal" << endl;
		cout << "5. Multiplication 2 Signal" << endl;
		cout << "6. DownSampling" << endl;
		cout << "7. Upsampling" << endl;
		cout << "8. EXIT" << endl;
		cout << "9. FadeIn Effect" << endl;
		cout << "10. FadeOut Effect" << endl;
		cout << "11. Echo Effect" << endl;
		cout << "12. Reverb Effect" << endl;
		cout << "13. Flanging Effect" << endl;
		cout << "14. Low Pass Filter (LPS) " << endl;
		cout << "15. Hight Pass Filter (HPF)" << endl;
		cout << "16. Band Pass Filter (BPF)" << endl;
		cout << "17.Band Stop Filter (BSF)" << endl;
		cout << "18.VolumeUp_Down" << endl;
		cout << "19. ChorusEffect" << endl;
		cout << "--------------------------" << endl;

		cin >> statement;
		switch (statement)
		{
		case 1:
			int n;
			cin >> n;
			timeTransfer(n);
			break;
		case 2:
			timeReverse();
			break;
		case 3:
			plusTwoSignal();
			break;
		case 4:
			subtractionSignal();
			break;
		case 5:
			multiplicationSignal();
			break;
		case 6:
			int k;
			cin >> k;
			downSampling(k);
			break;
		case 7:
			int m;
			cin >> m;
			upSampling(m);
			break;
		case 8:
			check = false;
			break;
		case 9:
			int delay;
			cin >> delay;
			fadeInEffect(delay);
			//eg:1
			break;
		case 10:
			int delay1;
			cin >> delay1;
			fadeOutEffect(delay1);
			//eg:1
			break;
		case 11:
			double delay2, decase;
			cin >> delay2 >> decase;
			echoEffect(delay2, decase);
			//0.5 0.5
			break;
		case 12:
			double decase1, mix, delayinMS, wet, reverberance;
			cin >> decase1 >> mix >> delayinMS >> wet >> reverberance;
			reverbEffect(decase1, mix, delayinMS, wet, reverberance);
			/*reverbEffect(0.5, 1, 200, 1, 0.5);*/
			break;
		case 13:
			double A, ro, f;
			cin >> A >> ro >> f;
			 flangingEffect( A,  ro,  f);
			//flangingEffect(0.5, 100, 0.2);
			break;
		case 14:
			cin >> N;
			cin >> fc;
			 LPFilter( N, fc);
			//LPFilter(21,3000);
			break;
		case 15:
			cin >> N;
			cin >> fc;
			 HPFilter( N,  fc);
			//HPFilter(20, 3000);
			break;
		case 16:
			cin >> N;
			cin >> fc1 >> fc2;
			 BPFilter( N,  fc1,  fc2);
			//BPFilter(21, 3000, 4000);
			break;
		case 17:
			cin >> N;
			cin >> fc1 >> fc2;
			 BSFilter( N,  fc1,  fc2);
			//BSFilter(6, 3000, 4000);
			break;
		case 18:
			double amly;
			cin >> amly;
			VolumeUp_Down(amly);
			//VolumeUp_Down(2);
			break;
		case 19:
			double depth, rate, delaytime;
			cin >> depth >> rate >> delaytime;
			chorusEffect( depth,  rate,  delaytime);
			//chorusEffect( 0.5, 1.5, 1);
			break;
		default:
			break;
		}
	}
	
}