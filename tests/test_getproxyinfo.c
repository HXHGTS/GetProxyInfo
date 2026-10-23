/*
 * test_getproxyinfo.c - 手工验证程序
 *
 * 编译 (在仓库根目录):
 *   cl /nologo /utf-8 /W4 /I. tests\test_getproxyinfo.c getproxyinfo.c
 *
 * 期望: 输出结果与 "设置 -> 网络和 Internet -> 代理" 中的配置一致.
 */

#include <stdio.h>

#include "getproxyinfo.h"

int main(void)
{
    GetProxyInfoResult r;
    int rc = GetProxyInfo(&r);

    printf("rc         = %d\n", rc);
    if (rc != 0)
        return 1;

    printf("enabled    = %d\n", r.enabled);
    printf("server     = %s\n", r.server);
    printf("raw        = %s\n", r.raw);
    printf("bypass     = %s\n", r.bypass);
    printf("pac_url    = %s\n", r.pac_url);
    printf("auto_detect= %d\n", r.auto_detect);

    return 0;
}
