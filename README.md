# solar_terms

## 1、介绍

**solar_terms** 是一个根据公历日期判断与二十四节气关系的工具软件包。

### 1.1 目录结构

| 名称 | 说明 |
| ---- | ---- |
| docs  | 文档目录 |
| examples | 例子目录 |
| inc  | 头文件目录 |
| src  | 源代码目录 |

### 1.2 许可证

`solar_terms` package 遵循 `MIT` 许可，详见 `LICENSE` 文件。

### 1.3 依赖

- RT-Thread 3.0+

## 2、如何打开 solar_terms

使用 solar_terms package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages
    tools packages --->
        [*] solar_terms: A tool package for judging the relationship between 24 solar terms according to the date.
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。

## 3、使用 solar_terms

在使用 solar_terms 软件包时首先要定义一个`char`数组来存储返回最近的下一个节气汉字，如：

```c
char str[16] = {'\0'};
```

调用的函数接口为：

```c
int get_solar_terms(int year, int month);
signed char sun2solar_terms(int year, int month, int day, char *solar_terms_str);
```

比如：

```c
static showsolar_terms(int argc, char const *argv[]) {
    if (argc == 4) {
        char str[16] = {'\0'};
        sun2solar_terms(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), str);
        printf("%s\n", str);
    }
    return 0;
}
```

在打开 solar_terms package 后，当进行 bsp 编译时，它会被加入到 bsp 工程中进行编译。

示例演示

在 MSH 中输入命令 `showsolar_terms 2021 4 21`，可以在串口助手上看到输出了距离最近的下一个节气信息。

```
msh />showsolar_terms 2021 4 21
距离 立夏 还有 14 天
```


## 4、注意事项

- 当前可以生成距离最近的下一个节气信息对应的阳历范围为 1900-1-1 到2099-12-31。

## 5、联系方式 & 感谢

* 维护：[XYX12306](https://github.com/XYX12306)
* 主页：https://github.com/XYX12306/solar_terms
