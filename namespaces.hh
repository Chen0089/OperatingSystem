// std
using std::cout;
using std::endl;
using std::string;
using std::wstring;
using std::vector;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::tm;
using std::time;
using std::time_t;
using std::ctime;
using std::gmtime;
using std::asctime;
using std::invalid_argument;
using std::stod;
using std::map;
using std::getline;
using std::istringstream;
using std::ostringstream;
using std::reverse;

// json
using json = nlohmann::json

// new:maths
namespace maths {
  //pi
  double PI(short precision) {
    // 假设 PI 已经是一个包含15位小数的字符串
    string PI = "3.141592653589793";  // 完成15位

    // 循环并去掉最后一个数字，直到只剩下 'precision' 位小数
    for (int i = 15; i > precision; i--) {
        PI = PI.substr(0, PI.length() - 1);  // 去掉最后一个字符
    }

    // 将 PI 字符串转换为 double 类型
    return stod(PI);  // 返回转换后的 double 值
  }
  // 自定义幂运算函数（支持小数指数）
  double power(double base, double exponent) {
    // 处理底数为0且指数为负数的情况
    if (base == 0 && exponent < 0) {
        throw invalid_argument("Base cannot be 0 when exponent is negative.");
    }

    return exp(exponent * log(base));  // 使用指数和对数来计算小数指数
  }
  // 计算矩形面积
    double rectangleArea(double length, double width) {
        return length * width;
    }

    // 计算矩形周长
    double rectanglePerimeter(double length, double width) {
        return 2 * (length + width);
    }

    // 计算圆形面积
    double circleArea(double radius) {
        return PI * power(radius, 2);  // M_PI 是数学常数圆周率
    }

    // 计算圆形周长
    double circleCircumference(double radius) {
        return 2 * PI * radius;
    }

    // 快速平方根（使用魔法数方法）
    double fastSqrt(double n) {
        long i = *(long*)&n;
        i = (i >> 1) + 0x1FBD1F3E;// 魔法数
        n = *(double*)&i;
        return n;
        // 此处引用原来的注释：What the fuck?
    }

    // 计算三角形的面积（赫伦公式）
    double triangleArea(double a, double b, double c) {
        double s = (a + b + c) / 2;  // 半周长
        return sqrt(s * (s - a) * (s - b) * (s - c));  // 赫伦公式
    }

    // 计算正方形面积
    double squareArea(double side) {
        return power(side, 2);
    }

    // 计算正方形周长
    double squarePerimeter(double side) {
        return 4 * side;
    }

    // 计算圆环面积
    double annulusArea(double outerRadius, double innerRadius) {
        return PI * (power(outerRadius, 2) - power(innerRadius, 2));
    }

    // 计算立方体的体积
    double cubeVolume(double side) {
        return power(side, 3);
    }

    // 计算立方体的表面积
    double cubeSurfaceArea(double side) {
        return 6 * power(side, 2);
    }
  //more
}
