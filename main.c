/*
 * main.c - GetProxyInfo 命令行入口
 *
 * 兼容旧版行为: 不带参数时把结果写入 config\proxy.ini.
 * 旧版需要管理员权限, 新版以 asInvoker 运行, 不触发 UAC.
 */

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

#include "getproxyinfo.h"

static void gpi_usage(const char *exe)
{
    printf("GetProxyInfo %s - read current user's proxy settings (no admin required)\n\n",
           GPI_VERSION);
    printf("Usage:\n");
    printf("  %s                  write config\\proxy.ini\n", exe);
    printf("  %s <path>           write result to <path>\n", exe);
    printf("  %s --ini <path>     write result to <path>\n", exe);
    printf("  %s --print          print \"proxy=...\" to stdout\n", exe);
    printf("  %s --help           show this help\n", exe);
    printf("\nOutput format: \"proxy=host:port\" when a proxy is configured, \"proxy=0\" otherwise.\n");
}

int main(int argc, char **argv)
{
    const char *ini_path = "config\\proxy.ini";
    int print_only = 0;
    int i;
    GetProxyInfoResult result;

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--print") == 0 || strcmp(argv[i], "-p") == 0)
        {
            print_only = 1;
        }
        else if (strcmp(argv[i], "--ini") == 0 && i + 1 < argc)
        {
            ini_path = argv[++i];
        }
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            gpi_usage(argv[0]);
            return 0;
        }
        else
        {
            ini_path = argv[i];
        }
    }

    if (GetProxyInfo(&result) != 0)
    {
        fprintf(stderr, "GetProxyInfo: failed to read proxy settings\n");
        return 1;
    }

    if (print_only)
    {
        printf("proxy=%s\n", result.enabled ? result.server : "0");
        return 0;
    }

    if (GetProxyInfoWriteIni(ini_path) != 0)
    {
        fprintf(stderr, "GetProxyInfo: failed to write %s\n", ini_path);
        return 1;
    }

    return 0;
}
