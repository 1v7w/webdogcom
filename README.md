# webdogcom

Implementation of Dr.com in web authentication.

```shell
Usage:
        webdogcom [options <argument>]...

Options:
        --interval <m>, -i <m>                authentication per m(int) minutes
        --daemon, -d                          set daemon flag
        --help, -h                            display this help
```

### Example:
```shell
$ webdogcom -d
$ webdogcom -d -i 10 # per 10 minutes to auth
```

### To build:
```shell
make
```

### Thanks:
[天津师范大学 校园网登陆脚本 Dr.Com](https://gist.github.com/binsee/4dfddb6b1be2803396250b7772056f1c)

### Special thanks:
[dogcom](https://github.com/mchome/dogcom)

### License:
![AGPL V3](https://cloud.githubusercontent.com/assets/7392658/20011165/a0caabdc-a2e5-11e6-974c-8d4961c7d6d3.png)