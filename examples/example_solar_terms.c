#include <rtthread.h>
#include <solar_terms.h>

static const char *usage = "        \n\
Welcome in showsolar_terms.         \n\
Usage:                              \n\
    showsolar_terms year month day  \n\
e.g:                                \n\
    showsolar_terms 2021 4 21       \n\
\n";

static showsolar_terms(int argc, char const *argv[])
{
    if (argc == 4)
    {
        char str[16] = {'\0'};
        signed char num;
        num = sun2solar_terms(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), str);
        if (num > 0)
        {
            printf("距离 %s 还有 %d 天\n", str, num);
        }
        else if (num == 0)
        {
            printf("今天是 %s \n", str);
        }
        else
        {
            printf("输入日期有误，请重新输入\n");
        }
    }
    else
    {
        printf("%s\n", usage);
    }
    return 0;
}

#ifdef FINSH_USING_MSH
#include <finsh.h>
MSH_CMD_EXPORT(showsolar_terms, showsolar_terms generator
               : showsolar_terms[year][month][day]);
#endif /* FINSH_USING_MSH */
