#include<stdio.h>
#include<stdlib.h>
FILE* proxy_ini;
char proxy[30],proxyurl[50],cmd[100];
int i;
int main() {
	system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyEnable\" > temp\\proxy.ini.0");
	if (system("type temp\\proxy.ini.0 | find \"0x0\"") == 0) {
		system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"AutoConfigURL\" > temp\\proxy.ini.0");
		if (system("type temp\\proxy.ini.0 | find \"AutoConfigURL\"") == 0) {
			proxy_ini = fopen("temp\\proxy.ini.0", "r");
			fscanf(proxy_ini, "    AutoConfigURL    REG_SZ    %s", proxyurl);
			fclose(proxy_ini);
			sprintf(cmd, "curl %s > temp\\proxy.ini.1",proxyurl);
			system(cmd);
			system("type temp\\proxy.ini.1 | find \"var proxy\" > temp\\proxy.ini.2");
			proxy_ini = fopen("temp\\proxy.ini.2", "r");
			fscanf(proxy_ini, "var proxy = \"PROXY %s\;\"\;", proxy);
			fclose(proxy_ini);
			for (i = 0; i <= 30; i++) {
				if (proxy[i] == ';' || proxy[i] == '"') {
					proxy[i]='\0';
				}
			}
			proxy_ini = fopen("config\\proxy.ini", "w");
			fprintf(proxy_ini, "proxy=%s", proxy);
		}
		else {
			proxy_ini = fopen("config\\proxy.ini", "w");
			fprintf(proxy_ini, "proxy=0");
		}
	}
	else {
		system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyServer\" > temp\\proxy.ini.1");
		proxy_ini = fopen("temp\\proxy.ini.1", "r");
		fscanf(proxy_ini, "    ProxyServer    REG_SZ    %s", proxy);
		fclose(proxy_ini);
		proxy_ini = fopen("config\\proxy.ini", "w");
		fprintf(proxy_ini, "proxy=%s", proxy);
	}
	fclose(proxy_ini);
	system("del temp\\proxy.ini.0");
	system("del temp\\proxy.ini.1");
	system("del temp\\proxy.ini.2");
	return 0;
}