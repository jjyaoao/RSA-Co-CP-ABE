#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h> 

#include <iostream>
// #include <string>
#include <cassert>
#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

#include <string>
#include <iostream>
#include "time.h"
#include <stdlib.h>

 
#define SERVER_PORT		5000
#define SERVER_IP		"127.0.0.1"
#define BUF_SIZE 1024

// unsigned char msg[1024]; // 明文
char buff[BUF_SIZE];
int C_uint[BUF_SIZE];
unsigned int Dec[BUF_SIZE];
unsigned int PP = 7; // 两个素数
unsigned int QQ = 17;

bool JudgePrimeNum(unsigned int num);
unsigned int RandomlyGenerateE(unsigned int t);
void Gcd(unsigned int BigNum, unsigned int SmallNum, unsigned int &MaxGcd);
bool JudgeGcd_1(unsigned int BigNum, unsigned int SmallNum);
int Moni(unsigned int e, unsigned int model, unsigned int *d);
void ProduceKey(unsigned int p, unsigned int q, unsigned int &e, unsigned int &d, unsigned int &n);
unsigned int Modular_Ex(unsigned int e1, int b, const unsigned int m);
int BianaryTransform(int num, int bin_num[]);
unsigned int Modular_Exonentiation(unsigned int a, int b, int n);
void RSA_Decrytion(unsigned int d2, const unsigned int n2);

 
std::string mpkparam = "AAAAFqpvyQeGOyICbZ29rFw+6s74bZxtcGsAAAHToQFBsgEEtLIBABYrNiM2C7hfU2U8ZVwX+GVM0OdcBuIj4hIjLPC2twfmCYVok4A7jzxbROZjaFu82qmRdY5IdDAmDFJd3zeKYKMJQYNyiN/Hx0fLT/7iSc5+1n/V+iTl+Y8laxbOY1kxqRz+1qcfSUCx0MJO05dz2hZoZAh+e6ZN5cWyut7fo+wTBAqmuS3Q1+ordLPzgfSeFChxwYo/UhDafl25cGEADvoVtnIqR8/8FAx2co1DoCz5PrVRkdii5D5Yxb5u/xpIzRF6ZyAR4rca1AUplr9bIRwXjmn8yZN6hHdFyZ9C6BCCEDh2+rW0HleGUmOxeYqAiBH2vkelLUZGbuB8FWY84MihAmcxoSSyoSECI7LiNuFqS8IDSCcFwqmWmneJdAD+7UtCBJZinvXdUbmhA2cxYaEksqEhAw2Z7n15ftNpsfjR5HsaJ1f9nEyITBLBw3BuNvpbjCVsoQJnMqFEs6FBAwIINf5u9ptuuMuZbmdq39aDAkrkJ02NEJEzWJOSZjOXCCPYGra1N4jUyz0dGgLUbK/MQAGuYVdSGDrZLHZVtTShAWuhJR0AAAAgWRCwJQBcFtxFRaR4eX7GFeVQlybDT21xQYPGsYGSasg=";

std::string mskparam = "AAAAFqpvyQC/De1L2WQAGcniiKHyeFVtc2sAAAB3oQVhbHBoYaEjsQAgCojxJbZh+cmJ85vRN8B9QptoCNO9tOfgKt/sII2uThKhA2cyYaFEs6FBAgnwkJIGhHv1yhBE+xqtogqC2ReeKRYHTK3SxLYM+n0EI82+iksjJ5verfLYITflKaNNmx7bMx7ZeNvs+H40IwM=";

// std::string PublicKey = 8191  // 默认测试时发送给Server的公钥

using namespace std;
using namespace oabe;
using namespace oabe::crypto;

int main(int argc, char *argv[]) {
	
	int ret = 0;
	int sockfd = 0;			// 通信套接字
	char *message = NULL;
	struct sockaddr_in serveraddr;
	int n = 0;

	
	if (2 != argc) {	
		fprintf(stderr, "Usage: ./echo_client message \n");
		exit(1);
	}
	
	// 获取第二个参数的字符串
	message = argv[1];
	printf("message: %s\n", message);
	
	string text = argv[1];

	// 实现cp-abe信息加密
	InitializeOpenABE();

	cout << "CP-ABE encrypted text:" << endl;

	OpenABECryptoContext cpabe("CP-ABE");

	string ct, pt1 = text, pt2;

	// cpabe.generateParams();
	std::string mpk, msk;
    mpk = mpkparam;
    msk = mskparam;
    cpabe.importPublicParams(mpk);
    cpabe.importSecretParams(msk);

	cpabe.keygen("|attr1|attr2", "key0");

	cpabe.encrypt("attr1 and attr2", pt1, ct);

	char *message2 = (char *)ct.c_str();
	int len = ct.length();
	

	printf("CP-ABE message: %s\n", message2);


	// 1.创建通信套接字
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		perror("create sockfd error");
		exit(-1);
	}
	
	// 2.清空标签，写上地址和端口号
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;							// IPv4
	inet_pton(AF_INET, SERVER_IP, &serveraddr.sin_addr);		// 服务器地址
	serveraddr.sin_port = htons(SERVER_PORT);					// 服务器端口号
	
	// 3.连接服务器
	ret = connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (-1 == ret) {
		perror("connect error");
		close(sockfd);
		exit(-2);
	}
	
	// 4.给服务器发送数据
	ret = write(sockfd, message2, len);
	if (-1 == ret) {
		perror("write error");
		close(sockfd);
		exit(-3);
	}
	
	// 5.接受服务器发送过来的数据
	n = read(sockfd, buff, sizeof(buff)-1);
	if (-1 == n) {
		perror("read error");
		close(sockfd);
		exit(-4);
	}
	
	if (n > 0) {
		buff[n] = '\0';
		printf("receive: %s\n", buff);

		unsigned int e = stoi(text);
		unsigned int d = 0;
    	unsigned int n = 0; // n=p*q
		ProduceKey(PP, QQ, e, d, n);
    	cout << "e:" << e << endl;
    	cout << "d:" << d << endl;
		// string aa = buff;
		// print << "aa: " << aa << endl;
		for (int k = 0; k < sizeof(buff); k++){
			C_uint[k] = int(buff[k]);
			// cout <<  C_uint[k];
    	}
		// cout << C_uint << "   " << buff << endl;
    	RSA_Decrytion(d, n); // 解密
		printf("real message：\n");
		for (int i = 0; i < sizeof(buff); i++){
        	cout << char(Dec[i]);
    	}
		cout << endl;

	} else {
		perror("error!!!\n");
	}
	
	printf("client finished.\n");
	// 6.关闭套接字
	close(sockfd);
	
	return 0;
}

// 判断p是否为素数
// 在产生密钥函数中调用
bool JudgePrimeNum(unsigned int num)
{
    unsigned int devider = 2;
    for (; devider < int(num / 2); devider++)
    {
        if (num % devider == 0)
            return false;
    }
    return true;
}

// 产生2到t-1的随机数e
// 在产生密钥函数中调用
unsigned int RandomlyGenerateE(unsigned int t)
{
    unsigned int e = 0;
    srand((int)time(0)); // 设置随机数种子，在rand之前调用一次即可
    e = 2 + rand() % (t - 3);
    return (e); // 随机数
}
// 求最大公因数
void Gcd(unsigned int BigNum, unsigned int SmallNum, unsigned int &MaxGcd)
{
    int tmp = 0;
    while (BigNum % SmallNum)
    {
        tmp = SmallNum;
        SmallNum = BigNum % SmallNum;
        BigNum = tmp;
    }
    MaxGcd = SmallNum;
}
// 判断最大公约数数是否是1，是1的话两个数就互质
// 在产生密钥函数中调用
bool JudgeGcd_1(unsigned int BigNum, unsigned int SmallNum)
{
    unsigned int M = 0;
    Gcd(BigNum, SmallNum, M);
    if (M == 1)
        return true;
    else
        return false;
}

// 　3*7 = 21; 21 % 20 = 1 ; 所以3,7 互为 20 的 模逆.
// 　9*3 = 27; 27 % 26 = 1 ; 所以9,3 互为 26 的 模逆.
//==利用加的模等于模的加求e*d = 1 mod model 中的d
// 在产生密钥函数中调用
int Moni(unsigned int e, unsigned int model, unsigned int *d)
{
    unsigned int i;
    unsigned int over = e;
    for (i = 1; i < model;)
    {
        over = over % model;
        if (over == 1)
        {
            *d = i;
            return 1;
        }
        else
        {
            if (over + e <= model)
            {
                do
                {
                    i++;
                    over += e;
                } while (over + e <= model);
            }
            else
            {
                i++;
                over += e;
            }
        }
    }
    return 0;
}
// 产生密钥函数  其中p q互异质数  ，公钥P{e,n)，私钥S{d,n}
// 在主函数调用
void ProduceKey(unsigned int p, unsigned int q, unsigned int &e, unsigned int &d, unsigned int &n)
{
    unsigned int t = 0;
    while (!JudgePrimeNum(p))
    {
        cout << "p is not a prime number, please re-enter p:";
        cin >> p;
    }
    while (!JudgePrimeNum(q))
    {
        cout << "q is not a prime number, please re-enter q:";
        cin >> q;
    }
    //  随机选择一个2<e<t ,使得gcd(t,e)=1
    n = p * q;
    t = (p - 1) * (q - 1);
    // e = RandomlyGenerateE(t);
    // e = 65537;
    while (!JudgeGcd_1(t, e))
    {
        cout << "The public key e and Euler function are not mutually exclusive, please check if the client's public key is correct!" << endl;
        // close(client_sock);
        break;
        // e = RandomlyGenerateE(t);
    }
    // 计算d,使得e*d=1 mod t
    Moni(e, t, &d);
}
unsigned int Modular_Ex(unsigned int e1, int b, const unsigned int m)
{
    unsigned int i;
    unsigned int tmp = b;
    for (i = 0; i < e1; i++)
    {
        b = b % m;
        b = b * tmp;
    }
    return b / tmp;
}
// 二进制转换
int BianaryTransform(int num, int bin_num[])
{

    int i = 0, mod = 0;

    // 转换为二进制，逆向暂存temp[]数组中
    while (num != 0)
    {
        mod = num % 2;
        bin_num[i] = mod;
        num = num / 2;
        i++;
    }
    // 返回二进制数的位数
    return i;
}

// 反复平方求幂
unsigned int Modular_Exonentiation(unsigned int a, int b, int n)
{
    int c = 0, bin_num[1000];
    long long d = 1;
    int k = BianaryTransform(b, bin_num) - 1;

    for (int i = k; i >= 0; i--)
    {
        c = 2 * c;
        d = (d * d) % n;
        if (bin_num[i] == 1)
        {
            c = c + 1;
            d = (d * a) % n;
        }
    }
    return d;
}


// RSA解密
void RSA_Decrytion(unsigned int d2, const unsigned int n2)
{
    unsigned int i;
    unsigned int tmp;
    int j;
    for (j = 0; j < sizeof(buff); j++)
    {

        Dec[j] = Modular_Exonentiation(C_uint[j], d2, n2);

		// cout << Dec[j];
    }
}

