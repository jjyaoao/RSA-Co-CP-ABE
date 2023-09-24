#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <iostream>
#include <cstring>
#include <string>
#include <cassert>
#include <openabe/openabe.h>
#include <openabe/zsymcrypto.h>

#include <string>
#include <iostream>
#include "time.h"
#include <stdlib.h>


using namespace oabe;
using namespace oabe::crypto;
using namespace std;

unsigned char msg[1024]; // 明文
unsigned int C_uint[1024];
unsigned int Dec[1024];
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
void RSA_Encrytion(unsigned int e1, const unsigned int n1);
void RunRSA(unsigned int RSAPublicKey);


#define SERVER_PORT 5000
#define BUF_SIZE 1024

std::string mpkparam = "AAAAFqpvyQeGOyICbZ29rFw+6s74bZxtcGsAAAHToQFBsgEEtLIBABYrNiM2C7hfU2U8ZVwX+GVM0OdcBuIj4hIjLPC2twfmCYVok4A7jzxbROZjaFu82qmRdY5IdDAmDFJd3zeKYKMJQYNyiN/Hx0fLT/7iSc5+1n/V+iTl+Y8laxbOY1kxqRz+1qcfSUCx0MJO05dz2hZoZAh+e6ZN5cWyut7fo+wTBAqmuS3Q1+ordLPzgfSeFChxwYo/UhDafl25cGEADvoVtnIqR8/8FAx2co1DoCz5PrVRkdii5D5Yxb5u/xpIzRF6ZyAR4rca1AUplr9bIRwXjmn8yZN6hHdFyZ9C6BCCEDh2+rW0HleGUmOxeYqAiBH2vkelLUZGbuB8FWY84MihAmcxoSSyoSECI7LiNuFqS8IDSCcFwqmWmneJdAD+7UtCBJZinvXdUbmhA2cxYaEksqEhAw2Z7n15ftNpsfjR5HsaJ1f9nEyITBLBw3BuNvpbjCVsoQJnMqFEs6FBAwIINf5u9ptuuMuZbmdq39aDAkrkJ02NEJEzWJOSZjOXCCPYGra1N4jUyz0dGgLUbK/MQAGuYVdSGDrZLHZVtTShAWuhJR0AAAAgWRCwJQBcFtxFRaR4eX7GFeVQlybDT21xQYPGsYGSasg=";

std::string mskparam = "AAAAFqpvyQC/De1L2WQAGcniiKHyeFVtc2sAAAB3oQVhbHBoYaEjsQAgCojxJbZh+cmJ85vRN8B9QptoCNO9tOfgKt/sII2uThKhA2cyYaFEs6FBAgnwkJIGhHv1yhBE+xqtogqC2ReeKRYHTK3SxLYM+n0EI82+iksjJ5verfLYITflKaNNmx7bMx7ZeNvs+H40IwM=";



int main(void) {
	
	int ret = 0;
	int sock;	// 通信套接字
	struct sockaddr_in server_addr;
	
	InitializeOpenABE();
    OpenABECryptoContext cpabe("CP-ABE");
    string ct, pt;

	std::string mpk, msk;
    mpk = mpkparam;
    msk = mskparam;
    cpabe.importPublicParams(mpk);
    cpabe.importSecretParams(msk);

	cpabe.keygen("|attr1|attr2", "key0");




	// 1.创建通信套接字
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sock) {
		fprintf(stderr, "create socket error, reason: %s\n", strerror(errno));
		exit(-1);
	}
	
	// 2.清空标签，写上地址和端口号
	bzero(&server_addr, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;	// 选择协议组ipv4
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 监听本地所有IP地址
	server_addr.sin_port = htons(SERVER_PORT);			// 绑定端口号
	
	// 3.绑定
	ret = bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (-1 == ret) {
		fprintf(stderr, "socket bind error, reason: %s\n", strerror(errno));
		close(sock);
		exit(-2);
	}
	
	// 4.监听，同时监听128个请求
	ret = listen(sock, 128);
	if (-1 == ret) {
		fprintf(stderr, "listen error, reason: %s\n", strerror(errno));
		close(sock);
		exit(-2);
	}
	
	printf("Waiting for a connection from the client\n");
	
	int done = 1;
	
	while (done) {
		
		struct sockaddr_in client;
		int client_sock;
		char client_ip[64];
		int len = 0;
		char buf[BUF_SIZE];
		
		socklen_t client_addr_len;		
		client_addr_len = sizeof(client);
		// 5.接受
		client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);
		if (-1 == client_sock) {
			perror("accept error");
			close(sock);
			exit(-3);
		}
		
		// 打印客户端IP地址和端口号
		printf("client ip: %s\t port: %d\n",
				inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)),
				ntohs(client.sin_port));
		
		
		// 6.读取客户端发送的数据
		len = read(client_sock, buf, sizeof(buf)-1);
		if (-1 == len) {
			perror("read error");
			close(sock);
			close(client_sock);
			exit(-4);
		}
		
		buf[len] = '\0';
		printf("recive[%d]: %s\n", len, buf);
		
		// 7.给客户端发送数据
 		pt = buf;

        bool result = cpabe.decrypt("key0", pt, ct);

        char *message2 = (char *)ct.c_str();
        len = ct.length();
        cout << "The result after CP-ABE decryption is :" << endl << ct << endl;


		// 用户RSA公钥为ct，运行RSA
		RunRSA(stoi(ct));
		
        // cout << "The length of ciphertext:" << len << endl;
		cout << "Please enter the message to be sent to Client:" << endl;
		fgets(buf, BUF_SIZE, stdin);

		cout << "Real message sent:";
		for (int i = 0; i < sizeof(buf); i++)
		{
			cout << char(C_uint[i]);
			buf[i] = char(C_uint[i]);
		}
		len = strlen(buf);
		cout << endl;
	
		len = write(client_sock, buf, len);
		if (-1 == len) {
			perror("write error");
			close(sock);
			close(client_sock);
			exit(-5);
		}
		memset(message2, 0, sizeof(message2));
        memset(buf, 0, sizeof(buf));
		printf("write finished. len: %d\n", len);
		// 8.关闭客户端套接字
		close(client_sock);
	}
	
	// 9.关闭服务器套接字
	close(sock);
	
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

// RSA加密
// 在主函数调用a*a%n =((a%n)*a)%n
void RSA_Encrytion(unsigned int e1, const unsigned int n1)
{
    unsigned int i;
    unsigned int tmp;
    int j;

    // cout << "Please enter the message to be sent to Client:" << endl;
    cin >> msg;
    for (j = 0; j < sizeof(msg); j++)
    {
        C_uint[j] = Modular_Exonentiation(msg[j], e1, n1);
    }
}


void RunRSA(unsigned int RSAPublicKey)
{
    unsigned int e = RSAPublicKey;
    unsigned int d = 0;
    unsigned int n = 0; // n=p*q
    ProduceKey(PP, QQ, e, d, n);
    cout << "e:" << e << endl;
    cout << "d:" << d << endl;
    RSA_Encrytion(e, n); // 加密
}
