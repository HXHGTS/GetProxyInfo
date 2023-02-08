#include<stdio.h>
#include<stdlib.h>
#include<io.h>

FILE* proxy_ini;
int ReturnValue,Num;
char temp[120], proxy[23],proxy_source[80];
char http_proxy[22], https_proxy[22], ftp_proxy[22];
int main() {
	if (_access("config", 0) == -1) {
		system("mkdir config");
	}
	if (_access("temp", 0) == -1) {
		system("mkdir temp");
	}
	system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyEnable\" > temp\\proxyswitcher.temp");
	if (system("type temp\\proxyswitcher.temp | find \"0x0\"") == 0) {//检测代理开启状态
		proxy_ini = fopen("config\\proxy.ini", "w");//没有设置代理的情况
		fprintf(proxy_ini, "proxy=0");
		fclose(proxy_ini);
	}
	else {
		system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyServer\" > temp\\proxy.txt");//检测一般代理
		proxy_ini = fopen("temp\\proxy.txt", "r");
		ReturnValue=fscanf(proxy_ini, "    ProxyServer    REG_SZ    %s", temp);//http=127.0.0.1:8888;https=127.0.0.1:8888;ftp=127.0.0.1:8888
		fclose(proxy_ini);
		if (temp[0] == 'h') {
			for (Num = 0; Num <= 120; Num++) {
				if (temp[Num] == ';') {
					temp[Num] = ' ';
				}
				else if (temp[Num] == '=') {//http 127.0.0.1:8888 https 127.0.0.1:8888 ftp 127.0.0.1:8888
					temp[Num] = ' ';
				}
				else if (temp[Num] == '\0') {
					break;
				}
			}
			proxy_ini = fopen("temp\\proxy.txt.out", "w");
			ReturnValue = fprintf(proxy_ini, "%s", temp);//http 127.0.0.1:8888 https 127.0.0.1:8888 ftp 127.0.0.1:8888
			fclose(proxy_ini);
			proxy_ini = fopen("temp\\proxy.txt.out", "r");
			ReturnValue = fscanf(proxy_ini, "http %s https %s ftp %s",http_proxy,https_proxy,ftp_proxy);
			fclose(proxy_ini);
			proxy_ini = fopen("config\\proxy.ini", "w");
			fprintf(proxy_ini, "proxy=%s", http_proxy);
			fclose(proxy_ini);
		}
		else {
			proxy_ini = fopen("temp\\proxy.txt", "r");
			ReturnValue = fscanf(proxy_ini, "    ProxyServer    REG_SZ    %s", proxy);
			fclose(proxy_ini);
			proxy_ini = fopen("config\\proxy.ini", "w");
			fprintf(proxy_ini, "proxy=%s", proxy);
			fclose(proxy_ini);
		}
	}
	system("del /F /S /Q temp\\proxy.txt");
	system("del /F /S /Q temp\\proxy.txt.out");
	system("del /F /S /Q temp\\proxyswitcher.temp");
	system("cls");
	return 0;
}