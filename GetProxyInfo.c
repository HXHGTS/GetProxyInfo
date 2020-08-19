#include<stdio.h>
#include<stdlib.h>
FILE* proxy_ini,*proxyurl_ini;
char proxy[40],proxyurl[150],cmd[200],ret1[20], ret2[20], ret3[20], ret4[20];
int i,j;
int main() {
	system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyEnable\" > temp\\proxyswitcher.temp");
	if (system("type temp\\proxyswitcher.temp | find \"0x0\"") == 0) {//检测代理开启状态
		system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"AutoConfigURL\" > temp\\proxyswitcher.temp");//检测pac文件
		if (system("type temp\\proxyswitcher.temp | find \"AutoConfigURL\"") == 0) {
			proxyurl_ini = fopen("temp\\proxyswitcher.temp", "r");
			fscanf(proxyurl_ini, "    AutoConfigURL    REG_SZ    %s", proxyurl);
			fclose(proxyurl_ini);
			sprintf(cmd, "curl \"%s\" > temp\\pac.txt",proxyurl);
			system(cmd);
			system("type temp\\pac.txt | find \"PROXY \" > temp\\proxy.txt");
			proxy_ini = fopen("temp\\proxy.txt", "r");
			fscanf(proxy_ini, "%s %s %s %s %s",ret1,ret2,ret3,ret4,proxy);
			fclose(proxy_ini);
			for (i = 0; i <= 40; i++) {
				if (proxy[i] == ';'|| proxy[i] == '\'' || proxy[i] == '"') {
					proxy[i] = '\0';
				}
			}
			proxy_ini = fopen("config\\proxy.ini", "w");
			fprintf(proxy_ini, "proxy=%s", proxy);
			fclose(proxy_ini);
		}
		else {
			proxy_ini = fopen("config\\proxy.ini", "w");//没有设置代理的情况
			fprintf(proxy_ini, "proxy=0");
			fclose(proxy_ini);
		}
	}
	else {
		system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyServer\" > temp\\proxy.txt");//检测一般代理
		proxy_ini = fopen("temp\\proxy.txt", "r");
		fscanf(proxy_ini, "    ProxyServer    REG_SZ    %s", proxy);
		fclose(proxy_ini);
		proxy_ini = fopen("config\\proxy.ini", "w");
		fprintf(proxy_ini, "proxy=%s", proxy);
		fclose(proxy_ini);
	}
	system("del temp\\proxyswitcher.temp");
	system("del temp\\pac.txt");
	system("del temp\\proxy.txt");
	return 0;
}