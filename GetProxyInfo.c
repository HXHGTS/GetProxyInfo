#include<stdio.h>
#include<stdlib.h>
#include<io.h>

FILE* proxy_ini;
int ReturnValue,Num;
char temp[100], proxy[23],proxy_source[80];
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
		ReturnValue=fscanf(proxy_ini, "    ProxyServer    REG_SZ    %s", temp);
		fclose(proxy_ini);
		if (temp[0] == 'h') {
			system("sed -i \"s/;/ /g\" temp\\proxy.txt");
			system("sed -i \"s/http=//g\" temp\\proxy.txt");
			system("sed -i \"s/https=//g\" temp\\proxy.txt");
			system("sed -i \"s/ftp=//g\" temp\\proxy.txt");
			proxy_ini = fopen("temp\\proxy.txt", "r");
			ReturnValue = fscanf(proxy_ini, "    ProxyServer    REG_SZ %s %s %s", http_proxy,https_proxy,ftp_proxy);
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
	system("move /Y sed.exe se.bak");
	system("del /f /s /q sed*");
	system("move /Y se.bak sed.exe");
	system("del /F /S /Q temp\\proxy.txt");
	system("del /F /S /Q temp\\proxyswitcher.temp");
	system("cls");
	return 0;
}