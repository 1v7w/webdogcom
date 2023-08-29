# webdogcom

Implementation of Dr.com in web authentication.

Drcom哆点Web认证的实现。

```shell
Usage:
        webdogcom [options <argument>]...

Options:
        --interval <m>, -i <m>                authentication per m(int) seconds
        --daemon, -d                          set daemon flag
        --help, -h                            display this help
```

### Example:
```shell
$ webdogcom -d
$ webdogcom -d -i 600 # per 10 minutes to auth
```

### To build:
```shell
make
```
Modify the configuration of webdogcom.conf.

修改webdogcom.conf中的配置。

| config/配置项      | NULL/可为空 | description/说明                       |
|:----------------|:---------|:-------------------------------------|
| server          | False    | auth server/认证服务器                    |
| port            | False    | server port/认证服务端口,Drcom default 801 |
| network_segment | False    | 校园网内网网段                              |
| subnet_mask     | False    | 子网掩码                                 |
| user_account    | False    | 校园网用户名                               |
| user_password   | False    | 校园网密码                                |
| wlan_ac_name    | True     | 连接的AC名                               |

### Thanks:
[天津师范大学 校园网登陆脚本 Dr.Com](https://gist.github.com/binsee/4dfddb6b1be2803396250b7772056f1c)

### Special thanks:
[dogcom](https://github.com/mchome/dogcom)

### License:
![AGPL V3](https://cloud.githubusercontent.com/assets/7392658/20011165/a0caabdc-a2e5-11e6-974c-8d4961c7d6d3.png)