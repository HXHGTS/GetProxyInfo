#include<stdio.h>
#include<stdlib.h>
FILE* proxy_ini;
char proxy[30];

int main() {
	system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyEnable\" > temp\\proxy.ini.0");
	if (system("type temp\\proxy.ini.0 | find \"0x0\"") == 0) {
		proxy_ini = fopen("config\\proxy.ini", "w");
		fprintf(proxy_ini, "proxy=0");
	}
	else {
		system("reg query \"HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\" | find \"ProxyServer\" > temp\\proxy.ini.1");
		proxy_ini = fopen("temp\\proxy.ini.1", "r");
		fscanf(proxy_ini, "    ProxyServer    REG_SZ    %s", proxy);
		fclose(proxy_ini);
		system("del temp\\proxy.ini.1");
		proxy_ini = fopen("config\\proxy.ini", "w");
		fprintf(proxy_ini, "proxy=%s", proxy);
	}
	fclose(proxy_ini);
	system("del temp\\proxy.ini.0");
	return 0;
}