# GetProxyInfo

读取当前用户 Windows 代理设置的 C 模块 + 命令行工具。

**不需要管理员权限，不触发 UAC。** 只读 `HKEY_CURRENT_USER`，不写注册表，不改系统设置。

## 特性

- 无需提权：可执行文件清单为 `asInvoker`，普通用户直接运行
- 只读当前用户配置：`HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings`
- 首选 `WinHttpGetIEProxyConfigForCurrentUser()`，与 IE / 系统代理界面同源
- API 不可用时自动回退到注册表读取
- 同时提供绕过列表（ProxyOverride）和 PAC 地址，旧版只解析 ProxyServer
- 无 `system()`、无 `reg.exe`、无临时文件、无固定长度缓冲区溢出风险
- 纯 C，只需 Win32 SDK，可直接把 `getproxyinfo.c` / `getproxyinfo.h` 复制进其它工程

## 构建

Visual Studio（`GetProxyInfo.sln`，v143 工具集），或任意已加载 MSVC 环境的命令行：

```bat
build.bat
```

手动编译：

```bat
cl /nologo /utf-8 /O2 getproxyinfo.c main.c ^
   /link /MANIFESTUAC:"level='asInvoker' uiAccess='false'"
```

## 命令行用法

```text
GetProxyInfo.exe                写 config\proxy.ini（兼容旧版）
GetProxyInfo.exe <path>         写到指定路径
GetProxyInfo.exe --ini <path>   写到指定路径
GetProxyInfo.exe --print        只打印 proxy=...，不写文件
GetProxyInfo.exe --help
```

输出格式与旧版一致，无换行：

```ini
proxy=127.0.0.1:7890
```

无代理时：

```ini
proxy=0
```

## 作为上游代码引入

把 `getproxyinfo.c` 和 `getproxyinfo.h` 复制到你的工程，调用：

```c
#include "getproxyinfo.h"

GetProxyInfoResult r;
if (GetProxyInfo(&r) == 0)
{
    if (r.enabled)
        printf("proxy     = %s\n", r.server);   // host:port
    printf("raw       = %s\n", r.raw);          // 原始 ProxyServer 串
    printf("bypass    = %s\n", r.bypass);       // ProxyOverride
    printf("pac       = %s\n", r.pac_url);      // 自动配置脚本
    printf("autodetect= %d\n", r.auto_detect);
}
```

直接生成配置文件：

```c
GetProxyInfoWriteIni("config\\proxy.ini");
```

MSVC 下 `.c` 内已通过 `#pragma comment(lib, ...)` 链接 `winhttp.lib` 与 `advapi32.lib`，手工编译时加上这两个库即可。

## 解析规则

`ProxyServer` 可能是单值，也可能是按协议分别配置：

| ProxyServer | server |
|---|---|
| `127.0.0.1:7890` | `127.0.0.1:7890` |
| `http=a:1;https=b:2;socks=c:3` | `a:1`（http 优先） |
| `https=b:2;socks=c:3` | `b:2` |
| `socks=c:3` | `c:3` |

`ProxyEnable=0` 或未配置时 `enabled=0`，写出的配置为 `proxy=0`。

## 关于管理员权限

- 读取 `HKCU` 下的当前用户设置，标准用户本来就有权限，**不需要 UAC**
- 旧版 exe 的 UAC 来自工程里的 `<UACExecutionLevel>RequireAdministrator</UACExecutionLevel>`，与代码逻辑无关
- 需要管理员的是：**写** `HKLM` 或系统级 WinHTTP 代理；读取 `HKLM` 同样不需要管理员
- 以管理员身份运行时，`HKCU` 可能变成另一个账户的配置单元，反而可能读到错误的代理

## 与 1.x 的差异

| 项目 | 1.x | 2.x |
|---|---|---|
| 权限 | 要求管理员（UAC） | `asInvoker`，无 UAC |
| 读取方式 | `reg.exe` + `find` 文本解析 | WinHTTP API + 注册表 API |
| 临时文件 | `temp\proxy.txt` 等 | 无 |
| 缓冲区 | `char[120]` / `char[23]` 有溢出风险 | 全部有界 |
| PAC / 绕过列表 | 不支持 | 支持 |

## 许可证

未指定。如需在其它项目中使用，请先与仓库作者确认。
