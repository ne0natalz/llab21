#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc == 1) {
        cout << "Please input numbers to find average." << endl;
        return 0;
    }

    double sum = 0.0;
    int count = argc - 1;

    for (int i = 1; i < argc; i++) {
        // รองรับจำนวนเต็ม/ทศนิยม/ติดลบ
        sum += stod(string(argv[i]));
    }

    double avg = sum / count;

    cout << "---------------------------------" << endl;
    cout << "Average of " << count << " numbers = ";

    // แสดงผลแบบในตัวอย่าง: ถ้าเป็นจำนวนเต็มให้โชว์เป็นเลขเต็ม ไม่ใส่ .0
    if (avg == static_cast<long long>(avg)) {
        cout << static_cast<long long>(avg) << endl;
    } else {
        // ให้พิมพ์ทศนิยมแบบ "พอดี" (ไม่บังคับจำนวนหลักตายตัว)
        cout << setprecision(15) << avg << endl;
    }

    cout << "---------------------------------" << endl;

    return 0;
}